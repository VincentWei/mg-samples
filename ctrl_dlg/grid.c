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
** $Id$
**
** grid.c: The GridView control demo program.
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
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_GRIDVIEW

#define IDC_GRID    10
#define IDC_CTRL1       20
#define IDC_CTRL2       30
#define IDC_CTRL3       40
#define IDC_CTRL4       50
#define IDC_EDIT        60
#define IDC_CTRL6       70
#define IDC_CTRL7       80
#define IDC_CTRL8       90
#define IDC_CTRL9       100
#define IDC_CTRL10      110
#define IDC_CTRL11      120
#define IDC_CTRL12      130

#define IDM_FILE    200

static HWND hMainWnd = HWND_INVALID;
static HWND hChildWnd1;
static HMENU hRightMenu;

static void create_ctrls (HWND hWnd)
{
    CreateWindow ("button", "Get header height",
          WS_CHILD
          | BS_PUSHBUTTON
          | WS_VISIBLE,
          IDC_CTRL1, 20, 260, 130, 20, hWnd, 0);
    CreateWindow ("button", "Get header width",
          WS_CHILD
          | BS_PUSHBUTTON
          | WS_VISIBLE,
          IDC_CTRL2, 160, 260, 80, 20, hWnd, 0);
    CreateWindow ("button", "Add a column",
          WS_CHILD
          | BS_PUSHBUTTON
          | WS_VISIBLE,
          IDC_CTRL3, 250, 260, 80, 20, hWnd, 0);
    CreateWindow ("button", "Delete a column",
          WS_CHILD
          | BS_PUSHBUTTON
          | WS_VISIBLE,
          IDC_CTRL4, 340, 260, 80, 20, hWnd, 0);
    CreateWindow ("edit", "",
          WS_CHILD
          | WS_VISIBLE,
          IDC_EDIT, 30, 230, 120, 20, hWnd, 0);
    CreateWindow ("button", "Set column header text",
          WS_CHILD
          | WS_VISIBLE,
          IDC_CTRL6, 20, 290, 80, 20, hWnd, 0);
    CreateWindow ("button", "set cell text",
          WS_CHILD
          | WS_VISIBLE,
          IDC_CTRL7, 110, 290, 120, 20, hWnd, 0);
    /*CreateWindow ("button", "sorting the items",
      WS_CHILD
      | WS_VISIBLE,
      IDC_CTRL8, 240, 290, 120, 20, hWnd, 0);
      CreateWindow ("button", "find item",
      WS_CHILD
      | WS_VISIBLE,
      IDC_CTRL9, 370, 290, 120, 20, hWnd, 0);
    */
}

static HMENU create_rightbutton_menu (void)
{
    int i;
    HMENU hMenu;
    MENUITEMINFO mii;
    char *msg[] = {
        "open",
        "copy",
        "delete",
        "rename",
        "properties"
    };

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"File";

    hMenu = CreatePopupMenu (&mii);

    for ( i = 0; i < 5; i ++ ) {
        memset ( &mii, 0, sizeof (MENUITEMINFO) );
        mii.type = MFT_STRING;
        mii.id = IDM_FILE+ i;
        mii.state = 0;
        mii.typedata= (DWORD) msg[i];
        InsertMenuItem ( hMenu, i, TRUE, &mii );
    }

    return hMenu;
    //return StripPopupHead(hMenu);
}

#if 0
static void
lv_notify_process (HWND hwnd, int id, int code, DWORD addData)
{
    if (code == LVN_KEYDOWN) {
        PLVNM_KEYDOWN down;
    int key;

    down = (PLVNM_KEYDOWN)addData;
    key = LOWORD(down->wParam);

    if (key == SCANCODE_REMOVE) {
        int nItem;
        nItem = SendMessage (hwnd, LVM_GETSELECTEDITEM, 0, 0);
        if (nItem > 0) {
            if( MessageBox (hMainWnd, "are you really want to delete this file?",
                "warning", MB_YESNO) == IDYES) {
            // not really delete yet.
            SendMessage (hwnd, LVM_DELITEM, nItem, 0);
        }
        }
    }
    if (key == SCANCODE_ENTER) {
    }
    }
    if (code == LVN_ITEMRUP) {
        PLVNM_ITEMRUP up;
    int x, y;

    up = (PLVNM_ITEMRUP)addData;
    x = LOSWORD(up->lParam);
    y = HISWORD(up->lParam);

    ClientToScreen (hChildWnd1, &x, &y);

        TrackPopupMenu (GetPopupSubMenu (hRightMenu), TPM_LEFTALIGN | TPM_LEFTBUTTON ,
            x, y, hMainWnd);
    }
    if (code == LVN_ITEMDBCLK) {
    int nItem = SendMessage (hwnd, LVM_GETSELECTEDITEM, 0, 0);
    if (nItem > 0) {
            if( MessageBox (hMainWnd, "Are you really want to open this file?",
                "Question", MB_YESNO) == IDYES) {
            MessageBox (hMainWnd, "Me too.", "Sorry", MB_OK);
        }
    }
    }
}
#endif

static LRESULT ControlTestWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BITMAP folder;

    switch (message)
    {
    case MSG_CREATE:
    {

        create_ctrls (hWnd);
        hRightMenu = create_rightbutton_menu ();

        LoadBitmapFromFile (HDC_SCREEN, &folder, "./res/folder.bmp");

        hChildWnd1 = CreateWindow (CTRL_GRID, "GRID",
                       WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                       WS_HSCROLL | WS_BORDER, IDC_GRID, 20, 20, 350,
                       200, hWnd, 0);

    }
    break;

    case MSG_COMMAND:
    {
        int id = LOWORD (wParam);
        //int code = HIWORD (wParam);
        int nSel, col, row;
        char buffer[101];

        GetWindowText (GetDlgItem(hWnd, IDC_EDIT), buffer, 100);
        nSel = atoi(buffer);

        switch (id) {
            case IDC_CTRL1:
                row = SendMessage(hChildWnd1, GRIDM_GETHEADHEIGHT, 0,0);
                sprintf(buffer, "%d", row);
                SetWindowText(GetDlgItem(hWnd, IDC_EDIT), buffer);
                break;

            case IDC_CTRL2:
                col = SendMessage(hChildWnd1, GRIDM_GETHEADWIDTH, 0,0);
                sprintf(buffer, "%d", col);
                SetWindowText(GetDlgItem(hWnd, IDC_EDIT), buffer);
                break;

            case IDC_CTRL3:
                col = SendMessage(hChildWnd1, GRIDM_GETCOLCOUNT, 0, 0);
                SendMessage(hChildWnd1, GRIDM_ADDCOL, (WPARAM)col, 0);
                break;

            case IDC_CTRL4:
                col = SendMessage(hChildWnd1, GRIDM_GETCOLCOUNT, 0, 0);
                SendMessage(hChildWnd1, GRIDM_DELCOL, (WPARAM)(col-1), 0);
                break;

            case IDC_CTRL6:
                break;

            case IDC_CTRL7:
                break;

            case IDC_CTRL8:
                break;

            case IDC_CTRL9:
                break;

            case IDM_FILE ... IDM_FILE+4:
                MessageBox (hMainWnd, "In Construction ...", "Sorry", MB_OK);
                break;
        }

    }
    break;

    case MSG_DESTROY:
        UnloadBitmap (&folder);
        DestroyMenu (hRightMenu);
        DestroyAllControls (hWnd);
        hMainWnd = HWND_INVALID;
        return 0;

    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        MainWindowCleanup (hWnd);
        return 0;

    }
    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = "Grid control";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor (1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 500;
    pCreateInfo->by = 350;
    pCreateInfo->iBkColor =
        GetWindowElementPixelEx (HWND_NULL, HDC_SCREEN, BKC_CONTROL_DEF);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void grid_demo (HWND hwnd)
{
    MAINWINCREATE CreateInfo;

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hwnd;

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd != HWND_INVALID)
    {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }
}

#else

void grid_demo (HWND hwnd)
{
    MessageBox (hwnd, "WARNING", "Please enable GRIDVIEW control support in MiniGUI.", MB_OK);
}

#endif /* _MGCTRL_GRIDVIEW */

