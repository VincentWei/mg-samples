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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgncs/mgncs.h>

#if defined _MGNCSCTRL_SPINBOX && defined _MGNCSCTRL_DIALOGBOX

#define ID_SPINNER1      101
#define ID_SHOWSPINNER1  201
#define ID_SPINNER2      102
#define ID_SHOWSPINNER2  202

#define MINVALUE        0
#define MAXVALUE        10
#define CURVALUE        5

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	//START_GET_CTRLS
	mSpinner * spn1, *spn2;
	mStatic  * show1, * show2;
	spn1 = (mSpinner*)_c(self)->getChild(self, ID_SPINNER1);
	spn2 = (mSpinner*)_c(self)->getChild(self, ID_SPINNER2);
	show1 = (mStatic*)_c(self)->getChild(self, ID_SHOWSPINNER1);
	show2 = (mStatic*)_c(self)->getChild(self, ID_SHOWSPINNER2);
	//END_GET_CTRLS

	//START_CONNECT_PROPS
	ncsConnectBindProps(
		NCS_CMPT_PROP(spn1, NCSN_SPNR_CHANGED, NCSP_SPNR_CURPOS, NCS_BT_INT, NCS_PROP_FLAG_READ),
		NCS_CMPT_PROP(show1, 0, NCSP_WIDGET_TEXT, NCS_BT_STR, NCS_PROP_FLAG_WRITE),
		NCS_BPT_SINGLE);

	ncsConnectBindProps(
		NCS_CMPT_PROP(spn2, NCSN_SPNR_CHANGED, NCSP_SPNR_CURPOS, NCS_BT_INT, NCS_PROP_FLAG_READ),
		NCS_CMPT_PROP(show2, 0, NCSP_WIDGET_TEXT, NCS_BT_STR, NCS_PROP_FLAG_WRITE),
		NCS_BPT_SINGLE);
	//END_CONNECT_PROPS

	//START_UPDATE_CTRLS
	ncsRaiseComponentBindProps((mComponent*)spn1, NCSN_SPNR_CHANGED);
	ncsRaiseComponentBindProps((mComponent*)spn2, NCSN_SPNR_CHANGED);
	//END_UPDATE_CTRLS


	return TRUE;
}

static void mymain_onClose (mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

//START_SET_PROPS
//Propties for
static NCS_PROP_ENTRY spinbox_props [] = {
    {NCSP_SPNR_MINPOS, MINVALUE},
    {NCSP_SPNR_MAXPOS, MAXVALUE},
    {NCSP_SPNR_CURPOS, CURVALUE},
    {NCSP_SPNR_LINESTEP, 1},
	{0, 0}
};
//END_SET_PROPS

static NCS_RDR_INFO spin_rdr_info[] =
{
    {"skin", "skin", NULL},
    //{"fashion", "fashion", NULL},
    //{"flat", "flat", NULL},
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_STATIC,
		ID_SHOWSPINNER1,
		100,40, 100,20,
		WS_VISIBLE,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_SPINNER ,
		ID_SPINNER1,
		30, 40, 20, 20,
		WS_VISIBLE | NCSS_SPNBOX_VERTICAL | NCSS_NOTIFY,
		WS_EX_NONE,
		"spinbox",
		spinbox_props, //props,
		spin_rdr_info, //NULL, //rdr_info
		NULL, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
	{
		NCSCTRL_STATIC,
		ID_SHOWSPINNER2,
		100, 100, 100, 20,
		WS_VISIBLE,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		0,
		0
	},

    {
		NCSCTRL_SPINNER ,
		ID_SPINNER2,
		30, 100, 20, 20,
		WS_VISIBLE | NCSS_SPNBOX_HORIZONTAL | NCSS_NOTIFY,
		WS_EX_NONE,
		"spinbox",
		spinbox_props, //props,
		spin_rdr_info, //NULL, //rdr_info
		NULL, //handlers,
		NULL, //controls
		0,
		0 //add data
	}
};


static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE,mymain_onCreate},
	{MSG_CLOSE,mymain_onClose},
	{0, NULL}
};

static NCS_RDR_INFO mymain_rdr_info[] =
{
    {"skin", "skin", NULL},
};


//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_DIALOGBOX,
	1,
	0, 0, 260, 200,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "Spinner Test ....",
	NULL,
	mymain_rdr_info,
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
		spin_rdr_info[0].glb_rdr = argv[1];
		spin_rdr_info[0].ctl_rdr = argv[1];
	}


	ncsInitialize();
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

	_c(mydlg)->doModal(mydlg, TRUE);

	ncsUninitialize();

	return 0;
}

#else //_MGNCSCTRL_SPINBOX _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the spinbox dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-spinbox --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_SPINBOX _MGNCSCTRL_DIALOGBOX
