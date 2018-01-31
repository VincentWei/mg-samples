/*
** $Id: button.c 553 2008-03-03 07:22:56Z malin $
**
** button.c: The Button control demo program.
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
**
** Create date: 2001/11/01
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_BUTTON          100
#define IDC_CHECKBOX        200
#define IDC_RADIOBUTTON     300


#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

static HWND hMainWnd = HWND_INVALID;

#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 2, 6)
static void my_notif_proc (HWND hwnd, LINT id, int nc, DWORD add_data)
{
    //printf ("From my_notif_proc: ID: %d, Code %d\n", id, nc);
}

#endif

static LRESULT ControlTestWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd;
    switch (message) {
        case MSG_CREATE:
            CreateWindow (CTRL_BUTTON,
                          Push_Button, 
                          WS_CHILD | BS_PUSHBUTTON | BS_CHECKED | WS_VISIBLE, 
                          IDC_BUTTON, 
                          10, 10, 80, 40, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Multiple_Lines_Push_Button, 
                          BS_PUSHBUTTON | BS_MULTLINE | WS_VISIBLE, 
                          IDC_BUTTON + 1, 
                          110, 10, 80, 40, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Normal_check_box, 
                          BS_CHECKBOX | BS_CHECKED | WS_VISIBLE, 
                          IDC_BUTTON + 2, 
                          220, 10, 150, 40, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Normal_radio_button, 
                          BS_RADIOBUTTON | WS_VISIBLE, 
                          IDC_BUTTON + 3, 
                          390, 10, 150, 40, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Auto_3_state_check_box, 
                          WS_CHILD | BS_AUTO3STATE | WS_VISIBLE, 
                          IDC_CHECKBOX, 
                          10, 60, 150, 30, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Auto_check_box_on_left, 
                          WS_CHILD | BS_AUTOCHECKBOX | BS_LEFTTEXT | BS_RIGHT | WS_VISIBLE, 
                          IDC_CHECKBOX + 1, 
                          190, 60, 150, 30, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Push_like_check_box, 
                          WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_VISIBLE, 
                          IDC_CHECKBOX + 2, 
                          390, 65, 110, 20, hWnd, 0);

            CreateWindow (CTRL_STATIC, 
                          A_Group_Box, 
                          WS_CHILD | SS_GROUPBOX | WS_VISIBLE,
                          IDC_STATIC, 
                          10, 100, 180, 140, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Auto_Radio_Button1, 
                          WS_CHILD | BS_AUTORADIOBUTTON | WS_VISIBLE | WS_GROUP,
                          IDC_RADIOBUTTON,
                          20, 120, 130, 30, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Auto_Radio_Button2, 
                          WS_CHILD | BS_AUTORADIOBUTTON | WS_VISIBLE,
                          IDC_RADIOBUTTON + 1,
                          20, 160, 130, 30, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Auto_Radio_Button3, 
                          WS_CHILD | BS_AUTORADIOBUTTON | WS_VISIBLE,
                          IDC_RADIOBUTTON + 2,
                          20, 200, 130, 30, hWnd, 0);

            CreateWindow (CTRL_STATIC, 
                          A_Group_Box, 
                          WS_CHILD | SS_GROUPBOX | WS_VISIBLE,
                          IDC_STATIC, 
                          220, 100, 170, 140, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Auto_Radio_Button1, 
                          WS_CHILD | BS_AUTORADIOBUTTON | BS_LEFTTEXT | BS_RIGHT | WS_VISIBLE | WS_GROUP,
                          IDC_RADIOBUTTON + 3,
                          230, 120, 140, 30, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Auto_Radio_Button2, 
                          WS_CHILD | BS_AUTORADIOBUTTON | BS_LEFTTEXT | BS_RIGHT | WS_VISIBLE,
                          IDC_RADIOBUTTON + 4,
                          230, 160, 140, 30, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Auto_Radio_Button3, 
                          WS_CHILD | BS_AUTORADIOBUTTON | BS_LEFTTEXT | BS_RIGHT | WS_VISIBLE,
                          IDC_RADIOBUTTON + 5,
                          230, 200, 140, 30, hWnd, 0);

            CreateWindow (CTRL_BUTTON, 
                          Push_like_Radio1, 
                          WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | WS_VISIBLE | WS_GROUP,
                          IDC_RADIOBUTTON + 3,
                          10, 260, 100, 30, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Push_like_Radio2, 
                          WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | WS_VISIBLE,
                          IDC_RADIOBUTTON + 4,
                          120, 260, 100, 30, hWnd, 0);
            CreateWindow (CTRL_BUTTON, 
                          Push_like_Radio3, 
                          WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | WS_VISIBLE,
                          IDC_RADIOBUTTON + 5,
                          230, 260, 100, 30, hWnd, 0);

            hwnd = CreateWindow (CTRL_BUTTON, 
                          Close, 
                          WS_CHILD | BS_PUSHBUTTON | BS_ICON |BS_REALSIZEIMAGE | BS_NOTIFY | WS_VISIBLE, 
                          IDC_BUTTON + 4, 
                          340, 260, 100, 30, hWnd, (DWORD) GetLargeSystemIcon (IDI_APPLICATION));

            CreateWindow (CTRL_BUTTON, 
                          Close, 
                          WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 
                          IDCANCEL, 
                          450, 260, 60, 30, hWnd, 0);


#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 2, 6)
            SetNotificationCallback (hwnd, my_notif_proc);
#endif

        break;

        case MSG_COMMAND:
        {
            if (wParam == IDCANCEL) {
                PostMessage (hWnd, MSG_CLOSE, 0, 0);
            }
        }
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

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = Button_controls;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 520;
    pCreateInfo->by = 350;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void button_demo (HWND hwnd)
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

