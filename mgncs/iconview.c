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
/**
 * Listing P2C14.3
 *
 * iconview.c: Sample program for mGNCS Programming Guide
 *      The demo application for IconView.
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

#if defined _MGNCSCTRL_ICONVIEW && defined _MGNCSCTRL_DIALOGBOX

#define IDC_ICONVIEW    100
#define IDC_ADD         600
#define IDC_DELETE      601

static BITMAP icon_demos [12];

static const char* iconfiles[12] =
{
    "./res/acroread.png",
    "./res/icons.png",
    "./res/looknfeel.png",
    "./res/package_games.png",
    "./res/tux.png",
    "./res/xemacs.png",
    "./res/gimp.png",
    "./res/kpilot.png",
    "./res/multimedia.png",
    "./res/realplayer.png",
    "./res/usb.png",
    "./res/xmms.png"
};

static const char *iconlabels[12] =
{
    "acroread",
    "icons",
    "looknfeel",
    "games",
    "tux",
    "xemacs",
    "gimp",
    "kpilot",
    "multimedia",
    "realplayer",
    "usb",
    "xmms"
};

static BOOL iconv_init(mDialogBox* self)
{
    NCS_ICONV_ITEMINFO info;
    static int i = 0, j = 0, pos = 0;
    mIconView *iconvObj;
    HWND iconvWnd;

    for(i = 0; i < TABLESIZE(icon_demos); i++)
    {
        LoadBitmap (HDC_SCREEN, &icon_demos[i], iconfiles[i]);
    }

    iconvWnd = GetDlgItem (self->hwnd, IDC_ICONVIEW);
    iconvObj = (mIconView*)ncsObjFromHandle(iconvWnd);

    if (!iconvObj)
        return FALSE;

// START_OF_ADDITEMS
    _c(iconvObj)->setIconSize(iconvObj, 90, 80);

    for(i = 0; i < TABLESIZE(icon_demos); i++)
    {
        pos = 0;
        memset (&info, 0, sizeof(NCS_ICONV_ITEMINFO));
        info.bmp = &icon_demos[i];
        info.index = TABLESIZE(icon_demos) * j + i;
        info.label = iconlabels[i];
        info.addData = (DWORD)iconlabels[i];
        _c(iconvObj)->addItem(iconvObj, &info, &pos);
    }
    _c(iconvObj)->setCurSel(iconvObj, 0);
// END_OF_ADDITEMS

    return TRUE;
}

// START_OF_WNDHANDLERS
static BOOL mainwnd_onKeyDown(mWidget* self,
        int message, int code, DWORD key_status)
{
    if (message == MSG_KEYDOWN) {
        if (code == SCANCODE_REMOVE) {
            mIconView *iconView;
            int curSel, count;
            HITEM delItem;

            iconView =
                (mIconView*)ncsObjFromHandle(GetDlgItem (self->hwnd, IDC_ICONVIEW));
            count = _c(iconView)->getItemCount(iconView);

            if (iconView) {
                curSel = _c(iconView)->getCurSel(iconView);

                if (curSel >= 0) {
                    delItem = _c(iconView)->getItem(iconView, curSel);
                    _c(iconView)->removeItem(iconView, delItem);
                    if (curSel == count -1)
                        curSel--;
                    _c(iconView)->setCurSel(iconView, curSel);
                }
            }
        }
    }
    return FALSE;
}

static NCS_EVENT_HANDLER mainwnd_handlers[] = {
	{MSG_KEYDOWN, mainwnd_onKeyDown},
	{0, NULL }
};
// END_OF_WNDHANDLERS

// START_OF_ICONVHANDLERS
static void iconv_notify (mWidget *self, int id, int nc, DWORD add_data)
{
    if (nc == NCSN_ICONV_CLICKED)
    {
        if (self) {
            int idx;
            const char  *text;
            mIconView   *iconvObj = (mIconView*)self;

            idx = _c(iconvObj)->indexOf(iconvObj, (HITEM)add_data);
            text = _c(iconvObj)->getText(iconvObj, (HITEM)add_data);
            fprintf (stderr, "click icon[%d], text is %s \n", idx, text);
        }
    }
}

static NCS_EVENT_HANDLER iconv_handlers[] = {
    NCS_MAP_NOTIFY(NCSN_ICONV_CLICKED, iconv_notify),
    NCS_MAP_NOTIFY(NCSN_ICONV_SELCHANGED, iconv_notify),
	{0, NULL }
};
// END_OF_ICONVHANDLERS

// START_OF_BTNHANDLERS
static void btn_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    mIconView *iconvObj =
        (mIconView *)ncsGetChildObj(GetParent(self->hwnd), IDC_ICONVIEW);

    if (!iconvObj)
        return;

    switch (id)
    {
        case IDC_ADD:
        {
            char    buff[12];
            int     count, pos = 0;
            NCS_ICONV_ITEMINFO info;

            count = _c(iconvObj)->getItemCount(iconvObj);
            sprintf (buff, "icon%i", count+1);

            memset (&info, 0, sizeof(NCS_ICONV_ITEMINFO));
            info.bmp = &icon_demos[0];
            info.index = count;
            info.label = buff;
            info.addData = (DWORD)"icon";

            if (_c(iconvObj)->addItem(iconvObj, &info, &pos))
                _c(iconvObj)->setCurSel(iconvObj, pos);

            break;
        }

        case IDC_DELETE:
        {
            int     count, sel;
            //char    *label = NULL;
            HITEM   hItem;

            sel     = _c(iconvObj)->getCurSel(iconvObj);
            count   = _c(iconvObj)->getItemCount(iconvObj);
            hItem   = _c(iconvObj)->getItem(iconvObj, sel);

            if (sel >= 0) {
                //label = (char*)_c(iconvObj)->getAddData(iconvObj, hItem);
                _c(iconvObj)->removeItem(iconvObj, hItem);

                if (sel == count -1)
                    sel --;

                _c(iconvObj)->setCurSel(iconvObj, sel);
            }
            break;
        }
    }
}

static NCS_EVENT_HANDLER btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_BUTTON_PUSHED, btn_notify),
	{0, NULL}
};
// END_OF_BTNHANDLERS

static NCS_RDR_INFO iconv_rdr_info[] = {
	{ "classic", "classic", NULL },
};

static NCS_WND_TEMPLATE _ctrl_tmpl[] = {
	{
		NCSCTRL_ICONVIEW,
		IDC_ICONVIEW,
        15, 10, 220, 250,
        WS_BORDER | WS_CHILD | WS_VISIBLE | NCSS_NOTIFY | NCSS_ICONV_LOOP,
		WS_EX_NONE,
		"",
		NULL,
		iconv_rdr_info,
		iconv_handlers,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_BUTTON,
		IDC_ADD,
        15, 280, 80, 30,
        WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"add",
		NULL,
		NULL,
		btn_handlers,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_BUTTON,
        IDC_DELETE,
        155, 280, 80, 30,
        WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"delete",
		NULL,
		NULL,
		btn_handlers,
		NULL,
		0,
		0
	},
};

static NCS_MNWND_TEMPLATE mainwnd_tmpl = {
	NCSCTRL_DIALOGBOX,
	7,
	0, 0, 260, 350,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "IconView Demo",
	NULL,
	NULL,
	mainwnd_handlers,
	_ctrl_tmpl,
	sizeof(_ctrl_tmpl)/sizeof(NCS_WND_TEMPLATE),
	0,
	0, 0,
};

int MiniGUIMain(int argc, const char* argv[])
{
	if (argc > 1) {
		iconv_rdr_info[0].glb_rdr = argv[1];
		iconv_rdr_info[0].ctl_rdr = argv[1];
	}

	ncsInitialize();
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mainwnd_tmpl, HWND_DESKTOP);

    iconv_init(mydlg);
	_c(mydlg)->doModal(mydlg, TRUE);
	ncsUninitialize();
	return 0;
}
#else //_MGNCSCTRL_ICONVIEW _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the iconview dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-iconview --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_ICONVIEW _MGNCSCTRL_DIALOGBOX
