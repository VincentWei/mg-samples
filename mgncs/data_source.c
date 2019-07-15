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
** Listing P2C1.6
**
** data_source.c: Sample program for mGNCS Programming Guide
**      Using static data source.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>

#if defined (_MGNCSCTRL_LISTVIEW) && defined (_MGNCSCTRL_DIALOGBOX) && defined (_MGNCSCTRL_TRACKBAR) && defined _MGNCSDB_DATASOURCE


#define IDC_LISTVIEW 100

// START_OF_DEFINEDS
static const NCS_LISTV_CLMRD _header[] = {
    {"Name", "", 100, NCSF_LSTCLM_LEFTALIGN},
    {"Sex", "", 80, NCSF_LSTCLM_LEFTALIGN},
    {"Age", "", 80, NCSF_LSTCLM_LEFTALIGN}
};

static const char* _content[][3] = {
    {"Jime", "Male", "15"},
    {"Lily", "Female", "12"},
    {"Tom", "Male", "11"}
};
// END_OF_DEFINEDS

// START_OF_SETDATA
static BOOL mymain_onCreate (mWidget* self, DWORD add_data)
{
    mListView *lv = (mListView*)_c(self)->getChild (self, IDC_LISTVIEW);

    if (lv) {
        mRecordSet *rs;
        rs = _c(g_pStaticDS)->selectRecordSet (g_pStaticDS,
                    "/listview/header", NCS_DS_SELECT_READ);
        _c(lv)->setSpecificData (lv, NCSSPEC_LISTV_HDR, (DWORD)rs, NULL);
        rs = _c(g_pStaticDS)->selectRecordSet (g_pStaticDS,
                    "/listview/content", NCS_DS_SELECT_READ);
        _c(lv)->setSpecificData (lv, NCSSPEC_OBJ_CONTENT, (DWORD)rs, NULL);
    }

    return TRUE;
}
// END_OF_SETDATA

static void mymain_onClose (mWidget* self, int message)
{
    DestroyMainWindow (self->hwnd);
    PostQuitMessage (self->hwnd);
}

static NCS_EVENT_HANDLER mymain_handlers[] = {
    {MSG_CREATE, mymain_onCreate},
    {MSG_CLOSE, mymain_onClose},
    {0, NULL}
};

static NCS_WND_TEMPLATE _ctrl_templ[] = {
    {
        NCSCTRL_LISTVIEW,
        IDC_LISTVIEW,
        10, 10, 320, 190,
        WS_BORDER | WS_VISIBLE | NCSS_LISTV_SORT
            | NCSS_LISTV_LOOP,
        WS_EX_NONE,
        "",
        NULL, NULL, NULL, NULL, 0, 0
    }
};

static NCS_MNWND_TEMPLATE mymain_templ = {
    NCSCTRL_MAINWND,
    1,
    0, 0, 350, 240,
    WS_CAPTION | WS_BORDER | WS_VISIBLE,
    WS_EX_NONE,
    "Data Source ....",
    NULL,
    NULL,
    mymain_handlers,
    _ctrl_templ,
    sizeof(_ctrl_templ)/sizeof(NCS_WND_TEMPLATE),
    0, 0, 0,
};

int MiniGUIMain (int argc, const  char* argv[])
{
    mDialogBox* mydlg;

    ncsInitialize ();

// START_OF_REGDS
    ncsRegisterStaticData ("/listview/header", (void*)_header, 3,
            sizeof(NCS_LISTV_CLMRD)/sizeof(DWORD), sizeof(DWORD),
            gListVColumnRecordTypes);
    ncsRegisterStaticData ("/listview/content", (void*)_content, 3,
            3, sizeof(char*), NULL);
// END_OF_REGDS

    mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

    _c(mydlg)->doModal (mydlg, TRUE);

    ncsUninitialize ();

    return 0;
}

#else //_MGNCSCTRL_LISTVIEW && defined _MGNCSCTRL_DIALOGBOX && _MGNCSCTRL_TRACKBAR

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the listview, dialogbox, trackbar contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-listview --enable-dialogbox --enable-trackbar==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//

