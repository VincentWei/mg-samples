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
#include <sys/time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "./../animate/common_animates/common_animates.h"
#include "stdio.h"
#include "string.h"

#define C2_KEY_LEFT     SCANCODE_CURSORBLOCKLEFT     /* 方向左键 */
#define C2_KEY_RIGHT    SCANCODE_CURSORBLOCKRIGHT    /* 方向右键 */
#define C2_KEY_UP       SCANCODE_CURSORBLOCKUP       /* 方向上键 */
#define C2_KEY_DOWN     SCANCODE_CURSORBLOCKDOWN     /* 方向下键 */

#define C2_KEY_OK       SCANCODE_ENTER          /* 确定键 */
#define C2_KEY_EXIT     SCANCODE_ESCAPE         /* 退出键 */
#define C2_KEY_SOURCE   SCANCODE_F1             /* 信源键 */
#define C2_KEY_USB      SCANCODE_F2             /* USB键 */

#define C2_KEY_PLAY     SCANCODE_P              /* 播放键 */
#define C2_KEY_PAUSE    SCANCODE_L              /* 暂停键 */
#define C2_KEY_STOP     SCANCODE_S              /* 停止键 */
#define C2_KEY_FORWORD  SCANCODE_F              /* 快进键 */
#define C2_KEY_REWIND   SCANCODE_R              /* 快退键 */
#define C2_KEY_NEXT     SCANCODE_N              /* 下一个键 */
#define C2_KEY_PREV     SCANCODE_R              /* 上一个键 */

#define C2_KEY_1        SCANCODE_1              /* 数字键 */
#define C2_KEY_2        SCANCODE_2
#define C2_KEY_3        SCANCODE_3
#define C2_KEY_4        SCANCODE_4
#define C2_KEY_5        SCANCODE_5
#define C2_KEY_6        SCANCODE_6
#define C2_KEY_7        SCANCODE_7
#define C2_KEY_8        SCANCODE_8
#define C2_KEY_9        SCANCODE_9
#define C2_KEY_0        SCANCODE_0

#define MAIN_WND_WIDTH  592
#define MAIN_WND_HEIGHT 523

#define MENU_START_X    25
#define MENU_START_Y    116
#define MENU_WIDTH      270
#define MENU_HEIGHT     50

#define DISK_START_X    297
#define DISK_START_Y    116
#define DISK_WIDTH      270
#define DISK_HEIGHT     50

#define PI 3.14159265358979323846

typedef struct _3_d_coordinate {
    int x;
    int y;
    int z;
}THREE_D_COORDINATE;

static char *res_file[] =
{
    "res/pic1.jpg",
    "res/pic2.jpg"
};
static BITMAP bmp_res[2];
static HDC     g_buffdc;


static void def_out_bkgnd(HDC hdc, const RECT* rtbk, void *param)
{
    PBITMAP bmpIn = (PBITMAP)param;
    FillBoxWithBitmap(hdc, 0, 0,
            RECTW(g_rcScr), RECTH(g_rcScr), bmpIn);
}

static void def_out_draw_animate(HDC hdc, ANIMATE*ani, void* context)
{
    //printf("%s: %d,%d,%d,%d, img=%p\n",__FUNCTION__, GetAnimateX(ani), GetAnimateY(ani), GetAnimateW(ani),
    //        GetAnimateH(ani));
    context = context;
    if(GetAnimateW(ani) != 0 && GetAnimateH(ani)!=0) {
        PBITMAP pbmp = (PBITMAP)ani->img;
        if (GetAnimateX(ani) >= -2 && GetAnimateX(ani) < 0 ) {
            SetAnimateX(ani, 0);
        }
        if (GetAnimateY(ani) >= -2 && GetAnimateY(ani) < 0) {
            SetAnimateY(ani, 0);
        }
        FillBoxWithBitmap(hdc,
                GetAnimateX(ani), GetAnimateY(ani),
                pbmp->bmWidth, pbmp->bmHeight,
                pbmp);
    }
}

static void on_end_draw_one_frame(ANIMATE_SENCE *as)
{
    if(as != NULL)
    {
        BitBlt(as->hdc, 0, 0, RECTW(as->rtArea),RECTH(as->rtArea), HDC_SCREEN, 0, 0, 0);
    }
}

static void GetSubBitmap(PBITMAP pbmp, PBITMAP psub, int x, int y, int w, int h)
{
    memcpy(psub, pbmp, sizeof(BITMAP));
    psub->bmWidth  = w;
    psub->bmHeight = h;
    psub->bmBits   = pbmp->bmBits + (pbmp->bmPitch * y)
        + x * pbmp->bmBytesPerPixel;
}


static void Center4SplitAnimate(HDC hdc, const RECT *rt,
        PBITMAP bmpIn, PBITMAP bmpOut, int frame_num, BOOL dir)
{
    int w,h;
    w = RECTWP(rt);
    h = RECTHP(rt);
    BITMAP bmp1, bmp2, bmp3, bmp4;
    int center_x = w/2, center_y = h/2;

    GetSubBitmap(bmpOut, &bmp1, 0, 0, w/2, h/2);
    GetSubBitmap(bmpOut, &bmp2, center_x, 0, w/2, h/2);
    GetSubBitmap(bmpOut, &bmp3, 0, center_y, w/2, h/2);
    GetSubBitmap(bmpOut, &bmp4, center_x, center_y, w/2, h/2);

    {
        ANIMATE_OPS ops = {
            def_out_draw_animate,
            def_out_bkgnd,
            NULL,
            NULL,
            on_end_draw_one_frame
        };
        SetInterval (50);
        if (dir){
            PUSH_PULL_OBJ objs[4] ={
                {&bmp1, 0, 0, -center_x, -center_y},
                {&bmp2, center_x, 0, w, -center_y},
                {&bmp3, 0, center_y, -center_x, h},
                {&bmp4, center_x, center_y, w, h},
            };
            RunPushPullAnimate(hdc, rt, objs, TABLESIZE(objs),  &ops, 20, bmpIn);
        }
        else
        {
            PUSH_PULL_OBJ objs[4] ={
                {&bmp1, -center_x, -center_y, 0, 0},
                {&bmp2, w, -center_y, center_x, 0},
                {&bmp3, -center_x, h, 0, center_y},
                {&bmp4, w, h, center_x, center_y},
            };

            RunPushPullAnimate(hdc, rt, objs, TABLESIZE(objs),  &ops, 20, bmpIn);
        }
    }
}

static LRESULT BlindDemoWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
        {
            int i;
            for(i=0;i<sizeof(res_file)/sizeof(char *);i++)
                LoadBitmap (HDC_SCREEN, &bmp_res[i], res_file[i]);

            break;
        }
        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            switch(scancode)
            {
                case SCANCODE_F1:
                {
                    static int count = 0;
                    RECT rcScr = GetScreenRect();
                    if ((count++ %2)){
                        Center4SplitAnimate (g_buffdc, &rcScr, &bmp_res[1],
                                &bmp_res[0], 20, FALSE);
                    }
                    else {
                        Center4SplitAnimate (g_buffdc, &rcScr, &bmp_res[1],
                                &bmp_res[0], 20, TRUE);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
#if 1
        case MSG_PAINT:
        {
            HDC hdc = BeginPaint(hWnd);
            FillBoxWithBitmap(hdc, g_rcScr.left, g_rcScr.top,
                    RECTW(g_rcScr), RECTH(g_rcScr), &bmp_res[0]);
            EndPaint(hWnd, hdc);
            break;
        }
#endif
        case MSG_CLOSE:
        {
            int i;
            for(i=0;i<sizeof(res_file)/sizeof(char *);i++)
                UnloadBitmap (&bmp_res[i]);
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
        }
        default:
            break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "blind", 0, 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE|WS_BORDER|WS_CAPTION;
    CreateInfo.dwExStyle = WS_NONE;//WS_EX_AUTOPRIVATEDC;
    CreateInfo.spCaption = "Please push F1,F2";
    //CreateInfo.hMenu = hMenu;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = BlindDemoWinProc;
    CreateInfo.lx = g_rcScr.left;
    CreateInfo.ty = g_rcScr.top;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom ;
    CreateInfo.iBkColor = COLOR_black;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    g_buffdc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(g_rcScr), RECTH(g_rcScr));
#if 0
    hdc = GetSecondaryDC(hMainWnd);
    SetSecondaryDC(hMainWnd, hdc, ON_UPDSECDC_DONOTHING);
    ReleaseSecondaryDC(hMainWnd, hdc);

    ShowWindow(hMainWnd, SW_SHOWNORMAL);
    UpdateAll (hMainWnd, TRUE);
    AlphaAnimate(hMainWnd, 30, TRUE);
#endif
    while (GetMessage(&Msg, hMainWnd))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    DeleteCompatibleDC(g_buffdc);
    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif
