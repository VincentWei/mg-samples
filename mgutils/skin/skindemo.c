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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef __MGUTILS_LIB__
#include "../../include/mgutils.h"
#else
#include <mgutils/mgutils.h>
#endif

#ifdef  _MGUTILS_SKIN

#define SIID_TITLE        1
#define SIID_PLAY        2
#define SIID_PAUSE        3
#define SIID_STOP        4
#define SIID_PROGRESS    5
#define SIID_SYSMENU    6
#define SIID_CLOSE        7
#define SIID_VOLUME        8
#define SIID_TIMER        9

#define DEF_WIDTH       284
#define DEF_HEIGHT      135
#define ID_TIME            100

/* 定义皮肤元素特定属性 */
static si_nrmslider_t progress     = { {0, 180, 0 }, 5 };
static si_nrmslider_t volume     = { {1, 100, 50}, 9 };
static si_bmplabel_t timer         = { "00:00", "0123456789:-" };

/* 定义皮肤元素数组 */
static skin_item_t skin_main_items [] =
{
    {SIID_PLAY, SI_TYPE_CHKBUTTON       | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE,
        205, 106, {}, {1}, "Play"},
    {SIID_PAUSE, SI_TYPE_CHKBUTTON   | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE,
        230, 106, {}, {2}, "Pause"},
    {SIID_STOP,  SI_TYPE_CHKBUTTON   | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE,
        254, 106, {}, {3}, "Stop"},
    {SIID_PROGRESS, SI_TYPE_NRMSLIDER| SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE
        | SI_NRMSLIDER_HORZ, 8, 91, {}, {4}, "Progress" , 0, &progress},
    {SIID_SYSMENU, SI_TYPE_CMDBUTTON | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE,
        9, 2, {}, {6}},
    {SIID_CLOSE, SI_TYPE_CMDBUTTON      | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE,
        263, 2, {}, {7}, "Close"},
    {SIID_VOLUME, SI_TYPE_NRMSLIDER  | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE
        | SI_NRMSLIDER_HORZ, 102, 55, {}, {8}, "Volume" , 0, &volume},
    {SIID_TIMER,   SI_TYPE_BMPLABEL  | SI_TEST_SHAPE_RECT | SI_STATUS_VISIBLE,
        20, 67, {}, {10}, "Time", 0, &timer}
};

/* 定义皮肤 */
skin_head_t main_skin =
{
    "播放器皮肤窗口",
    SKIN_STYLE_TOOLTIP, NULL, NULL,
    0, 8, skin_main_items, FALSE
};

/* 位图数组 */
const char *bmp_name[] = {
"main.png", "play.png", "pause.png", "stop.png", "progress-bk.png", "progress.png",
"sysmenu.png", "close.png", "volume-bk.png", "volume.png", "timer.png"
};

static int cur_pos = 0;

/* 位图资源装/卸载函数 */
void load_skin_bmps (skin_head_t *skin, BOOL load)
{
    int i, bmp_num = sizeof (bmp_name)/sizeof(char *);

    /* 如果load为真，则将位图装载到skin的bmps数组，否则卸载bmps数组中的位图 */
    if (load)
        skin->bmps = (BITMAP* ) calloc (bmp_num, sizeof(BITMAP));

    for (i = 0 ; i < bmp_num ; i++ ) {
        if (load) {
            if (LoadBitmapFromFile (HDC_SCREEN, (PBITMAP)&(skin->bmps[i]), bmp_name[i])) {
                free ((void*)skin->bmps);
                skin->bmps = NULL;
            }
        }
        else
            UnloadBitmap ((PBITMAP)&(skin->bmps[i]));
    }

    if (!load)
        free ((void*)skin->bmps);
}

/* 皮肤事件回调函数 */
static int main_event_cb (HWND hwnd, skin_item_t* item, int event, void* data)
{
    char buf[256];
    int minute = 0;
    int second = 0;
    int pos = 0;

    if (event == SIE_BUTTON_CLICKED) {
        switch (item->id) {
        case SIID_PLAY:        /* 皮肤元素SIID_PLAY的SIE_BUTTON_CLICKED事件在这里进行处理 */
            SetTimer (hwnd, ID_TIME, 100);
            skin_set_check_status (item->hostskin, SIID_PAUSE, FALSE);
            skin_set_check_status (item->hostskin, SIID_STOP, FALSE);
            break;
        case SIID_PAUSE:
            skin_set_check_status (item->hostskin, SIID_PLAY, FALSE);
            skin_set_check_status (item->hostskin, SIID_STOP, FALSE);
            KillTimer (hwnd, ID_TIME);
            break;
        case SIID_STOP:
            cur_pos = 0;
            skin_set_check_status (item->hostskin, SIID_PLAY, FALSE);
            skin_set_check_status (item->hostskin, SIID_PAUSE, FALSE);
            KillTimer (hwnd, ID_TIME);
            skin_set_thumb_pos (item->hostskin, SIID_PROGRESS, 0);
            skin_set_item_label (item->hostskin, SIID_TIMER, "00:00");
            break;
        case SIID_CLOSE:
            KillTimer (hwnd, ID_TIME);
            SendMessage (hwnd, MSG_CLOSE, 0, 0);
            break;
        }
    }
    else if (event == SIE_SLIDER_CHANGED && item->id != SIID_VOLUME) {
        pos = (int)(intptr_t)data;
        cur_pos = pos;
        minute = pos / 60;
        second = pos % 60;
        if (second > 9) {
            sprintf (buf, "0%d:%d", minute, second);
        } else {
            sprintf (buf, "0%d:0%d", minute,second);
        }
        skin_set_item_label (item->hostskin, SIID_TIMER, buf);
    }

    return 1;
}

/* 皮肤窗口消息回调函数 */
static int msg_event_cb (HWND hwnd, int message, WPARAM wparam, LPARAM lparam, int* result)
{
    int minute = 0;
    int second = 0;
    char buf[64];
    skin_head_t *hostskin = NULL;

    switch (message) {
        case MSG_TIMER:
            if (cur_pos++ > 180)
                cur_pos = 0;
            minute = cur_pos / 60;
            second = cur_pos % 60;
            if (second > 9)
                sprintf (buf, "0%d:%d", minute, second);
            else
                sprintf (buf, "0%d:0%d", minute,second);

            hostskin = get_window_skin (hwnd);
            skin_set_thumb_pos (hostskin, SIID_PROGRESS, cur_pos);
            skin_set_item_label (hostskin, SIID_TIMER, buf);
            break;
    }
    return 1;
}

int MiniGUIMain (int argc, const char *argv[])
{
    MSG msg;
    HWND hWndMain;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "skindemo" , 0 , 0);
#endif

    load_skin_bmps (&main_skin, TRUE);    /* 装载位图资源 */
    if (main_skin.bmps == NULL) {
        printf ("Could not load skin bitmaps!\n");
        return 3;
    }

    if ( !skin_init (&main_skin, main_event_cb, msg_event_cb) ){    /* 初始化皮肤 */
        printf ("skin init fail!\n");
    }
    else{
        hWndMain = create_skin_main_window (&main_skin,
                        HWND_DESKTOP, 100, 100, 100 + DEF_WIDTH, 100 + DEF_HEIGHT,
                        FALSE);

        while (GetMessage (&msg, hWndMain)) {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }

        MainWindowCleanup (hWndMain);
        skin_deinit (&main_skin);        /* 撤销皮肤 */
    }

    load_skin_bmps (&main_skin, FALSE);    /* 卸载位图资源 */

    return 0;
}

#else

int MiniGUIMain (int argc, const char* argv[])
{
    fprintf (stderr, "Please enable the support for Skin module in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_SKIN */

