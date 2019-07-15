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
** Listing P2C1.5
**
** data_binding.c: Sample program for mGNCS Programming Guide
**      Using data binding.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>

#if defined (_MGNCSCTRL_DIALOGBOX) && (_MGNCSCTRL_TRACKBAR)

#define IDC_TRACKBAR     101
#define IDC_SLEDIT       102

// START_OF_BINDING
static BOOL mymain_onCreate (mWidget* self, DWORD add_data)
{
    mTrackBar * tb = (mTrackBar*)_c(self)->getChild (self, IDC_TRACKBAR);
    mSlEdit   * se = (mSlEdit*) _c(self)->getChild (self, IDC_SLEDIT);

    ncsConnectBindProps (NCS_CMPT_PROP (tb, NCSN_TRKBAR_CHANGED,
            NCSP_TRKBAR_CURPOS, NCS_BT_INT,
            NCS_PROP_FLAG_READ|NCS_PROP_FLAG_WRITE),
        NCS_CMPT_PROP (se, NCSN_EDIT_CHANGE,
            NCSP_WIDGET_TEXT, NCS_BT_STR,
            NCS_PROP_FLAG_READ|NCS_PROP_FLAG_WRITE),
        NCS_BPT_DBL);

    ncsAutoReflectObjectBindProps ((mObject*)se);
    return TRUE;
}
// END_OF_BINDING

static void mymain_onClose (mWidget* self, int message)
{
    DestroyMainWindow (self->hwnd);
    PostQuitMessage (self->hwnd);
}

static NCS_EVENT_HANDLER mymain_handlers[] = {
    {MSG_CREATE, mymain_onCreate },
    {MSG_CLOSE, mymain_onClose },
    {0, NULL }
};

static NCS_PROP_ENTRY trackbar_props [] = {
    {NCSP_TRKBAR_MINPOS, 0},
    {NCSP_TRKBAR_MAXPOS, 20},
    {NCSP_TRKBAR_CURPOS, 10},
    {NCSP_TRKBAR_LINESTEP, 2},
    {NCSP_TRKBAR_PAGESTEP, 5},
    {0, 0}
};

static NCS_RDR_INFO trackbar_rdr_info[] =
{
    {"flat", "flat", NULL},
};

static NCS_WND_TEMPLATE _ctrl_templ[] = {
    {
        NCSCTRL_TRACKBAR,
        IDC_TRACKBAR,
        10, 10, 240, 40,
        WS_BORDER | WS_VISIBLE | NCSS_TRKBAR_NOTICK | NCSS_NOTIFY,
        WS_EX_TRANSPARENT,
        "",
        trackbar_props,
        trackbar_rdr_info,
        NULL, NULL, 0, 0,
        MakeRGBA(255,0,0,255)
    },
    {
        NCSCTRL_SLEDIT,
        IDC_SLEDIT,
        10, 60, 240, 30,
        WS_BORDER | WS_VISIBLE | NCSS_EDIT_CENTER | NCSS_NOTIFY,
        WS_EX_NONE,
        "edit",
        NULL, NULL, NULL, NULL, 0, 0
    },
};

static NCS_MNWND_TEMPLATE mymain_templ = {
    NCSCTRL_DIALOGBOX,
    1,
    0, 0, 268, 130,
    WS_CAPTION | WS_BORDER | WS_VISIBLE,
    WS_EX_NONE,
    "Data Binding",
    NULL,
    NULL,
    mymain_handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0,
    MakeRGBA(255,255,255,255)
};

int MiniGUIMain(int argc, const char* argv[])
{
    mDialogBox* mydlg;

    if (argc > 1) {
        trackbar_rdr_info[0].glb_rdr = argv[1];
        trackbar_rdr_info[0].ctl_rdr = argv[1];
    }

    ncsInitialize ();

    mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

    _c(mydlg)->doModal (mydlg, TRUE);

    ncsUninitialize ();

    return 0;
}
#else //defined _MGNCSCTRL_DIALOGBOX && _MGNCSCTRL_TRACKBAR

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the dialogbox, trackbar contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-dialogbox --enable-trackbar==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//

