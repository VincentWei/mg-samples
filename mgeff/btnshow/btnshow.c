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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#define dbg()   printf ("F:%s L:%d\n", __FUCTOIN__, __LINE__)

#include <mgeff/mgeff.h>
#include <minigui/control.h>

#define BTN_AMOUNT  48
#define BTN_ROWLEN  12

typedef struct _G_BTN_HANDLE {
    HWND hWnd;
    MGEFF_ANIMATION handle;
    MGEFF_ANIMATION handle_move;
    int is_clicked;
    int lx;
    int ty;
    int rx;
    int by;
} G_BTN_HANDLE;

static G_BTN_HANDLE g_btn_handle[BTN_AMOUNT];

/*buttons' mouse move in motion*/

static WNDPROC  old_button_proc[BTN_AMOUNT];
static int is_finished = 0;

void finished_callback (MGEFF_ANIMATION handle)
{
    is_finished = 1;
}

void set_property (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    HDC hdc;
    hdc = BeginPaint (hWnd);
    MoveWindow (hWnd,
            g_btn_handle[id].lx - *value / 2,
            g_btn_handle[id].ty - *value / 2,
            g_btn_handle[id].rx + *value,
            g_btn_handle[id].by + *value,
            TRUE);
    //fprintf (stderr, "%d\n", *value);
    EndPaint (hWnd, hdc);
}

static LRESULT motion_button_proc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int is_move_out = 1;
    static int old_id = -1;

    MGEFF_ANIMATION handle = 0;
    int i;
    for (i = 0; i < BTN_AMOUNT; ++i) {
        if (g_btn_handle[i].hWnd == hWnd) {
            handle = g_btn_handle[i].handle;
            break;
        }
    }

    if (message == MSG_MOUSEMOVEIN && i != old_id) {
        if (is_move_out) {
            //mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, MGEFF_DIR_FORWARD);
            mGEffAnimationAsyncRun (handle);
            //is_move_out = 0;
            if (g_btn_handle[i].is_clicked) {
                g_btn_handle[i].is_clicked = 0;
            }
        } else {
            mGEffAnimationSetProperty (handle, MGEFF_PROP_DIRECTION, MGEFF_DIR_BACKWARD);
            if (is_finished) {
                mGEffAnimationAsyncRun (handle);
                is_finished = 0;
            }
            is_move_out = 1;
        }
        old_id = i;
    }

    return (*old_button_proc[i]) (hWnd, message, wParam, lParam);
}

void set_animation (MGEFF_ANIMATION handle, int id)
{
    int value;
    value = 1;
    mGEffAnimationSetStartValue (handle, &value);
    value = 40;
    mGEffAnimationSetEndValue (handle, &value);
    mGEffAnimationSetDuration (handle, 500);
    mGEffAnimationSetProperty (handle, MGEFF_PROP_LOOPCOUNT, 1);
    mGEffAnimationSetFinishedCb (handle, finished_callback);
    switch (id) {
        case 0:
            mGEffAnimationSetCurve (handle, OutCirc);
            break;
        case 1:
            mGEffAnimationSetCurve (handle, OutElastic);
            break;
        case 2:
            mGEffAnimationSetCurve (handle, Linear);
            break;
        case 3:
            mGEffAnimationSetCurve (handle, OutBack);
            break;
        case 4:
            mGEffAnimationSetCurve (handle, OutCurve);
            break;
        default:
            break;
    }
}

/*buttons' moving together motion*/

void set_click_property (MGEFF_ANIMATION handle_move, HWND hWnd, int id, int *value)
{
    HDC hdc;
    hdc = BeginPaint (hWnd);
    //do
    MoveWindow (hWnd,
            g_btn_handle[id].lx,
            g_btn_handle[id].ty + *value,
            g_btn_handle[id].rx,
            g_btn_handle[id].by,
            TRUE);
    //fprintf (stderr, "%d\n", *value);
    EndPaint (hWnd, hdc);
}

void set_click_animation (MGEFF_ANIMATION handle_move, int id)
{
    int value;
    value = 1;
    mGEffAnimationSetStartValue (handle_move, &value);
    value = 350 - 25 * id;
    mGEffAnimationSetEndValue (handle_move, &value);
    mGEffAnimationSetDuration (handle_move, 2000);
    mGEffAnimationSetProperty (handle_move, MGEFF_PROP_LOOPCOUNT, 1);
    mGEffAnimationSetCurve (handle_move, OutBounce);
}

void set_move_property (MGEFF_ANIMATION handle_move, HWND hWnd, int id, int *value)
{
    int i = (id + id / BTN_ROWLEN) % 2;
    HDC hdc;
    hdc = BeginPaint (hWnd);

    //do
    if (i) {
        MoveWindow (hWnd,
                g_btn_handle[id].lx - 20 - *value,
                g_btn_handle[id].ty - 20 + *value,
                g_btn_handle[id].rx + 40,
                g_btn_handle[id].by + 40,
                TRUE);
    } else {
         MoveWindow (hWnd,
                g_btn_handle[id].lx - 20 + *value,
                g_btn_handle[id].ty - 20 - *value,
                g_btn_handle[id].rx + 40,
                g_btn_handle[id].by + 40,
                TRUE);
    }
    //fprintf (stderr, "%d\n", *value);
    EndPaint (hWnd, hdc);
}

void set_move_animation (MGEFF_ANIMATION handle_move, int id)
{
    int value;
    value = -5;
    mGEffAnimationSetStartValue (handle_move, &value);
    value = 0;
    mGEffAnimationSetEndValue (handle_move, &value);
    mGEffAnimationSetDuration (handle_move, 200);
    mGEffAnimationSetProperty (handle_move, MGEFF_PROP_LOOPCOUNT, 3);
    mGEffAnimationSetCurve (handle_move, Linear);
}

void click_the_btn (HWND hWnd)
{
    MGEFF_ANIMATION handle_move;
    int i;
    for (i = 0; i < BTN_AMOUNT; ++i) {
        if (hWnd == g_btn_handle[i].hWnd) {
            handle_move = mGEffAnimationCreate ((void *)g_btn_handle[i].hWnd, (void *)set_click_property, i, MGEFF_INT);
            mGEffAnimationSetProperty (handle_move, MGEFF_PROP_KEEPALIVE, 0);
            set_click_animation (handle_move, i / BTN_ROWLEN);
            g_btn_handle[i].is_clicked = 1;
        }
        else {
            if (g_btn_handle[i].is_clicked) {
                continue;
            }
            handle_move = mGEffAnimationCreate ((void *)g_btn_handle[i].hWnd, (void *)set_move_property, i, MGEFF_INT);
            mGEffAnimationSetProperty (handle_move, MGEFF_PROP_KEEPALIVE, 0);
            set_move_animation (handle_move, i);
        }
        mGEffAnimationAsyncRun (handle_move);
        g_btn_handle[i].handle_move = handle_move;
    }
}

/*main windonw proc*/
static LRESULT main_win_proc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND btn_hWnd;
    MGEFF_ANIMATION handle;
    switch (message) {
        case MSG_CREATE:
            {
                int i;
                int lx = 50;
                int ty = 50;
                int rx = 20;
                int by = 20;

                mGEffInit ();

                for (i = 0; i < BTN_AMOUNT; ++i) {
                    btn_hWnd = CreateWindow (CTRL_BUTTON,
                            "mgeff",
                            WS_CHILD | WS_VISIBLE,
                            101 + i,
                            lx, ty, rx, by,
                            hWnd,
                            0);

                    old_button_proc[i] = SetWindowCallbackProc (btn_hWnd, motion_button_proc);

                    handle = mGEffAnimationCreate ((void *)btn_hWnd, (void *)set_property, i, MGEFF_INT);

                    set_animation (handle, 0);

                    mGEffAnimationAsyncRun (handle);

                    g_btn_handle[i].hWnd = btn_hWnd;
                    g_btn_handle[i].handle = handle;
                    g_btn_handle[i].is_clicked = 0;
                    g_btn_handle[i].lx = lx;
                    g_btn_handle[i].ty = ty;
                    g_btn_handle[i].rx = rx;
                    g_btn_handle[i].by = by;

                    lx += 60;
                    if ((i % BTN_ROWLEN) == (BTN_ROWLEN - 1)) {
                        ty += 60;
                        lx = 50;
                    }
                }
            }
            break;
        case MSG_PAINT:
            break;
        case MSG_COMMAND:
            click_the_btn ((HWND)lParam);
            break;
        case MSG_CLOSE:
            {
                int i;
                for (i = 0; i < BTN_AMOUNT; ++i) {
                    mGEffAnimationDelete (g_btn_handle[i].handle);
                    //mGEffAnimationDelete (g_btn_handle[i].handle_move);
                }
                mGEffDeinit ();
                DestroyMainWindow (hWnd);
                PostQuitMessage (hWnd);
            }
            break;
        default:
            break;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

/*main*/

int MiniGUIMain (int argc, const char **argv)
{
    MSG msg;
    HWND hMainHwnd;
    MAINWINCREATE createInfo;

#ifdef  _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, "btnshow", 0, 0);
#endif
    
    createInfo.dwExStyle = WS_EX_NONE;
    createInfo.spCaption = "btnshow";
    createInfo.hMenu = 0;
    createInfo.hCursor = GetSystemCursor (0);
    createInfo.hIcon = 0;
    createInfo.MainWindowProc = main_win_proc;
    createInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    createInfo.lx = 0;
    createInfo.ty = 0;
    createInfo.rx = 1024;
    createInfo.by = 768;
    createInfo.iBkColor = MakeRGBA (0x2e, 0x1d, 0x01, 0xff);
    createInfo.dwAddData = 0;
    createInfo.hHosting = HWND_DESKTOP;
    hMainHwnd = CreateMainWindow (&createInfo);
    if (hMainHwnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainHwnd, SW_SHOWNORMAL);
    
    while (GetMessage (&msg, hMainHwnd)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    MainWindowThreadCleanup (hMainHwnd);
   
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
