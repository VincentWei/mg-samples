
/* 
** $Id: icon.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 2.1
**
** helloworld.c: Sample program for MiniGUI Programming Guide
**      The first MiniGUI application.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/endianrw.h>

#define align_32_bits(b) (((b) + 3) & -4)
#define SIZEOF_ICONDIRENTRY 16

void print_icon (void *image, int icon_size, int colornum)
{
    BYTE *data;
    int i, j;

    data = image;
    printf ("\n\nimage data: --->\n\n");
    for (i = 0; i < icon_size; i++) {
	for (j = 0; j < colornum*4; j++) {
	    printf ("0x%02x, ", *(data + i*colornum*4 + j));
	}
	printf ("\n");
	if ((i+1)%colornum*4 == 0)
	    printf ("\n");
    }
}

HICON GUIAPI myLoadIconFromFile (HDC hdc, const char* filename, int which)
{
    FILE* fp;
    WORD wTemp;
    BYTE bTemp;

    int  w, h, colornum;
    DWORD size, offset;
    DWORD imagesize, imagew, imageh;
    BYTE* image;
    
    if( !(fp = fopen(filename, "rb")) ) return 0;

    fseek(fp, sizeof(WORD), SEEK_SET);

    // the cbType of struct ICONDIR.
    wTemp = MGUI_ReadLE16FP (fp);
    if(wTemp != 1) goto error;

    // get ICON images count.
    wTemp = MGUI_ReadLE16FP (fp);
    if (which >= wTemp)
        which = wTemp - 1;
    if (which < 0)
        which = 0;

    // seek to the right ICONDIRENTRY if needed.
    if (which != 0)
        fseek (fp, SIZEOF_ICONDIRENTRY * which, SEEK_CUR);

    // cursor info, read the members of struct ICONDIRENTRY.
    w = fgetc (fp);       // the width of first cursor
    h = fgetc (fp);       // the height of first cursor
    if ((w%16) != 0 || (h%16) != 0) goto error;
    bTemp = fgetc (fp);   // the bColorCount
    if(bTemp != 2 && bTemp != 16) goto error;
    fseek(fp, sizeof(BYTE), SEEK_CUR); // skip the bReserved
    wTemp = MGUI_ReadLE16FP (fp);   // the wPlanes
    if(wTemp != 0) goto error;
    wTemp = MGUI_ReadLE16FP (fp);   // the wBitCount
    if(wTemp > 4) goto error;
    size = MGUI_ReadLE32FP (fp);
    offset = MGUI_ReadLE32FP (fp);

    // read the cursor image info.
    fseek(fp, offset, SEEK_SET);
    fseek(fp, sizeof(DWORD), SEEK_CUR); // skip the biSize member.
    imagew = MGUI_ReadLE32FP (fp);
    imageh = MGUI_ReadLE32FP (fp);
    // check the biPlanes member;
    wTemp = MGUI_ReadLE16FP (fp);
    if(wTemp != 1) goto error;
    // check the biBitCount member;
    wTemp = MGUI_ReadLE16FP (fp);
    if(wTemp > 4) goto error;
    colornum = (int)wTemp;
    fseek(fp, sizeof(DWORD), SEEK_CUR); // skip the biCompression members.

    if (colornum == 1)
        imagesize = align_32_bits(w>>3) * h;
    else
        imagesize = align_32_bits(w>>1) * h;

    imagesize += align_32_bits(w>>3) * h;
    fseek(fp, sizeof(DWORD), SEEK_CUR);

    // skip the rest members and the color table.
    fseek(fp, sizeof(DWORD)*4 + sizeof(BYTE)*(4<<colornum), SEEK_CUR);
    
    // allocate memory for image.
    // Use alloca, the stack may be enough for the image.
    image = (BYTE*) alloca (imagesize);
    // read image
    fread(image, imagesize, 1, fp);

    fclose(fp);
    
#ifdef _DEBUG
    printf ("color number = %d\n", colornum);
    printf ("icon size = %d\n", w);
    print_icon (image + (imagesize - (align_32_bits(w>>3) * h)), w, 1);
    print_icon (image, w, colornum);
#endif
    
    return CreateIcon (hdc, w, h, 
                    image + (imagesize - (align_32_bits(w>>3) * h)), image, colornum);

error:
    fclose(fp);
    return 0;
}

static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HICON myicon;
    HDC hdc;

    switch (message) {
	case MSG_CREATE:
	    myicon = myLoadIconFromFile(HDC_SCREEN, "question.ico", 1);
	break;

        case MSG_PAINT:
	    hdc = BeginPaint(hWnd);
	    DrawIcon(hdc, 0, 0, 0, 0, myicon);
	    EndPaint(hWnd, hdc);
        break;

        case MSG_CLOSE:
	    DestroyIcon(myicon);
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
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
    JoinLayer(NAME_DEF_LAYER , "icon" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "HelloWorld";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = HelloWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 320;
    CreateInfo.by = 240;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
	
    hMainWnd = CreateMainWindow (&CreateInfo);
	
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
