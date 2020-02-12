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
** $Id: mycontrol.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 3.1
**
** mycontrol.c: Sample program for MiniGUI Programming Guide
**      Use my own control to print "Hello, world!".
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define MY_CTRL_NAME "mycontrol"

static LRESULT MyControlProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message) {
    case MSG_PAINT:
        /* Output hello, world */
        hdc = BeginPaint (hwnd);
        TextOut (hdc, 0, 0, "Hello, world! - from my control.");
        EndPaint (hwnd, hdc);
        return 0;
    }

    return DefaultControlProc (hwnd, message, wParam, lParam);
}

static BOOL RegisterMyControl (void)
{
    WNDCLASS MyClass;

    MyClass.spClassName = MY_CTRL_NAME;
    MyClass.dwStyle     = WS_NONE;
    MyClass.dwExStyle   = WS_EX_NONE;
    MyClass.hCursor     = GetSystemCursor (IDC_ARROW);
#ifdef _MGSCHEMA_COMPOSITING
    MyClass.dwBkColor   = RGBA_lightwhite;
#else
    MyClass.iBkColor    = PIXEL_lightwhite;
#endif
    MyClass.WinProc     = MyControlProc;

    return RegisterWindowClass (&MyClass);
}

static void UnregisterMyControl (void)
{
    UnregisterWindowClass (MY_CTRL_NAME);
}

static LRESULT HelloWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
            CreateWindow (MY_CTRL_NAME, "", WS_VISIBLE, IDC_STATIC, 85, 100, 400, 20, hWnd, 0);
            return 0;

        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            return 0;

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

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "mycontrol" , 0 , 0);
#endif

    RegisterMyControl ();

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Hello, world";
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
    UnregisterMyControl ();

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

