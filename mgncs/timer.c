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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgncs/mgncs.h>
#include <time.h>

#ifdef _MGNCSCTRL_DIALOGBOX

//START_UPDATE_TIME
static BOOL update_time(mStatic *listener,
		mTimer* sender,
		int id,
		DWORD total_count)
{
	char szText[100];
	time_t tim;
	struct tm *ptm;

	time(&tim);
	ptm = localtime(&tim);

	sprintf(szText,
			"%02d:%02d:%d",
			ptm->tm_hour,
			ptm->tm_min,
            ptm->tm_sec);

	SetWindowText(listener->hwnd, szText);
    InvalidateRect(listener->hwnd, NULL, TRUE);

	return FALSE;
}
//END_UPDATE_TIME


//START_TIMER_CONNECT
static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	//TODO : initialize
	mTimer * timer = SAFE_CAST(mTimer,
					_c(self)->getChild(self, 100));
	if(timer)
	{
		ncsAddEventListener((mObject*)timer,
						(mObject*)ncsGetChildObj(self->hwnd, 101),
						(NCS_CB_ONPIECEEVENT)update_time,
						MSG_TIMER);
		_c(timer)->start(timer);
	}
	return TRUE;
}
//END_TIMER_CONNECT

static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

//Controls
//START_DECLARE_TIMER
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_TIMER,
		100,
		10, 10, 0, 0,
		WS_BORDER | WS_VISIBLE,
		WS_EX_NONE,
		"",
		NULL, //props,
		NULL, //rdr_info
		NULL, //timer_props, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
//END_DECLARE_TIMER
	{
		NCSCTRL_STATIC,
		101,
		10,10, 100, 30,
		WS_VISIBLE,
		0,
		"",
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		0
	}
};


static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{0, NULL}
};

//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_DIALOGBOX,
	1,
	0, 0, 150, 80,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "Digital Clock",
	NULL,
	NULL,
	mymain_handlers,
	_ctrl_templ,
	sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
	0,
	0, 0,
};

int MiniGUIMain(int argc, const char* argv[])
{
	ncsInitialize();
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

	_c(mydlg)->doModal(mydlg, TRUE);

	ncsUninitialize();

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
