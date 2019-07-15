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
** $Id: scrollwnd.c 741 2009-03-31 07:16:18Z weiym $
**
** scrollwnd.c: Sample program for MiniGUI Programming Guide
**      Usage of SCROLLWND control.
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

#ifdef _MGCTRL_SCROLLVIEW

#ifdef _LANG_ZHCN
#include "scrollwnd_res_cn.h"
#elif defined _LANG_ZHTW
#include "scrollwnd_res_tw.h"
#else
#include "scrollwnd_res_en.h"
#endif

#define IDC_SCROLLWND          100
#define ID_ZOOMIN              200
#define ID_ZOOMOUT             300

static HWND hScrollWnd;
static BITMAP bmp_bkgnd;
static float current_scale = 1;

static LRESULT pic_container_proc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint (hWnd);
                FillBoxWithBitmap (hdc, 0, 0, current_scale * bmp_bkgnd.bmWidth, 
                        current_scale * bmp_bkgnd.bmHeight, &bmp_bkgnd);
                EndPaint (hWnd, hdc);
                return 0;
            }
    }

    return DefaultContainerProc (hWnd, message, wParam, lParam);
}

static LRESULT ImageViewerProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

        case MSG_INITDIALOG:
            {
                hScrollWnd = GetDlgItem (hDlg, IDC_SCROLLWND);

                SendMessage (hScrollWnd, SVM_SETCONTAINERPROC, 0, (LPARAM)pic_container_proc);
                SendMessage (hScrollWnd, SVM_SETCONTRANGE, bmp_bkgnd.bmWidth, bmp_bkgnd.bmHeight);

                break;
            }

        case MSG_COMMAND:
            {
                int id = LOWORD(wParam);

                if (id == ID_ZOOMIN || id == ID_ZOOMOUT) {
                    current_scale += (id == ID_ZOOMIN) ? 0.2 : -0.2;
                    if (current_scale < 0.1)
                        current_scale = 0.1;

                    SendMessage (hScrollWnd, SVM_SETCONTRANGE, 
                            current_scale * bmp_bkgnd.bmWidth, 
                            current_scale * bmp_bkgnd.bmHeight);
                    InvalidateRect (hScrollWnd, NULL, TRUE);
                }

                break;
            }

        case MSG_CLOSE:
            EndDialog (hDlg, 0);
            return 0;

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA CtrlViewer[] =
{
    {
        CTRL_SCROLLWND,
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
         10, 10, 300, 200,
        IDC_SCROLLWND,
        CAPTION,
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        20, 220, 60, 25,
        ID_ZOOMIN, 
        BUTTON_LEFT,
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 
        220, 220, 60, 25,
        ID_ZOOMOUT, 
        BUTTON_RIGHT,
        0
    }
};

static DLGTEMPLATE DlgViewer =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 350, 285,
    CAPTION,
    0, 0,
    TABLESIZE(CtrlViewer), CtrlViewer,
    0
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "scrollwnd" , 0 , 0);
#endif

    if (LoadBitmap (HDC_SCREEN, &bmp_bkgnd, "res/bkgnd.jpg"))
         return 1;

    DialogBoxIndirectParam (&DlgViewer, HWND_DESKTOP, ImageViewerProc, 0L);

    UnloadBitmap (&bmp_bkgnd);
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

