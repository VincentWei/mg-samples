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
** $Id: monthcal.c 759 2009-09-21 05:22:47Z dongjunjie $
**
** monthcal.c: Sample program for MiniGUI Programming Guide
**      Usage of MONTHCALENDAR control.
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

#include <stdlib.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_MONTHCAL

#define IDC_MC                100
#define IDC_OK                200

#ifdef _LANG_ZHCN
#include "monthcal_res_cn.h"
#elif defined _LANG_ZHTW
#include "monthcal_res_tw.h"
#else
#include "monthcal_res_en.h"
#endif

CTRLDATA CtrlTime[]=
{
        {
                "monthcalendar",
#ifdef _LANG_ZHCN
                WS_CHILD | WS_VISIBLE | MCS_NOTIFY | MCS_CHN,
#else
                WS_CHILD | WS_VISIBLE | MCS_NOTIFY | MCS_ENG_L,
#endif
                10, 10, 220, 180,
                IDC_MC,
                "",
                0
        },
        {
                "button",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 195, 50, 22,
                IDC_OK,
                ok_,
                0
        }
};

DLGTEMPLATE DlgTime = 
{
        WS_VISIBLE | WS_CAPTION | WS_BORDER,
        WS_EX_NONE,
        0, 0, 250, 260,
        date_,
        0, 0,
        2, CtrlTime,
        0
};

/*******************************************************************************/

static LRESULT TimeWinProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
        break;

        case MSG_COMMAND:
        {
            int id = LOWORD(wParam);
            if (id == IDC_OK) {
                char info[100];
                SYSTEMTIME date;
                SendMessage (GetDlgItem(hDlg, IDC_MC), MCM_GETCURDATE, 0, (LPARAM)&date);
                sprintf (info, the_date_you_selected_is, date.year, date.month, date.day);
                MessageBox (hDlg, info, "Date", MB_OK);
                EndDialog (hDlg, 0);
            }
        }
        break;
	case MSG_CSIZECHANGED:
		{
			HWND hwnd = GetDlgItem(hDlg, IDC_MC);
			MoveWindow(hwnd, 10, 10, wParam - 20, lParam - 40, TRUE);
		}
		return 0;

    case MSG_CLOSE:
        {
            EndDialog (hDlg, 0);
        }
        return 0;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

/*******************************************************************************/

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "monthcal" , 0 , 0);
#endif

	if(argc > 1)
		SetDefaultWindowElementRenderer(argv[1]);
    
    DlgTime.controls = CtrlTime;
    
    DialogBoxIndirectParam (&DlgTime, HWND_DESKTOP, TimeWinProc, 0L);

    return 0;
}

#else /* _MGCTRL_MONTHCAL */

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for MONTHCAL control in MiniGUI.\n");
    return 0;
}

#endif /* _MGCTRL_MONTHCAL */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

