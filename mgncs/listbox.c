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
/**
 * Listing P2C14.2
 *
 * listbox.c: Sample program for mGNCS Programming Guide
 *      The demo application for ListBox.
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

#if defined _MGNCSCTRL_LISTBOX && defined _MGNCSCTRL_DIALOGBOX

#define IDC_LIST    100
#define IDC_DELETE  200

static char* items[] = {
    "Apples",
    "Apricots",
    "Bananas",
    "Grapefruit",
    "Kiwi",
    "Oranges",
    "Peaches",
    "The Longest String"
};

static void lstbox_init(mDialogBox *dialog)
{
    mListBox *lstboxObj;
    int i;

    lstboxObj = (mListBox *)ncsGetChildObj(dialog->hwnd, IDC_LIST);

// START_OF_ADDITEMS
    _c(lstboxObj)->freeze(lstboxObj, TRUE);
    for (i = 0; i < TABLESIZE(items); i++) {
        _c(lstboxObj)->addString (lstboxObj, items[i], 0);
    }
    _c(lstboxObj)->setCurSel(lstboxObj, 0);
    _c(lstboxObj)->freeze(lstboxObj, FALSE);
// END_OF_ADDITEMS
}

// START_OF_BTNHANDLERS
static void btn_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    mListBox    *lstboxObj =
        (mListBox *)ncsGetChildObj(GetParent(self->hwnd), IDC_LIST);
// START_OF_DELITEMS
    int sel     = _c(lstboxObj)->getCurSel(lstboxObj);
    int count   = _c(lstboxObj)->getItemCount(lstboxObj);

    if (sel >= 0) {
        _c(lstboxObj)->delString(lstboxObj, sel);

        if (sel == count -1)
            sel --;

        _c(lstboxObj)->setCurSel(lstboxObj, sel);
    }
// END_OF_DELITEMS
}

static NCS_EVENT_HANDLER btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_BUTTON_PUSHED, btn_notify),
	{0, NULL}
};
// END_OF_BTNHANDLERS
static NCS_WND_TEMPLATE _ctrl_tmpl[] = {
	{
		NCSCTRL_LISTBOX,
		IDC_LIST,
		20, 15, 170, 200,
		WS_BORDER | WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_BUTTON,
		IDC_DELETE,
        15, 230, 80, 30,
        WS_VISIBLE | WS_TABSTOP,
		WS_EX_NONE,
		"Delete",
		NULL,
		NULL,
		btn_handlers,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_BUTTON,
		IDCANCEL,
        115, 230, 80, 30,
        WS_VISIBLE | WS_TABSTOP,
		WS_EX_NONE,
		"Cancel",
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		0
	},
};


static NCS_MNWND_TEMPLATE mainwnd_tmpl = {
	NCSCTRL_DIALOGBOX,
	1,
	100, 100, 220, 300,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "ListBox Demo",
	NULL,
	NULL,
	NULL,
	_ctrl_tmpl,
	sizeof(_ctrl_tmpl)/sizeof(NCS_WND_TEMPLATE),
	0,
	0, 0,
};

int MiniGUIMain(int argc, const char* argv[])
{
	ncsInitialize();

	mDialogBox* dialog =
        (mDialogBox *)ncsCreateMainWindowIndirect (&mainwnd_tmpl, HWND_DESKTOP);

    lstbox_init(dialog);
	_c(dialog)->doModal(dialog, TRUE);

	ncsUninitialize();
	return 0;
}
#else //_MGNCSCTRL_LISTBOX _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the listbox, dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-listbox --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_LISTBOX _MGNCSCTRL_DIALOGBOX
