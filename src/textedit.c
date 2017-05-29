/*
** $Id: textedit.c 780 2010-01-21 03:30:50Z houhuihua $
**
** textedit.c: Sample program for MiniGUI Programming Guide
**      Usage of TEXTEDIT control.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
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
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>


#define IDC_TEXTEDIT	100
#define IDC_BT          200
#define IDC_BT2         300
#define IDC_BT3         400
#define IDC_BT4         500

#define mytext \
"MiniGUI 3.0 is the latest MiniGUI version released by FMSoft. \n" \
"This version offers a lot of advanced features that will help you develop an embedded product with better user experience, like 3D UI."
                
#define newtext \
"mStudio is a visual integrated development environment (IDE) for MiniGUI application development. \n" \
"mStudio is based on Eclipse platform. It can provide WYSIWYG UI design for developers by generating code automatically and maintaining MiniGUI project framework."

#define shorttext \
"Welcome to the world of MiniGUI!"

static int
BookProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    HWND htextedit;

    switch (message)
    {

        case MSG_INITDIALOG:
            htextedit = GetDlgItem (hDlg, IDC_TEXTEDIT);
            SetFocus (htextedit);
            SendMessage (htextedit, EM_SETTITLETEXT, -1, (LPARAM)"Title: ");
            break;

        case MSG_COMMAND:
            {
                int id = LOWORD (wParam);
                htextedit = GetDlgItem (hDlg, IDC_TEXTEDIT);

                switch (id) {
                    case IDC_BT:
                        SendMessage (htextedit, MSG_SETTEXT, 0, (LPARAM)newtext);
                        break;
                    case IDC_BT2:
                        {
                            SendMessage (htextedit, MSG_SETTEXT, 0, (LPARAM)mytext);
                            break;
                        }
                    case IDC_BT3:
                        SendMessage (htextedit, TEM_RESETCONTENT, 0, 0);
                        break;
                    case IDC_BT4:
                        {
                            char buffer[40001];
                            int len;
                            len = SendMessage (htextedit, EM_GETTITLETEXT, 100, (LPARAM)buffer);
                            break;
                        }
                }
            }
            break;

        case MSG_CLOSE:
            EndDialog (hDlg, 0);
            break;

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA CtrlBook[] =
{
    {
        CTRL_BUTTON,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         40, 350, 50, 34,
        IDC_BT,
        "add",
        0
    },
    {
        CTRL_BUTTON,
        WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         100, 350, 50, 34,
        IDC_BT2,
        "del",
        0
    },
    {
        CTRL_BUTTON,
        WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         160, 350, 50, 34,
        IDC_BT3,
        "reset",
        0
    },
    {
        CTRL_BUTTON,
        WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         230, 350, 50, 34,
        IDC_BT4,
        "get",
        0
    },
    {
        "textedit",
        WS_BORDER | WS_CHILD | WS_VISIBLE | 
        WS_VSCROLL | WS_HSCROLL | 
        TES_BASELINE | TES_TITLE,
         10, 20, 420, 200,
        IDC_TEXTEDIT,
        shorttext,
        0
    },
};

static DLGTEMPLATE DlgBook =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 600, 450,
    "book list",
    0, 0,
    TABLESIZE(CtrlBook), NULL,
    0
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "textedit" , 0 , 0);
#endif
    
    DlgBook.controls = CtrlBook;
    
    DialogBoxIndirectParam (&DlgBook, HWND_DESKTOP, BookProc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

