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
** Listing P2C11.1
**
** propsheet.c: Sample program for mGNCS Programming Guide
**      The demo application for PropSheet.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// START_OF_INCS
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgncs/mgncs.h>
// END_OF_INCS

#if defined _MGNCSCTRL_PROPSHEET && defined _MGNCSCTRL_DIALOGBOX

#define PAGE_VERSION	1
#define PAGE_CPU        2
#define PAGE_MEMINFO	3
#define PAGE_PARTITION  4
#define PAGE_MINIGUI    5

#define BUF_LEN         10240
#define IDC_PROPSHEET	100
#define IDC_SYSINFO     101
#define IDC_REFRESH     102

static size_t read_sysinfo (const char* file, char* buff, size_t buf_len)
{
     size_t size;
     FILE   *fp = fopen (file, "r");

     if (fp == NULL) return 0;

     size = fread (buff, 1, buf_len, fp);

     fclose (fp);
     return size;
}

static void get_systeminfo (mPage* page)
{
    int     type;
    HWND    hwnd;
    char    buff [BUF_LEN + 1];
    size_t  size = 0;

    type = (int)GetWindowAdditionalData (page->hwnd);
	hwnd = GetDlgItem(_c(page)->getPanel(page), IDC_SYSINFO);
    buff [BUF_LEN] = 0;

    switch (type) {
    case PAGE_VERSION:
	    size = read_sysinfo ("/proc/version", buff, BUF_LEN);
        buff [size] = 0;
	    break;

    case PAGE_CPU:
	    size = read_sysinfo ("/proc/cpuinfo", buff, BUF_LEN);
    	    buff [size] = 0;
	    break;

    case PAGE_MEMINFO:
	    size = read_sysinfo ("/proc/meminfo", buff, BUF_LEN);
    	    buff [size] = 0;
	    break;

    case PAGE_PARTITION:
	    size = read_sysinfo ("/proc/partitions", buff, BUF_LEN);
        buff [size] = 0;
	    break;

    case PAGE_MINIGUI:
	    size = snprintf (buff, BUF_LEN,
                    "MiniGUI version %d.%d.%d.\n"
                    "Copyright (C) 1998-2009 Feynman Software.\n\n"
                    "MiniGUI is a mature, rich-featured, embedded "
                    "GUI support system.\n\n"
                    "For more information, please visit\n\n"
                    "http://www.minigui.com\n",
                    MINIGUI_MAJOR_VERSION, MINIGUI_MINOR_VERSION, MINIGUI_MICRO_VERSION);
	    break;
    }

    if (size) {
	    SetWindowText (hwnd, buff);
    }
    GetWindowText(hwnd, buff, BUF_LEN+1);
}

// START_OF_PAGEHANDLERS
static void page_onInitPage(mWidget* self, DWORD add_data)
{
	get_systeminfo ((mPage*)self);
}

static int page_onShowPage(mWidget* self, HWND hwnd, int show_cmd)
{
    return 1;
}

static int page_onSheetCmd(mWidget* self, DWORD wParam, DWORD lParam)
{
    if (wParam == IDC_REFRESH) {
        get_systeminfo ((mPage*)self);
    }

    return 0;
}

static NCS_EVENT_HANDLER page_handlers[] = {
	{MSG_INITPAGE, page_onInitPage},
	{MSG_SHOWPAGE, page_onShowPage},
	{MSG_SHEETCMD, page_onSheetCmd},
	{0 , NULL }
};
// END_OF_PAGEHANDLERS

static void btn_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    mPropSheet *obj =
        (mPropSheet *)ncsGetChildObj(GetParent(self->hwnd), IDC_PROPSHEET);

    if (obj) {
        _c(obj)->broadCastMsg(obj, IDC_REFRESH, 0);
    }
}

static NCS_EVENT_HANDLER btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_BUTTON_PUSHED, btn_notify),
	{0, NULL}
};

static NCS_RDR_INFO btn_rdr_info[] =
{
	{"classic","classic", NULL}
};

static NCS_WND_TEMPLATE _ctrl_tmpl[] = {
	{
		NCSCTRL_BUTTON,
        IDC_REFRESH,
        10, 240, 70, 25,
        WS_VISIBLE | WS_TABSTOP,
		WS_EX_NONE,
        "Refresh",
		NULL,
		btn_rdr_info,
		btn_handlers,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_BUTTON,
        IDCANCEL,
        330, 240, 70, 25,
        WS_VISIBLE | WS_TABSTOP,
		WS_EX_NONE,
        "Close",
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		0
	},
};

static DLGTEMPLATE PageSysInfo =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 0, 0,
    "",
    0, 0,
    1, NULL,
    0
};

static CTRLDATA CtrlSysInfo [] =
{
    {
        CTRL_STATIC,
        WS_VISIBLE | SS_LEFT,
        10, 10, 370, 180,
        IDC_SYSINFO,
        "test",
        0
    }
};

static NCS_RDR_INFO prop_rdr_info[] =
{
    {"classic", "classic", NULL},
};

static int init_propsheet (mDialogBox* self)
{
// START_OF_CREATEPRPSHT
    mPropSheet *propsheet =
        (mPropSheet*) ncsCreateWindow (NCSCTRL_PROPSHEET,
                       "", WS_VISIBLE | NCSS_PRPSHT_SCROLLABLE, WS_EX_NONE,
                       IDC_PROPSHEET,
                       10, 10, 390, 225, self->hwnd,
                       NULL, prop_rdr_info, NULL, 0);
// END_OF_CREATEPRPSHT

    if (!propsheet) {
        fprintf (stderr, "Error> Create propsheet failure.\n");
        return 1;
    }

// START_OF_ADDPAGES
    PageSysInfo.controls = CtrlSysInfo;
    PageSysInfo.caption = "Version Info";
    PageSysInfo.dwAddData = PAGE_VERSION;
    _c(propsheet)->addPage(propsheet, &PageSysInfo, page_handlers);

    PageSysInfo.caption = "CPU Info";
    PageSysInfo.dwAddData = PAGE_CPU;
    _c(propsheet)->addPage(propsheet, &PageSysInfo, page_handlers);

    PageSysInfo.caption = "MEM Info";
    PageSysInfo.dwAddData = PAGE_MEMINFO;
    _c(propsheet)->addPage(propsheet, &PageSysInfo, page_handlers);

    PageSysInfo.caption = "Partition Info";
    PageSysInfo.dwAddData = PAGE_PARTITION;
    _c(propsheet)->addPage(propsheet, &PageSysInfo, page_handlers);

    PageSysInfo.caption = "MiniGUI Info";
    PageSysInfo.dwAddData = PAGE_MINIGUI;
    _c(propsheet)->addPage(propsheet, &PageSysInfo, page_handlers);
// END_OF_ADDPAGES

    return 0;
}

static NCS_MNWND_TEMPLATE dialog_tmpl = {
	NCSCTRL_DIALOGBOX,
	1,
	0, 0, 420, 305,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "PropSheet Demo",
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
	if (argc > 1) {
		btn_rdr_info[0].glb_rdr = argv[1];
		btn_rdr_info[0].ctl_rdr = argv[1];
	}

	ncsInitialize();
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect
                                (&dialog_tmpl, HWND_DESKTOP);

    init_propsheet(mydlg);
	_c(mydlg)->doModal(mydlg, TRUE);

	ncsUninitialize();
	return 0;
}

#else //_MGNCSCTRL_PROPSHEET _MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the propsheet, dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-propsheet --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_PROPSHEET _MGNCSCTRL_DIALOGBOX
