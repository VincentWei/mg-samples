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
** $Id: monthcalendar.c 616 2008-08-25 03:17:42Z weiym $
**
** monthcalendar.c: The MonthCalendar control demo program.
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
**
** Create date: 2001/11/01
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_MONTHCAL

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

#define IDC_CTRL1     10
#define IDC_CTRL2     20
#define IDC_CTRL3     30
#define IDC_CTRL4     40
#define IDC_CTRL5     50
#define IDC_CTRL6     60
#define IDC_CTRL7     70
#define IDC_CTRL8    80
#define IDC_CTRL9    90
#define IDC_CTRL10    100
#define IDC_CTRL11    110
#define IDC_CTRL12    120
#define IDC_CTRL13    130
#define IDC_CTRL14     140
#define IDC_CTRL15    150
#define IDC_CTRL16    160
#define IDC_CTRL17    170

static HWND hMainWnd = HWND_INVALID;

static LRESULT ControlTestWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hChildWnd1, hMeditWnd1, hMeditWnd2;

    switch (message) {
        case MSG_CREATE:
            hChildWnd1 = CreateWindow ("monthcalendar",
                                        monthcalendar,
                                        WS_CHILD |
                                        WS_VISIBLE |
                                        MCS_ENG_L |
                                        MCS_NOTIFY,
                                        IDC_CTRL1,
                                        50, 20, 250, 180, hWnd, 0);
            CreateWindow ("button",
                                     get_current_day,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL2,
                                     140, 220, 160, 24, hWnd, 0);
            hMeditWnd1 = CreateWindow ("medit",
                                      NULL,
                                      WS_CHILD | WS_BORDER |
                                      WS_VISIBLE,
                                      IDC_CTRL3,
                                      350, 20, 230, 100, hWnd, 0);

            CreateWindow ("button",
                                     get_current_month,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL4,
                                     140, 260, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     get_current_year,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL5,
                                     140, 300, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     set_current_day,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL6,
                                     140, 340, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     set_current_month,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL7,
                                     140, 380, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     set_current_year,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL8,
                                     140, 420, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     set_current_date_as_today,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL9,
                                     140, 460, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     get_first_day_of_week,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL10,
                                     140, 500, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     get_current_date,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL11,
                                     350, 220, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     get_today,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL12,
                                     350, 260, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     get_minreqrectsize,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL13,
                                     350, 300, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     set_current_date,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL14,
                                     350, 340, 160, 24, hWnd, 0);
            hMeditWnd2 = CreateWindow ("medit",
                                      NULL,
                                      WS_CHILD | WS_BORDER |
                                      WS_VISIBLE,
                                      IDC_CTRL15,
                                      350, 150, 230, 50, hWnd, 0);
            CreateWindow ("button",
                                     set_color,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL16,
                                     350, 380, 160, 24, hWnd, 0);
            CreateWindow ("button",
                                     get_color,
                                     WS_CHILD
                                     | BS_PUSHBUTTON
                                     | WS_VISIBLE,
                                     IDC_CTRL17,
                                     350, 420, 160, 24, hWnd, 0);

        break;

        case MSG_COMMAND:
        {
            int id   = LOWORD(wParam);
            int code = HIWORD(wParam);

            switch (id) {
                case IDC_CTRL2:
                {
                    int day;
                    char chday[50];
                    day = SendMessage (hChildWnd1, MCM_GETCURDAY, 0, 0);
                    sprintf (chday, "Current day=%d\n", day);
                    SetWindowText (hMeditWnd1, chday);
                }
                    break;

                case IDC_CTRL4:
                {
                    int month;
                    char chmon[50];
                    month = SendMessage (hChildWnd1, MCM_GETCURMONTH, 0, 0);
                    sprintf (chmon, "Current month=%d\n", month);
                    SetWindowText (hMeditWnd1, chmon);
                }
                break;

                case IDC_CTRL5:
                {
                    int year;
                    char chyear[50];
                    year = SendMessage (hChildWnd1, MCM_GETCURYEAR, 0, 0);
                    sprintf (chyear, "Current year=%d\n", year);
                    SetWindowText (hMeditWnd1, chyear);
                }
                break;

                case IDC_CTRL6:
                {
                    int newday = 31;
                    SendMessage (hChildWnd1, MCM_SETCURDAY, newday, 0);
                }
                break;

                case IDC_CTRL7:
                {
                    int newmonth = 2;
                    SendMessage (hChildWnd1, MCM_SETCURMONTH, newmonth, 0);
                }
                break;

                case IDC_CTRL8:
                {
                    int newyear = 1997;
                    SendMessage (hChildWnd1, MCM_SETCURYEAR, newyear, 0);
                }
                break;

                case IDC_CTRL9:
                {
                    SendMessage (hChildWnd1, MCM_SETTODAY, 0, 0);
                }
                break;

                case IDC_CTRL10:
                {
                    int weekday1;
                    char buffer[50];
                    weekday1 = SendMessage (hChildWnd1, MCM_GETFIRSTWEEKDAY, 0, 0);
                    sprintf (buffer, "first day of week =%d\n", weekday1);
                    SetWindowText (hMeditWnd1, buffer);
                }
                break;

                case IDC_CTRL11:
                {
                    SYSTEMTIME curdate;
                    char buffer[300];
                    SendMessage (hChildWnd1, MCM_GETCURDATE, 0, (LPARAM) &curdate);
                    sprintf (buffer, "current year = %d\ncurrent month = %d\ncurrent day = %d\ncurrent day of week = %d\n",    curdate.year, curdate.month, curdate.day, curdate.weekday);
                    SetWindowText (hMeditWnd1, buffer);
                }
                break;

                case IDC_CTRL12:
                {
                    SYSTEMTIME today;
                    char buffer[300];

                    SendMessage (hChildWnd1, MCM_GETTODAY, 0, (LPARAM) &today);
                    sprintf (buffer, "today is \nyear %d\nmonth %d\nday %d\nday of week %d\n",
                                    today.year, today.month, today.day, today.weekday);
                    SetWindowText (hMeditWnd1, buffer);
                }
                break;

                case IDC_CTRL13:
                {
                    int minw, minh;
                    char buffer[100];

                    minw = SendMessage (hChildWnd1, MCM_GETMINREQRECTW, 0, 0);
                    minh = SendMessage (hChildWnd1, MCM_GETMINREQRECTH, 0, 0);
                    sprintf (buffer, "the minimum required window width = %d\nthe minimum required window height =%d\n", minw, minh);
                    SetWindowText (hMeditWnd1, buffer);
                }
                break;

                case IDC_CTRL14:
                {
                    SYSTEMTIME curdate;

                    curdate.year = 1997;
                    curdate.month = 7;
                    curdate.day = 1;
                    SendMessage (hChildWnd1, MCM_SETCURDATE, 0, (LPARAM) &curdate);
                }
                break;

                case IDC_CTRL1:
                    switch (code) {
                        case MCN_DATECHANGE:
                            SetWindowText (hMeditWnd2, "the current date is changed\n");
                        break;

                    }
                break;

                case IDC_CTRL16:
                {
                    MCCOLORINFO mcci;

                    memset (&mcci, -1, 11*sizeof(int));
                    //mcci.clr_titlebk         = COLOR_darkgray;
                    //mcci.clr_titletext         = PIXEL_lightwhite;
                    mcci.clr_arrow             = PIXEL_darkblue;
                    mcci.clr_arrowHibk         = COLOR_yellow;
                    mcci.clr_daybk            = COLOR_yellow;
                    mcci.clr_dayHibk        = COLOR_magenta;
                    mcci.clr_daytext        = PIXEL_darkblue;
                    mcci.clr_trailingtext    = COLOR_green;
                    mcci.clr_dayHitext        = COLOR_yellow;
                    mcci.clr_weekcaptbk        = COLOR_darkgreen;
                    mcci.clr_weekcapttext     = COLOR_yellow;

                    SendMessage (hChildWnd1, MCM_SETCOLOR, 0, (LPARAM) &mcci);
                }
                break;

                case IDC_CTRL17:
                {
                    char buffer[300];
                    MCCOLORINFO mcci;

                    SendMessage (hChildWnd1, MCM_GETCOLOR, 0, (LPARAM) &mcci);
                    sprintf (buffer, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
                            mcci.clr_titlebk, mcci.clr_titletext,
                            mcci.clr_arrow, mcci.clr_arrowHibk,
                            mcci.clr_daybk, mcci.clr_dayHibk,
                            mcci.clr_daytext, mcci.clr_trailingtext,
                            mcci.clr_dayHitext, mcci.clr_weekcaptbk,
                            mcci.clr_weekcapttext);
                    SetWindowText (hMeditWnd1, buffer);
                }
                break;

                default:
                break;
            }

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
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = MonthCalendar_control ;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 640;
    pCreateInfo->by = 480;
    pCreateInfo->iBkColor =
        GetWindowElementPixelEx (HWND_NULL, HDC_SCREEN, WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void monthcalendar_demo (HWND hwnd)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo(&CreateInfo);
    CreateInfo.hHosting = hwnd;

    hMainWnd = CreateMainWindow(&CreateInfo);

}

#else

void monthcalendar_demo (HWND hwnd)
{
    MessageBox (hwnd, "WARNING", "Please enable MONTHCALENDAR control support in MiniGUI.", MB_OK);
}

#endif /* _MGCTRL_MONTHCAL */
