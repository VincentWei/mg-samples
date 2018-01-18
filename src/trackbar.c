/* 
** $Id: trackbar.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 26.1
**
** trackbar.c: Sample program for MiniGUI Programming Guide
**      Usage of TRACKBAR control.
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
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_TRACKBAR

#ifdef _LANG_ZHCN
#include "trackbar_res_cn.h"
#elif defined _LANG_ZHTW
#include "trackbar_res_tw.h"
#else
#include "trackbar_res_en.h"
#endif

static int radius = 0;
static RECT rcCircle = {0, 60, 240, 300};
static void my_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    if (nc == TBN_CHANGE) {
        radius = SendMessage (hwnd, TBM_GETPOS, 0, 0);
        InvalidateRect (GetParent (hwnd), &rcCircle, TRUE);
    }
}

static LRESULT TrackBarWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd;
    switch (message) {
    case MSG_CREATE:
        hwnd = CreateWindow (CTRL_TRACKBAR, 
                          "", 
                          WS_VISIBLE | TBS_NOTIFY, 
                          100, 
                          10, 10, 210, 50, hWnd, 0);

        SendMessage (hwnd, TBM_SETRANGE, 0, 80);
        SendMessage (hwnd, TBM_SETLINESIZE, 1, 0);
        SendMessage (hwnd, TBM_SETPAGESIZE, 10, 0);
        SendMessage (hwnd, TBM_SETTICKFREQ, 10, 0);
        SendMessage (hwnd, TBM_SETPOS, radius, 0);

        SetNotificationCallback (hwnd, my_notif_proc);
        SetFocus (hwnd);
        break;

    case MSG_PAINT:
    {
        HDC hdc = BeginPaint (hWnd);

        ClipRectIntersect (hdc, &rcCircle);
        Circle (hdc, 100, 180, radius);

        EndPaint (hWnd, hdc);
        return 0;
    }

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
    JoinLayer(NAME_DEF_LAYER , "trackbar" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CAPTION;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = TrackBarWinProc;
    CreateInfo.lx = 0; 
    CreateInfo.ty = 0;
    CreateInfo.rx = 240;
    CreateInfo.by = 320;
    CreateInfo.iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    CreateInfo.dwAddData = 0;
    CreateInfo.dwReserved = 0;
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

#else /* _MGCTRL_TRACKBAR */

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for TRACKBAR control in MiniGUI.\n");
    return 0;
}

#endif /* _MGCTRL_TRACKBAR */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
