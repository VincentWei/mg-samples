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
 *   This file is part of mGEff, a component for MiniGUI.
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
#define dbg() printf("F:%s L:%d\n", __FUNCTION__, __LINE__)

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/minigui.h>

#include <mgeff/mgeff.h>

static RECT s_old_rc;
static HWND s_hwnd;
static MGEFF_ANIMATION animation = NULL;

void mycb(MGEFF_ANIMATION handle, HWND hwnd, int id, RECT *rc) {
    // printf("%ld %s %d %d,%d,%d,%d\n", times(NULL), __FUNCTION__, __LINE__, rc->left, rc->top, rc->right, rc->bottom);
    HDC dc;
    dc = GetClientDC(hwnd);
    SetBrushColor(dc, RGB2Pixel(dc, 0xff, 0xff, 0xff));
    FillBox(dc, s_old_rc.left, s_old_rc.top, RECTW(s_old_rc), RECTH(s_old_rc));

    SetBrushColor(dc, RGB2Pixel(dc, 0, 0, 0));
    FillBox(dc, rc->left, rc->top, RECTWP(rc), RECTHP(rc));

    ReleaseDC(dc);

    memcpy(&s_old_rc, rc, sizeof(s_old_rc));
}

static LRESULT mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
            {
                RECT rc;
                //int x, y;

                s_hwnd = hwnd;

                mGEffInit();
                animation = mGEffAnimationCreate((void *)hwnd, (void *)mycb, 1, MGEFF_RECT);
                GetClientRect(hwnd, &rc);
                //x = (rc.left + rc.right) / 2;
                //y = (rc.top + rc.bottom) / 2;
                // printf("x,y=%d,%d\n", x, y);
                // rc.left = x-20; rc.right = x+20; rc.top = y-10; rc.bottom = y+10;
                rc.left = 0; rc.right = 50; rc.top = 0; rc.bottom = 50;
                mGEffAnimationSetStartValue(animation, &rc);

                rc.left = 400; rc.right = 450; rc.top = 400; rc.bottom = 450;
                mGEffAnimationSetEndValue(animation, &rc);

                mGEffAnimationSetDuration(animation, 1000);

                mGEffAnimationSetProperty(animation, MGEFF_PROP_LOOPCOUNT, 100);

                mGEffAnimationSetCurve (animation, OutInQuart);

                mGEffAnimationAsyncRun (animation);

                mGEffAnimationWait ((void *)&s_hwnd, animation);

                memcpy(&s_old_rc, &rc, sizeof(s_old_rc));
            }
            break;
        case MSG_PAINT:
            {
                EndPaint(hwnd, BeginPaint(hwnd));
            }
            break;
        case MSG_ERASEBKGND:
            {
                HDC dc;
                RECT rcBg;
                GetClientRect(hwnd, &rcBg);
                dc = GetClientDC(hwnd);
                SetBrushColor(dc, RGB2Pixel(dc, 0xff, 0xff, 0xff));
                FillBox(dc, rcBg.left, rcBg.top, RECTW(rcBg), RECTH(rcBg));
                ReleaseDC(dc);
            }
            return 0;
        case MSG_CLOSE:
            if (animation != NULL) {
                mGEffAnimationDelete (animation);
            }

            mGEffDeinit();
            DestroyMainWindow (hwnd);
            PostQuitMessage (hwnd);
            break;
        default:
            break;
    }
    return DefaultMainWinProc(hwnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG msg;
    HWND hMainHwnd;
    MAINWINCREATE createInfo;

#ifdef _MGRM_PROCESSES   
    JoinLayer(NAME_DEF_LAYER, "animation", 0, 0);
#endif

    createInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    createInfo.dwExStyle = WS_EX_NONE;
    createInfo.spCaption = "animation";
    createInfo.hMenu = 0;
    createInfo.hCursor = GetSystemCursor(0);
    createInfo.hIcon = 0;
    createInfo.MainWindowProc = mainWindowProc;
    createInfo.lx = 0;  
    createInfo.ty = 0;  
    createInfo.rx = g_rcScr.right; 
    createInfo.by = g_rcScr.bottom; 
    createInfo.iBkColor = COLOR_black;
    createInfo.dwAddData = 0;
    createInfo.hHosting = HWND_DESKTOP;
    hMainHwnd = CreateMainWindow(&createInfo);
    if (hMainHwnd == HWND_INVALID)
    {
        return -1;
    }

    ShowWindow(hMainHwnd, SW_SHOWNORMAL);

    while (GetMessage(&msg, hMainHwnd))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    MainWindowThreadCleanup(hMainHwnd);

    return 0;
}
