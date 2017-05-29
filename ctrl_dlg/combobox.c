/*
** $Id: combobox.c 662 2008-09-05 02:50:46Z houhuihua $
**
** combobox.c: The ComboBox control demo program.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2001/11/01
*/

/*
**  This source is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public
**  License as published by the Free Software Foundation; either
**  version 2 of the License, or (at your option) any later version.
**
**  This software is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public
**  License along with this library; if not, write to the Free
**  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
**  MA 02111-1307, USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __NOUNIX__
#include <unistd.h>
#include <dirent.h>
#endif

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_BOX1        100
#define IDC_BOX2        200
#define IDC_BOX3        300
#define IDC_BOX4        400
#define IDC_BOX5        500

#define IDC_SELECTED    600

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

static HWND hMainWnd = HWND_INVALID;

static void create_combobox (HWND parent)
{
    int i;
    HWND hwnd1, hwnd2, hwnd3, hwnd4, hwnd5;
    char temp[20];
    DIR*    dir;
    struct  dirent* dir_ent;

    hwnd1 = CreateWindow (CTRL_COMBOBOX, 
            "combobox",
            WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOTIFY | CBS_READONLY,
            IDC_BOX1,
            10, 10, 180, 24,
            parent, 100);

    hwnd2 = CreateWindowEx (CTRL_COMBOBOX,
            "combobox",
#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 5, 3)
            WS_VISIBLE | CBS_SPINLIST | CBS_SPINARROW_LEFTRIGHT | WS_TABSTOP | CBS_SORT,
#else
            WS_VISIBLE | CBS_SPINLIST | CBS_EDITNOBORDER | WS_TABSTOP | CBS_SORT,
#endif
            WS_EX_NONE,
            IDC_BOX2,
            10, 50, 180, 24,
            parent, 0);
    hwnd3 = CreateWindow (CTRL_COMBOBOX,
            "0",
            WS_VISIBLE | CBS_AUTOSPIN | WS_TABSTOP,
            IDC_BOX3,
            200, 90, 180, 24,
            parent, 0);

    hwnd4 = CreateWindow (CTRL_COMBOBOX,
            "0",
            WS_VISIBLE | CBS_SIMPLE | CBS_SORT | WS_TABSTOP,
            IDC_BOX4,
            10, 90, 180, 24,
            parent, 100);

    hwnd5 = CreateWindowEx (CTRL_COMBOBOX,
            "combobox",
#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 5, 3)
            WS_VISIBLE | CBS_SPINLIST | CBS_SPINARROW_TOPBOTTOM | CBS_EDITNOBORDER | WS_TABSTOP | CBS_SORT,
#else
            WS_VISIBLE | CBS_SPINLIST | CBS_EDITNOBORDER | WS_TABSTOP | CBS_SORT,
#endif
            WS_EX_NONE,
            IDC_BOX2,
            200, 40, 180, 40,
            parent, 0);

    dir = opendir ("./");
    while ( (dir_ent = readdir ( dir )) != NULL ) {
        SendMessage (hwnd1, CB_ADDSTRING, 0, (LPARAM)dir_ent->d_name);
        SendMessage (hwnd4, CB_ADDSTRING, 0, (LPARAM)dir_ent->d_name);
    }
    closedir (dir);

    for (i = 0; i < 10; i++) { 
        sprintf (temp, "Welcome: %03d", i);
        SendMessage (hwnd2, CB_ADDSTRING, 0, (LPARAM)temp);
        SendMessage (hwnd5, CB_ADDSTRING, 0, (LPARAM)temp);
    }

}

static int ControlTestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
              create_combobox (hWnd);

              CreateWindow (CTRL_STATIC,
                        selected_, 
                        WS_CHILD | WS_VISIBLE | SS_SIMPLE, 
                        IDC_SELECTED, 
                        210, 10, 200, 25, hWnd, 0);

              CreateWindow (CTRL_BUTTON,
                        Close, 
                        WS_CHILD | WS_VISIBLE, 
                        IDCANCEL, 
                        320, 190, 60, 25, hWnd, 0);

        break;

        case MSG_COMMAND:
        {
            int id   = LOWORD(wParam);
            int code = HIWORD(wParam);
            char str [NAME_MAX + 20], file [NAME_MAX + 1];
            int selected; 

            if (wParam == IDCANCEL) {
                PostMessage (hWnd, MSG_CLOSE, 0, 0);
                break;
            }

            switch (id) {
            case IDC_BOX1:
                if (code != CBN_SELCHANGE)
                    break;

                selected = SendDlgItemMessage (hWnd, IDC_BOX1, CB_GETCURSEL,0,0);
                if (selected >= 0) {
                    SendDlgItemMessage (hWnd, IDC_BOX1, CB_GETLBTEXT, selected, (LPARAM)file);
                    if (strlen (file)) {
                        sprintf (str, "Selected: %s", file);
                        SetDlgItemText (hWnd, IDC_SELECTED, str);
                    }
                }
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
            MainWindowThreadCleanup (hWnd);
            return 0;
    }

    return  DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = "ComboBox controls";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(IDC_ARROW);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 400;
    pCreateInfo->by = 260;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY); 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void combobox_demo (HWND hwnd)
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


