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
** $Id: edit.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 22.1
**
** edit.c: Sample program for MiniGUI Programming Guide
**             Usage of EDIT control.
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

#ifdef _LANG_ZHCN
#include "edit_res_cn.h"
#elif defined _LANG_ZHTW
#include "edit_res_tw.h"
#else
#include "edit_res_en.h"
#endif

static DLGTEMPLATE DlgBoxInputChar =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    0, 0, 400, 230, 
    please_input_letters,
    0, 0,
    4, NULL,
    0
};

#define IDC_CHAR        100
#define IDC_CHARS       110

static CTRLDATA CtrlInputChar [] =
{ 
    {
        CTRL_STATIC,
        WS_VISIBLE,
        10, 10, 380, 18, 
        IDC_STATIC, 
        please_input_a_letter,
        0
    },
    {
        CTRL_SLEDIT,
        WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER,
        10, 40, 80, 25,
        IDC_CHAR,
        NULL,
        0
    },
    {
        CTRL_MLEDIT,
        WS_VISIBLE | WS_BORDER | WS_VSCROLL 
            | ES_BASELINE | ES_AUTOWRAP | ES_NOHIDESEL | ES_NOHIDESEL,//| WS_DISABLED,
        10, 80, 380, 70, 
        IDC_CHARS, 
        NULL,
        0 
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        170, 160, 60, 25,
        IDOK, 
        OK,
        0
    }
};

static void my_notif_proc (HWND hwnd, LINT id, int nc, DWORD add_data)
{
    char buff [256] = {0};
    if (id == IDC_CHAR && nc == EN_CHANGE) {

        GetWindowText (hwnd, buff, 4);
        SendMessage (hwnd, EM_SETCARETPOS, 0, 0);
        SendMessage (GetDlgItem (GetParent (hwnd), IDC_CHARS), MSG_CHAR, buff[0], 0L);
    }
    else if (id == IDC_CHARS && nc == EN_CHANGE) {
        GetWindowText (hwnd, buff, 255);
    }
}       

static LRESULT InputCharDialogBoxProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd;

    switch (message) {
    case MSG_INITDIALOG:
        hwnd = GetDlgItem (hDlg, IDC_CHAR);
        SetNotificationCallback (hwnd, my_notif_proc);
        SendMessage (hwnd, MSG_KEYDOWN, SCANCODE_INSERT, 0L);
        return 1;
        
    case MSG_CLOSE:
        EndDialog (hDlg, IDCANCEL);
        break;
        
    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            EndDialog (hDlg, wParam);
            break;
        }
        break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "edit" , 0 , 0);
#endif

#ifndef _MGRM_THREADS
    if (!InitVectorialFonts ()) {
        printf ("InitVectorialFonts: error.\n");
        return 1;
    }
#endif

    DlgBoxInputChar.controls = CtrlInputChar;
    DialogBoxIndirectParam (&DlgBoxInputChar, HWND_DESKTOP, InputCharDialogBoxProc, 0L);

#ifndef _MGRM_THREADS
    TermVectorialFonts ();
#endif
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

