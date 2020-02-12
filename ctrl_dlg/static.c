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
** $Id: static.c 658 2008-09-03 07:54:24Z xwyan $
**
** static.c: The Static control demo program.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
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

#define IDC_STATIC1     101
#define IDC_STATIC2     102
#define IDC_STATIC3     103
#define IDC_STATIC4     104

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif


static HWND hMainWnd = HWND_INVALID;

static LRESULT ControlTestWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
        {
            CreateWindow (CTRL_STATIC,
                            This_is_a_simple_static_control,
                            WS_CHILD | SS_NOTIFY | SS_SIMPLE | WS_VISIBLE | WS_BORDER,
                            IDC_STATIC1,
                            10, 10, 180, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            This_is_a_left_aligned_static_control,
                            WS_CHILD | SS_NOTIFY | SS_LEFT | WS_VISIBLE | WS_BORDER,
                            IDC_STATIC2,
                            10, 40, 180, 45, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            This_is_a_right_aligned_static_control,
                            WS_CHILD | SS_NOTIFY | SS_RIGHT | WS_VISIBLE | WS_BORDER,
                            IDC_STATIC3,
                            10, 90, 180, 45, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            This_is_a_center_aligned_static_control,
                            WS_CHILD | SS_NOTIFY | SS_CENTER | WS_VISIBLE | WS_BORDER,
                            IDC_STATIC4,
                            10, 140, 180, 45, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            An_icon_static_control,
                            WS_CHILD | SS_RIGHT | WS_VISIBLE,
                            IDC_STATIC,
                            250, 40, 150, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_ICON | WS_VISIBLE,
                            IDC_STATIC,
                            420, 20, 50, 50, hWnd, (DWORD)GetLargeSystemIcon (IDI_INFORMATION));

            CreateWindow (CTRL_STATIC,
                            A_bitmap_static_control,
                            WS_CHILD | SS_RIGHT | WS_VISIBLE,
                            IDC_STATIC,
                            250, 100, 150, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_ICON | WS_VISIBLE,
                            IDC_STATIC,
                            420, 80, 50, 50, hWnd, (DWORD)GetLargeSystemIcon (IDI_APPLICATION));

            CreateWindow (CTRL_STATIC,
                            A_bitmap_static_control_center,
                            WS_CHILD | SS_RIGHT | WS_VISIBLE,
                            IDC_STATIC,
                            250, 160, 150, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_ICON | SS_REALSIZEIMAGE | SS_CENTERIMAGE | WS_VISIBLE,
                            IDC_STATIC,
                            420, 140, 50, 50, hWnd, (DWORD)GetLargeSystemIcon (IDI_STOP));

            CreateWindow (CTRL_STATIC,
                            A_black_box,
                            WS_CHILD |  WS_VISIBLE,
                            IDC_STATIC,
                            10, 200, 100, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_BLACKRECT | WS_VISIBLE,
                            IDC_STATIC,
                            110, 200, 50, 20, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            A_gray_box,
                            WS_CHILD | WS_VISIBLE,
                            IDC_STATIC,
                            10, 230, 100, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_GRAYRECT | WS_VISIBLE,
                            IDC_STATIC,
                            110, 230, 50, 20, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            A_white_box,
                            WS_CHILD | WS_VISIBLE ,
                            IDC_STATIC,
                            10, 260, 100, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_WHITERECT | WS_VISIBLE,
                            IDC_STATIC,
                            110, 260, 50, 20, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            A_black_frame,
                            WS_CHILD | WS_VISIBLE,
                            IDC_STATIC,
                            170, 200, 90, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_BLACKFRAME | WS_VISIBLE,
                            IDC_STATIC,
                            260, 200, 50, 20, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            A_gray_frame,
                            WS_CHILD | WS_VISIBLE,
                            IDC_STATIC,
                            170, 230, 100, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_GRAYFRAME | WS_VISIBLE,
                            IDC_STATIC,
                            260, 230, 50, 20, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            A_white_frame,
                            WS_CHILD | WS_VISIBLE,
                            IDC_STATIC,
                            170, 260, 100, 20, hWnd, 0);
            CreateWindow (CTRL_STATIC,
                            "",
                            WS_CHILD | SS_WHITEFRAME | WS_VISIBLE,
                            IDC_STATIC,
                            260, 260, 50, 20, hWnd, 0);

            CreateWindowEx (CTRL_STATIC,
                            A_Group_Box,
                            WS_CHILD | SS_GROUPBOX | WS_VISIBLE,
                            WS_EX_TRANSPARENT,
                            IDC_STATIC,
                            350, 190, 160, 90, hWnd, 0);

            CreateWindow (CTRL_STATIC,
                            Tabs_are_expanded,
                            WS_CHILD | SS_LEFTNOWORDWRAP | WS_VISIBLE | WS_BORDER,
                            IDC_STATIC,
                            10, 290, 500, 30, hWnd, 0);

            CreateWindow (CTRL_BUTTON,
                            Close,
                            WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                            IDCANCEL,
                            450, 330, 60, 25, hWnd, 0);
        }
        break;

        case MSG_COMMAND:
            if (wParam == IDCANCEL)
                PostMessage (hWnd, MSG_CLOSE, 0, 0L);
            break;

        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
            return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = Static_controls;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 520;
    pCreateInfo->by = 390;
    pCreateInfo->iBkColor = GetWindowElementPixelEx (HWND_NULL,
            HDC_SCREEN, WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void static_demo (HWND hwnd)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hwnd;

    hMainWnd = CreateMainWindow (&CreateInfo);
}

