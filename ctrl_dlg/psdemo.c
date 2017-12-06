/*
** $Id: psdemo.c 554 2008-03-03 09:09:48Z malin $
**
** psdemo.c: The PropertySheet demo program.
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

#ifdef _MGCTRL_PROPSHEET

#include "dlgdemo.h"
#include "resouce.h"

static int 
PageProc1 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
    case MSG_INITPAGE:
        break;
    case MSG_SHOWPAGE:
        return 1;

    case MSG_SHEETCMD:
        if (wParam == IDOK) {
            char buffer [20];
            GetDlgItemText (hDlg, IDC_EDIT1, buffer, 18);
            buffer [18] = '\0';

            if (buffer [0] == '\0') {
                MessageBox (hDlg, 
                            "Please input something in the first edit box.", 
                            "Warning!", 
                            MB_OK | MB_ICONEXCLAMATION | MB_BASEDONPARENT);
                return -1;
            }
        }
        return 0;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
        break;
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int 
PageProc2 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITPAGE:
        break;

    case MSG_SHOWPAGE:
        return 1;
        
    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
        break;
        
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int 
PageProc3 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    int i;
    char temp [20];

    switch (message) {
    case MSG_INITPAGE:
        for (i = 0; i < 20; i++) { 
            sprintf (temp, "%d-Welcome", i);
            SendDlgItemMessage (hDlg, IDC_BOX1, CB_ADDSTRING, 0, (LPARAM)temp) ;
            SendDlgItemMessage (hDlg, IDC_BOX2, CB_ADDSTRING, 0, (LPARAM)temp) ;
            SendDlgItemMessage (hDlg, IDC_BOX4, CB_ADDSTRING, 0, (LPARAM)temp) ;
        } 
        break;

    case MSG_SHOWPAGE:
        return 1;

    case MSG_GETDLGCODE:
        return DLGC_WANTARROWS;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
        break;
        
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int 
PageProc4 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITPAGE:
        break;
    case MSG_SHOWPAGE:
        return 1;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int PropSheetProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
        {
            HWND pshwnd = GetDlgItem (hDlg, IDC_PROPSHEET);

            DlgStructParams.controls = CtrlStructParams;
            SendMessage (pshwnd, PSM_ADDPAGE,
                            (WPARAM)&DlgStructParams, (LPARAM)PageProc1);
            
            DlgPassword.controls = CtrlPassword;
            SendMessage ( pshwnd, PSM_ADDPAGE, 
                            (WPARAM)&DlgPassword,(LPARAM) PageProc2);
            
            DlgStartupMode.controls = CtrlStartupMode;
            SendMessage ( pshwnd, PSM_ADDPAGE, 
                            (WPARAM)&DlgStartupMode,(LPARAM)PageProc3);

            DlgInitProgress.controls = CtrlInitProgress;
            SendMessage ( pshwnd, PSM_ADDPAGE, 
                            (WPARAM)&DlgInitProgress, (LPARAM) PageProc4);

            break;
        }

        case MSG_COMMAND:
        switch (wParam) 
        {
            case IDC_APPLY:
            break;

            case IDOK:
            {
                int index = SendDlgItemMessage (hDlg, IDC_PROPSHEET, 
                                PSM_SHEETCMD, IDOK, 0);
                if (index) {
                    SendDlgItemMessage (hDlg, IDC_PROPSHEET, 
                                    PSM_SETACTIVEINDEX, index - 1, 0);
                }
                else 
                    EndDialog (hDlg, wParam);

                break;
            }
            case IDCANCEL:
                EndDialog (hDlg, wParam);
            break;
        }
        break;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

void testPropertySheet (HWND hWnd)
{
    DlgPropertySheet.controls = CtrlPropertySheet;
    
    DialogBoxIndirectParam (&DlgPropertySheet, hWnd, PropSheetProc, 0L);
}

#else /* _MGCTRL_PROPSHEET */

void testPropertySheet (HWND hwnd)
{
    MessageBox (hwnd, "WARNING", "Please enable PROPSHEET control support in MiniGUI.", MB_OK);
}

#endif /* _MGCTRL_PROPSHEET */
