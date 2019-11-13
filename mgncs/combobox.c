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
** Listing P2C1.4
**
** combobox.c: Sample program for mGNCS Programming Guide
**      Using Combobox.
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

#if defined _MGNCSCTRL_COMBOBOX && defined _MGNCSCTRL_DIALOGBOX

#define ID_BTN  201
#define ID_NAME 202
#define ID_TYPE 203
#define ID_LOCA 204

static const char *file_type[] =
{
    "All File(*.*)",
    "Text File(*.txt)",
    "Bin File(*.bin)",
};

static BOOL mymain_onCreate (mWidget* _this, DWORD add_data)
{
    int i;

    // get combobox
    mCombobox *com = (mCombobox *)ncsGetChildObj(_this->hwnd, ID_TYPE);

    // add items
    for (i = 0; i < sizeof(file_type)/sizeof(file_type[0]); i++)
    {
        _c(com)->addItem(com, file_type[i], 0);
    }

    // set the selected item
    _c(com)->setProperty(com, NCSP_COMB_SELECT, 1);

    return TRUE;
}

static void mymain_onClose (mWidget* _this, int message)
{
    DestroyMainWindow (_this->hwnd);
    PostQuitMessage (_this->hwnd);
}

static void mymain_onPaint(mWidget *self, HDC hdc, const CLIPRGN* inv)
{
    SetBkMode (hdc, BM_TRANSPARENT);
	TextOut (hdc, 40, 10, "New File Wizard ...");
}

static NCS_EVENT_HANDLER mymain_handlers[] = {
    {MSG_CREATE, mymain_onCreate},
    {MSG_CLOSE,  mymain_onClose},
    {MSG_PAINT,  mymain_onPaint},
    {0, NULL}
};

static void btn_onClicked(mWidget* _this, int id, int nc, HWND hCtrl)
{
    if(nc == NCSN_WIDGET_CLICKED)
    {
        PostMessage(GetParent(_this->hwnd), MSG_CLOSE, 0, 0);
    }
};

static NCS_EVENT_HANDLER btn_handlers[] =
{
    {NCS_NOTIFY_CODE(NCSN_WIDGET_CLICKED), btn_onClicked},
    {0, NULL}
};

static NCS_RDR_INFO btn_rdr_info[] =
{
    {"fashion","fashion", NULL}
};

//START_OF_INITIAL_PROPS
static NCS_PROP_ENTRY combo_props[] =
{
    { 0, 0 }
};
//END_OF_INITIAL_PROPS

//START_OF_TEMPLATE
static NCS_WND_TEMPLATE _ctrl_templ[] =
{
    {
        NCSCTRL_STATIC,
        0,
        10, 40, 70, 30,
        WS_VISIBLE,
        WS_EX_NONE,
        "Name:",
        NULL,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_SLEDIT,
        ID_NAME,
        85, 45, 160, 25,
        WS_VISIBLE | WS_BORDER,
        WS_EX_NONE,
        "",
        combo_props,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_STATIC,
        0,
        10, 80, 70, 30,
        WS_VISIBLE,
        WS_EX_NONE,
        "Type:",
        NULL,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_COMBOBOX,
        ID_TYPE,
        85, 85, 160, 25,
        WS_VISIBLE | NCSS_CMBOX_DROPDOWNLIST,
        WS_EX_NONE,
        "",
        combo_props,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_STATIC,
        0,
        10, 120, 70, 30,
        WS_VISIBLE,
        WS_EX_NONE,
        "Location:",
        NULL,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_SLEDIT,
        ID_LOCA,
        85, 125, 160, 25,
        WS_VISIBLE | WS_BORDER,
        WS_EX_NONE,
        "",
        combo_props,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_BUTTON,
        ID_BTN,
        50, 170, 80, 25,
        WS_VISIBLE | NCSS_NOTIFY,
        WS_EX_NONE,
        "Finish",
        NULL,
        btn_rdr_info,
        btn_handlers, NULL, 0, 0
    },
    {
        NCSCTRL_BUTTON,
        ID_BTN,
        170, 170, 80, 25,
        WS_VISIBLE | NCSS_NOTIFY,
        WS_EX_NONE,
        "Cancel",
        NULL,
        btn_rdr_info,
        btn_handlers, NULL, 0, 0
    },

};
//END_OF_TEMPLATE

static NCS_MNWND_TEMPLATE mymain_templ =
{
    NCSCTRL_DIALOGBOX,
    1,
    0, 0, 320, 240,
    WS_CAPTION | WS_BORDER | WS_VISIBLE,
    WS_EX_NONE,
    "Combobox",
    NULL,
    NULL,
    mymain_handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,
    0, 0,
};

int MiniGUIMain (int argc, const char* argv[])
{
	if (argc > 1) {
		btn_rdr_info[0].glb_rdr = argv[1];
		btn_rdr_info[0].ctl_rdr = argv[1];
	}

    ncsInitialize ();

    mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                (&mymain_templ, HWND_DESKTOP);

    _c(mydlg)->doModal (mydlg, TRUE);

    ncsUninitialize ();

    return 0;
}

#else //_MGNCSCTRL_COMBOBOX _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the combobox dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-combobox --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_COMBOBOX _MGNCSCTRL_DIALOGBOX

