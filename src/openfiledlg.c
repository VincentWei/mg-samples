/* 
** $Id: openfiledlg.c 770 2009-12-24 03:04:19Z wanzheng $
**
** openfiledlg.c: moved to mGUtils component.
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
#include <minigui/mgext.h>
#include <minigui/newfiledlg.h>

#define IDC_OPENDLG     140
#define IDC_SAVEDLG     150

static int retvalue;

static NEWFILEDLGDATA newfiledlg =
{
    FALSE,
    "",
    "",
    "./",
    "All file(*.*)|Text file(*.txt)|Bitmap file(*.bmp)",
    1
};

static void create_control(HWND hwnd)
{
    CreateWindow (CTRL_BUTTON, "Open ...", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
                  IDC_OPENDLG, 30, 10, 100, 30, hwnd, 0);
    CreateWindow (CTRL_BUTTON, "Save ...", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                  IDC_SAVEDLG, 150, 10, 100, 30, hwnd, 0);
    CreateWindow (CTRL_STATIC, "", WS_CHILD | SS_NOTIFY | WS_VISIBLE, IDC_STATIC,
                  10, 80, 200, 100, hwnd, 0);
}

static int ToolBarWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        create_control (hWnd);
        break;
    case MSG_COMMAND:
    {
        char retmessage[100];
       
        switch (wParam) {
        case IDC_OPENDLG:
            newfiledlg.IsSave = 0;
            retvalue = ShowOpenDialog (hWnd, 100, 100, 350, 250, &newfiledlg);
            strcpy (retmessage, "Open file: ");
            strcat (retmessage, newfiledlg.filefullname);
            if (retvalue == IDOK)
                SetWindowText (GetDlgItem (hWnd, IDC_STATIC), retmessage);
            else if (retvalue == IDCANCEL)
                SetWindowText (GetDlgItem (hWnd, IDC_STATIC), "Canceled");
            else if (retvalue == FILE_ERROR_PARAMERR)
                SetWindowText (GetDlgItem (hWnd, IDC_STATIC), "File not found");
            break;
        case IDC_SAVEDLG:
            newfiledlg.IsSave = 1;
            retvalue = ShowOpenDialog (hWnd, 100, 100, 350, 250, &newfiledlg);
            strcpy (retmessage, "Save file: ");
            strcat (retmessage, newfiledlg.filefullname);
            if (retvalue == IDOK)
                SetWindowText (GetDlgItem (hWnd, IDC_STATIC), retmessage);
            else if (retvalue == IDCANCEL)
                SetWindowText (GetDlgItem (hWnd, IDC_STATIC), "Canceled");
            break;
        }
        break;
    }
    case MSG_DESTROY:
        DestroyAllControls (hWnd);
        return 0;
    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "openfiledlg" , 0 , 0);
#endif

    if (!InitMiniGUIExt ()) {
        return 2;
    }

    CreateInfo.dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "File Dialog";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ToolBarWinProc;
    CreateInfo.lx = 0; 
    CreateInfo.ty = 0;
    CreateInfo.rx = 300;
    CreateInfo.by = 220;
    CreateInfo.iBkColor = GetWindowElementColor (BKC_CONTROL_DEF);
    CreateInfo.dwAddData = 0;
    CreateInfo.dwReserved = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    MiniGUIExtCleanUp ();

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
