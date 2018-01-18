/* 
** $Id: colorupdate.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 6.1
**
** colorupdate.c: Sample program for MiniGUI Programming Guide
**      Demo for control color update.
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_STATIC1         100
#define IDC_STATIC2         101
#define IDC_EDIT            102
#define IDC_BUTTON_TEST     103
#define IDC_BUTTON_CHANGE   104

static DLGTEMPLATE DlgChangeColor = 
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    120, 100, 250, 250,
    "Change The Color",
    0, 0,
    5, NULL,
    0
};

static CTRLDATA CtrlColorUpdate [] =
{
    {
        "static",
        WS_VISIBLE | WS_BORDER | SS_SIMPLE,
        40, 30, 60, 30,
        IDC_STATIC1,
        "static1",
        0
    },
    {
        "static",
        WS_VISIBLE | WS_BORDER | SS_SIMPLE,
        40, 80, 60, 30,
        IDC_STATIC2,
        "static2",
        0
    },
    {
        "sledit",
        WS_VISIBLE | WS_BORDER,
        150, 30, 60, 30,
        IDC_EDIT,
        "edit",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON,
        150, 80, 60, 30,
        IDC_BUTTON_TEST,
        "button",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON,
        //40, 130, 60, 30,
        80, 150, 100, 30,
        IDC_BUTTON_CHANGE,
        "change",
        0
    },
    /*
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON,
        150, 130, 60, 30,
        IDOK,
        "OK",
        0
    }
    */
};

static void getRandomPixel (Uint32 *p)
{
    int r, g, b;

    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;

    *p = RGB2Pixel (HDC_SCREEN, r, g, b);
}

static LRESULT InitDialogBoxProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    Uint32 pixel;
    int i;

    switch (message) {
    case MSG_INITDIALOG:
        return 1;
    case MSG_COMMAND:
        switch (wParam) {
        case IDC_BUTTON_CHANGE:
            for (i = 0; i < 4; i++) {
                getRandomPixel (&pixel);
                SetWindowElementColorEx(GetDlgItem(hDlg, IDC_STATIC1 + i), FGC_CONTROL_NORMAL, pixel);
                getRandomPixel (&pixel);
                SetWindowElementColorEx(GetDlgItem(hDlg, IDC_STATIC1 + i), FGC_CONTROL_DISABLED, pixel);
                getRandomPixel (&pixel);
                SetWindowBkColor(GetDlgItem(hDlg, IDC_STATIC1 + i), pixel);
                getRandomPixel (&pixel);
                SetWindowBkColor(hDlg, pixel);
                getRandomPixel (&pixel);
                SetWindowElementColorEx(hDlg, BKC_CAPTION_ACTIVED, pixel);
            }
            UpdateWindow(hDlg, TRUE);
            break;
        }
        return 1;
    case MSG_CLOSE:
        EndDialog(hDlg, wParam);
        break;
    }
   

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static void InitialDialogBox (HWND hWnd)
{
    DlgChangeColor.controls = CtrlColorUpdate;

    DialogBoxIndirectParam (&DlgChangeColor, hWnd, InitDialogBoxProc, 0L);
}


int MiniGUIMain (int argc, const char *argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "colorupdate" , 0 , 0);
#endif

    InitialDialogBox (HWND_DESKTOP);
    
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
