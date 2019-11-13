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
#include <sys/times.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include <mgeff/mgeff.h>
#define _POS 0
#define PARTICLE_MAX 64

#define IDC_BTN_0    100
#define BTN_WIDTH   80
#define BTN_NUM 5
#define RECT_SIZE 64

//static POINT s_point[PARTICLE_MAX];

struct _particle_t {
    int x, y;
    gal_pixel color;
    MGEFF_ANIMATION anim;
} g_particles[PARTICLE_MAX];

MGEFF_ANIMATION g_parallel_group;
HDC  g_particles_tile;

void mycb(MGEFF_ANIMATION handle, HWND hwnd, int id, POINT *pos) {
    int i;
    HDC dc = GetClientDC(hwnd);
    SetBrushColor(dc, RGB2Pixel(dc, 0xff, 0xff, 0xff));
    FillBox(dc, g_particles[id].x, g_particles[id].y, RECT_SIZE, RECT_SIZE);

    memcpy(&(g_particles[id].x), pos, sizeof(POINT));
#if 0
    SetBrushColor(dc, RGB2Pixel(dc, 99, 128, 44));
    FillBox(dc, g_particles[id].x, g_particles[id].y, RECT_SIZE, RECT_SIZE);
#else
    if (id == PARTICLE_MAX - 1)
    {
        for (i = 0; i < PARTICLE_MAX; ++i)
        {
            BitBlt(g_particles_tile, 0, 0, -1, -1, dc, g_particles[i].x, g_particles[i].y, 0);
//            FillBox(dc, g_particles[i].x, g_particles[i].y, RECT_SIZE, RECT_SIZE);
        }
    }
#endif

    ReleaseDC(dc);
}

typedef POINT (*CalcPosition)(POINT* pt, int index);

POINT fixed_pos(POINT* pt, int index)
{
    POINT point = {0, 0};
    POINT pos = (NULL != pt) ? *pt : point;
    return pos;
}

POINT figure8_pos(POINT* pt, int i)
{
    POINT point = {0, 0};
    POINT pos = (NULL != pt) ? *pt : point;
    point.x = pos.x + sin((i / 63.0) * 6.28) * 250;
    point.y = pos.y + sin(((i * 2)/63.0) * 6.28) * 250;
    return point;
}

POINT ellipse_pos(POINT* pt, int i)
{
    POINT point = {0, 0};
    POINT pos = (NULL != pt) ? *pt : point;
    point.x = pos.x + cos((i / 63.0) * 6.28) * 250;
    point.y = pos.y + sin((i / 63.0) * 6.28) * 250;

    return point;
}

POINT rectangle_pos(POINT* pt, int i)
{
    POINT point = {0, 0};
    POINT pos = (NULL != pt) ? *pt : point;
    point.x = pos.x + ((i % 8) - 4) * (RECT_SIZE + 10) + 50;
    point.y = pos.y + ((i / 8) - 4) * (RECT_SIZE + 10) + 10;
    return point;
}

POINT random_pos(POINT* pt, int i)
{
    POINT point = {0, 0};
    POINT pos = (NULL != pt) ? *pt : point;
    point.x = pos.x -300 + rand() % 600;
    point.y = pos.y -300 + rand() % 500;
    return point;
}

POINT current_pos(POINT* pt, int i)
{
    POINT point = {0, 0};
    //POINT pos = (NULL != pt) ? *pt : point;
    point.x = g_particles[i].x;
    point.y = g_particles[i].y;
    return point;
}

HDC createDCByPicture(HDC ref_dc, const char* path)
{
    BITMAP bmp;
    HDC dc;
    LoadBitmap(ref_dc, &bmp, path);
    dc = CreateCompatibleDCEx(ref_dc, bmp.bmWidth, bmp.bmHeight);
    FillBoxWithBitmap(dc, 0, 0, bmp.bmWidth, bmp.bmHeight, &bmp);
    UnloadBitmap(&bmp);
    return dc;
}

static MGEFF_ANIMATION animationGroupInit(void * target, RECT *rc, CalcPosition calc_start_pos, CalcPosition calc_end_pos)
{
    POINT center_pos = {0, 0};
    POINT point = {0, 0};
    int i = 0;
    if (NULL == target || NULL == rc) return NULL;
    MGEFF_ANIMATION group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);
    center_pos.x = (rc->left + rc->right - 80) >> 1;
    center_pos.y = (rc->top + rc->bottom - 80) >> 1;
    for (i = 0; i < PARTICLE_MAX; ++i) {
        /* Bug:由于 MGEFF 接口尚未完善，此处未先释放之前分配的动画句柄，存在内存泄漏 */
        MGEFF_ANIMATION animation = mGEffAnimationCreate(target, (void *)mycb, i, MGEFF_POINT);
        /* make all g_particles in center of screen at first.*/
        if (NULL != calc_start_pos)
        {
            point = (*calc_start_pos)(&center_pos, i);
        }
        mGEffAnimationSetStartValue(animation, &point);
        memcpy(&(g_particles[i].x), &point, sizeof(POINT));
        if (NULL != calc_start_pos)
        {
            point = (*calc_end_pos)(&center_pos, i);
        }

        mGEffAnimationSetEndValue(animation, &point);
        mGEffAnimationSetDuration(animation, 1000 + i * 25);
        mGEffAnimationSetCurve(animation, InOutBack);
        g_particles[i].anim = animation;
        mGEffAnimationAddToGroup(group, animation);
    }
    return group;
}

static LRESULT mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
            {
                RECT rc;
                //POINT point;
                int i = 0;
                //MGEFF_ANIMATION animation;
                char *caption[BTN_NUM] = {"Figure8", "Ellipse", "Rectangle", "Center", "Random"};
                HDC hdc = GetDC(hwnd);
                g_particles_tile = createDCByPicture(hdc, "./core.png");
                ReleaseDC(hdc);
                GetClientRect(hwnd, &rc);

                for (i = 0; i < BTN_NUM; ++i)
                {
                    CreateWindowEx ("button", 
                            caption[i], 
                            WS_VISIBLE, 
                            WS_EX_USEPARENTCURSOR,
                            IDC_BTN_0 + i, 
                            80 + i * (BTN_WIDTH + 30), rc.bottom - 30, BTN_WIDTH, 25, hwnd, 0);
                }

                mGEffInit();
                g_parallel_group = animationGroupInit((void*)hwnd, &rc, current_pos, ellipse_pos);

                mGEffAnimationAsyncRun(g_parallel_group);
            }
            break;
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint(hwnd);
                //int i;
                RECT rc;
                GetClientRect(hwnd, &rc);
                EndPaint(hwnd, hdc);
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
        case MSG_COMMAND:
            {
                //int i = 0;
                //int cx, cy;
                //POINT new_pos;
                RECT rc;
                GetClientRect(hwnd, &rc);
                mGEffAnimationPause(g_parallel_group);
                switch (wParam)
                {
                        case IDC_BTN_0:
                            g_parallel_group = 
                                animationGroupInit((void*)hwnd, &rc, current_pos, figure8_pos);
                            break;
                        case IDC_BTN_0 + 1:
                            g_parallel_group = 
                                animationGroupInit((void*)hwnd, &rc, current_pos, ellipse_pos);
                            break;
                        case IDC_BTN_0 + 2:
                            g_parallel_group = 
                                animationGroupInit((void*)hwnd, &rc, current_pos, rectangle_pos);
                            break;
                        case IDC_BTN_0 + 3:
                            g_parallel_group = 
                                animationGroupInit((void*)hwnd, &rc, current_pos, fixed_pos);
                            break;
                         case IDC_BTN_0 + 4:
                            g_parallel_group = 
                                animationGroupInit((void*)hwnd, &rc, current_pos, random_pos);
                            break;
                       default:
                            break;
                }

                mGEffAnimationAsyncRun(g_parallel_group);
            }
            break;
        case MSG_CLOSE:
            mGEffAnimationWait ((void *)&hwnd, g_parallel_group);
            mGEffAnimationDelete (g_parallel_group);
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

    //createInfo.dwStyle = WS_VISIBLE | WS_BORDER;
    createInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    createInfo.dwExStyle = WS_EX_NONE;
    createInfo.spCaption = "particle system";
    createInfo.hMenu = 0;
    createInfo.hCursor = GetSystemCursor(0);
    createInfo.hIcon = 0;
    createInfo.MainWindowProc = mainWindowProc;
    createInfo.lx = 0;  
    createInfo.ty = 0;  
    createInfo.rx = 708; 
    createInfo.by = 647; 
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
