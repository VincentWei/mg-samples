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
#define CAPTION     "mywindow �ӿ�ʾ��"
#define EXAMPLE1    "MyWins ʾ��"
#define EXAMPLE2    "myMessageBox ʾ��"
#define EXAMPLE3    "myWinMessage ʾ��"
#define EXAMPLE4    "myWinTernary ʾ��"
#define EXAMPLE5    "errorWindow ʾ��"
#define EXAMPLE6    "StatusWin ʾ��"
#define EXAMPLE7    "ѡ��"
#define EXAMPLE8    "ProcessWin ʾ����"
#define EXAMPLE9    "����"
#define EXAMPLE10   "�˵�"
#define EXAMPLE11   "���"
#define EXAMPLE12   "�˳�"

char * Flavor [] = {
        "��ɫ",
        "��ɫ",
        "ѡ����ϲ������ɫ��",
        NULL
    };

char *Ternary [] = {
        "��ϲ��",
        "��ϲ��",
        "�ǳ�ϲ��",
        NULL
};

char* prompts [] = {
    "myMessageBox ��һ���򵥵���Ϣ�� , ����ʹ��printf���ĸ�ʽ��",
    "myWinMessage ��һ����ʾ����,����һ����ť��",
    "��ϲ�������Ϸ��?",
    "���Ǹ������õ���ʾ���ڣ�",
    "ϵͳ���ڳ�ʼ��,���Ե�...",
    0
};

char * Pmenu [] = {
        "��Ŀ1",
        "��Ŀ2",
        "��Ŀ 3",
        "��Ŀ 4",
        NULL
};

myWINBUTTON MenuItems[] = {
    {
        "�ַ���1",
        IDC_WB1,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "�ַ���2",
        IDC_WB2,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "�ַ���3",
        IDC_WB3,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
    NULL
    }
};

myWINENTRY EntryItems [] = {
    {
        "�༭��1",
        &Pmenu[0] ,
        80,
        ES_LEFT ,
    },
    {
        "�༭��2",
        &Pmenu[1] ,
        80,
        ES_LEFT ,
    },
    {
        "�༭��3",
        &Pmenu[2] ,
        80,
        ES_LEFT ,
    },
    {
    NULL
    }
};

#define HELP_TITLE     "������Ϣ"
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

