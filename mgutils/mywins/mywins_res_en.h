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
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
#define CAPTION     "Examples for mywindow interfaces"
#define EXAMPLE1    "MyWins Examples"
#define EXAMPLE2    "myMessageBox example"
#define EXAMPLE3    "myWinMessage example"
#define EXAMPLE4    "myWinTernary example"
#define EXAMPLE5    "errorWindow example"
#define EXAMPLE6    "StatusWin example"
#define EXAMPLE7    "Choice"
#define EXAMPLE8    "ProcessWin example!"
#define EXAMPLE9    "Help"
#define EXAMPLE10   "Menu"
#define EXAMPLE11   "Entry"
#define EXAMPLE12   "Exit"

char * Flavor [] = {
        "Red",
        "Blue",
        "Choice you like color!",
        NULL
    };

char *Ternary [] = {
        "I like it",
        "I don't like it",
        "I like it very much",
        NULL
};

char* prompts [] = {
    "myMessageBox is a simple messagebox window and have printf like format.",
    "myWinMessage is a message Window include a button.",
    "Would you like this game?",
    "This is a very usefull window!",
    "The System is initialize , please wait...",
    0
};

char * Pmenu [] = {
        "Item 1",
        "Item 2",
        "Item 3",
        "Item 4",
        NULL
};

myWINBUTTON MenuItems[] = {
    {
        "string 1",
        IDC_WB1,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "string 2",
        IDC_WB2,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "string 3",
        IDC_WB3,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
    NULL
    }
};

myWINENTRY EntryItems [] = {
    {
        "Entry 1",
        &Pmenu[0] ,
        80,
        ES_LEFT ,
    },
    {
        "Entry 2",
        &Pmenu[1] ,
        80,
        ES_LEFT ,
    },
    {
        "Entry 3",
        &Pmenu[2] ,
        80,
        ES_LEFT ,
    },
    {
    NULL
    }
};

#define HELP_TITLE     "HelpMessage"
#define HELP_MSG     \
        "We use the SpinBox control in this Help Message Box.\n\n" \
        "You can click the up arrow of the control to scroll up " \
        "the message, and click the down arrow of the control to scroll down. " \
        "You can also scroll the message by typing ArrowDown and ArrowUp keys.\n\n" \
        "In your application, you can call 'myWinHelpMessage' function " \
        "to build a Help Message box like this.\n\n" \
        "The Help Message Box is useful for some PDA-like applications.\n\n" \
        "The SpinBox control allways have the fixed width and height. " \
        "You can read the source of 'ext/control/spinbox.c' to know how to " \
        "build such a control.\n\n" \
        "If you want to know how to use this control, please read the " \
        "source of 'ext/mywins/helpwin.c' in the MiniGUI source tree."

