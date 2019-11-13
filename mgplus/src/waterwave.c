/*
 *   This file is part of mGPlus, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgplus/mgplus.h>



static char* g_pBitmapName [] = {"./res/1.bmp",
                                 "./res/2.bmp",
                                 "./res/3.bmp",
                                 "./res/4.bmp",
                                 "./res/5.bmp",
                                 "./res/6.bmp",
                                 "./res/7.bmp"};
static int g_nTotalCount = sizeof (g_pBitmapName) / sizeof (g_pBitmapName [0]);
static BITMAP g_stTestBitmap;
static BITMAP g_CoverBitmap [7];

static int
WaterWaveProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
        {
            PLOGFONT plogfont = GetWindowFont(hWnd);
            MGPlusWaterWaveFlameInit ();
            if(!plogfont)
                printf("window font is null\n");
            break;
        }
        case MSG_RBUTTONDOWN:
        {
            HDC hdc = GetClientDC(hWnd);
            FillBox (hdc, 0, 0, g_rcScr.right, g_rcScr.bottom); 
            ReleaseDC(hdc);
        }
        break;
        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            switch(scancode)
            {
                case SCANCODE_F1:
                {
                    HDC hdc = GetClientDC(hWnd);
                    int i;
                    //HGRAPHICS hgs = MGPlusGraphicCreate(640, 480);
                    HGRAPHICS hgs = MGPlusGraphicCreateFromDC(hdc);
                    if (hgs == MP_GENERIC_ERROR)
                    {
                        printf("create graphics failed\n");
                        return 0;
                    }

                    unsigned int nTick = 0;
                    unsigned int nBackTick = 0;
                    MGPlusGraphicLoadBitmap (hgs, 0, &g_CoverBitmap [0]);
                    MGPlusGraphicLoadBitmap (hgs, 1, &g_CoverBitmap [1]);

                    for (i = 0; i <= 18; i ++)
                    {
                        MGPlusWaterWaveFlame (hgs, 0, 1, i, 18);
                        MGPlusGraphicSave (hgs, hdc, 0, 0, 0, 0, 0, 0);
                    }
#if 0
                    for (i = 0; i <= 10; i ++)
                    {
                        MGPlusWaterWaveFlame (hgs, 0, 1, i, 10);
                        MGPlusGraphicSave (hgs, hdc, 0, 0, 0, 0, 0, 0);
                    }
#endif
                    MGPlusGraphicDelete (hgs);
                    ReleaseDC (hdc);
                    break;
                }
                default:
                break;
            }
            break;
        }
        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}



#ifdef _MISC_MOUSECALIBRATE
static void
mouse_calibrate(void)
{
    POINT   src_pts[5] =
        { {5, 10}, {600, 20}, {620, 450}, {20, 470}, {310, 234} };
    POINT   dst_pts[5] = { {0, 0}, {639, 0}, {639, 479}, {0, 479}, {320, 240} };

    SetMouseCalibrationParameters(src_pts, dst_pts);
}
#endif /* !_MISC_MOUSECALIBRATE */

 

int
MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;
    int i;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "mgplustest", 0, 0);
#endif

#ifdef _MISC_MOUSECALIBRATE
    mouse_calibrate();
#endif

    for (i = 0; i < g_nTotalCount; i ++)
    {
        LoadBitmap (HDC_SCREEN, &g_CoverBitmap [i], g_pBitmapName [i]);
    }

    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_USEPARENTFONT;// WS_EX_NONE;
    CreateInfo.spCaption = "waterwave";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = WaterWaveProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_black;
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

    for (i = 0; i < g_nTotalCount; i ++)
    {
        UnloadBitmap (&g_CoverBitmap [i]);
    }

    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif
