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
** $Id: palette.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 26.5
**
** palette.c: Sample program for MiniGUI Programming Guide
**       Demonstration of palette operation
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

static BOOL g_bShowBall = FALSE;

/*
 * Calculate an 8-bit grey palette 
 */
static void InitMyPalette(GAL_Color *colors)
{
    int i;

    for(i = 0; i < 256; i++) {
        int r;
        r = 256 - i;
        colors[i].r = r;
        colors[i].g = r;
        colors[i].b = r;
    }
}

static LRESULT PaletteWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    int x, y;
    static GAL_Color colorPal[256], myPal[256];

    switch (message) {
    case MSG_CREATE:
        GetPalette(HDC_SCREEN, 0, 256, colorPal);
#ifdef _DEBUG
        for (i = 0; i < 256; i++) {
            printf ("i = %d\n, red = %d\n, green = %d\n, blue = %d\n\n", 
                    i, colorPal[i].r, colorPal[i].g, colorPal[i].b);
        }
#endif
        break;

    case MSG_PAINT:
        {
            if (g_bShowBall)
                SendMessage (hWnd, MSG_LBUTTONDOWN, 0, 0);
        }
        break;
    case MSG_LBUTTONDOWN:
        g_bShowBall = TRUE;
        hdc = GetClientDC(hWnd);
        InitMyPalette(myPal);
        SetPalette(hdc, 0, 256, myPal);
        SetBrushColor(hdc, COLOR_red);
        FillCircle(hdc, 10, 10, 8);
        for (y = 0; y < 240; y += 20) {
            for (x = 0; x < 320; x += 20) {
              BitBlt(hdc, 0, 0, 20, 20, hdc, x, y, 0);
              SetColorfulPalette(hdc);
            }
        }
        ReleaseDC(hdc);
        break;

    case MSG_RBUTTONDOWN:
        SetColorfulPalette(HDC_SCREEN);
        SetBrushColor(HDC_SCREEN, PIXEL_yellow);
        FillCircle(HDC_SCREEN, 50, 50, 15);
        break;

    case MSG_CLOSE:
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
    JoinLayer(NAME_DEF_LAYER , "palette" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Palette demo";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = PaletteWinProc;
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

