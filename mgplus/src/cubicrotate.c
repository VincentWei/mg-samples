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

#include <mgplus/mgplus.h>
#include "common_animates.h"
#include "stdio.h"
#include "string.h"

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

typedef void (* ANIMATE_DRAW)(int index, void *context);

typedef struct _rotate_info
{
    int     frame_num;  /* */
    int     w;          /* w of hdc */
    int     h;          /* h of hdc */
    int     inc_x;      /* increment of y */
    int     inc_y;      /* increment of y */
    float   inc_a;      /* increment of rotate angle*/
    int     z_prp;
    HPATH   path;
    MPPOINT center;
    MPPOINT pt[8];
    HDC     hdc;        /* the destination DC */
    HGRAPHICS hgp;  /*  */
    PBITMAP bmp_bk; /* the picture of background */
    PBITMAP bmp_fg; /* the picture of foreground */
}ROTATEINFO;

void InitRotateInfo(HWND hwnd, ROTATEINFO* info, int frame_num,
        PBITMAP bmp_bk, PBITMAP bmp_fg)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    info->frame_num = frame_num;
    info->w = RECTW(rc);
    info->h = RECTH(rc);

    info->inc_x = (RECTW(rc)+frame_num)/(2*frame_num);
    info->inc_y = (RECTH(rc)+frame_num)/(2*frame_num);
    info->inc_a = (2*360*PI)/(180.0*frame_num);

    info->z_prp = RECTW(g_rcScr)*3;
    info->center.x = info->w/2;
    info->center.y = -info->w/2;

    info->bmp_bk = bmp_bk;
    info->bmp_fg = bmp_fg;

    info->hdc = GetClientDC(hwnd);
    info->hgp = MGPlusGraphicCreateFromDC(info->hdc);
    MGPlusGraphicLoadBitmap (info->hgp, 1, info->bmp_bk);
    MGPlusGraphicLoadBitmap (info->hgp, 2, info->bmp_fg);
    MGPlusSetInterpolationMode(info->hgp, MP_INTERPOLATION_MODE_BILINEAR);

    info->path = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);
}

void DestroyRotateInfo(ROTATEINFO* info)
{
    MGPlusPathDelete(info->path);
    MGPlusGraphicDelete(info->hgp);
    ReleaseDC(info->hdc);
}

static void def_rotate_bkgnd(HDC hdc, const RECT* rtbk, void * context)
{
    ROTATEINFO *info = (ROTATEINFO *)context;

    /*draw background picture*/
    MGPlusGraphicClear(info->hgp, 0xFF000000);
    MGPlusDrawImageWithPoints(info->hgp, 1, info->pt+4, 4);

}

static void def_rotate_draw_animate(HDC hdc, ANIMATE*ani, void* context)
{
    ROTATEINFO *info = (ROTATEINFO *)context;

    /*draw background picture*/
    MGPlusDrawImageWithPoints(info->hgp, 2, info->pt, 4);
    MGPlusGraphicSave(info->hgp, info->hdc, 0, 0, 0, 0, 0, 0);
}

static void on_end_draw_one_frame(ANIMATE_SENCE *as)
{
    if(as != NULL)
    {
    }
}

void calc_cubicrotate_coor(ANIMATE* ani, int frame_index, void* context)
{
    THREE_D_COORDINATE three_d;
    ROTATEINFO *info = (ROTATEINFO *)context;
    int index = frame_index;
    MPPOINT* pt = info->pt;

    index += 1;
    if (index <= info->frame_num)
    {
        int count = 0;
        MPPOINT* m_pt;
        MGPlusPathReset(info->path);
        MGPlusPathResetTransform (info->path);
        MGPlusPathRotateAroundPoint(info->path, &(info->center), (90/info->frame_num)*index);
        //MGPlusPathAddRectangleI (info->path, 0, -info->w, info->w, info->w);
        MGPlusPathMoveto (info->path, 0, -info->w);
        MGPlusPathLineto (info->path, info->w, -info->w);
        MGPlusPathLineto (info->path, info->w, 0);
        MGPlusPathLineto (info->path, 0, 0);
        MGPlusPathGetPoints (info->path, &count, &m_pt);

        /*the left-bottom vertex*/
        three_d.x = m_pt[3].x;
        three_d.z = m_pt[3].y;
        three_d.y = info->h;
        pt[3].x = three_d.x * info->z_prp/(info->z_prp-three_d.z);
        pt[3].y = three_d.y * info->z_prp/(info->z_prp-three_d.z);

        /*the left-top vertex*/
        pt[0].x = pt[3].x;
        pt[0].y = info->h - pt[3].y;

        /*the right-bottom vertex*/
        three_d.x = m_pt[2].x;
        three_d.z = m_pt[2].y;
        three_d.y = info->h;
        pt[2].x = three_d.x * info->z_prp/(info->z_prp-three_d.z);
        pt[2].y = three_d.y * info->z_prp/(info->z_prp-three_d.z);

        /*the right-top vertex*/
        pt[1].x = pt[2].x;
        pt[1].y = info->h - pt[2].y;

        /*the left-top vertex*/
        pt[4].x = pt[1].x;
        pt[4].y = pt[1].y;

        /*the left-bottom vertex*/
        pt[7].x = pt[2].x;
        pt[7].y = pt[2].y;

        /*the right-bottom vertex*/
        three_d.x = m_pt[1].x;
        three_d.z = m_pt[1].y;
        three_d.y = info->h;
        //pt[6].x = three_d.x * info->z_prp/(info->z_prp-three_d.z);
        pt[6].y = three_d.y * info->z_prp/(info->z_prp-three_d.z);
        pt[6].x = info->w + pt[4].x;

        /*the left-bottom vertex*/
        pt[5].x = pt[6].x;
        pt[5].y = info->h - pt[6].y;
    }
}

void calc_rotate_coor(ANIMATE* ani, int frame_index, void* context)
{
    THREE_D_COORDINATE three_d;
    ROTATEINFO *info = (ROTATEINFO *)context;
    int index = frame_index;
    int r = (info->w - 2*index*info->inc_x)/2;
    MPPOINT* pt = info->pt;
    int x0 = index * info->inc_x + r;
    int y0 = index * info->inc_y;

    if (index <= info->frame_num-1)
    {
        /*draw foreground picture*/
        /*the left-top Vertex*/
        three_d.x = x0 + cos(PI-info->inc_a*index)*r;
        three_d.y = y0;
        three_d.z = sin(PI-info->inc_a*index)*r;

        pt[0].x = three_d.x + cos(PI/6)*three_d.z;
        pt[0].y = three_d.y - sin(PI/6)*three_d.z;

        /*the right-top Vertex*/
        three_d.x = x0 + cos(2*PI-info->inc_a*index)*r;
        three_d.y = y0;
        three_d.z = sin(-info->inc_a*index)*r;

        pt[1].x = three_d.x + cos(PI/6)*three_d.z;
        pt[1].y = three_d.y - sin(PI/6)*three_d.z;

        /*the right-bottom Vertex*/
        pt[2].x = pt[1].x;
        pt[2].y = pt[1].y + (info->h - 2*index*info->inc_y);

        /*the left-bottom Vertex*/
        pt[3].x = pt[0].x;
        pt[3].y = pt[0].y + (info->h - 2*index*info->inc_y);

    }
}

ROTATEINFO g_info;

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
                    ANIMATE_OPS ops = {
                        def_rotate_draw_animate,
                        def_rotate_bkgnd,
                        NULL,
                        NULL,
                        on_end_draw_one_frame
                    };
                    SetInterval (50);
                    InitRotateInfo(hWnd, &g_info, 30, &bmp_res[1], &bmp_res[0]);
                    RunCommonAnimate(g_buffdc, &g_rcScr, &bmp_res[1], calc_cubicrotate_coor,
                            &ops, 31, &g_info);
                    break;
                }
                default:
                break;
            }
            break;
        }
        case MSG_PAINT:
        {
            HDC hdc = BeginPaint(hWnd);
            FillBoxWithBitmap(hdc, g_rcScr.left,g_rcScr.top,
                    RECTW(g_rcScr), RECTH(g_rcScr), &bmp_res[0]);
            EndPaint(hWnd, hdc);
            break;
        }
        case MSG_CLOSE:
        {
            int i;
            for(i=0;i<sizeof(res_file)/sizeof(char *);i++)
                UnloadBitmap (&bmp_res[i]);
            DestroyRotateInfo(&g_info);
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
    CreateInfo.dwExStyle = WS_NONE;
    CreateInfo.spCaption = "Please push F1,F2";
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

    g_buffdc = CreateCompatibleDCEx(HDC_SCREEN, RECTW(g_rcScr), RECTH(g_rcScr));
    if (hMainWnd == HWND_INVALID)
        return -1;
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
