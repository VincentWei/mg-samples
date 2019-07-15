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
 *   This file is part of mGNCS, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
/*
** Listing P2C1.4
**
** event_listener.c: Sample program for mGNCS Programming Guide
**      Using event listener.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <mgncs/mgncs.h>

#if defined _MGNCSCTRL_PROGRESSBAR && defined _MGNCSCTRL_DIALOGBOX

#define IDC_EXIT      100
#define IDC_STOP     101
#define IDC_PROG      200

static void mymain_onClose (mWidget* self, int message);

// START_OF_EVENTLISTENER
static BOOL mymain_onExit (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    mymain_onClose ((mWidget*)self, MSG_CLOSE);
    return TRUE;
}

static BOOL mymain_onStop (mMainWnd *self, mButton *sender, int id, DWORD param)
{
    KillTimer (self->hwnd, 100);
    return TRUE;
}
// END_OF_EVENTLISTENER

// START_OF_HANDLERS
static BOOL mymain_onCreate (mWidget* self, DWORD addData)
{
    mButton *btn;

    btn = (mButton*)_c(self)->getChild (self, IDC_EXIT);
    ncsAddEventListener ((mObject*)btn, (mObject*)self,
                (NCS_CB_ONOBJEVENT)mymain_onExit, NCSN_WIDGET_CLICKED);

    btn = (mButton*)_c(self)->getChild (self, IDC_STOP);
    ncsAddEventListener ((mObject*)btn, (mObject*)self,
                (NCS_CB_ONOBJEVENT)mymain_onStop, NCSN_WIDGET_CLICKED);

    SetTimer (self->hwnd, 100, 20);
    return TRUE;
}

static void mymain_onTimer (mWidget *self, int id, DWORD count)
{
    static int pb_pos = 0;
    mProgressBar *pb = (mProgressBar*)ncsGetChildObj (self->hwnd, IDC_PROG);
    if (pb)
        _c(pb)->setProperty (pb, NCSP_PROG_CURPOS, pb_pos++);
}

static void mymain_onClose (mWidget* self, int message)
{
    DestroyMainWindow (self->hwnd);
    PostQuitMessage (self->hwnd);
}

static NCS_EVENT_HANDLER mymain_handlers[] = {
    {MSG_CREATE, mymain_onCreate},
    {MSG_CLOSE, mymain_onClose},
    {MSG_TIMER, mymain_onTimer},
    {0, NULL}
};
// END_OF_HANDLERS

static NCS_RDR_INFO btn1_rdr_info[] =
{
    {"fashion", "fashion", NULL}
};

static NCS_RDR_INFO btn2_rdr_info[] =
{
    {"skin", "skin", NULL}
};

// START_OF_PROPERTIES
static NCS_PROP_ENTRY progress_props [] = {
    {NCSP_PROG_MAXPOS, 100},
    {NCSP_PROG_MINPOS, 0 },
    {NCSP_PROG_LINESTEP, 1},
    {NCSP_PROG_CURPOS, 0 },
    { 0, 0 }
};
// END_OF_PROPERTIES

// START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] = {
    {
        NCSCTRL_PROGRESSBAR,
        IDC_PROG,
        10, 10, 290, 30,
        WS_BORDER | WS_VISIBLE,
        WS_EX_NONE,
        "",
        progress_props,
        NULL,
        NULL,
        NULL,
        0,
        0
    },
    {
        NCSCTRL_BUTTON,
        IDC_STOP,
        10, 50, 100, 25,
        WS_BORDER | WS_VISIBLE,
        WS_EX_NONE,
        "Stop",
        NULL,
        btn1_rdr_info,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_BUTTON,
        IDC_EXIT,
        200, 50, 100, 25,
        WS_VISIBLE,
        WS_EX_NONE,
        "Exit",
        NULL,
        btn2_rdr_info,
        NULL, NULL, 0, 0
    },
};

static NCS_MNWND_TEMPLATE mymain_templ = {
    NCSCTRL_DIALOGBOX,
    1,
    0, 0, 320, 110,
    WS_CAPTION | WS_BORDER | WS_VISIBLE,
    WS_EX_NONE,
    "Event Listener",
    NULL,
    NULL,
    mymain_handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,
    0, 0,
};
// END_OF_TEMPLATE

int MiniGUIMain (int argc, const char* argv[])
{
    mDialogBox* mydlg;

	if (argc > 1) {
		btn1_rdr_info[0].glb_rdr = argv[1];
		btn1_rdr_info[0].ctl_rdr = argv[1];
		btn2_rdr_info[0].glb_rdr = argv[1];
		btn2_rdr_info[0].ctl_rdr = argv[1];
	}

    ncsInitialize ();

    mydlg = (mDialogBox *)ncsCreateMainWindowIndirect (&mymain_templ,
                                HWND_DESKTOP);

    _c(mydlg)->doModal (mydlg, TRUE);

    ncsUninitialize ();

    return 0;
}
#else //_MGNCSCTRL_PROGRESSBAR && defined _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the progressbar and dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-progressbar --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_PROGRESSBAR _MGNCSCTRL_DIALOGBOX

