/*
** $Id: trackbar.c 759 2009-09-21 05:22:47Z dongjunjie $
**
** trackbar.c: The TrackBar control demo program.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
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

#ifdef _MGCTRL_TRACKBAR

#define IDC_TRACKBAR1     100
#define IDC_TRACKBAR2     101

static HWND hMainWnd = HWND_INVALID;
static HWND hTrackWnd1, hTrackWnd2, hShowWnd;
static int MaxLength = 1000;
static int CurrentPos = 0;
static char CurPosBuffer[16];

static void TrackbarChanged(int code, HWND ctrl)
{
	switch (code)
	{
	    case TBN_REACHMAX:
	    	CurrentPos = MaxLength;
	    	sprintf(CurPosBuffer, "%d", CurrentPos);
            SendMessage (hShowWnd,  MSG_SETTEXT, 0, (LPARAM)CurPosBuffer);
	    break;

	    case TBN_REACHMIN:
	    	CurrentPos = 0;
	    	sprintf(CurPosBuffer, "%d", CurrentPos);
            SendMessage (hShowWnd,  MSG_SETTEXT, 0, (LPARAM)CurPosBuffer);
	    break;

	    case TBN_CHANGE:
	    	CurrentPos = SendMessage (hTrackWnd1, TBM_GETPOS, 0, 0);
	    	sprintf(CurPosBuffer, "%d", CurrentPos);
            SendMessage (hShowWnd,  MSG_SETTEXT, 0, (LPARAM)CurPosBuffer);
	    break;

	    default:
	    break;
	}
}

static int ControlTestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
            hShowWnd = CreateWindow (CTRL_STATIC, 
                              "", 
                              WS_VISIBLE, 
                              IDC_STATIC, 
                              10, 20, 60, 20, hWnd, 0);

            hTrackWnd1 = CreateWindow (CTRL_TRACKBAR, 
                              "", 
                              WS_VISIBLE | TBS_NOTIFY, 
                              IDC_TRACKBAR1, 
                              10, 40, 180, 100, hWnd, 0);
            SendMessage (hTrackWnd1, TBM_SETRANGE, 0, 1000);
            SendMessage (hTrackWnd1, TBM_SETLINESIZE, 1, 0);
            SendMessage (hTrackWnd1, TBM_SETPAGESIZE, 10, 0);
            SendMessage (hTrackWnd1, TBM_SETTICKFREQ, 1000, 0);

            hTrackWnd2 = CreateWindow (CTRL_TRACKBAR, 
                              "", 
                              WS_VISIBLE | TBS_NOTIFY | TBS_TIP, 
                              IDC_TRACKBAR1 + 1, 
                              200, 40, 180, 50, hWnd, 0);
            SendMessage (hTrackWnd2, TBM_SETTIP, (WPARAM)"Min", (LPARAM)"Max");

            hTrackWnd2 = CreateWindow (CTRL_TRACKBAR, 
                              "", 
                              WS_VISIBLE | TBS_NOTIFY | TBS_VERTICAL, 
                              IDC_TRACKBAR1 + 2, 
                              10, 100, 50, 200, hWnd, 0);
            CreateWindow (CTRL_TRACKBAR, 
                              "", 
                              WS_VISIBLE | TBS_NOTIFY | TBS_VERTICAL | TBS_NOTICK, 
                              IDC_TRACKBAR1 + 3, 
                              200, 100, 50, 200, hWnd, 0);

            SendMessage (hTrackWnd2, TBM_SETRANGE, 0, 10000);
            SendMessage (hTrackWnd2, TBM_SETLINESIZE, 1, 0);
            SendMessage (hTrackWnd2, TBM_SETPAGESIZE, 10, 0);
            SendMessage (hTrackWnd2, TBM_SETTICKFREQ, 10000, 0);

            CurrentPos = 0;
            sprintf (CurPosBuffer, "%d", CurrentPos);
            SendMessage (hShowWnd,  MSG_SETTEXT, 0, (LPARAM)CurPosBuffer);
        break;

        case MSG_COMMAND:
        {
            int id   = LOWORD(wParam);
            int code = HIWORD(wParam);
            HWND ctrl = (HWND)(lParam);
            char buffer [256];
            HDC hdc;

            sprintf (buffer, "ID: %d, Code: %x", id, code);

            hdc = GetClientDC (hWnd);
            TextOut (hdc, 0, 0, buffer);
            ReleaseDC (hdc);

            if (id == IDC_TRACKBAR1)
                TrackbarChanged(code, ctrl);
        }
        break;

	case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
	    return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;

        default:
            break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = "TrackBar controls" ;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(IDC_ARROW);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 460;
    pCreateInfo->by = 380;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->dwReserved = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void trackbar_demo (HWND hwnd)
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

#else /* _MGCTRL_TRACKBAR */

void trackbar_demo (HWND hwnd)
{
    MessageBox (hwnd, "WARNING", "Please enable TRACKBAR control support in MiniGUI.", MB_OK);
}

#endif /* _MGCTRL_TRACKBAR */
