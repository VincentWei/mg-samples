///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
 *   This file is part of mGUtils, a component for MiniGUI.
 * 
 *   Copyright (C) 2003~2018, Beijing FMSoft Technologies Co., Ltd.
 *   Copyright (C) 1998~2002, WEI Yongming
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
/*
** $Id: picview.c 5 2008-02-01 06:07:24Z wangjian $
**
** picview.c: A simple picture viewer.
**
** Copyright (C) 2003 ~ 2017 FMSoft
** Copyright (C) 2001 ~ 2002 Jiang Jun and others.
*/

/*
**  This source is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public
**  License as published by the Free Software Foundation; either
**  version 2 of the License, or (at your option) any later version.
**
**  This software is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public
**  License along with this library; if not, write to the Free
**  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
**  MA 02111-1307, USA
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef __MGUTILS_LIB__
#include "../../include/mgutils.h"
#else
#include <mgutils/mgutils.h>
#endif

#ifdef _MGUTILS_DLGOPENFILE

#ifdef _LANG_ZHCN
#include "picview_res_cn.h"
#elif defined _LANG_ZHTW
#include "picview_res_tw.h"
#else
#include "picview_res_en.h"
#endif

static int MAINWINDOW_LX;
static int MAINWINDOW_TY;
static int MAINWINDOW_RX;
static int MAINWINDOW_BY;

#define TB_BEGIN_X             2
#define TB_BEGIN_Y             2
#define TB_HEIGHT              16
#define TB_WIDTH               22

#define IDC_TB_SELF            100
#define IDC_TB_OPEN            110
#define IDC_TB_SAVE            120
#define IDC_TB_PREVIOUS           130
#define IDC_TB_NEXT             140
#define IDC_TB_SMALL           150
#define IDC_TB_BIG             160
#define IDC_TB_FULL             170

typedef struct tagFILEITEM{
    char                 name[NAME_MAX];
    struct tagFILEITEM*  previous; 
    struct tagFILEITEM*  next; 
}FILEITEM;

static BOOL IsSupport (const char *filename)
{
    char *extend,*temp;
    extend = rindex(filename,'.');
    if (extend ==NULL)
        return FALSE;
    extend++;
    temp = extend;
    while (*temp) {
        *temp = tolower(*temp);
        temp++;
    }

    if(
#ifdef _MGIMAGE_JPG
        !strncmp(extend,"jpg",3) ||
#endif
#ifdef _MGIMAGE_GIF
        !strncmp(extend,"gif",3) ||
#endif
#ifdef _MGIMAGE_PCX
        !strncmp(extend,"pcx",3) ||
#endif
#ifdef _MGIMAGE_LBM
        !strncmp(extend,"lbm",3) ||
#endif
#ifdef _MGIMAGE_TGA
        !strncmp(extend,"tga",3) ||
#endif
#ifdef _MGIMAGE_PNG
        !strncmp(extend,"png",3) ||
#endif
        !strncmp(extend,"bmp",3) )
        return TRUE;

    return FALSE;
}

static FILEITEM* findmatchfile (NEWFILEDLGDATA *one, FILEITEM **cur)
{
    struct  dirent    *pDir;
    struct  stat      ftype;
    DIR               *dir;
    char              fullpath [PATH_MAX + NAME_MAX + 1];
    FILEITEM          *head = NULL;
    FILEITEM          *temp = NULL;

    dir = opendir (one->filepath);

    while ( (pDir = readdir ( dir )) != NULL ) {
        strncpy (fullpath, one->filepath, PATH_MAX);

        if (fullpath[ strlen(fullpath) - 1 ] != '/')
            strcat (fullpath, "/");

        strcat (fullpath, pDir->d_name);

        if (lstat (fullpath, &ftype) < 0 ) {
            Ping();
            continue;
        }

        if (S_ISREG(ftype.st_mode) && IsSupport(pDir->d_name)) {
            if( temp == NULL ) {
                temp = (FILEITEM *) malloc(sizeof(FILEITEM));
                temp->previous = NULL;
                temp->next = NULL;
                strcpy ( temp->name, fullpath);
                head = temp;
            }
            else {
                temp->next = (FILEITEM *) malloc(sizeof(FILEITEM));
                temp->next->previous = temp;
                temp->next->next     = NULL;
                temp = temp->next;
                strcpy ( temp->name, fullpath);
            }
        /*indicate current file pointer*/
        if ( !strcmp( pDir->d_name, one->filename ) )
        *cur = temp;
        }
    }
    closedir(dir);
    return head;
}

static void ReleaseList(FILEITEM *head)
{
    FILEITEM *temp;
    while(head)
    {
        temp = head->next;
        free (head);
        head = temp;
    }
}

static int 
show_picture (HWND hWnd, HDC hdc, const char *path, float newrate)
{
    BITMAP Bitmap1;
    int x,y;

    if (LoadBitmap(HDC_SCREEN, &Bitmap1, path) < 0) {
        fprintf(stderr,"Load Bitmap %s Error\n",path);
        return -1;
    }

    x = (MAINWINDOW_RX - MAINWINDOW_LX)/2 - Bitmap1.bmWidth*newrate/2;
    if (Bitmap1.bmHeight*newrate <= (MAINWINDOW_BY - MAINWINDOW_TY - TB_HEIGHT-4))
        y = (MAINWINDOW_BY - MAINWINDOW_TY - TB_HEIGHT - 4 )/2 
            - Bitmap1.bmHeight*newrate/2 + TB_HEIGHT+2;
    else
        y = TB_HEIGHT + 4; 

    FillBoxWithBitmap (hdc, x, y,
            Bitmap1.bmWidth*newrate, Bitmap1.bmHeight*newrate, &Bitmap1);
    UnloadBitmap(&Bitmap1);

    return 0;
}

static struct toolbar_items
{
    int id;
    char* name;
} toolbar_items [] =
{
    {IDC_TB_OPEN, "open"},
    {IDC_TB_PREVIOUS, "previous"},
    {IDC_TB_NEXT, "next"},
    {IDC_TB_BIG, "big"},
    {IDC_TB_SMALL, "small"},
    {IDC_TB_SAVE, "save"},
    {IDC_TB_FULL, "full"},
};

static BITMAP      ntb_bmp;

static void InitNewToolBar (HWND hWnd)
{
    int         i;
    NTBINFO     ntb_info;
    NTBITEMINFO ntbii;
    HWND        ntb;
    gal_pixel   pixel;

    ntb_info.nr_cells = TABLESIZE (toolbar_items);
    ntb_info.w_cell  = 22;
    ntb_info.h_cell  = 16;
    ntb_info.nr_cols = 0;
    ntb_info.image = &ntb_bmp;

    ntb = CreateWindow (CTRL_NEWTOOLBAR, "",
                    WS_CHILD | WS_VISIBLE, 
                    IDC_TB_SELF,
                    TB_BEGIN_X, TB_BEGIN_Y, 
                    TB_WIDTH * TABLESIZE (toolbar_items),
                    TB_HEIGHT, hWnd, 
                    (DWORD) &ntb_info);

    pixel = GetPixelInBitmap (&ntb_bmp, 0, 0);
    SetWindowBkColor (ntb, pixel);
    InvalidateRect (ntb, NULL, TRUE);

    for (i = 0; i < TABLESIZE (toolbar_items); i++) {
        memset (&ntbii, 0, sizeof (ntbii));

        ntbii.flags = NTBIF_PUSHBUTTON;
        ntbii.id = toolbar_items [i].id;
        ntbii.bmp_cell = i;
        SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);
    }
}

static LRESULT ViewWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static FILEITEM    *currentfile;
    static FILEITEM    *head        = NULL;
    static float       rate         = 1;
    static NEWFILEDLGDATA filepathdata;
    RECT               rect;

    GetClientRect (hWnd,&rect);
    rect.top +=TB_HEIGHT + 4;
    switch (message) {
        case MSG_CREATE:

        if (LoadBitmap (HDC_SCREEN, &ntb_bmp, "res/newtoolbar.bmp"))
            return -1;

        InitNewToolBar (hWnd);
        break;
        
        case MSG_COMMAND:
        {
            int id   = HIWORD(wParam);
            switch (id) {  
                case  IDC_TB_OPEN:
                {
                    int choise=0;

                    filepathdata.IsSave = FALSE;
                    filepathdata.filterindex = 1;
                    strcpy (filepathdata.filepath, "./");
                    strcpy (filepathdata.filter, 
                            "All file(*.*) | Bitmap file(*.bmp)");
                    memset (filepathdata.filename, 0, sizeof(filepathdata.filename));

                    choise = ShowOpenDialog (hWnd, 100, 100, 350, 250, &filepathdata);

                    if (choise == IDOK && IsSupport(filepathdata.filename) ) {
                        rate = 1;
                        if (head) {
                            ReleaseList(head);
                        }
                    head = findmatchfile (&filepathdata, &currentfile);
                    InvalidateRect (hWnd, &rect, TRUE);
                   }
               }
               break;

            case  IDC_TB_PREVIOUS:
                if ( currentfile != NULL && currentfile->previous != NULL ) {
                    rate = 1;
                    currentfile = currentfile->previous;
                    InvalidateRect(hWnd,&rect,TRUE);
                }
                break;

            case  IDC_TB_NEXT:
                if ( currentfile != NULL && currentfile->next!= NULL ) {
                    rate = 1;
                    currentfile = currentfile->next;
                    InvalidateRect(hWnd,&rect,TRUE);
                }                    
                break;
            
            case  IDC_TB_SMALL:
                if(rate > 0.125) {
                    rate = rate/2;
                    InvalidateRect(hWnd,&rect,TRUE);
                }
                break;
            
            case  IDC_TB_BIG:
                if (rate < 8) {
                    rate = rate*2;
                    InvalidateRect(hWnd,&rect,TRUE);
                }
                break;
            }
        }
        break;
        
        case MSG_PAINT:
        {
            HDC hdc = BeginPaint (hWnd);
            if (currentfile) {
                show_picture (hWnd, hdc, currentfile->name, rate);
                char caption [300];
                sprintf (caption, "%s - %s", PV_ST_CAPTION, currentfile->name);
                SetWindowCaption (hWnd, caption);
            }
            else 
                SetWindowCaption (hWnd, PV_ST_CAPTION);
            EndPaint (hWnd, hdc);
            return 0;
        }
        
        case MSG_KEYDOWN:
        {
            switch ( LOWORD(wParam) ) {
            case SCANCODE_F1:
                // OpenHelpWindow (HWND_DESKTOP, IDAPP_PIC, IDMSG_PIC);
                return 0;
                
            case SCANCODE_PAGEUP:
                if (currentfile != NULL && currentfile->previous != NULL) {
                    rate = 1;
                    currentfile = currentfile->previous;
                    InvalidateRect (hWnd, &rect, TRUE);
                }
                return 0;

            case SCANCODE_PAGEDOWN:
                if (currentfile != NULL && currentfile->next!= NULL) {
                    rate = 1;
                    currentfile = currentfile->next;
                    InvalidateRect(hWnd,&rect,TRUE);
                }
                return 0;            

            case SCANCODE_BACKSPACE:
                if (currentfile != NULL && currentfile->previous != NULL)    {
                    rate = 1;
                    currentfile = currentfile->previous;
                    InvalidateRect(hWnd,&rect,TRUE);
                }
                return 0;            

            default:
                break;
            }
            break;
        }
        
        case MSG_CHAR:
        {
            RECT  rect;
            GetClientRect(hWnd,&rect);
            rect.top +=TB_HEIGHT+4;
            if (LOBYTE(wParam) == ' ') {
                if (currentfile != NULL && currentfile->next!= NULL) {
                    rate = 1;
                    currentfile = currentfile->next;
                    InvalidateRect(hWnd,&rect,TRUE);
                }
            }
        }
        return 0;
        
        case MSG_CLOSE:
        {
            if (head)
                ReleaseList(head);
            
            DestroyAllControls (hWnd);
            currentfile = NULL;
            head = NULL;
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        }
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    
#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        return 1;
    }

#endif

    MAINWINDOW_LX = 0;
    MAINWINDOW_TY = 0;
    MAINWINDOW_RX = g_rcScr.right;
    MAINWINDOW_BY = g_rcScr.bottom;
    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = PV_ST_CAPTION;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ViewWinProc;
    CreateInfo.lx = MAINWINDOW_LX;
    CreateInfo.ty = MAINWINDOW_TY;
    CreateInfo.rx = MAINWINDOW_RX;
    CreateInfo.by = MAINWINDOW_BY;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return 3;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#else

int MiniGUIMain (int argc, const char* argv[])
{
    fprintf (stderr, "Please enable the support for Open File Dialog Box in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_DLGOPENFILE */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

