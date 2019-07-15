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
** $Id: drawicon.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 10.1
**
** drawicon.c: Sample program for MiniGUI Programming Guide
**      Demo of loading and drawing icon
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
#include "drawicon_res_cn.h"
#elif defined _LANG_ZHTW
#include "drawicon_res_tw.h"
#else
#include "drawicon_res_en.h"
#endif

static LRESULT DrawiconWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HICON myicon_small, myicon_large;
    HDC hdc;

    switch (message) {
    case MSG_CREATE:
        myicon_small = LoadIconFromFile(HDC_SCREEN, "myicon.ico", 0);
        if (myicon_small == 0)
            fprintf (stderr, "Load icon file failure!");
        myicon_large = LoadIconFromFile(HDC_SCREEN, "myicon.ico", 1);
        if (myicon_large == 0)
            fprintf (stderr, "Load icon file failure!");
        break;

    case MSG_PAINT:
        hdc = BeginPaint(hWnd);
        if (myicon_small != 0)
            DrawIcon(hdc, 10, 10, 0, 0, myicon_small);
        if (myicon_large != 0)
            DrawIcon(hdc, 60, 60, 0, 0, myicon_large);
        EndPaint(hWnd, hdc);
        return 0;

    case MSG_CLOSE:
        DestroyIcon(myicon_small);
        DestroyIcon(myicon_large);
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
    JoinLayer(NAME_DEF_LAYER , "drawicon" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = DI_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DrawiconWinProc;
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

