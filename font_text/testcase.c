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
/* $Id$
**
** bmpf-test.c: Font performance  test.
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
** Create date: 2008/07/25
*/

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/fixedmath.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static PLOGFONT logfont_vbf_14, logfont_vbf_15 ,logfont_vbf_16, logfont_vbf_17 ;

void test_font_style1(HDC hdc)
{
    SetBkColor(hdc, COLOR_lightwhite);
    SelectFont(hdc, logfont_vbf_14);
    TabbedTextOut(hdc, 0, 10, "MiniGUI Font Style ,the font size 14");

    SelectFont(hdc, logfont_vbf_15);
    TabbedTextOut(hdc, 0, 50, "MiniGUI Font Style ,the font size 15 ");

    SelectFont(hdc, logfont_vbf_16);
    TabbedTextOut(hdc, 0, 90, "MiniGUI Font Style ,the font size 16");

    SelectFont(hdc, logfont_vbf_17);
    TabbedTextOut(hdc, 0, 140, "MiniGUI Font Style ,the font size 16");
}


static int GlyphTestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message) {

        case MSG_CREATE:
            logfont_vbf_14 = CreateLogFont (FONT_TYPE_NAME_BITMAP_VAR, "Times", "ISO8859-1", 
                        FONT_WEIGHT_DEMIBOLD, FONT_SLANT_ITALIC, FONT_FLIP_NIL,
                        FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                       14, 0);

            logfont_vbf_15 = CreateLogFont (FONT_TYPE_NAME_BITMAP_VAR, "Times", "ISO8859-1", 
                        FONT_WEIGHT_DEMIBOLD, FONT_SLANT_ITALIC, FONT_FLIP_NIL,
                        FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                        15, 0);
    
            logfont_vbf_16 = CreateLogFont (FONT_TYPE_NAME_BITMAP_VAR, "Times", "ISO8859-1", 
                        FONT_WEIGHT_DEMIBOLD, FONT_SLANT_ITALIC, FONT_FLIP_NIL,
                        FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                        16, 0);

            logfont_vbf_17 = CreateLogFont (FONT_TYPE_NAME_BITMAP_VAR, "Times", "ISO8859-1", 
                        FONT_WEIGHT_DEMIBOLD, FONT_SLANT_ITALIC, FONT_FLIP_NIL,
                        FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                        17, 0);
            break;

        case MSG_PAINT:
        {
            hdc = BeginPaint (hWnd);
           
                 test_font_style1(hdc);
                 break;

            EndPaint (hWnd, hdc);
        }
            return 0;

        case MSG_KEYDOWN: {
            break;
        }

        case MSG_CLOSE:

            DestroyLogFont(logfont_vbf_14);
            DestroyLogFont(logfont_vbf_15);
            DestroyLogFont(logfont_vbf_16);
            DestroyLogFont(logfont_vbf_17);
            DestroyMainWindow (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
//void testcase1 (HWND hWnd)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif


    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Glyph font test";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = GlyphTestWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    //CreateInfo.iBkColor = COLOR_blue;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
     
         ShowWindow(hMainWnd, SW_SHOWNORMAL);
    
    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

