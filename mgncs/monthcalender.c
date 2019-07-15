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
** Listing P2C15.1
**
** monthcalender.c: Sample program for mGNCS Programming Guide
**      The first mGNCS application.
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

#if defined _MGNCSCTRL_MONTHCALENDAR && defined _MGNCSCTRL_DIALOGBOX

#define ID_CALENDER      100
#define ID_EDIT_YEAR     101
#define ID_EDIT_MONTH    102
#define ID_EDIT_DAY      103

#define ID_BUTTON_SET    104
#define ID_STATIC_YEAR   105
#define ID_STATIC_MONTH  106
#define ID_STATIC_DAY    107

#define SLEDIT_W  35
#define SLEDIT_H  20
#define STATIC_H SLEDIT_H
#define STATIC_W 30

static char buff[20];

static BOOL input_restrict (mWidget* self, int max_num, int scancode)
{
	char str[25];
	int num;
	GetWindowText (self->hwnd, str, 25);
	str[23] = scancode;
	str[24] = '\0';
	strcat (str, &str[23]);
	num = atoi (str);
	if ((num > max_num) || str[0] == '0'){
		SetWindowText (self->hwnd, "");
		return TRUE;
	}
	return FALSE;
}

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	int year, month, day;

	mMonthCalendar* calender = (mMonthCalendar *)ncsGetChildObj(self->hwnd, ID_CALENDER);
	year  = _c(calender)->getProperty(calender, NCSP_CDR_YEAR);
	month = _c(calender)->getProperty(calender, NCSP_CDR_MONTH);
	day   = _c(calender)->getProperty(calender, NCSP_CDR_DAY);


	sprintf (buff, "%d", year);
	SetWindowText (GetDlgItem(self->hwnd, ID_EDIT_YEAR), buff);

	sprintf (buff, "%d", month);
	SetWindowText (GetDlgItem(self->hwnd, ID_EDIT_MONTH), buff);

	sprintf (buff, "%d", day);
	SetWindowText (GetDlgItem(self->hwnd, ID_EDIT_DAY), buff);


	//TODO : initialize
	return TRUE;
}

static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static BOOL mymain_oncsizechanged(mWidget *self, int cx, int cy)
{
	HWND hwnd ;
	int global_y;
	int year_x, month_x, day_x, set_x;

	global_y = cy*0.9 - 8;
	year_x  = cx*0.13-(SLEDIT_W+STATIC_W)/2;
	month_x = cx*0.38-(SLEDIT_W+STATIC_W)/2;
	day_x   = cx*0.62-(SLEDIT_W+STATIC_W)/2;
	set_x   = cx*0.77-(SLEDIT_W+STATIC_W)/2;

	hwnd = GetDlgItem(self->hwnd, ID_CALENDER);
	MoveWindow(hwnd, 0, 0, cx, cy*0.8,TRUE);

	hwnd = GetDlgItem(self->hwnd, ID_EDIT_YEAR);
	MoveWindow(hwnd, year_x, global_y, SLEDIT_W, SLEDIT_H,TRUE);
	hwnd = GetDlgItem(self->hwnd, ID_STATIC_YEAR);
	MoveWindow(hwnd, year_x + SLEDIT_W, global_y, STATIC_W, STATIC_H,TRUE);

	hwnd = GetDlgItem(self->hwnd, ID_EDIT_MONTH);
	MoveWindow(hwnd, month_x, global_y, SLEDIT_W, SLEDIT_H,TRUE);
	hwnd = GetDlgItem(self->hwnd, ID_STATIC_MONTH);
	MoveWindow(hwnd, month_x + SLEDIT_W, global_y, STATIC_W, STATIC_H,TRUE);

	hwnd = GetDlgItem(self->hwnd, ID_EDIT_DAY);
	MoveWindow(hwnd, day_x, global_y, SLEDIT_W, SLEDIT_H,TRUE);
	hwnd = GetDlgItem(self->hwnd, ID_STATIC_DAY);
	MoveWindow(hwnd, day_x + SLEDIT_W, global_y, STATIC_W, STATIC_H,TRUE);

	hwnd = GetDlgItem(self->hwnd, ID_BUTTON_SET);
	MoveWindow(hwnd, set_x + SLEDIT_W, global_y, STATIC_W, STATIC_H,TRUE);

	return FALSE;
}

static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{MSG_CSIZECHANGED, mymain_oncsizechanged},
	{0, NULL}
};

static NCS_RDR_INFO month_rdr_info[] =
{
    {"flat", "flat", NULL},
   // {"skin", "skin", NULL},
//	{"fashion","fashion", btn_rdr_elements}
};

static BOOL year_onChar (mWidget* self, int message, int scancode, DWORD key_status)
{
	if ((scancode <= '9' && scancode >= '0')||(scancode == 127)){
		if (input_restrict (self, 2039, scancode)){
			return TRUE;
		}else{
			return FALSE;
		}
	}else {
		return TRUE;
	}
}


static BOOL month_onChar (mWidget* self, int message, int scancode, DWORD key_status)
{
	if ((scancode <= '9' && scancode >= '0')||(scancode == 127)){
		if (input_restrict (self, 12, scancode)){
			return TRUE;
		}else{
			return FALSE;
		}
	}else {
		return TRUE;
	}
}


static BOOL day_onChar (mWidget* self, int message, int scancode, DWORD key_status)
{
	if ((scancode <= '9' && scancode >= '0')||(scancode == 127)){
		if (input_restrict (self, 31, scancode)){
			return TRUE;
		}else{
			return FALSE;
		}
	}else {
		return TRUE;
	}
}

static void Button_onPushed (mButton* self, int id, int nc)
{
	int year, month, day;

	GetWindowText (GetDlgItem (GetParent (self->hwnd), ID_EDIT_YEAR), buff, sizeof (buff));
	year = atoi (buff);

	GetWindowText (GetDlgItem (GetParent (self->hwnd), ID_EDIT_MONTH), buff, sizeof (buff));
	month = atoi (buff);

	GetWindowText (GetDlgItem (GetParent (self->hwnd), ID_EDIT_DAY), buff, sizeof (buff));
	day = atoi (buff);


	mMonthCalendar* calender = (mMonthCalendar *)ncsGetChildObj(GetParent (self->hwnd), ID_CALENDER);
	year  = _c(calender)->setProperty(calender, NCSP_CDR_YEAR, year);
	month = _c(calender)->setProperty(calender, NCSP_CDR_MONTH, month);
	day  = _c(calender)->setProperty(calender, NCSP_CDR_DAY, day);
}

static void year_changed (mButton* self, int id, int nc, DWORD add_data)
{
	int year;
	mMonthCalendar* calender = (mMonthCalendar *)ncsGetChildObj(GetParent (self->hwnd), ID_CALENDER);
	year  = _c(calender)->getProperty(calender, NCSP_CDR_YEAR);
	sprintf (buff, "%d", year);
	SetWindowText (GetDlgItem(GetParent (self->hwnd), ID_EDIT_YEAR), buff);

}

static void month_changed (mButton* self, int id, int nc, DWORD add_data)
{
	int month;
	mMonthCalendar* calender = (mMonthCalendar *)ncsGetChildObj(GetParent (self->hwnd), ID_CALENDER);
	month = _c(calender)->getProperty(calender, NCSP_CDR_MONTH);
	sprintf (buff, "%d", month);
	SetWindowText (GetDlgItem(GetParent (self->hwnd), ID_EDIT_MONTH), buff);
}

static void day_changed (mButton* self, int id, int nc, DWORD add_data)
{
	int day;
	mMonthCalendar* calender = (mMonthCalendar *)ncsGetChildObj(GetParent (self->hwnd), ID_CALENDER);
	day   = _c(calender)->getProperty(calender, NCSP_CDR_DAY);
	sprintf (buff, "%d", day);
	SetWindowText (GetDlgItem(GetParent (self->hwnd), ID_EDIT_DAY), buff);
}

static NCS_EVENT_HANDLER year_handlers [] = {
	{MSG_CHAR, year_onChar},
	{0, NULL}
};

static NCS_EVENT_HANDLER month_handlers [] = {
	{MSG_CHAR, month_onChar},
	{0, NULL}
};

static NCS_EVENT_HANDLER day_handlers [] = {
	{MSG_CHAR, day_onChar},
	{0, NULL}
};

static NCS_EVENT_HANDLER button_handlers [] = {
	{NCS_NOTIFY_CODE (NCSN_BUTTON_PUSHED), Button_onPushed},
	{0, NULL}
};

static NCS_EVENT_HANDLER calender_handlers [] ={
	NCS_MAP_NOTIFY(NCSN_CDR_YEAR_CHANGED, year_changed),
	NCS_MAP_NOTIFY(NCSN_CDR_MONTH_CHANGED, month_changed),
	NCS_MAP_NOTIFY(NCSN_CDR_DAY_CHANGED, day_changed),
	{0, NULL}
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_MONTHCALENDAR,
		ID_CALENDER,
		10, 10, 320, 200,
		WS_BORDER | WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"",
		NULL, //props,
		month_rdr_info, //rdr_info
		calender_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
	{
		NCSCTRL_SLEDIT,
		ID_EDIT_YEAR,
		0, 0, 10, 10,
		WS_BORDER | WS_VISIBLE | NCSS_EDIT_LEFT,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		year_handlers, NULL, 0,0
	},
	{
		NCSCTRL_SLEDIT,
		ID_EDIT_MONTH,
		0, 0, 10, 10,
		WS_BORDER | WS_VISIBLE | NCSS_EDIT_LEFT,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		month_handlers, NULL, 0,0
	},
	{
		NCSCTRL_SLEDIT,
		ID_EDIT_DAY,
		0, 0, 10, 10,
		WS_BORDER | WS_VISIBLE | NCSS_EDIT_LEFT,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		day_handlers, NULL, 0,0
	},
	{
		NCSCTRL_STATIC,
		ID_STATIC_YEAR,
		0, 0, 10, 10,
		WS_VISIBLE,
		WS_EX_NONE,
		"Year",
		NULL,
		NULL,
		NULL, NULL, 0,0
	},
	{
		NCSCTRL_STATIC,
		ID_STATIC_MONTH,
		0, 0, 10, 10,
		WS_VISIBLE,
		WS_EX_NONE,
		"Month",
		NULL,
		NULL,
		NULL, NULL, 0,0
	},
	{
		NCSCTRL_STATIC,
		ID_STATIC_DAY,
		0, 0, 10, 10,
		WS_VISIBLE,
		WS_EX_NONE,
		"Day",
		NULL,
		NULL,
		NULL, NULL, 0,0
	},
	{
		NCSCTRL_BUTTON,
		ID_BUTTON_SET,
		0, 0, 10, 10,
		WS_VISIBLE,
		WS_EX_NONE,
		"Set",
		NULL,
		NULL,
		button_handlers, NULL, 0,0
	},
};

//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_DIALOGBOX,
	1,
	0, 0, 320, 240,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "MonthCalendar",
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
	if(argc > 1){
		month_rdr_info[0].glb_rdr = argv[1];
		month_rdr_info[0].ctl_rdr = argv[1];
	}

	ncsInitialize();
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

	_c(mydlg)->doModal(mydlg, TRUE);

	ncsUninitialize();

	return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

#else //_MGNCSCTRL_MONTHCALENDAR _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the monthcalendar dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-monthcalendar --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_MONTHCALENDAR _MGNCSCTRL_DIALOGBOX
