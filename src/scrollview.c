/*
** $Id: scrollview.c 741 2009-03-31 07:16:18Z weiym $
**
** scrollview.c: Sample program for MiniGUI Programming Guide
**      Usage of SCROLLVIEW control.
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
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_SCROLLVIEW

#ifdef _LANG_ZHCN
#include "scrollview_res_cn.h"
#elif defined _LANG_ZHTW
#include "scrollview_res_tw.h"
#else
#include "scrollview_res_en.h"
#endif

#define IDC_SCROLLVIEW    100
#define IDC_BT          200
#define IDC_BT2         300
#define IDC_BT3         400
#define IDC_BT4         500

static HWND hScrollView;

static void myDrawItem (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const char *name = (const char*)scrollview_get_item_adddata (hsvi);

    SetBkMode (hdc, BM_TRANSPARENT);
    SetTextColor (hdc, PIXEL_black);

    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        SetBrushColor (hdc, PIXEL_blue);
        FillBox (hdc, rcDraw->left+1, rcDraw->top+1, RECTWP(rcDraw)-2, RECTHP(rcDraw)-1);
        SetBkColor (hdc, PIXEL_blue);
        SetTextColor (hdc, PIXEL_lightwhite);
    }

    Rectangle (hdc, rcDraw->left, rcDraw->top, rcDraw->right - 1, rcDraw->bottom);
    TextOut (hdc, rcDraw->left + 3, rcDraw->top + 2, name);
}

static int myCmpItem (HSVITEM hsvi1, HSVITEM hsvi2)
{
    const char *name1 = (const char*)scrollview_get_item_adddata (hsvi1);
    const char *name2 = (const char*)scrollview_get_item_adddata (hsvi2);

    return strcmp (name1, name2);
}

static LRESULT BookProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

    case MSG_INITDIALOG:
    {
        SVITEMINFO svii;
        static int i = 0;

        hScrollView = GetDlgItem (hDlg, IDC_SCROLLVIEW);
        SetWindowBkColor (hScrollView, PIXEL_lightwhite);

        SendMessage (hScrollView, SVM_SETITEMCMP, 0, (LPARAM)myCmpItem);
        SendMessage (hScrollView, SVM_SETITEMDRAW, 0, (LPARAM)myDrawItem);

        for (i = 0; i < TABLESIZE(people); i++) {
            svii.nItemHeight = 32;
            svii.addData = (DWORD)people[i];
            svii.nItem = i;
            SendMessage (hScrollView, SVM_ADDITEM, 0, (LPARAM)&svii);
            SendMessage (hScrollView, SVM_SETITEMADDDATA, i, (DWORD)people[i]);
        }
        break;
    }

    case MSG_COMMAND:
    {
        int id = LOWORD (wParam);
        int code = HIWORD (wParam);

        switch (id) {
        case IDC_SCROLLVIEW:
            if (code == SVN_CLICKED) {
                int sel;
                sel = SendMessage (hScrollView, SVM_GETCURSEL, 0, 0);
                SendMessage (hScrollView, SVM_GETITEMADDDATA, sel, 0);
            }
            break;

        }
        break;
    }

    case MSG_KEYDOWN:
        if (wParam == SCANCODE_REMOVE) {
            int idx = SendMessage (hScrollView, SVM_GETCURSEL, 0, 0);
            SendMessage (hScrollView, SVM_DELITEM, idx, 0);
            SendMessage (hScrollView, SVM_SETCURSEL, idx, 0);
            return 0;
        }
        break;

    case MSG_CLOSE:
    {
        EndDialog (hDlg, 0);
        return 0;
    }

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA CtrlBook[] =
{
    {
        "scrollview",
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                SVS_AUTOSORT,
         10, 10, 320, 150,
        IDC_SCROLLVIEW,
        "",
        0
    },
};

static DLGTEMPLATE DlgBook =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 350, 200,
    CAPTION,
    0, 0,
    TABLESIZE(CtrlBook), NULL,
    0
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "scrollview" , 0 , 0);
#endif

    DlgBook.controls = CtrlBook;
    
    DialogBoxIndirectParam (&DlgBook, HWND_DESKTOP, BookProc, 0L);

    return 0;
}

#else /* _MGCTRL_SCROLLVIEW */

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for SCROLLVIEW control in MiniGUI.\n");
    return 0;
}

#endif /* _MGCTRL_SCROLLVIEW */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

