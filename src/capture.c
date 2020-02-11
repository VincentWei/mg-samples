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
** $Id: capture.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 9.2
**
** capture.c: Sample program for MiniGUI Programming Guide
**      Demo of using mouse capture
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
#include "capture_res_cn.h"
#elif defined _LANG_ZHTW
#include "capture_res_tw.h"
#else
#include "capture_res_en.h"
#endif

#define IDC_MYBUTTON    100

/* a simple button control */
static LRESULT MybuttonWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static int status = 0;

    switch (message) {
    case MSG_LBUTTONDOWN:
        status = 1;
        SetCapture (hWnd);
        InvalidateRect (hWnd, NULL, TRUE);
    break;

    case MSG_LBUTTONUP:
        if (GetCapture() != hWnd)
            break;
        status = 0;
        ReleaseCapture ();
        InvalidateRect (hWnd, NULL, TRUE);
    break;

    case MSG_PAINT:
        hdc = BeginPaint (hWnd);
        if (status) {
            SetBkMode(hdc, BM_TRANSPARENT);
            TextOut(hdc,10, 0, CP_ST_PRESSED);
        }
        EndPaint(hWnd, hdc);
        return 0;

    case MSG_DESTROY:
        return 0;
    }

    return DefaultControlProc (hWnd, message, wParam, lParam);
}

BOOL RegisterMybutton (void)
{
    WNDCLASS WndClass;

    WndClass.spClassName = "mybutton";
    WndClass.dwStyle     = 0;
    WndClass.dwExStyle   = 0;
    WndClass.hCursor     = GetSystemCursor(0);
#ifdef _MGSCHEMA_COMPOSITING
    WndClass.dwBkColor   = COLOR_lightgray;
#else
    WndClass.iBkColor    = PIXEL_lightgray;
#endif
    WndClass.WinProc     = MybuttonWindowProc;

    return RegisterWindowClass (&WndClass);
}

/* main windoww proc */
static LRESULT CaptureWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        RegisterMybutton();
        CreateWindow ("mybutton", "", WS_VISIBLE | WS_CHILD, IDC_MYBUTTON, 
                110, 80, 70, 20, hWnd, 0);
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
    JoinLayer(NAME_DEF_LAYER , "capture" , 0 , 0);
#endif
    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CP_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = CaptureWinProc;
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

