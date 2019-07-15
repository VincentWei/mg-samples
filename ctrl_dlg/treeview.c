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
** $Id: treeview.c 748 2009-04-13 08:28:04Z houhuihua $
**
** treeview.c: The TreeView control demo program.
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

#ifdef _MGCTRL_TREEVIEW

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

#define IDC_CTRL1     100
#define IDC_CTRL2     110
#define IDC_CTRL3     120
#define IDC_CTRL4     130
#define IDC_CTRL5     140
#define IDC_CTRL6     150

static HWND hMainWnd = HWND_INVALID;

static LRESULT ControlTestWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hTreeview, hEdit1, hEdit2;
    TVITEMINFO tvItemInfo = {0};

    switch (message) {
        case MSG_CREATE:
        {
            int i = 0;
            LRESULT item;
            hTreeview = CreateWindow (CTRL_TREEVIEW, "",
                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
                    | WS_BORDER | TVS_SORT | TVS_WITHICON,
                    IDC_CTRL1,
                    10, 20, 320, 150, hWnd, 0);
            hEdit1 = CreateWindow ("Edit", Add_a_child_to_current_node,
                    WS_CHILD | WS_VISIBLE | WS_BORDER,
                    IDC_CTRL2,
                    10, 190, 250, 25, hWnd, 0);
            hEdit2 = CreateWindow ("Edit", Find_a_node_in_treeview,
                    WS_CHILD | WS_VISIBLE | WS_BORDER,
                    IDC_CTRL3,
                    10, 230, 250, 25, hWnd, 0);
            CreateWindow ("button", Add,
                    WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                    IDC_CTRL4,
                    270, 190, 60, 24, hWnd, 0);
            CreateWindow ("button", Find,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    IDC_CTRL6,
                    270, 230, 60, 24, hWnd, 0);
            CreateWindow ("button", Del,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    IDC_CTRL5,
                    270, 270, 60, 24, hWnd, 0);

            tvItemInfo.text = "a";
            item = SendMessage (hTreeview, TVM_ADDITEM, 0, (LPARAM) &tvItemInfo);

            tvItemInfo.text = "b";
            for (i = 0; i <= 10; i++)
                SendMessage (hTreeview, TVM_ADDITEM, 0, (LPARAM) &tvItemInfo);

            tvItemInfo.text = "c";
            SendMessage (hTreeview, TVM_ADDITEM, 0, (LPARAM) &tvItemInfo);

            tvItemInfo.text = "a.a";
            SendMessage (hTreeview, TVM_ADDITEM, (WPARAM) item, (LPARAM) &tvItemInfo);
            tvItemInfo.text = "a.b";
            SendMessage (hTreeview, TVM_ADDITEM, (WPARAM) item, (LPARAM) &tvItemInfo);
        }
        break;

        case MSG_COMMAND:
        {
            int id   = LOWORD(wParam);
            //int code = HIWORD(wParam);
            char buffer[255];
            LRESULT item;

            switch (id) {
            case IDC_CTRL4:
                SendMessage (hEdit1, MSG_GETTEXT, 99, (LPARAM) buffer);
                if (buffer[0] != '\0') {
                    tvItemInfo.text = buffer;
                    item = SendMessage (hTreeview, TVM_GETSELITEM, 0, 0);
                    SendMessage (hTreeview, TVM_ADDITEM, (WPARAM)item, (LPARAM) &tvItemInfo);
                }
                break;

                case IDC_CTRL5:
                    item = SendMessage (hTreeview, TVM_GETSELITEM, 0, 0);
                    SendMessage (hTreeview, TVM_DELTREE, item, 0);
                    //SendMessage (hTreeview, TVM_DELTREE, 0, 0);
                break;

                case IDC_CTRL6:
                {
                    SendMessage (hEdit2, MSG_GETTEXT, 99, (LPARAM) buffer);
                    if (buffer[0] != '\0') {
                        item = SendMessage (hTreeview, TVM_SEARCHITEM, 0, (LPARAM) buffer);
                        if (item > 0) {
                            SendMessage (hTreeview, TVM_SETSELITEM, (WPARAM)item, 0L);
                        }
                    }
                }
                break;

                default:
                break;
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

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle    = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle  = WS_EX_IMECOMPOSE;
    pCreateInfo->spCaption  = TreeView_controls ;
    pCreateInfo->hMenu      = 0;
    pCreateInfo->hCursor    = GetSystemCursor(0);
    pCreateInfo->hIcon      = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx         = 0;
    pCreateInfo->ty         = 0;
    pCreateInfo->rx         = 360;
    pCreateInfo->by         = 340;
    pCreateInfo->iBkColor   = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData  = 0;
    pCreateInfo->hHosting   = HWND_DESKTOP;
}

void treeview_demo (HWND hwnd)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo(&CreateInfo);
    CreateInfo.hHosting = hwnd;

    hMainWnd = CreateMainWindow(&CreateInfo);
}

#else

void treeview_demo (HWND hwnd)
{
    MessageBox (hwnd, "WARNING", "Please enable TREEVIEW control support in MiniGUI.", MB_OK);
}

#endif /* _MGCTRL_TREEVIEW */

