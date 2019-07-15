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
** $Id: painter.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 15.1
**
** painter.c: Sample program for MiniGUI Programming Guide
**      Painting by using mouse.
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

#ifdef _LANG_ZHCN
#include "painter_res_cn.h"
#elif defined _LANG_ZHTW
#include "painter_res_tw.h"
#else
#include "painter_res_en.h"
#endif

static LRESULT PainterWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static BOOL bdraw = FALSE;
    static int pre_x, pre_y;

    switch (message) {
    case MSG_LBUTTONDOWN:
        SetCapture(hWnd);
        bdraw = TRUE;
        pre_x = LOWORD (lParam);
        pre_y = HIWORD (lParam);
        break;

    case MSG_MOUSEMOVE:
    {
        int x = LOWORD (lParam);
        int y = HIWORD (lParam);

        if (bdraw) {
            ScreenToClient(hWnd, &x, &y);
            hdc = GetClientDC(hWnd);
            SetPenColor(hdc, PIXEL_red);
            MoveTo(hdc, pre_x, pre_y);
            LineTo(hdc, x, y);
            ReleaseDC(hdc);
            pre_x = x;
            pre_y = y;
        }
        break;
    }

    case MSG_LBUTTONUP:
    {
        int x = LOWORD (lParam);
        int y = HIWORD (lParam);

        if (bdraw) {
            ScreenToClient(hWnd, &x, &y);
            hdc = GetClientDC(hWnd);
            SetPixel (hdc, x, y, PIXEL_red);
            ReleaseDC(hdc);
            bdraw = FALSE;
            ReleaseCapture();
        }
        break;
    }

    case MSG_RBUTTONDOWN:
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case MSG_CLOSE:
        DestroyAllControls (hWnd);
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "painter" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CAPTION;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = PainterWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 240;
    CreateInfo.by = 320;
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

