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
** $Id: tiny_listbox2.c 406 2008-02-01 03:12:21Z zhounuohua $
**
** tiny_listbox2.c:
**
** Copyright (C) 2007 ~ 2017 FMSoft (http://www.fmsoft.cn).
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define MY_IDC_LISTBOX2             301
extern HWND hwnd_my;

static HICON icon1, icon2, icon3, icon4, icon5, icon6, icon7, icon8, icon9, icon10, icon11;

static WNDPROC old_callback;
static LRESULT notif_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_COMMAND:
            switch (wParam) {
                case IDOK:
                    SendMessage (GetParent(hwnd), MSG_CLOSE, 0, 0);
                    ShowWindow (hwnd_my, SW_SHOWNORMAL);
                    return 0;
                case IDCANCEL:
                    SendMessage (GetParent(hwnd), MSG_CLOSE, 0, 0);
                    ShowWindow (hwnd_my, SW_SHOWNORMAL);
                    return 0;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return old_callback(hwnd, message, wParam, lParam);
}

static CTRLDATA CtrlYourTaste[] =
{ 
    {
        CTRL_LISTBOX,
        WS_VISIBLE | WS_CHILD | LBS_USEICON | WS_VSCROLL, 
        0, 0, 240, 320,
        MY_IDC_LISTBOX2,
        "Mutimedia",
        0,
        WS_EX_TRANSPARENT |
        WS_EX_LFRDR_CUSTOM1 | WS_EX_LFRDR_CUSTOM4
            //| WS_EX_LFRDR_CUSTOM3
            //| WS_EX_LFRDR_CUSTOM2,

            //| WS_EX_NOCLOSEBOX,
            ,
        "tiny",
        0
    }
};

static DLGTEMPLATE DlgYourTaste =
{
    WS_VISIBLE,
    WS_EX_NONE,
    0, 0, 240, 320,
    "Tiny test",
    0, 0,
    TABLESIZE(CtrlYourTaste), CtrlYourTaste,
    0
};

static LRESULT DialogBoxProc2 (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_INITDIALOG:
            {
                LISTBOXITEMINFO lbii;
                lbii.hIcon = icon1;
                lbii.string = "Read Card";
            
                lbii.cmFlag = LBS_USEICON;
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_SETITEMHEIGHT, 0, 32);

                lbii.hIcon = icon2;
                lbii.string = "Clock";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon3;
                lbii.string = "Digital Cenima";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon4;
                lbii.string = "Dictionary";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon5;
                lbii.string = "Receive Email";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon6;
                lbii.string = "Image View";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon7;
                lbii.string = "Recorde";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon8;
                lbii.string = "Month Calendar";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon9;
                lbii.string = "Music";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon10;
                lbii.string = "Note Book";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon11;
                lbii.string = "Mobile Set";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX2), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                old_callback = SetWindowCallbackProc (GetDlgItem (hDlg, MY_IDC_LISTBOX2), notif_proc);
                break;
            }
        case MSG_CLOSE:
            EndDialog (hDlg, 0);
            break;
        default:
            break;
    }
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int tiny_listbox2 (HWND hwnd)
{
    icon1 = LoadIconFromFile (HDC_SCREEN, "res/card.ico", 1);
    icon2 = LoadIconFromFile (HDC_SCREEN, "res/clock.ico", 1);
    icon3 = LoadIconFromFile (HDC_SCREEN, "res/dc.ico", 1);
    icon4 = LoadIconFromFile (HDC_SCREEN, "res/dict.ico", 1);
    icon5 = LoadIconFromFile (HDC_SCREEN, "res/email.ico", 1);
    icon6 = LoadIconFromFile (HDC_SCREEN, "res/ime_view.ico", 1);
    icon7 = LoadIconFromFile (HDC_SCREEN, "res/mac_phone.ico", 1);
    icon8 = LoadIconFromFile (HDC_SCREEN, "res/month_cal.ico", 1);
    icon9 = LoadIconFromFile (HDC_SCREEN, "res/music.ico", 1);
    icon10 = LoadIconFromFile (HDC_SCREEN, "res/notebook.ico", 1);
    icon11 = LoadIconFromFile (HDC_SCREEN, "res/set.ico", 1);

    DialogBoxIndirectParamEx (&DlgYourTaste, hwnd, DialogBoxProc2, 0L,
           "tiny", 0, 0, 0 );

    return 0;
}
