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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/minigui.h>

#include <mgeff/mgeff.h>

/*****************************************************************************/
#define CAPTION    "group_animation_sequential"
#define BAR_HEIGHT 50
#define DURATION   (5 * 1000)
#define START_VAL  0x00
#define END_VAL    0xFF
#define ANIMATION_NUM 3

/*****************************************************************************/
static int g_color[ANIMATION_NUM] = { 0 };

/*****************************************************************************/
/* main window proc */
static LRESULT mainWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
/* draw a frame */
static void draw_frame (HWND hWnd);
/* callback function called when property change */
static void property_callback (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value);
/* create and run an animation */
static int do_animation (HWND hWnd);

/*****************************************************************************/
int MiniGUIMain (int argc, const char *argv[])
{
    HWND hMainHwnd;
    MAINWINCREATE createInfo;
    MSG msg;

#ifdef _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, "animation", 0, 0);
#endif

    createInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    createInfo.dwExStyle = WS_EX_NONE;
    createInfo.spCaption = CAPTION;
    createInfo.hMenu = 0;
    createInfo.hCursor = GetSystemCursor (0);
    createInfo.hIcon = 0;
    createInfo.MainWindowProc = mainWindowProc;
    createInfo.lx = 0;
    createInfo.ty = 0;
    createInfo.rx = 240;
    createInfo.by = 320;
    createInfo.iBkColor = COLOR_lightwhite;
    createInfo.dwAddData = 0;
    createInfo.hHosting = HWND_DESKTOP;

    hMainHwnd = CreateMainWindow (&createInfo);

    if (hMainHwnd == HWND_INVALID) {
        return -1;
    }

    ShowWindow (hMainHwnd, SW_SHOWNORMAL);

    while (GetMessage (&msg, hMainHwnd)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    MainWindowThreadCleanup (hMainHwnd);

    return 0;
}

/*****************************************************************************/
static LRESULT mainWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        /* init mgeff library */
        mGEffInit ();

        do_animation (hWnd);
        break;

    case MSG_PAINT:
        draw_frame (hWnd);
        break;

    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);

        /* deinit mgeff library */
        mGEffDeinit ();
        break;

    default:
        break;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void draw_frame (HWND hWnd)
{
    HDC dc;
    RECT rc;

    int client_w, client_h;
    char str[ANIMATION_NUM][64];

    dc = BeginPaint (hWnd);

    /* get client rect */
    GetClientRect (hWnd, &rc);

    client_w = RECTW (rc);
    client_h = RECTH (rc);// - BAR_HEIGHT) * g_value / (END_VAL - START_VAL);

    /* draw */
    /* red */
    SetBrushColor (dc, RGB2Pixel (dc, g_color[0], 0, 0));
    FillBox (dc, 0, client_h * 0 / ANIMATION_NUM, client_w, client_h / ANIMATION_NUM);

    sprintf (str[0], "%d", g_color[0]);
    TextOut (dc, 0, client_h * 0 / ANIMATION_NUM, str[0]);

    /* green */
    SetBrushColor (dc, RGB2Pixel (dc, 0, g_color[1], 0));
    FillBox (dc, 0, client_h * 1 / ANIMATION_NUM, client_w, client_h / ANIMATION_NUM);

    sprintf (str[1], "%d", g_color[1]);
    TextOut (dc, 0, client_h * 1 / ANIMATION_NUM, str[1]);

    /* blue */
    SetBrushColor (dc, RGB2Pixel (dc, 0, 0, g_color[2]));
    FillBox (dc, 0, client_h * 2 / ANIMATION_NUM, client_w, client_h / ANIMATION_NUM);

    sprintf (str[2], "%d", g_color[2]);
    TextOut (dc, 0, client_h * 2 / ANIMATION_NUM, str[2]);

    EndPaint (hWnd, dc);
}

static void property_callback (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    g_color[id] = *value;

    InvalidateRect (hWnd, NULL, TRUE);
}

static int do_animation (HWND hWnd)
{
    MGEFF_ANIMATION animation[ANIMATION_NUM];
    MGEFF_ANIMATION group_animation;

    int duration;;
    int start_val;
    int end_val;

    int i;

    /* set value */
    duration = DURATION;
    start_val = START_VAL;
    end_val = END_VAL;

    /* create an animation group */
    //group_animation = mGEffAnimationCreateGroup (MGEFF_PARALLEL);
    group_animation = mGEffAnimationCreateGroup (MGEFF_SEQUENTIAL);

    /* create and animation and add it to a group */
    for (i = 0; i < ANIMATION_NUM; i++) {
        /* create animation */
        animation[i] = mGEffAnimationCreate ((void *) hWnd, (void *) property_callback, i, MGEFF_INT);

        /* set property */
        /* duration */
        mGEffAnimationSetDuration (animation[i], duration);

        /* start value */
        mGEffAnimationSetStartValue (animation[i], &start_val);

        /* end value */
        mGEffAnimationSetEndValue (animation[i], &end_val);

        /* add animation to group */
        mGEffAnimationAddToGroup (group_animation, animation[i]);
    }

    /* running */
    mGEffAnimationAsyncRun (group_animation);

    /* wait animation end */
    mGEffAnimationWait ((void *) &hWnd, group_animation);

    /* delete the animation object */
    mGEffAnimationDelete (group_animation);

    return 0;
}

