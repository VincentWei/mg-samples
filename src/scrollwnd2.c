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
** $Id: scrollwnd2.c 741 2009-03-31 07:16:18Z weiym $
**
** scrollwnd2.c: Sample program for MiniGUI Programming Guide
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
#include "scrollwnd2_res_cn.h"
#elif defined _LANG_ZHTW
#include "scrollwnd2_res_tw.h"
#else
#include "scrollwnd2_res_en.h"
#endif

#define IDC_SCROLLWND          100
#define ID_ZOOMIN              200
#define ID_ZOOMOUT             300


static int pic_container_proc (HWND hWnd, int message, 
                WPARAM wParam, LPARAM lParam)
{
    return DefaultContainerProc (hWnd, message, wParam, lParam);
}

#define IDC_PROMPTINFO	100
#define IDC_PROGRESS  	110

static CTRLDATA controls [] =
{ 
    {
        "static",
        WS_VISIBLE | SS_SIMPLE,
        10, 10, 380, 16, 
        IDC_PROMPTINFO, 
        INIT,
        0
    },
    {
        "progressbar",
        WS_VISIBLE,
        10, 40, 380, 20,
        IDC_PROGRESS,
        NULL,
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE | WS_BORDER, 
        170, 70, 60, 25,
        IDOK, 
        BUTTON_TEXT,
        0
    }
};

static LRESULT ImageViewerProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {

    case MSG_INITDIALOG:
    {
        HWND hScrollWnd;

        hScrollWnd = GetDlgItem (hDlg, IDC_SCROLLWND);

        SendMessage (hScrollWnd, SVM_SETCONTAINERPROC, 0, 
                        (LPARAM)pic_container_proc);
        SendMessage (hScrollWnd, SVM_SETCONTRANGE, 
                        640, 480);

        SendMessage (hScrollWnd, SVM_ADDCTRLS, 3, (LPARAM)controls);
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
        "Scroll Window",
        0
    }
};

static DLGTEMPLATE DlgViewer =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 350, 280,
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

    DialogBoxIndirectParam (&DlgViewer, HWND_DESKTOP, ImageViewerProc, 0L);

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

