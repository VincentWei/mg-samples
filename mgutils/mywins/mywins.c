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

#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>

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

#ifdef  _MGUTILS_MYWINS

#define IDC_MMB                 101
#define IDC_MWM                 102
#define IDC_TCB                 103
#define IDC_EEW                 104
#define IDC_STW                 105
#define IDC_COLOR               106
#define IDC_PROMPT              107
#define IDC_HELPMSG             108
#define IDC_MENU                109
#define IDC_ENTRY               110

#define IDC_PROGRESS            200

#define IDC_WB1                 210
#define IDC_WB2                 211
#define IDC_WB3                 212

#define MSG_INITBOX     MSG_USER+16
#define MSG_PROGRESS    MSG_USER+32

#ifdef _LANG_ZHCN
#include "mywins_res_cn.h"
#elif defined _LANG_ZHTW
#include "mywins_res_tw.h"
#else
#include "mywins_res_en.h"
#endif

static DLGTEMPLATE DlgYourTaste =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NOCLOSEBOX ,
    0, 0, 370, 280,
    CAPTION,
    0, 0,
    12, NULL,
    0
};

static CTRLDATA CtrlYourTaste[] =
{ 
    {
        "static",
        WS_VISIBLE | SS_GROUPBOX, 
        16, 10, 230, 215,
        IDC_STATIC,
        EXAMPLE1,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON ,
        36, 38, 200, 20,
        IDC_MMB,
        EXAMPLE2,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON, 
        36, 64, 200, 20, 
        IDC_MWM, 
        EXAMPLE3,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON,
        36, 90, 200, 20,
        IDC_TCB,
        EXAMPLE4,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON,
        36, 116, 200, 20,
        IDC_EEW,
        EXAMPLE5,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON,
        36, 142, 200, 20,
        IDC_STW,
        EXAMPLE6,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON, 
        250, 40, 100, 25,
        IDC_COLOR,
        EXAMPLE7,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON,
        36, 168, 200, 20,
        IDC_PROMPT,
        EXAMPLE8,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON ,
        250, 120, 100, 25,
        IDC_HELPMSG,
        EXAMPLE9,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON ,
        36, 194, 96, 20,
        IDC_MENU,
        EXAMPLE10,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON ,
        140, 194, 96, 20,
        IDC_ENTRY,
        EXAMPLE11,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP,
        250, 190, 100, 25,
        IDOK, 
        EXAMPLE12,
        0
    },
    {
        NULL
    }
};

static int pListItem = 0;

static void my_notif_proc (HWND hwnd, LINT id, int nc, DWORD add_data)
{
    if (nc == BN_CLICKED) {
        switch (id)
        {
            case IDC_MMB:
                myMessageBox(GetParent (hwnd) , MB_OK , "myMessageBox:" , "NOTE: \n %s \n" , prompts [id - IDC_MMB]);
            break;    
            case IDC_MWM:
                myWinMessage(GetParent (hwnd) , "myWinMessage:" , "my button!" , "NOTE: \n%s \n" , prompts [id - IDC_MMB]);
            break;
            case IDC_COLOR:
                myWinChoice(GetParent (hwnd) , "myWinChoice" , Flavor[0] , Flavor[1] , "NOTE: \n%s \n" , Flavor[2]); 
            break;
            case IDC_TCB:
                myWinTernary(GetParent (hwnd) , "myWinTernary" , Ternary[0] , Ternary[1] , Ternary[2] , "NOTE: \n %s \n" , prompts [id - IDC_MMB]); 
            break;
            case IDC_EEW:
               errorWindow(GetParent (hwnd) , prompts [id - IDC_MMB] , "ErrorWindow");       
            break;
            case IDC_STW:
                PostMessage(GetParent (hwnd) , MSG_INITBOX , id , 0);
            break;
            case IDC_PROMPT:
                PostMessage(GetParent (hwnd) , MSG_PROGRESS , IDC_PROGRESS , 0);
            break;
            case IDC_HELPMSG:
                myWinHelpMessage(GetParent (hwnd) , 300 , 200 , 
                        HELP_TITLE, HELP_MSG);
            break;

            case IDC_MENU:
                myWinMenu(GetParent (hwnd) , "Menu" , "Items" , 80 , 200 , Pmenu , &pListItem , MenuItems);
            break;
            case IDC_ENTRY:
                myWinEntries(GetParent (hwnd) , "Entry" , "Entries" , 120 , 80 , TRUE , EntryItems , MenuItems); 
            break;
        }
    }
}

static LRESULT DialogBoxProc2 (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND stahwnd = 0;
    static HWND prohwnd = 0;
    static HWND tiphwnd = 0;
    static int  ppos;

    switch (message) {
        case MSG_INITDIALOG:
        {
            int i;
            for (i = IDC_MMB; i <= IDC_ENTRY; i++)
                SetNotificationCallback (GetDlgItem (hDlg, i), my_notif_proc);
        }
        return 1;
        
        case MSG_COMMAND:
            switch (wParam) {
            case IDOK:
                EndDialog (hDlg, wParam);
                break;
            }
            break;

        case MSG_INITBOX: 
            if (!stahwnd)
            {
               stahwnd = createStatusWin(hDlg , 400 , 50 , "status" , prompts [wParam - IDC_MMB]);
               SetTimer(hDlg , 110 , 1000);
            }
            break;

        case MSG_PROGRESS:
#ifdef _MGCTRL_PROGRESSBAR
            if (!prohwnd)
            {
                prohwnd = createProgressWin(hDlg , "Progress" , "setup...." , IDC_PROGRESS , 100);
                SetTimer(hDlg , 100  , 100);
                SendDlgItemMessage(prohwnd , IDC_PROGRESS , PBM_SETSTEP , 10 , 0);
            }
#endif
            break;
        case MSG_MOUSEMOVE:
        {
            int pos_x , pos_y;
            RECT rc;
            pos_x = LOSWORD(lParam);
            pos_y = HISWORD(lParam);
            GetWindowRect(GetDlgItem (hDlg , IDC_COLOR) , &rc);
            
            if (PtInRect(&rc , pos_x , pos_y) && !tiphwnd)
            {
                tiphwnd = createToolTipWin (hDlg , pos_x , pos_y , 1000 , "This is the tip window!");
            }

            if (!PtInRect(&rc , pos_x , pos_y) && tiphwnd)
            {
               destroyToolTipWin(tiphwnd); 
               tiphwnd = 0;
            }
            
        }
               break;
        case MSG_TIMER:
        {
#ifdef _MGCTRL_PROGRESSBAR
                if (wParam == 100 && prohwnd)
                { 
                    if (ppos < 10)
                    {
                        SendDlgItemMessage(prohwnd , IDC_PROGRESS , PBM_SETPOS , 10*ppos , 0);
                        ppos++;
                    }
                    else
                    {
                        KillTimer(hDlg , 100);
                        destroyProgressWin(prohwnd);
                        prohwnd = 0;
                        ppos = 0;
                    }
                }
#endif
              if (wParam == 110 && stahwnd)
              {     
                KillTimer(hDlg , 110);
                destroyStatusWin (stahwnd);
                stahwnd = 0;
              }
        }
        break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

    DlgYourTaste.controls = CtrlYourTaste;
    
    DialogBoxIndirectParam (&DlgYourTaste, HWND_DESKTOP, DialogBoxProc2, 0L);

    return 0;
}

#else

int MiniGUIMain (int argc, const char* argv[])
{
    fprintf (stderr, "Please enable the support for MyWins module in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_MYWINS */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

