/*
** $Id: Japanfontdemo.c 369 2008-01-25 07:15:29Z zhounuohua $
**
** The demo of font.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2002/01/17
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

/*
** TODO:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#define IDM_EXIT            160

#define IDM_TEXTOUT1        200
#define IDM_TEXTOUT2        210
#define IDM_TEXTOUT3        215
#define IDM_TEXTOUT4        216
#define IDM_TEXTOUT5        217

#define IDM_ABOUT           310

static HMENU createpmenuabout (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"About";
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT;
    mii.typedata    = (DWORD)"About MiniGUI...";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenufile (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"File";
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_EXIT;
    mii.typedata    = (DWORD)"Exit";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenudialogs (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"Text";
    hmnu = CreatePopupMenu (&mii);
    
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUT1;
    mii.typedata    = (DWORD)"*-fixed-rrncnn-*-14-JISX0208-1...";
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUT2;
    mii.typedata    = (DWORD)"*-Courier-rrncnn-*-16-JISX0208-1...";
    InsertMenuItem(hmnu, 1, TRUE, &mii);
     
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUT3;
    mii.typedata    = (DWORD)"rbf-gothic-rrncnn-6-12-JISX0201-1...";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUT4;
    mii.typedata    = (DWORD)"rbf-gothic-rrncnn-12-12-JISX0208-1 font...";
    InsertMenuItem(hmnu, 3, TRUE, &mii);
     
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUT5;
    mii.typedata    = (DWORD)"rbf-mincho-rrncnn-12-12-JISX0208-1 font...";
    InsertMenuItem(hmnu, 4, TRUE, &mii);  
    
    return hmnu;
}

static HMENU createmenu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"File";
    mii.hsubmenu    = createpmenufile ();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"Fonts";
    mii.hsubmenu    = createpmenudialogs ();
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)"About";
    mii.hsubmenu    = createpmenuabout ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);
                   
    return hmnu;
}

#define MODE_TEXTOUT0           10
#define MODE_TEXTOUT1           11
#define MODE_TEXTOUT2           12
#define MODE_TEXTOUT3           13
#define MODE_TEXTOUT4           14
#define MODE_TEXTOUT5           15

static PLOGFONT  logfont1, logfont2, logfont3, logfont4, logfont5; 

static const char* shift_jis_text = "Shift_JIS encoded text:\n"
"人材募集\n"
"Googleは、このインターネット業界において、確実な売り上げを伸ばし、順調に成長している企業です。高度な次世代のテクノロジーを利用したGoogleの検索エンジンは、世界中のユーザに高く評価され、たくさんのユーザに利用されています。Googleのアクセスの半分以上は、アメリカ以外の世界各国からのアクセスが占めています。　\n"
"Googleの１員として、Googleジャパンにて、チームワークを尊重し、柔軟性をもって働いて下さる方を募集しています\n";


static const char*font_name1 =
"this is *-fixed-rrncnn-*-14-JISX0208-1 font:";

static const char*katakana=
"Half-width katakana :";

static const char*ascii_roman=
"ASCII/JIS-Roman :";

static const char*StandChar=
"Two-byte Standard Characters :";

static const char*font_name2 =
"this is *-Courier-rrncnn-*-16-JISX0208-1 font:";

static const char*font_name3 =
"rbf-gothic-rrncnn-6-12-JISX0201-1 font :";

static const char*font_name4 =
"rbf-gothic-rrncnn-12-12-JISX0208-1 font:";

static const char*font_name5 =
"rbf-mincho-rrncnn-12-12-JISX0208-1 font:";

static char halfkata[50], ascii[200];

#define LEN_ONE_AREA ((0xFD-0x80 + 0x7F - 0x40) * 2)

static unsigned char jiskan_one_area [LEN_ONE_AREA];
static unsigned char jiskan_first_byte = 0x81;
    
static void make_jisx0201_text (unsigned char *mchar, unsigned char start, unsigned char end)
{
    int i = 0;
    unsigned char c;
   
    for (c = start; c <= end; c++) {
        mchar [i++] = c;
    }
}

static void make_jisx0208_one_area_text (unsigned char *mchar, unsigned char first_byte)
{
    int i = 0;
    unsigned char c;
   
    for (c = 0x40; c <= 0x7E; c++) {
        mchar [i++] = first_byte;
        mchar [i++] = c;
    }

    for (c = 0x80; c <= 0xFC; c++) {
        mchar [i++] = first_byte;
        mchar [i++] = c;
    }
}

static void next_jisx0208_area (void)
{
    jiskan_first_byte ++;
    if (jiskan_first_byte == 0xA0)
        jiskan_first_byte = 0xE0;

    if (jiskan_first_byte == 0xF0)
        jiskan_first_byte = 0x81;

    make_jisx0208_one_area_text (jiskan_one_area, jiskan_first_byte);
}

static void prev_jisx0208_area (void)
{
    jiskan_first_byte --;
    if (jiskan_first_byte == 0x80)
        jiskan_first_byte = 0xEF;

    if (jiskan_first_byte == 0xDF)
        jiskan_first_byte = 0x9F;

    make_jisx0208_one_area_text (jiskan_one_area, jiskan_first_byte);
}

void OnModeTextOut(HDC hdc, PLOGFONT logfont, const char*font_name)
{
    RECT rc;

    SetBkColor (hdc, COLOR_lightwhite);
    SetBkMode (hdc, BM_TRANSPARENT);

    SelectFont (hdc, logfont);
    
    TextOut (hdc, 0, 0, font_name);
    TextOut (hdc, 0, 30, katakana);
    TextOut (hdc, 0, 50, halfkata);
    TextOut (hdc, 0, 80, ascii_roman);
    TextOut (hdc, 0, 100, ascii);
    TextOut (hdc, 0, 150, StandChar);
   
    
    rc.left = 30; rc.top = 200;
    rc.right = 600; rc.bottom = 400;
    DrawText (hdc, jiskan_one_area, LEN_ONE_AREA, &rc, DT_LEFT | DT_WORDBREAK);
    
    rc.top += 100;
    rc.bottom += 100;
    DrawText (hdc, shift_jis_text, strlen(shift_jis_text), &rc, DT_LEFT | DT_WORDBREAK);
}

void OnModeTextOut1 (HDC hdc)
{
    OnModeTextOut(hdc, logfont1, font_name1); 
}

void OnModeTextOut2 (HDC hdc)
{
    OnModeTextOut(hdc, logfont2, font_name2); 
}

void OnModeTextOut3 (HDC hdc)
{
    OnModeTextOut(hdc, logfont3, font_name3); 
}

void OnModeTextOut4 (HDC hdc)
{
    OnModeTextOut(hdc, logfont4, font_name4); 
}
    
void OnModeTextOut5 (HDC hdc)
{
    OnModeTextOut(hdc, logfont5, font_name5); 
}

int FontTestMainWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static int mode = MODE_TEXTOUT1;

    switch (message) {
        case MSG_CREATE:
            logfont1 = GetSystemFont(SYSLOGFONT_WCHAR_DEF);
            logfont2 = GetSystemFont(SYSLOGFONT_FIXED);
            logfont3 = GetSystemFont(SYSLOGFONT_CAPTION);
            logfont4 = GetSystemFont(SYSLOGFONT_MENU);
            logfont5 = GetSystemFont(SYSLOGFONT_CONTROL);
            make_jisx0201_text (halfkata, 0xA1, 0xDF);
            make_jisx0201_text (ascii, 0x21, 0x7E);
            make_jisx0208_one_area_text (jiskan_one_area, jiskan_first_byte);
            break;

        case MSG_LBUTTONDOWN:
            next_jisx0208_area ();
            InvalidateRect (hWnd, NULL, TRUE);
            break;

        case MSG_RBUTTONDOWN:
            prev_jisx0208_area ();
            InvalidateRect (hWnd, NULL, TRUE);
            break;

        case MSG_PAINT:
        {
            HDC hdc;

            hdc = BeginPaint (hWnd);

            switch (mode) {
            case MODE_TEXTOUT1:
                OnModeTextOut1 (hdc);
                break;
            case MODE_TEXTOUT2:
                OnModeTextOut2 (hdc);
                break;
            case MODE_TEXTOUT3:
                OnModeTextOut3 (hdc);
                break;
            case MODE_TEXTOUT4:
                OnModeTextOut4 (hdc);
                break;
            case MODE_TEXTOUT5:
                OnModeTextOut5 (hdc);
                break;
            }

            EndPaint (hWnd, hdc);
            return 0;
        }
        
        case MSG_COMMAND:
        switch (wParam) 
        {
            case IDM_EXIT:
                SendMessage (hWnd, MSG_CLOSE, 0, 0L);
            break;

            case IDM_TEXTOUT1:
                mode = MODE_TEXTOUT1;
                InvalidateRect (hWnd, NULL, TRUE);
            break;
            
            case IDM_TEXTOUT2:
                mode = MODE_TEXTOUT2;
                InvalidateRect (hWnd, NULL, TRUE);
            break;
            
            case IDM_TEXTOUT3:
                mode = MODE_TEXTOUT3;
                InvalidateRect (hWnd, NULL, TRUE);
            break;

            case IDM_TEXTOUT4:
                mode = MODE_TEXTOUT4;
                InvalidateRect (hWnd,NULL,TRUE);
                break;

            case IDM_TEXTOUT5:
                mode = MODE_TEXTOUT5;
                InvalidateRect (hWnd,NULL,TRUE);
                break;
            
            case IDM_ABOUT:
                break;
        }
        break;

        case MSG_CLOSE:
            if (MessageBox (hWnd, 
                            "Are you sure to quit?", 
                            "FontTest", 
                            MB_YESNOCANCEL | MB_ICONQUESTION) != IDYES)
                return 0;

            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = "The japanese font test main window";
    pCreateInfo->hMenu = createmenu ();
    pCreateInfo->hCursor = GetSystemCursor (IDC_ARROW);
    pCreateInfo->hIcon = GetSmallSystemIcon (IDI_APPLICATION);
    pCreateInfo->MainWindowProc = FontTestMainWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 1024;
    pCreateInfo->by = 700;
    pCreateInfo->iBkColor = COLOR_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;
    
    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        exit (3);

    ShowWindow (hMainWnd, SW_SHOWNORMAL);
    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
