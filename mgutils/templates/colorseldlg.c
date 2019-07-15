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
 *   This file is part of mGUtils, a component for MiniGUI.
 * 
 *   Copyright (C) 2003~2018, Beijing FMSoft Technologies Co., Ltd.
 *   Copyright (C) 1998~2002, WEI Yongming
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
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h> 
#include <minigui/minigui.h> 
#include <minigui/gdi.h> 
#include <minigui/window.h> 
#include <minigui/control.h>

#ifdef __MGUTILS_LIB__
#include "../../include/mgutils.h"
#else
#include <mgutils/mgutils.h>
#endif

#ifdef _MGUTILS_DLGCOLORSEL

#define DEFAULT_WIDTH       600
#define DEFAULT_HEIGHT      400

#define IDC_BUTTON_COLOR1    100
#define IDC_BUTTON_COLOR2    101

#define IDC_LABEL_INFO      100

static LRESULT ColorHookProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
            {
                /* Set custom color 0 */
                GetDlgItem(hDlg, IDC_CSD_CUSTOM_COLOR_0);
                // SetWindowBkColor(tmp, PIXEL_red);

                /* Set custom color 1 */
                GetDlgItem(hDlg, IDC_CSD_CUSTOM_COLOR_1);
                // SetWindowBkColor(tmp, PIXEL_blue);

                /* to do other... */

                return 0;
            }


        case MSG_COLORSELOK:
            {
                // TODO
                // PFILEDLGDATA pfdd = (PFILEDLGDATA) lParam;
                
                return 0;
            }
    }

    return 0;
}


static void button_callback (HWND hwnd, LINT id, int nc, DWORD add_data)
{
    COLORDLGDATA pcdd = {0};
    pcdd.hook = ColorHookProc;


    if (nc == BN_CLICKED && id == IDC_BUTTON_COLOR1) {
        if (ColorSelectDialog(NULL, hwnd, NULL, &pcdd)) {
			SetWindowBkColor(GetParent(hwnd), pcdd.pixel);
			InvalidateRect(GetParent(hwnd), NULL, TRUE);
        }
    }
    else if (nc == BN_CLICKED && id == IDC_BUTTON_COLOR2) {
        if (ColorSelectDialog((PDLGTEMPLATE) &DefSimpleColorDlg, hwnd, NULL, &pcdd)) {
			SetWindowBkColor(GetParent(hwnd), pcdd.pixel);
			InvalidateRect(GetParent(hwnd), NULL, TRUE);
        }
    }
}

static LRESULT MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND button;

    switch (message) {
        case MSG_CREATE:
            button = CreateWindow(CTRL_BUTTON, "Select Color 1",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_COLOR1,
                    10, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

            button = CreateWindow(CTRL_BUTTON, "Select Color 2",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_COLOR2,
                    160, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

            return 0;

        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_VISIBLE | WS_BORDER;
    pCreateInfo->dwExStyle = 0;
    pCreateInfo->spCaption = "Color Select Dialog";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = 0;
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MainWndProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = pCreateInfo->lx + DEFAULT_WIDTH;
    pCreateInfo->by = pCreateInfo->ty + DEFAULT_HEIGHT;
    pCreateInfo->iBkColor = PIXEL_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain(int argc, const char* argv[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp (argv[i], "-layer") == 0) {
            layer = argv[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, argv[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#else

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for Color Selection Dialog Box in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_DLGCOLORSEL */
