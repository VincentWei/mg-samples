/*
** $Id: coolbar.c 557 2008-03-04 08:41:17Z malin $
**
** coolbar.c: The CoolBar control demo program.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2001/11/01
*/

/*
**  This source is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public
**  License as published by the Free Software Foundation; either
**  version 2 of the License, or (at your option) any later version.
**
**  This software is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public
**  License along with this library; if not, write to the Free
**  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
**  MA 02111-1307, USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_COOLBAR1     110
#define IDC_COOLBAR2     120
#define IDC_COOLBAR3     130
#define IDC_COOLBAR4     140

#define IDC_BAR         500
#define IDC_BARCOPY     501
#define IDC_BARCUT      502
#define IDC_BARDEL      503
#define IDC_BARPASTE    504
#define IDC_BAREXE      505
#define IDC_SLEPRO      506

#define NM_BARPIC 6

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

static HWND hMainWnd = HWND_INVALID;

static char * barpic[] =
{
 "res/copy.bmp",
 "res/cut.bmp",
 "res/delete.bmp",
 "res/paste.bmp",
 "res/exepro.bmp",
 "res/closefolder.bmp"
};

static char * barhint[] =
{
 "Copy",
 "Cut",
 "Delete",
 "Paste",
 "Execute",
 "Close"
};

static BITMAP bmps [6];

static int ControlTestWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
    {
        int i;
        COOLBARITEMINFO cbii;
        HWND coolbar1, coolbar2, coolbar3, coolbar4;

        coolbar1 = CreateWindow (CTRL_COOLBAR,
                              "",
                              WS_CHILD | WS_VISIBLE | CBS_BMP_CUSTOM,
                              IDC_COOLBAR1,
                              0, 0,
                              52 * (NM_BARPIC + 1), 0,
                              hWnd, MAKELONG (48, 48));
           
        coolbar2 = CreateWindow (CTRL_COOLBAR,
                              "",
                              WS_CHILD | WS_VISIBLE | CBS_BMP_32X32,
                              IDC_COOLBAR2,
                              0, 100,
                              36 * (NM_BARPIC + 1), 0, hWnd, 0);

        coolbar3 = CreateWindow (CTRL_COOLBAR,
                              "",
                              WS_CHILD | WS_VISIBLE,
                              IDC_COOLBAR3,
                              0, 230,
                              20 * (NM_BARPIC + 1), 0, hWnd, 0);
           
        coolbar4 = CreateWindow (CTRL_COOLBAR,
                              "res/bkgnd.gif",
                              WS_CHILD | WS_VISIBLE | CBS_USEBKBMP | CBS_BMP_CUSTOM,
                              IDC_COOLBAR4,
                              0, 170,
                              240, 0, hWnd, MAKELONG (20, 20));
           
        for (i = 0; i < NM_BARPIC; i++) {
            LoadBitmapFromFile (HDC_SCREEN, bmps+i, barpic[i]);
            cbii.id = IDC_BAR + i + 1;
            cbii.ItemType = TYPE_BMPITEM;
            cbii.Bmp = bmps + i;
            cbii.ItemHint = barhint[i];
            cbii.Caption = NULL;
            cbii.dwAddData = 0;
            SendMessage (coolbar1, CBM_ADDITEM, 0, (LPARAM)&cbii);
            SendMessage (coolbar2, CBM_ADDITEM, 0, (LPARAM)&cbii);
            SendMessage (coolbar3, CBM_ADDITEM, 0, (LPARAM)&cbii);

            if (i == 3) {
                cbii.ItemType = TYPE_BARITEM;
                SendMessage (coolbar4, CBM_ADDITEM, 0, (LPARAM)&cbii);
            }

            if (i >= 3) {
                cbii.ItemType = TYPE_TEXTITEM;
                cbii.Bmp = NULL;
                cbii.ItemHint = barhint[i];
                cbii.Caption = barhint[i];
                cbii.dwAddData = 0;
            }

            SendMessage (coolbar4, CBM_ADDITEM, 0, (LPARAM)&cbii);
        }
        break;
    }
       	
    case MSG_COMMAND:
    {
#if 0
        int id = LOWORD (wParam);
        int nc = HIWORD (wParam);
#endif

        break;
    }

    case MSG_DESTROY:
    {
        int i;

        for (i = 0; i < 6; i++)
            UnloadBitmap (bmps+i);

        hMainWnd = HWND_INVALID;
        return 0;
    }

    case MSG_CLOSE:
        DestroyAllControls (hWnd);
        DestroyMainWindow (hWnd);
        MainWindowCleanup (hWnd);
        return 0;

    default:
        break;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER|WS_VISIBLE;  
    pCreateInfo->dwExStyle = WS_EX_IMECOMPOSE;
    pCreateInfo->spCaption = CoolBar_control;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 400;
    pCreateInfo->by = 300;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void coolbar_demo (HWND hwnd)
{
	MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

	InitCreateInfo(&CreateInfo);
    CreateInfo.hHosting = hwnd;

	hMainWnd = CreateMainWindow(&CreateInfo);
}

