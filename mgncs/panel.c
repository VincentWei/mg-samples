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
** Listing P2C6.1
**
** panel.c: Sample program for mGNCS Programming Guide
**      The demo application for Panel.
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

#define     ID_PANEL_GROUP1     101
#define     ID_PANEL_GROUP2     102

#define     ID_RED1             201
#define     ID_RED2             202
#define     ID_BLUE1            301
#define     ID_BLUE2            302

// START_OF_REDGROUP
static NCS_WND_TEMPLATE _ctrl_group1[] = {
	{
		NCSCTRL_BUTTON, 
		ID_RED1,
		30, 40, 80, 30,
		WS_VISIBLE,
		WS_EX_NONE,
		"RED 1",
		NULL,
		NULL,
		NULL,
		NULL,
        0,
	},
	{
		NCSCTRL_BUTTON, 
		ID_RED2,
		30, 110, 80, 30,
		WS_VISIBLE,
		WS_EX_NONE,
		"RED 2",
		NULL,
		NULL,
		NULL,
		NULL,
        0,
	},
};
// END_OF_REDGROUP

// START_OF_BLUEGROUP
static NCS_WND_TEMPLATE _ctrl_group2[] = {
	{
		NCSCTRL_BUTTON, 
		ID_BLUE1,
		30, 40, 80, 30,
		WS_VISIBLE,
		WS_EX_NONE,
		"BLUE 1",
		NULL,
		NULL,
		NULL,
		NULL,
        0,
	},
	{
		NCSCTRL_BUTTON, 
		ID_BLUE2,
		30, 110, 80, 30,
		WS_VISIBLE,
		WS_EX_NONE,
		"BLUE 2",
		NULL,
		NULL,
		NULL,
		NULL,
        0,
	},
};
// END_OF_BLUEGROUP

// START_OF_PANEL
static NCS_WND_TEMPLATE panel_tmpl[] = {
	{
		NCSCTRL_PANEL, 
		ID_PANEL_GROUP1,
		10, 10, 140, 180,
		WS_VISIBLE,
		WS_EX_NONE,
		"Red Group",
		NULL,
		NULL,
		NULL,
		_ctrl_group1, 
		sizeof(_ctrl_group1)/sizeof(NCS_WND_TEMPLATE),
		0,
        0xFF0000FF,
	},
	{
		NCSCTRL_PANEL, 
		ID_PANEL_GROUP2,
		160, 10, 140, 180,
		WS_VISIBLE,
		WS_EX_NONE,
		"Blue Group",
		NULL,
		NULL,
		NULL,
		_ctrl_group2,
		sizeof(_ctrl_group2)/sizeof(NCS_WND_TEMPLATE), 
		0,
        0xFFFF0000,
	},
};
// END_OF_PANEL

// START_OF_HANDLERS
static BOOL mainwnd_onCreate(mWidget* self, DWORD add_data)
{
    _c(self)->addChildren(self, panel_tmpl, \
		sizeof(panel_tmpl)/sizeof(NCS_WND_TEMPLATE));
	return TRUE;
}

static void mainwnd_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}


static NCS_EVENT_HANDLER mainwnd_handlers[] = {
	{MSG_CREATE, mainwnd_onCreate},
	{MSG_CLOSE, mainwnd_onClose},
	{0, NULL}
};
// END_OF_HANDLERS

int MiniGUIMain(int argc, const char* argv[])
{
    MSG Msg;

    ncsInitialize ();

    mWidget* mainwnd = ncsCreateMainWindow (
        NCSCTRL_MAINWND, "Panel Demo",
        WS_CAPTION | WS_BORDER | WS_VISIBLE,
        WS_EX_NONE,
        1, 
        0, 0, 320,240,
        HWND_DESKTOP,
        0, 0,
        NULL,
        NULL,
        mainwnd_handlers,
        0);
    
// START_OF_MSGLOOP
    while (GetMessage (&Msg, mainwnd->hwnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }
// END_OF_MSGLOOP
    
    ncsUninitialize ();

    return 0;
}
