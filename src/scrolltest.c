/* 
** $Id: helloworld.c 616 2008-08-25 03:17:42Z weiym $
**
** Listing 2.1
**
** helloworld.c: Sample program for MiniGUI Programming Guide
**      The first MiniGUI application.
**
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

RECT* LWindowToClient(HWND hwnd,const RECT* rcWin, RECT* rcCli)
{
	const WINDOW_ELEMENT_RENDERER* rdr;
	int border;
	if(rcWin == NULL || rcCli == NULL)
		return NULL;
	
	rcCli->left = rcCli->top = 0;
	rcCli->right = RECTWP(rcWin);
	rcCli->bottom = RECTHP(rcWin);

	const WINDOWINFO* wi = GetWindowInfo(hwnd);
	if(wi == NULL ||(rdr = wi->we_rdr) == NULL){
		return rcCli;
	}
	
	
	border = ((*rdr->calc_we_metrics)(hwnd, NULL, LFRDR_METRICS_BORDER)*2);
	
	rcCli->right -= border;
	rcCli->right -= (*rdr->calc_we_metrics)(hwnd, NULL, LFRDR_METRICS_VSCROLL_W);

	rcCli->bottom -= border;
	if(IsMainWindow(hwnd)){
		rcCli->bottom -= (*rdr->calc_we_metrics)(hwnd, NULL, LFRDR_METRICS_CAPTION_H);
		rcCli->bottom -= (*rdr->calc_we_metrics)(hwnd, NULL, LFRDR_METRICS_MENU_H);
	}
	rcCli->bottom -= (*rdr->calc_we_metrics)(hwnd, NULL, LFRDR_METRICS_HSCROLL_H);
		
	return rcCli;
}


/*
BOOL UpdateScroll(HWND hwnd, 
	int iSBar, 
	int doc_size, 
	int cli_size, 
	int* new_pos,
	BOOL bRedraw)
{
	SCROLLINFO si;
	DWORD dwStyle;
	int scrollsize = 0;

	if(!IsWindow(hwnd))
		return FALSE;

	memset(&si,0,sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	if(cli_size <= 0)
	{
		RECT rt;
		GetClientRect(hwnd, &rt);
		if(iSBar == SB_HORZ)
			cli_size = RECTW(rt);
		else
			cli_size = RECTH(rt);
	}

	dwStyle = GetWindowStyle(hwnd);

	if( doc_size <= (cli_size+scrollsize)) //hide scrollbar
	{
		SetScrollInfo(hwnd, iSBar, &si, FALSE);
		EnableScrollBar(hwnd, iSBar, FALSE);
		ShowScrollBar(hwnd, iSBar, FALSE);
		ExcludeWindowStyle(hwnd, iSBar==SB_HORZ?WS_HSCROLL:WS_VSCROLL);
		if(new_pos)
			*new_pos = -si.nPos;
		return FALSE;
	}
	
	IncludeWindowStyle(hwnd, iSBar==SB_HORZ?WS_HSCROLL:WS_VSCROLL);

	GetScrollInfo(hwnd, iSBar, &si);
	si.nMax = doc_size;
	si.nMin = 0;
	si.nPage = cli_size;
	if(si.nPos<0)
		si.nPos = 0;
	else if(si.nPos > (si.nMax-si.nPage))
		si.nPos = si.nMax - si.nPage;
	
	SetScrollInfo(hwnd, iSBar,&si, bRedraw);

	//ShowScrollbar
	EnableScrollBar(hwnd, iSBar, TRUE);
	ShowScrollBar(hwnd, iSBar, TRUE);

	if(new_pos)
		*new_pos = -si.nPos;

	return TRUE;
}*/

#define INIT_SCROLLINFO(si)  do{ \
	memset(&si, 0, sizeof(si)); \
	si.cbSize = sizeof(si); \
	si.fMask = SIF_ALL; \
}while(0)

int GetWindowMetrics(HWND hwnd, int which){
	const WINDOW_ELEMENT_RENDERER* rdr;
	const WINDOWINFO* wi = GetWindowInfo(hwnd);
	if(wi == NULL ||(rdr = wi->we_rdr) == NULL){
		return 0;
	}
	
	return (*rdr->calc_we_metrics)(hwnd, NULL, which);
}

static inline void set_scroll_info(HWND hwnd, int iSbar, SCROLLINFO *psi, int doc_size, int view_size, BOOL boldShowed)
{	
	GetScrollInfo(hwnd, iSbar, psi);
	psi->nMin = 0;
	psi->nMax = doc_size;
	psi->nPage = view_size;
	if(psi->nPos < psi->nMin)
		psi->nPos = psi->nMin;
	else if(psi->nPos > (psi->nMax - psi->nPage))
		psi->nPos = psi->nMax - psi->nPage;
		
	if(!boldShowed) //show scroll
	{
		IncludeWindowStyle(hwnd, iSbar==SB_HORZ?WS_HSCROLL:WS_VSCROLL);
		SetScrollInfo(hwnd, iSbar, psi,FALSE);
		EnableScrollBar(hwnd, iSbar, TRUE);
		ShowScrollBar(hwnd, iSbar, TRUE);
	}
	else
		SetScrollInfo(hwnd, iSbar, psi,FALSE);
}

BOOL UpdateHVScroll(HWND hwnd, 
	const RECT* rtdoc,
	const RECT* rtview,
	int *x_offset,
	int *y_offset,
	BOOL bAutoUpdate)
{
	SCROLLINFO hsi, vsi;
	DWORD dwStyle, dwOldStyle;
	int scrollsize ;
	int h_view_width;
	int h_doc_width;
	int v_view_height;
	int v_doc_height;
	int hscroll_height;
	int vscroll_width;
	int h_diff;
	int v_diff;

	if(!IsWindow(hwnd) || rtdoc == NULL)
		return FALSE;

	h_doc_width = RECTWP(rtdoc);
	v_doc_height = RECTHP(rtdoc);

	if(rtview == NULL)
	{
		RECT rt;
		GetClientRect(hwnd, &rt);
		h_view_width = RECTW(rt);
		v_view_height = RECTH(rt);
	}
	else{
		h_view_width = RECTWP(rtview);
		v_view_height = RECTHP(rtview);
	}

	dwOldStyle = dwStyle = GetWindowStyle(hwnd);

	scrollsize = GetWindowElementAttr(HWND_NULL,WE_METRICS_SCROLLBAR);
	
	vscroll_width = GetWindowMetrics(hwnd,LFRDR_METRICS_VSCROLL_W);
	hscroll_height = GetWindowMetrics(hwnd,LFRDR_METRICS_HSCROLL_H);

	if(vscroll_width == 0)
		h_view_width -= scrollsize;
	if(hscroll_height == 0)
		v_view_height -= scrollsize;

	h_diff = h_doc_width - h_view_width;
	v_diff = v_doc_height - v_view_height;

	INIT_SCROLLINFO(hsi);
	INIT_SCROLLINFO(vsi);

	if(h_diff <= scrollsize && v_diff <= scrollsize) //hide HScroll and VScroll
	{
		dwStyle &= ~(WS_HSCROLL|WS_VSCROLL);
		if(dwStyle != dwOldStyle){
			SetScrollInfo(hwnd, SB_HORZ, &hsi, FALSE);
			SetScrollInfo(hwnd, SB_VERT, &hsi, FALSE);
			ExcludeWindowStyle(hwnd, WS_HSCROLL|WS_VSCROLL);
			if(x_offset) 
				*x_offset = 0;
			if(y_offset)
				*y_offset = 0;
			if(bAutoUpdate)
				UpdateWindow(hwnd,TRUE);
			return TRUE;
		}
		else{
			if(bAutoUpdate)
				InvalidateRect(hwnd, NULL, TRUE);
		}
		return FALSE;
	}
	else if(h_diff <= 0 && v_diff > scrollsize) //hide  HSCROLL, show VScroll
	{
		if(hscroll_height == 0)
			v_view_height += scrollsize;
		dwStyle &= ~WS_HSCROLL;
		dwStyle |= WS_VSCROLL;
	}
	else if(v_diff <= 0 && h_diff > scrollsize) // hide VSCROLL, show HScroll
	{
		if(vscroll_width == 0)
			h_view_width += scrollsize;
		dwStyle &= ~WS_VSCROLL;
		dwStyle |= WS_HSCROLL;
	}
	else //show VScroll and HScroll
	{
		dwStyle |= (WS_VSCROLL|WS_HSCROLL);
	}

	if(dwStyle & WS_HSCROLL){
		set_scroll_info(hwnd, SB_HORZ, &hsi, h_doc_width, h_view_width, dwOldStyle&WS_HSCROLL);

	}
	else
	{
		if(dwOldStyle & WS_HSCROLL) //hide
		{
			SetScrollInfo(hwnd, SB_HORZ, &hsi,FALSE);
			EnableScrollBar(hwnd, SB_HORZ, FALSE);
			ShowScrollBar(hwnd, SB_HORZ, FALSE);
			ExcludeWindowStyle(hwnd, WS_HSCROLL);
		}
	}

	if(dwStyle & WS_VSCROLL){
		set_scroll_info(hwnd, SB_VERT, &vsi, v_doc_height, v_view_height, dwOldStyle&WS_VSCROLL);
	}
	else
	{
		if(dwOldStyle & WS_VSCROLL) //hide
		{
			SetScrollInfo(hwnd, SB_VERT, &vsi,FALSE);
			EnableScrollBar(hwnd, SB_VERT, FALSE);
			ShowScrollBar(hwnd, SB_VERT, FALSE);
			ExcludeWindowStyle(hwnd, WS_VSCROLL);
		}
	}

	if(x_offset)
		*x_offset = -hsi.nPos;
	if(y_offset)
		*y_offset = -vsi.nPos;
	
	
	if(bAutoUpdate){
		UpdateWindow(hwnd,TRUE);
	}
	
	return TRUE;
}

int ProcessScrollMessage(HWND hwnd, int iSBar, int nc,int pos, int line, BOOL bRedraw)
{
	SCROLLINFO si;
	int nPos;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	if(!GetScrollInfo(hwnd, iSBar, &si))
		return -1;
	nPos = si.nPos;

	switch(nc)
	{
	case SB_LINEUP:
	case SB_LINELEFT:
		nPos -= line;
		break;
	case SB_LINEDOWN:
	case SB_LINERIGHT:
		nPos += line;
		break;
	case SB_PAGEUP:
	case SB_PAGELEFT:
		nPos -= si.nPage;
		break;
	case SB_PAGEDOWN:
	case SB_PAGERIGHT:
		nPos += si.nPage;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nPos = pos;
		break;
	case SB_ENDSCROLL:
		return nPos;
	default:
		return -1;
	}

	if(nPos < si.nMin)
		nPos = si.nMin;
	else if(nPos > (si.nMax - si.nPage))
		nPos = si.nMax - si.nPage ;

	si.nPos = nPos;
	SetScrollInfo(hwnd, iSBar, &si, bRedraw);
	return nPos;
}

///////////////////////////////////////

#define IDC_NTB_LEFT    100
#define IDC_NTB_RIGHT   110
#define IDC_NTB_UP      120
#define IDC_NTB_SETBMP  130

static int ntb_height = 38;
static BITMAP ntb_bmp;
static void create_new_toolbar (HWND hWnd)
{
    HWND ntb;
    NTBINFO ntb_info;
    NTBITEMINFO ntbii;
    gal_pixel pixel;
	
     if (LoadBitmap (HDC_SCREEN, &ntb_bmp, "res/new2.jpg"))
		 return;

    ntb_info.nr_cells = 4;
    ntb_info.w_cell  = 0;
    ntb_info.h_cell  = 0;
    ntb_info.nr_cols = 0;
    ntb_info.image = &ntb_bmp;

    ntb = CreateWindow (CTRL_NEWTOOLBAR,
                    "",
                    WS_CHILD | WS_VISIBLE, 
                    100,
                    0, 0, 1024, 0,
                    hWnd,
                    (DWORD) &ntb_info);


    pixel = GetPixelInBitmap (&ntb_bmp, 0, 0);
    SetWindowBkColor (ntb, pixel);
    InvalidateRect (ntb, NULL, TRUE);

    memset (&ntbii, 0, sizeof (ntbii));
    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_LEFT;
    ntbii.bmp_cell = 1;
    SendMessage(ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_RIGHT;
    ntbii.bmp_cell = 2;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_SETBMP;
    ntbii.bmp_cell = 3;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);
    
    ntbii.flags = NTBIF_SEPARATOR;
    ntbii.id = 0;
    ntbii.bmp_cell = 0;
    ntbii.text = NULL;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_UP;
    ntbii.bmp_cell = 0;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

	RECT rt;
	GetClientRect(ntb, &rt);
	ntb_height = RECTH(rt);
}

static DIR* dir;
static char img_path[256];
static BITMAP bmp;

void SetImagePath(const char* path)
{
	struct stat buf;
	if(stat(path, &buf)==0 && S_ISDIR(buf.st_mode))
	{
		strcpy(img_path, path);
	}
	else
		strcpy(img_path, ".");
	dir = opendir(img_path);
}

void ClearImagePath(void)
{
	if(dir)
		closedir(dir);
	if(bmp.bmBits)
		UnloadBitmap(&bmp);
}

BITMAP *get_next_bitmap(BOOL isprev)
{
	char file[512];
	struct dirent* dp;

	if(dir == NULL)
		return NULL;
	
	while((dp = readdir(dir)) != NULL )
	{
		if(bmp.bmBits != NULL)
			UnloadBitmap(&bmp);

		sprintf(file,"%s/%s",img_path,dp->d_name);
		//printf("file=%s\n",file);
		if(LoadBitmapFromFile(HDC_SCREEN, &bmp, file) == 0)
		{
			if(isprev)
				seekdir(dir, telldir(dir)-2);
			return &bmp;
		}
		
		if(isprev)
		{
			seekdir(dir,telldir(dir)-2);
		}
	}
	return NULL;
}


static BITMAP* pbmp;
static int zoom_scale = 100;
static int x_pos = 0; 
static int y_pos = 0;
void UpdateScrolls(HWND hwnd, RECT* prt, BOOL bRedraw)
{
	RECT rt;
	RECT rtdoc;
	if(pbmp == NULL)
		return;

	if(prt == NULL)
		GetClientRect(hwnd, &rt);
	else
		rt = *prt;

	rt.top += ntb_height;
	
	rtdoc.left = 0;
	rtdoc.top = 0;
	rtdoc.right = pbmp->bmWidth * zoom_scale / 100;
	rtdoc.bottom = pbmp->bmHeight * zoom_scale / 100;

	UpdateHVScroll(hwnd, &rtdoc, &rt, &x_pos, &y_pos, bRedraw);
}

void ShowBitmap(HDC hdc)
{
	if(pbmp == NULL)
		return;
	/*printf("x=%d,y=%d,w=%d,h=%d\n", x_pos, y_pos+ntb_height,
		pbmp->bmWidth*zoom_scale/100,
		pbmp->bmHeight*zoom_scale/100);*/
	FillBoxWithBitmap(hdc, x_pos, y_pos+ntb_height,
		pbmp->bmWidth*zoom_scale/100,
		pbmp->bmHeight*zoom_scale/100,
		pbmp);
}

static LRESULT ScrollTestProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message) {
        case MSG_CREATE:
			create_new_toolbar(hWnd);
			pbmp = get_next_bitmap(FALSE);
            break;
		case MSG_COMMAND:
			switch(HIWORD(wParam))
			{
			case IDC_NTB_LEFT:
				pbmp = get_next_bitmap(TRUE);
				UpdateScrolls(hWnd,NULL,TRUE);
				//UpdateWindow(hWnd,TRUE);
				break;
			case IDC_NTB_RIGHT:
				pbmp = get_next_bitmap(FALSE);
				UpdateScrolls(hWnd,NULL,TRUE);
				//UpdateWindow(hWnd,TRUE);
				break;
			case IDC_NTB_UP: //zoom in
				zoom_scale /= 2;
				if(zoom_scale < 10)
					zoom_scale = 10;
				UpdateScrolls(hWnd,NULL,TRUE);
				//UpdateWindow(hWnd,TRUE);
				break;
			case IDC_NTB_SETBMP: //zoom out
				zoom_scale *= 2;
				UpdateScrolls(hWnd,NULL,TRUE);
				//UpdateWindow(hWnd,TRUE);
				break;
			}
			break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
			ShowBitmap(hdc);
            EndPaint (hWnd, hdc);
            return 0;
		case MSG_SIZECHANGING:
			{
				RECT rt;
				LWindowToClient(hWnd,(const RECT*)wParam, &rt);
				UpdateScrolls(hWnd,&rt,FALSE);
			}
			break;

       case MSG_KEYDOWN:
           return 0;
		
		case MSG_HSCROLL:
			x_pos = -ProcessScrollMessage(hWnd,SB_HORZ,wParam,lParam,10,TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case MSG_VSCROLL:
			y_pos = -ProcessScrollMessage(hWnd,SB_VERT,wParam,lParam,10,TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
			break;	

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

	printf("Usage: %s <image directory>\n", argv[0]);

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION ;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "ScrollTest";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ScrollTestProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_darkcyan;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

	if(argc > 1)
		SetImagePath(argv[1]);
	else 
		SetImagePath(".");
    

    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);


    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

	ClearImagePath();

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

