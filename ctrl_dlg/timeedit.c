/*
** $Id: timeedit.c 492 2008-02-15 08:24:04Z zhounuohua $
**
** timeedit.c: A time editor, use SpinBox and Edit controls.
**
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

#define IDC_EDIT    100
#define IDC_SPINBOX 110

static HWND hMainWnd = HWND_INVALID;

static PLOGFONT timefont;

static WNDPROC old_edit_proc;

static void on_down_up (HWND hwnd, int offset)
{
    char time [10];
    int caretpos;
    int hour, minute, second;

    GetWindowText (hwnd, time, 8);
    caretpos = SendMessage (hwnd, EM_GETCARETPOS, 0, 0);

    hour = atoi (time);
    minute = atoi (time + 3);
    second = atoi (time + 6);

    if (caretpos > 5) { /* change second */
        second += offset;
        if (second < 0)
            second = 59;
        if (second > 59)
            second = 0;
    }
    else if (caretpos > 2) { /* change minute */
        minute += offset;
        if (minute < 0)
            minute = 59;
        if (minute > 59)
            minute = 0;
    }
    else { /* change hour */
        hour += offset;
        if (hour < 0)
            hour = 23;
        if (hour > 23)
            hour = 0;
    }

    sprintf (time, "%02d:%02d:%02d", hour, minute, second);
    SetWindowText (hwnd, time);
    SendMessage (hwnd, EM_SETCARETPOS, 0, caretpos);
}

static int TimeEditBox (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    if (message == MSG_KEYDOWN) {
        switch (wParam) {
        case SCANCODE_CURSORBLOCKUP:
            on_down_up (hwnd, 1);
            return 0;
        case SCANCODE_CURSORBLOCKDOWN:
            on_down_up (hwnd, -1);
            return 0;
        case SCANCODE_PAGEUP:
            on_down_up (hwnd, 10);
            return 0;
        case SCANCODE_PAGEDOWN:
            on_down_up (hwnd, -10);
            return 0;

        case SCANCODE_CURSORBLOCKLEFT:
        case SCANCODE_CURSORBLOCKRIGHT:
                break;
        default:
                return 0;
        }
    }

    if (message == MSG_KEYUP || message == MSG_CHAR)
        return 0;

    return (*old_edit_proc) (hwnd, message, wParam, lParam);
}


static int ControlTestWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
    {
        HDC hdc;
        HWND timeedit, spin;
        SIZE size;

        CreateWindow (CTRL_STATIC, 
                        time_editor,
                        WS_CHILD | WS_VISIBLE | SS_LEFT, 
                        IDC_STATIC, 
                        10, 10, 380, 200, hWnd, 0);

        timefont = CreateLogFont (NULL, "Arial", "ISO8859-1", 
                        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                        FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                        30, 0);

        hdc = GetClientDC (hWnd);
        SelectFont (hdc, timefont);
        GetTextExtent (hdc, "00:00:00", -1, &size);
        ReleaseDC (hdc);

        timeedit = CreateWindow (CTRL_SLEDIT, 
                        "00:00:00", 
                        WS_CHILD | WS_VISIBLE | ES_BASELINE, 
                        IDC_EDIT, 
                        120, 220, size.cx + 4, size.cy + 4, hWnd, 0);

        SetWindowFont (timeedit, timefont);
        old_edit_proc = SetWindowCallbackProc (timeedit, TimeEditBox);

        spin = CreateWindowEx (CTRL_SPINBOX, 
                        "", 
                        WS_CHILD | WS_VISIBLE, 
                        WS_EX_NONE,
                        IDC_SPINBOX, 
                        120 + size.cx + 6, 220, 
                        size.cy + 4, size.cy + 4, hWnd, 0);
        SendMessage (spin, SPM_SETTARGET, 0, timeedit);
        break;
    }

    case MSG_DESTROY:
        DestroyAllControls (hWnd);
        DestroyLogFont (timefont);
        hMainWnd = HWND_INVALID;
	return 0;

    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        MainWindowCleanup (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_IMECOMPOSE;
    pCreateInfo->spCaption = time_caption;
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

void timeeditor (HWND hwnd)
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

