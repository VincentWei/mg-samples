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
** $Id: lf_tiny.c 582 2008-04-11 06:54:21Z zhounuohua $
**
** iconview.c: Sample program for MiniGUI Programming Guide
**      Usage of ICONVIEW control.
**
** Copyright (C) 2004 ~ 2008 FMSoft.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>

#ifdef _MGLF_RDR_TINY
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

extern int tiny_iconview (HWND hwnd);
HWND hwnd_my;

static int MainWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_PAINT:
            {
                HDC hdc;
                char* txt  = "MiniGUI Tiny Demo";
                RECT rc = {10, 80, 200, 300};
                int old_mode;

                hdc = BeginPaint (hWnd);
                old_mode = SetBkMode (hdc, BM_TRANSPARENT);
                DrawText (hdc, txt, -1, &rc, DT_CENTER | DT_WORDBREAK);
                SetBkMode (hdc, old_mode);
                EndPaint (hWnd, hdc);
            }
            return 0;
        case MSG_COMMAND:
            {
                int id = LOWORD(wParam);
                switch (id)
                {
                    case IDOK:
                        ShowWindow (hWnd, SW_HIDE);
                        tiny_iconview (hwnd_my);
                        break;
                    case IDCANCEL:
                        SendMessage (hWnd, MSG_CLOSE, 0, 0);
                        break;
                }
            }
            return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int CreateMyDesktop (void)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    CreateInfo.dwExStyle = WS_EX_TRANSPARENT | WS_EX_LFRDR_CUSTOM1 |
        WS_EX_LFRDR_CUSTOM2 | WS_EX_LFRDR_CUSTOM3;
    CreateInfo.spCaption = "lf_tiny";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = MainWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 240;
    CreateInfo.by = 320;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindowEx (&CreateInfo, "tiny",
            0, 0, 0);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    hwnd_my = hMainWnd;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

int MiniGUIMain (int argc, const char* argv[])
{

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "lf_tiny" , 0 , 0);
#endif

    CreateMyDesktop ();
    return 0;
}
#else
HWND hwnd_my;
int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please open the support for tiny LF in MiniGUI. \n");
    return 0;
}
#endif

#ifndef _MGRM_THREADS
#include <minigui/dti.c>
#endif
