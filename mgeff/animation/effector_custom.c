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
#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/minigui.h>

#include <mgeff/mgeff.h>

/*****************************************************************************/
#define CAPTION    "effector_custom"

//custom effector define
#define MGEFF_MINOR_custom "custom"
#define MGEFF_EFFECTOR_CUSTOM mGEffStr2Key(MGEFF_MINOR_custom)

typedef struct _EffCustomContext
{
    int prev_reach;
} EffCustomContext;

void eff_get_rect (HDC hdc, RECT * rect)
{
    rect->left = 0;
    rect->top = 0;
    rect->right = GetGDCapability (hdc, GDCAP_MAXX) + 1;
    rect->bottom = GetGDCapability (hdc, GDCAP_MAXY) + 1;
}

static MGEFF_EFFECTOR effcustomeffector_init (MGEFF_EFFECTOR effector)
{
    EffCustomContext *context = (EffCustomContext *) calloc (1, sizeof (EffCustomContext));
    context->prev_reach = 0;
    mGEffEffectorSetContext (effector, context);
    return effector;
}

static void effcustomeffector_finalize (MGEFF_EFFECTOR effector)
{
    EffCustomContext *context = (EffCustomContext *) mGEffEffectorGetContext (effector);
    free (context);
}

static void effcustomeffector_ondraw (MGEFF_ANIMATION anim, MGEFF_EFFECTOR effector, HDC sink_dc, intptr_t id, void *value)
{
    EffCustomContext *context = (EffCustomContext *) mGEffEffectorGetContext (effector);
    MGEFF_SOURCE src1 = mGEffEffectorGetSource (effector, 0);
    MGEFF_SOURCE src2 = mGEffEffectorGetSource (effector, 1);
    HDC src1_dc = mGEffGetSourceDC (src1);
    HDC src2_dc = mGEffGetSourceDC (src2);
    RECT rc1, rc2, rc_sink;
    eff_get_rect (src1_dc, &rc1);
    eff_get_rect (src2_dc, &rc2);
    eff_get_rect (sink_dc, &rc_sink);

    int reach2 = RECTH (rc2) * (*(float *) value);
    BitBlt (src2_dc, rc2.left, reach2, RECTW (rc2), rc2.bottom, sink_dc, 0, 0, 0);
    if (context->prev_reach < reach2) {
        int reach1 = RECTH (rc1) - reach2;
        BitBlt (src1_dc, rc1.left, reach1, RECTW (rc1), rc1.bottom, sink_dc, 0, reach1, 0);
    }

    context->prev_reach = reach2;
}

static void effcustomeffector_begindraw (MGEFF_ANIMATION anim, MGEFF_EFFECTOR effector)
{
    float s = 1.0;
    float e = 0.0;

    EffCustomContext *context = (EffCustomContext *) mGEffEffectorGetContext (effector);
    context->prev_reach = s;
    mGEffAnimationSetStartValue (anim, &s);
    mGEffAnimationSetEndValue (anim, &e);
    mGEffAnimationSetCurve (anim, OutBounce);
}

static void effcustomeffector_enddraw (MGEFF_ANIMATION anim, MGEFF_EFFECTOR effector)
{
}

static int effcustomeffector_setproperty (MGEFF_EFFECTOR effector, int property_id, int value)
{
    return 0;
}

static int effcustomeffector_getproperty (MGEFF_EFFECTOR effector, int property_id, int *pValue)
{
    return 0;
}

MGEFF_EFFECTOROPS custom = {
    MGEFF_MINOR_custom,
    MGEFF_FLOAT,
    effcustomeffector_init,
    effcustomeffector_finalize,
    effcustomeffector_ondraw,
    effcustomeffector_begindraw,
    effcustomeffector_enddraw,
    effcustomeffector_setproperty,
    effcustomeffector_getproperty,
};

/*-------------------------------------------------------------------------*/
static HDC createDCByPicture (HDC hdc, int color)
{
    HDC dc;
    int w, h;

    w = GetGDCapability (hdc, GDCAP_MAXX) + 1;
    h = GetGDCapability (hdc, GDCAP_MAXY) + 1;
    dc = CreateCompatibleDCEx (hdc, w, h);
    SetBrushColor (dc, color);
    FillBox (dc, 0, 0, w, h);

    return dc;
}

int fillAnimation (HDC src1_dc, HDC src2_dc, HDC dst_dc, const char *eff, int duration)
{
    unsigned long key = mGEffStr2Key (eff);
    MGEFF_ANIMATION handle;

    MGEFF_EFFECTOR effector = mGEffEffectorCreate (key);

    MGEFF_SOURCE source1 = mGEffCreateSource (src1_dc);
    MGEFF_SOURCE source2 = mGEffCreateSource (src2_dc);

    MGEFF_SINK sink = mGEffCreateHDCSink (dst_dc);

    mGEffEffectorAppendSource (effector, source1);
    mGEffEffectorAppendSource (effector, source2);

    mGEffSetBufferSink (sink, src1_dc);

    mGEffEffectorSetSink (effector, sink);

    handle = mGEffAnimationCreateWithEffector (effector);

    mGEffAnimationSetDuration (handle, duration);

    mGEffAnimationSyncRun (handle);
    mGEffAnimationDelete (handle);

    mGEffEffectorDelete (effector);

    return 0;
}

static int wndCreate (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

static int wndPaint (HWND hwnd, HDC hdc, int message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

static int wndKeyDown (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC src1, src2;


    hdc = GetClientDC (hwnd);
    src1 = createDCByPicture (hdc, 0xff9abbe3);
    src2 = createDCByPicture (hdc, 0xffd7f1be);

    fillAnimation (src1, src2, hdc, MGEFF_MINOR_custom, 1000);

    DeleteMemDC (src1);
    DeleteMemDC (src2);

    ReleaseDC (hdc);

    return 0;
}

static LRESULT wndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        mGEffInit ();
        mGEffEffectorRegister (&custom);
        wndCreate (hwnd, message, wParam, lParam);
        break;
    case MSG_PAINT:
        {
            HDC hdc;

            hdc = BeginPaint (hwnd);
            wndPaint (hwnd, hdc, message, wParam, lParam);
            EndPaint (hwnd, hdc);
        }
        break;
    case MSG_KEYDOWN:
        wndKeyDown (hwnd, message, wParam, lParam);
        break;
    case MSG_CLOSE:
        mGEffEffectorUnRegister (&custom);
        mGEffDeinit ();
        DestroyMainWindow (hwnd);
        PostQuitMessage (hwnd);
        break;
    default:
        break;
    }

    return DefaultMainWinProc (hwnd, message, wParam, lParam);
}

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
    createInfo.MainWindowProc = wndProc;
    createInfo.lx = 0;
    createInfo.ty = 0;
    createInfo.rx = 400;
    createInfo.by = 400;
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
