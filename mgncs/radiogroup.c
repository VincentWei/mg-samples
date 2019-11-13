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
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
/*
** Listing P2C5.3
**
** radiogroup.c: Sample program for mGNCS Programming Guide
**      The first mGNCS application.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// START_OF_INCS
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
// END_OF_INCS

#ifdef _MGNCSCTRL_DIALOGBOX

#define ID_GROUP 200
#define ID_BTN1  101
#define ID_BTN2  102

// START_OF_HANDLERS
static BOOL mymain_onRadioChanged(mMainWnd* self, mButtonGroup* sender,
                                  int event_id, DWORD param)
{
    mWidget* sel = (mWidget*)_c(sender)->getProperty(sender, NCSP_BTNGRP_SELOBJ);

    if(sel)
    {
        MessageBox(self->hwnd, (const char*)_c(sel)->getProperty(sel,
            NCSP_WIDGET_TEXT), "Info", 0);
    }

    return TRUE;
}

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
    //connect
    ncsAddEventListener((mObject*)(_c(self)->getChild(self, ID_GROUP)),
        (mObject*)self, (NCS_CB_ONOBJEVENT)mymain_onRadioChanged,
        NCSN_BTNGRP_SELCHANGED);
    return TRUE;
}

static void mymain_onClose(mWidget* self, int message)
{
    DestroyMainWindow(self->hwnd);
    PostQuitMessage(0);
}

static NCS_EVENT_HANDLER mymain_handlers[] = {
    {MSG_CREATE,mymain_onCreate},
    {MSG_CLOSE,mymain_onClose},
    {0, NULL}
};
// END_OF_HANDLERS

// START_OF_RDRINFO
static NCS_RDR_INFO btn_rdr_info[] =
{
    {"skin", "skin", NULL},
};
// END_OF_RDRINFO

// START_OF_TEMPLATE
static NCS_PROP_ENTRY radioGroup_props [] = {
    {NCSP_BUTTON_GROUPID, 200},
    {0, 0}
};

static NCS_WND_TEMPLATE _ctrl_templ[] = {
    {
        NCSCTRL_BUTTONGROUP ,
        ID_GROUP,
        5, 10, 200, 120,
        WS_VISIBLE|NCSS_NOTIFY,
        WS_EX_NONE,
        "Button Group",
        NULL,          //props,
        btn_rdr_info,  //rdr_info
        NULL,          //handlers,
        NULL,          //controls
        0,
        0              //add data
    },
    {
        NCSCTRL_RADIOBUTTON,
        ID_BTN1,
        20, 30, 80, 25,
        WS_VISIBLE,
        WS_EX_NONE,
        "option1",
        radioGroup_props, //props,
        btn_rdr_info,     //rdr_info
        NULL,             //handlers,
        NULL,             //controls
        0,
        0                 //add data
    },
    {
        NCSCTRL_RADIOBUTTON,
        ID_BTN2,
        20, 60, 80, 25,
        WS_VISIBLE,
        WS_EX_NONE,
        "option2",
        radioGroup_props, //props,
        btn_rdr_info,     //rdr_info
        NULL,             //handlers,
        NULL,             //controls
        0,
        0                 //add data
    },
    {
        NCSCTRL_RADIOBUTTON,
        ID_BTN2,
        20, 90, 80, 25,
        WS_VISIBLE,
        WS_EX_NONE,
        "option3",
        radioGroup_props, //props,
        btn_rdr_info,     //rdr_info
        NULL,             //handlers,
        NULL,             //controls
        0,
        0                 //add data
    },
};

static NCS_MNWND_TEMPLATE mymain_templ = {
    NCSCTRL_DIALOGBOX,
    1,
    0, 0, 260, 180,
    WS_CAPTION | WS_BORDER | WS_VISIBLE,
    WS_EX_NONE,
    "Radio button",
    NULL,
    NULL,
    mymain_handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,
    0, 0,
};
// END_OF_TEMPLATE

int MiniGUIMain(int argc, const char* argv[])
{
	if (argc > 1) {
		btn_rdr_info[0].glb_rdr = argv[1];
		btn_rdr_info[0].ctl_rdr = argv[1];
	}

    ncsInitialize();

    mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                (&mymain_templ, HWND_DESKTOP);

    _c(mydlg)->doModal(mydlg, TRUE);

    ncsUninitialize ();

    return 0;
}

#else //_MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_DIALOGBOX
