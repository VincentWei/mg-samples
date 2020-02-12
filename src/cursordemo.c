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
** $Id: cursordemo.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 10.1
**
** cursordemo.c: Sample program for MiniGUI Programming Guide
**      Demo of using cursor
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
#include "cursordemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "cursordemo_res_tw.h"
#else
#include "cursordemo_res_en.h"
#endif

#define IDC_TRAP    100

static HWND hTrapWin, hMainWnd;
static RECT rc;

static LRESULT TrapwindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL bTrapped = FALSE;

    switch (message) {
    case MSG_KEYDOWN:
        if (bTrapped) {
            ClipCursor (NULL);
            bTrapped = FALSE;
        }
        break;

    case MSG_MOUSEMOVE:
        if (!bTrapped) {
            GetWindowRect(hWnd, &rc);
            ClientToScreen(hMainWnd, &rc.left, &rc.top);
            ClientToScreen(hMainWnd, &rc.right, &rc.bottom);
            ClipCursor(&rc);
            bTrapped = TRUE;
        }
        break;
    }

    return DefaultControlProc(hWnd, message, wParam, lParam);
}

BOOL RegisterTrapwindow (void)
{
    WNDCLASS WndClass;

    WndClass.spClassName = "trap";
    WndClass.dwStyle     = 0;
    WndClass.dwExStyle   = 0;
    WndClass.hCursor     = GetSystemCursor(IDC_HAND_POINT);
#ifdef _MGSCHEMA_COMPOSITING
    WndClass.dwBkColor   = RGBA_black;
#else
    WndClass.iBkColor    = PIXEL_black;
#endif
    WndClass.WinProc     = TrapwindowProc;

    return RegisterWindowClass (&WndClass);
}

static LRESULT CursordemoWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int count = 0;
    switch (message) {
    case MSG_CREATE:
        RegisterTrapwindow();
        hTrapWin = CreateWindow("trap", "", WS_VISIBLE | WS_CHILD, IDC_TRAP, 
                10, 10, 100, 100, hWnd, 0);
        break;

    case MSG_LBUTTONDOWN:
        if (count >= 0)
            count = ShowCursor(FALSE);
        break;

    case MSG_RBUTTONDOWN:
        if (count < 0)
            count = ShowCursor(TRUE);
        break;

    case MSG_SETCURSOR:
        SetCursor (GetSystemCursor (IDC_IBEAM));
        return 0;

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
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "cursordemo" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CSD_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = CursordemoWinProc;
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

