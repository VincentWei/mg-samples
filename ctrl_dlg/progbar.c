/*
** $Id: progbar.c 434 2008-02-03 07:30:11Z mgwang $
**
** progbar.c: The ProgressBar control demo program.
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

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

#define IDC_CTRL1     100
#define IDC_CTRL2     110
#define IDC_CTRL3     120
#define IDC_CTRL4     130

static HWND hMainWnd = HWND_INVALID;

static int ControlTestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HWND hChildWnd1, hChildWnd2, hChildWnd3, hChildWnd4, hChildWnd5;
    static int pos;

    switch (message) {
        case MSG_CREATE:
            hChildWnd1 = CreateWindow ("progressbar", 
                              "", 
                              WS_VISIBLE, 
                              IDC_CTRL1, 
                              10, 20, 100, 20, hWnd, 0);
            hChildWnd4 = CreateWindow ("progressbar", 
                              "", 
                              WS_VISIBLE, 
                              IDC_CTRL4, 
                              120, 20, 150, 20, hWnd, 0);
            SendMessage (hChildWnd4, PBM_SETRANGE, 0, 1000);
            SendMessage (hChildWnd4, PBM_SETSTEP, 10, 0);

            hChildWnd2 = CreateWindow ("progressbar", 
                              NULL, 
                              WS_VISIBLE, 
                              IDC_CTRL2, 
                              10, 50, 260, 20, hWnd, 0);

            hChildWnd3 = CreateWindow ("progressbar", 
                              NULL, 
                              WS_VISIBLE | PBS_VERTICAL | PBS_NOTIFY,
                              IDC_CTRL3,
                              10, 80, 20, 120, hWnd, 0);

            hChildWnd5 = CreateWindow ("progressbar", 
                              NULL, 
                              WS_VISIBLE | PBS_VERTICAL | PBS_NOTIFY,
                              IDC_CTRL3,
                              120, 80, 20, 120, hWnd, 0);
            SendMessage (hChildWnd5, PBM_SETRANGE, 0, 1000);
            SendMessage (hChildWnd5, PBM_SETSTEP, 10, 0);

            CreateWindow (CTRL_BUTTON, 
                          Close, 
                          WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 
                          IDCANCEL, 
                          200, 170, 70, 30, hWnd, 0);

            pos = 0;
            SetTimer (hWnd, 100, 10);
        break;

        case MSG_TIMER:
            pos += 4;
            SendMessage (hChildWnd1, PBM_SETPOS, pos, 0);
            SendMessage (hChildWnd2, PBM_DELTAPOS, 10, 0);
            SendMessage (hChildWnd3, PBM_DELTAPOS, 2, 0);
            SendMessage (hChildWnd4, PBM_STEPIT, 0, 0);
            SendMessage (hChildWnd5, PBM_STEPIT, 0, 0);
        break;

        case MSG_COMMAND:
        {
            int id   = LOWORD(wParam);
            int code = HIWORD(wParam);
            char buffer [256];
            HDC hdc;

            if (wParam == IDCANCEL) {
                PostMessage (hWnd, MSG_CLOSE, 0, 0L);
                break;
            }

            sprintf (buffer, "ID: %d, Code: %x", id, code);

            hdc = GetClientDC (hWnd);
            TextOut (hdc, 0, 0, buffer);
            ReleaseDC (hdc);
        }
        break;
        
	case MSG_DESTROY:
            KillTimer (hWnd, 100);
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
	    return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = ProgressBar_controls ;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(IDC_ARROW);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 300;
    pCreateInfo->by = 230;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void progressbar_demo (HWND hwnd)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hwnd;

    hMainWnd = CreateMainWindow (&CreateInfo);
}

