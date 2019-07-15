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
** $Id: propsheet.c 759 2009-09-21 05:22:47Z dongjunjie $
**
** Listing 28.1
**
** propsheet.c: Sample program for MiniGUI Programming Guide
**      Usage of PORPSHEET control.
**
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_PROPSHEET

#define PAGE_VERSION	1
#define PAGE_CPU	2
#define PAGE_MEMINFO	3
#define PAGE_PARTITION  4
#define PAGE_MINIGUI    5

#define IDC_PROPSHEET	100

#define IDC_SYSINFO	100

#ifdef _LANG_ZHCN
#include "propsheet_res_cn.h"
#elif defined _LANG_ZHTW
#include "propsheet_res_tw.h"
#else
#include "propsheet_res_en.h"
#endif

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
        WS_VISIBLE | SS_LEFT | WS_BORDER, 
        10, 10, 370, 160,
        IDC_SYSINFO,
        "",
        0
    }
};

static size_t read_sysinfo (const char* file, char* buff, size_t buf_len)
{
     size_t size;
     FILE* fp = fopen (file, "r");

     if (fp == NULL) return 0;

     size = fread (buff, 1, buf_len, fp);

     fclose (fp);
     return size;
}

#define BUF_LEN 10240

static void get_systeminfo (HWND hDlg)
{
    int type;
    HWND hwnd;
    char buff [BUF_LEN + 1];
    size_t size = 0;

    type = (int)GetWindowAdditionalData (hDlg);
    hwnd = GetDlgItem (hDlg, IDC_SYSINFO);

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
		"Copyright (C) 1998-2018 FMSoft.\n\n"
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
}

static LRESULT SysInfoPageProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITPAGE:
	get_systeminfo (hDlg);
        break;

    case MSG_SHOWPAGE:
        return 1;

    case MSG_SHEETCMD:
	if (wParam == IDOK) 
	    get_systeminfo (hDlg);
        return 0;
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static LRESULT PropSheetProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
        {
            HWND pshwnd = GetDlgItem (hDlg, IDC_PROPSHEET);

            PageSysInfo.controls = CtrlSysInfo;
	    PageSysInfo.caption = version_info;
			PageSysInfo.dwAddData = PAGE_VERSION;
            SendMessage (pshwnd, PSM_ADDPAGE, (WPARAM)&PageSysInfo, (LPARAM)SysInfoPageProc);
	    PageSysInfo.caption = CPU_info;
	    PageSysInfo.dwAddData = PAGE_CPU;
            SendMessage (pshwnd, PSM_ADDPAGE, (WPARAM)&PageSysInfo, (LPARAM)SysInfoPageProc);
	    PageSysInfo.caption = mem_info;
	    PageSysInfo.dwAddData = PAGE_MEMINFO;
            SendMessage (pshwnd, PSM_ADDPAGE, (WPARAM)&PageSysInfo, (LPARAM)SysInfoPageProc);
	    PageSysInfo.caption = partition_info;
	    PageSysInfo.dwAddData = PAGE_PARTITION;
            SendMessage (pshwnd, PSM_ADDPAGE, (WPARAM)&PageSysInfo, (LPARAM)SysInfoPageProc);
	    PageSysInfo.caption = MiniGUI_info;
	    PageSysInfo.dwAddData = PAGE_MINIGUI;
            SendMessage (pshwnd, PSM_ADDPAGE, (WPARAM)&PageSysInfo, (LPARAM)SysInfoPageProc);
            break;
        }

        case MSG_COMMAND:
        switch (wParam) {
            case IDOK:
                SendDlgItemMessage (hDlg, IDC_PROPSHEET, PSM_SHEETCMD, IDOK, 0);
                break;

            case IDCANCEL:
                EndDialog (hDlg, wParam);
            break;
        }
        break;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static DLGTEMPLATE DlgPropSheet =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 410, 285,
    System_info,
    0, 0,
    3, NULL,
    0
};

static CTRLDATA CtrlPropSheet[] =
{ 
    {
        CTRL_PROPSHEET,
        WS_VISIBLE | PSS_COMPACTTAB, 
        10, 10, 390, 200,
        IDC_PROPSHEET,
        "",
        0
    },
    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP,
        10, 220, 70, 25,
        IDOK, 
        Refresh,
        0
    },
    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        330, 220, 70, 25,
        IDCANCEL,
        Close,
        0
    },
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "propsheet" , 0 , 0);
#endif

    DlgPropSheet.controls = CtrlPropSheet;
    
    DialogBoxIndirectParam (&DlgPropSheet, HWND_DESKTOP, PropSheetProc, 0L);

    return 0;
}

#else /* _MGCTRL_PROPSHEET */

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for PROPSHEET control in MiniGUI.\n");
    return 0;
}

#endif /* _MGCTRL_PROPSHEET */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

