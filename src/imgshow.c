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
** $Id: imgshow.c 770 2009-12-24 03:04:19Z wanzheng $
**
** iconview.c: Sample program for MiniGUI Programming Guide
**      Usage of ICONVIEW control.
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
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/mywindows.h>
#include <minigui/control.h>

#define IDC_RESET 101
#define IDC_AUTO 102
#define IDC_BACK 103
#define IDC_FORWARD 104
#define IDC_ALPHA 105
#define IDC_ZOOMIN 106
#define IDC_ZOOMOUT 107

#define IMG_NUM 14

static BITMAP bmp;
int     Img_count = 0;
int     zoomin_count = 0;
int     zoomout_count = 0;
int     Auto_control = 0;
int     Alpha_control = 0;
int     wide = 400;
int     high = 300;
BOOL    Alpha_show = FALSE;
BOOL    Load_NewBK = FALSE;

void
Img_lib(int i)
{
    switch (i)
    {
    case 1:
        LoadBitmap(HDC_SCREEN, &bmp, "bkgnd.jpg");
        break;
    case 2:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "0.gif");
        break;
    case 3:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "1.gif");
        break;
    case 4:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "2.gif");
        break;
    case 5:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "3.gif");
        break;
    case 6:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "4.gif");
        break;
    case 7:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "5.gif");
        break;
    case 8:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "6.gif");
        break;
    case 9:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "7.gif");
        break;
    case 10:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "8.gif");
        break;
    case 11:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "9.gif");
        break;
    case 12:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "10.gif");
        break;
    case 13:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "11.gif");
        break;
    case 14:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "12.gif");
        break;
    default:
        UnloadBitmap(&bmp);
        LoadBitmap(HDC_SCREEN, &bmp, "banner.gif");
        break;
    }
}

static void
auto_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    Auto_control++;
    Alpha_control = 0;
    Alpha_show = FALSE;
    Load_NewBK = FALSE;

    if (Auto_control % 2 == 0)
    {
        KillTimer(GetParent(hwnd), 1010);
    }
    else
    {
        SetTimer(GetParent(hwnd), 1010, 70);
    }
}

static void
reset_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    Alpha_control = 0;
    Img_count = 0;
    Alpha_show = FALSE;
    Load_NewBK = FALSE;

    if (Auto_control != 0)
    {
        Auto_control = 0;
        KillTimer(GetParent(hwnd), 1010);
    }

    LoadBitmap(HDC_SCREEN, &bmp, "banner.gif");
    InvalidateRect(GetParent(hwnd), NULL, TRUE);
}

static void
alpha_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    HDC     hdc, hdc_mem;
    char    buf[50];

    Alpha_control++;
    Alpha_show = TRUE;

    hdc_mem =
        CreateMemDC(400, 300, 16, MEMDC_FLAG_SWSURFACE, 0x0000f000, 0x00000f00,
                    0x000000f0, 0x0000000f);
    hdc = GetClientDC(GetParent(hwnd));

    if (Alpha_control == 5)
    {
        Alpha_control = 0;
        FillBoxWithBitmap(hdc, 0, 0, wide, high, &bmp);
    }

    SetBrushColor(hdc_mem,
                  RGBA2Pixel(hdc_mem, 0xff, 0xff, 0xff, Alpha_control * 63));
    FillBox(hdc_mem, 0, 0, 400, 240);
    BitBlt(hdc_mem, 0, 0, 400, 300, hdc, 0, 0, 0);

    if (Alpha_control == 0)
    {
        sprintf(buf, "Value of alpha: %d%%   \n", 0);
        TextOut(hdc, 240, 0, buf);
    }
    else
    {
        sprintf(buf, "Value of alpha:%d%%  \n", Alpha_control * 25);
        TextOut(hdc, 240, 0, buf);
    }

    InvalidateRect(GetParent(hwnd), NULL, FALSE);
    DeleteMemDC(hdc_mem);
    ReleaseDC(hdc);
}

static void
forward_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    Img_count++;
    Alpha_control = 0;
    Alpha_show = FALSE;
    Load_NewBK = FALSE;

    if (Img_count == (IMG_NUM + 1))
    {
        Img_count = 1;
        Img_lib(Img_count);
    }
    else
        Img_lib(Img_count);

    InvalidateRect(GetParent(hwnd), NULL, TRUE);
}

static void
back_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    Img_count--;
    Alpha_control = 0;
    Alpha_show = FALSE;
    Load_NewBK = FALSE;

    if (Img_count <= 0)
    {
        Img_count = 14;
        Img_lib(Img_count);
    }
    else
        Img_lib(Img_count);

    InvalidateRect(GetParent(hwnd), NULL, TRUE);
}

static void
zoomin_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    HDC     hdc;

    zoomin_count++;
    Alpha_control = 0;
    Load_NewBK = TRUE;
    Alpha_show = FALSE;

    hdc = GetClientDC(GetParent(hwnd));

    if (wide <= 1600)
    {
        wide = wide * 2;
        high = high * 2;
    }

    InvalidateRect(GetParent(hwnd), NULL, TRUE);
    ReleaseDC(hdc);
}

static void
zoomout_proc(HWND hwnd, int id, int nc, DWORD add_data)
{
    HDC     hdc;

    zoomout_count++;
    Alpha_control = 0;
    Load_NewBK = TRUE;
    Alpha_show = FALSE;

    hdc = GetClientDC(GetParent(hwnd));

    if (wide >= 100)
    {
        wide = wide / 2;
        high = high / 2;
    }


    InvalidateRect(GetParent(hwnd), NULL, TRUE);
    ReleaseDC(hdc);
}

static LRESULT HelloWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC     hdc;
    HWND    button1, button2, button3, button4, button5, button6, button7;

    switch (message)
    {
    case MSG_CREATE:
        {
            button1 =
                CreateWindow(CTRL_BUTTON, "Auto",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_AUTO, 330, 245, 60, 25, hWnd, 0);
            SetNotificationCallback(button1, auto_proc);

            button2 =
                CreateWindow(CTRL_BUTTON, "reset",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_RESET, 250, 245, 60, 25, hWnd, 0);
            SetNotificationCallback(button2, reset_proc);

            button3 =
                CreateWindow(CTRL_BUTTON, "<------",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_BACK, 10, 245, 60, 25, hWnd, 0);
            SetNotificationCallback(button3, back_proc);

            button4 =
                CreateWindow(CTRL_BUTTON, "------>",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_FORWARD, 90, 245, 60, 25, hWnd, 0);
            SetNotificationCallback(button4, forward_proc);

            button5 =
                CreateWindow(CTRL_BUTTON, "Alpha Bl",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_ALPHA, 170, 245, 60, 25, hWnd, 0);
            SetNotificationCallback(button5, alpha_proc);

            button6 =
                CreateWindow(CTRL_BUTTON, "-",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_ZOOMOUT, 5, 140, 22, 22, hWnd, 0);
            SetNotificationCallback(button6, zoomout_proc);

            button7 =
                CreateWindow(CTRL_BUTTON, "+",
                             WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
                             IDC_ZOOMIN, 5, 90, 22, 22, hWnd, 0);
            SetNotificationCallback(button7, zoomin_proc);

            return 0;
        }

    case MSG_ERASEBKGND:
        {
            HDC     hdc = (HDC) wParam;
            const RECT *clip = (const RECT *)lParam;
            RECT    rcTemp;
            BOOL    fGetDC = FALSE;

            if (hdc == 0)
            {
                hdc = GetClientDC(hWnd);
                fGetDC = TRUE;
            }

            if (clip)
            {
                rcTemp = *clip;
                ScreenToClient(hWnd, &rcTemp.left, &rcTemp.top);
                ScreenToClient(hWnd, &rcTemp.right, &rcTemp.bottom);
                IncludeClipRect(hdc, &rcTemp);
            }

            if (Img_count == 0)
            {
                LoadBitmap(HDC_SCREEN, &bmp, "banner.gif");
                FillBoxWithBitmap(hdc, 0, 0, 400, 300, &bmp);
            }
            else if (Load_NewBK)
            {
                FillBox(hdc, 0, 0, 400, 300);
                FillBoxWithBitmap(hdc, 0, 0, wide, high, &bmp);
            }
            else
                FillBoxWithBitmap(hdc, 0, 0, 400, 300, &bmp);

            if (fGetDC)
                ReleaseDC(hdc);

            return 0;
        }


    case MSG_PAINT:
        {
            char    buf[30], buf1[30];
            float   zoom;

            hdc = BeginPaint(hWnd);
            SetTextColor(hdc, COLOR_red);

            if (!Load_NewBK)
            {
                wide = 400;
                high = 300;
            }
            zoom = wide / 400.0;

            if (Img_count == 0)
                TextOut(hdc, 140, 50, "Image Show!!!\n");
            else if (!Alpha_show)
            {
                sprintf(buf, "The %d ImG  \n", Img_count);
                sprintf(buf1, "   x %f    \n", zoom);
                TextOut(hdc, 315, 0, buf);
                TextOut(hdc, 315, 16, buf1);
            }
            else
            {
                sprintf(buf1, "   x %f    \n", zoom);
                TextOut(hdc, 315, 16, buf1);
            }

            EndPaint(hWnd, hdc);
            return 0;
        }

    case MSG_TIMER:
        {
            Img_count++;
            Alpha_control = 0;
            Alpha_show = FALSE;
            Load_NewBK = FALSE;

            if (Img_count == (IMG_NUM + 1))
            {
                Img_count = 1;
                Img_lib(Img_count);
            }
            else
                Img_lib(Img_count);

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }

    case MSG_CLOSE:
        UnloadBitmap(&bmp);
        KillTimer(hWnd, 1010);
        DestroyMainWindow(hWnd);
        PostQuitMessage(hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int
MiniGUIMain(int argc, const char *argv[])
{

    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Imgdemo";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = HelloWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 400;
    CreateInfo.by = 300;
    CreateInfo.iBkColor = COLOR_red;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    MainWindowThreadCleanup(hMainWnd);
    return 0;
}
