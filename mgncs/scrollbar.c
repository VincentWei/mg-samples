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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgncs/mgncs.h>

#if defined _MGNCSCTRL_SCROLLBAR && defined _MGNCSCTRL_DIALOGBOX

#define ID_SB1  100
#define ID_SB2  101

static RECT rcCircle = {0, 0, 400, 400};
static int radius_x = 0;
static int radius_y = 0;

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	return TRUE;
}
static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static void mymain_onPaint(mWidget* self, HDC hdc, const PCLIPRGN clip_rgn)
{
	ClipRectIntersect(hdc, &rcCircle);
	Ellipse(hdc, 130, 200, radius_x, radius_y);
}

//define the progress properites
static NCS_PROP_ENTRY scrollbar_props [] = {
	{NCSP_SCRLBR_MAXPOS, 255},
	{NCSP_SCRLBR_MINPOS, 0 },
	{NCSP_SCRLBR_LINESTEP, 5},
	{NCSP_SCRLBR_CURPOS, 10 },
	{ 0, 0 }
};


static void scrollbar_notify(mScrollBar* self, int id, int code, DWORD add_data)
{
	HWND hWnd = GetParent(self->hwnd);

	if(id == ID_SB1)
		radius_x = _c(self)->getProperty(self, NCSP_SCRLBR_CURPOS);

	if(id == ID_SB2)
		radius_y = _c(self)->getProperty(self, NCSP_SCRLBR_CURPOS);

	InvalidateRect(hWnd, &rcCircle, TRUE);
}

static NCS_EVENT_HANDLER scrollbar_notifies[] = {
	NCS_MAP_NOTIFY(NCSN_SCRLBR_CHANGED, scrollbar_notify),
	{0, NULL}
};

static NCS_RDR_INFO sb_rdr_info[] =
{
    {"flat", "flat", NULL},
    //{"skin", "skin", NULL},
	//{"fashion","fashion", NULL}
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_SCROLLBAR,
		ID_SB1,
		0, 440, 270, 25,
		WS_BORDER | NCSS_NOTIFY | WS_VISIBLE | NCSS_SCRLBR_ARROWS,
		WS_EX_NONE,
		"",
		scrollbar_props, //props,
		sb_rdr_info, //rdr_info
		scrollbar_notifies,
		NULL, //controls
		0,
		0 //add data
	},
	{
		NCSCTRL_SCROLLBAR,
		ID_SB2,
		270, 10, 20, 430,
		WS_BORDER | NCSS_NOTIFY | WS_VISIBLE \
			| NCSS_SCRLBR_ARROWS | NCSS_SCRLBR_VERTICAL,
		WS_EX_NONE,
		"",
		scrollbar_props, //props,
		sb_rdr_info, //rdr_info
		scrollbar_notifies,
		NULL, //controls
		0,
		0 //add data
	},
};


static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{MSG_PAINT, mymain_onPaint},
	{0, NULL}
};

//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_DIALOGBOX,
	1,
	0, 0, 300, 500,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "scrollbar Test ....",
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
	if(argc>1)
	{
		sb_rdr_info[0].glb_rdr = argv[1];
		sb_rdr_info[0].ctl_rdr = argv[1];
	}

	ncsInitialize();
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

	_c(mydlg)->doModal(mydlg, TRUE);

	ncsUninitialize();

	return 0;
}

#else //_MGNCSCTRL_SCROLLBAR _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the scrollbar dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-scrollbar --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_SCROLLBAR _MGNCSCTRL_DIALOGBOX
