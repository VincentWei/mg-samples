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
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgeff/mgeff.h>

/*****************************************************************************/
#define ME_DIRECTION_LEFT       0
#define ME_DIRECTION_TOP        1
#define ME_DIRECTION_RIGHT      2
#define ME_DIRECTION_BOTTOM     3

#define ME_COVER_NO     0
#define ME_COVER_YES    1

/*****************************************************************************/
typedef struct _EffMECtxt {
    float prev_reach;
} EffMECtxt;

static int g_index = 0;
static HDC g_hdc_pic[2];

/*****************************************************************************/
static void eff_get_rect (HDC hdc, RECT * rect);
static void eff_bitblt (HDC src1_dc, HDC src2_dc, HDC sink_dc,
                        PRECT prc, int reach, int direction, int cover);
static HDC createDCByPicture (HDC hdc, const char *path, int w, int h);

/*****************************************************************************/
/* mGEffEffector1 sample */
#define MGEFF_MINOR_me1    "me1"
#define MGEFF_EFFECTOR_ME1 mGEffStr2Key(MGEFF_MINOR_me1)

/* function */
static MGEFF_EFFECTOR effme1effector_init (MGEFF_EFFECTOR effector);
static void effme1effector_finalize (MGEFF_EFFECTOR effector);
static void effme1effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value);
static void effme1effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector);

/* struct */
MGEFF_EFFECTOROPS me1 = {
    MGEFF_MINOR_me1,
    MGEFF_FLOAT,
    effme1effector_init,
    effme1effector_finalize,
    effme1effector_ondraw,
    effme1effector_begindraw,
    NULL,
    NULL,
    NULL,
};

/*****************************************************************************/
/* mGEffEffector2 sample */
#define MGEFF_MINOR_me2    "me2"
#define MGEFF_EFFECTOR_ME2  mGEffStr2Key(MGEFF_MINOR_me2)

/* function */
static MGEFF_EFFECTOR effme2effector_init (MGEFF_EFFECTOR effector);
static void effme2effector_finalize (MGEFF_EFFECTOR effector);
static void effme2effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value);
static void effme2effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector);

/* struct */
MGEFF_EFFECTOROPS me2 = {
    MGEFF_MINOR_me2,
    MGEFF_FLOAT,
    effme2effector_init,
    effme2effector_finalize,
    effme2effector_ondraw,
    effme2effector_begindraw,
    NULL,
    NULL,
    NULL,
};

/*****************************************************************************/
/* mGEffEffector3 sample */
#define MGEFF_MINOR_me3    "me3"
#define MGEFF_EFFECTOR_ME3  mGEffStr2Key(MGEFF_MINOR_me3)

/* function */
static void effme3effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value);
static void effme3effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector);

/* struct */
MGEFF_EFFECTOROPS me3 = {
    MGEFF_MINOR_me3,
    MGEFF_FLOAT,
    NULL,
    NULL,
    effme3effector_ondraw,
    effme3effector_begindraw,
    NULL,
    NULL,
    NULL
};

/*****************************************************************************/
/* mGEffEffector4 sample */
#define MGEFF_MINOR_me4     "me4"
#define MGEFF_EFFECTOR_ME4  mGEffStr2Key(MGEFF_MINOR_me4)

/* function */
static MGEFF_EFFECTOR effme4effector_init (MGEFF_EFFECTOR effector);
static void effme4effector_finalize (MGEFF_EFFECTOR effector);
static void effme4effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value);
static void effme4effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector);

/* struct */
MGEFF_EFFECTOROPS me4 = {
    MGEFF_MINOR_me4,
    MGEFF_FLOAT,
    effme4effector_init,
    effme4effector_finalize,
    effme4effector_ondraw,
    effme4effector_begindraw,
    NULL,
    NULL,
    NULL,
};

/*****************************************************************************/
/* mGEffEffector5 sample */
#define MGEFF_MINOR_me5     "me5"
#define MGEFF_EFFECTOR_ME5  mGEffStr2Key(MGEFF_MINOR_me5)

/* function */
static MGEFF_EFFECTOR effme5effector_init (MGEFF_EFFECTOR effector);
static void effme5effector_finalize (MGEFF_EFFECTOR effector);
static void effme5effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value);
static void effme5effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector);

/* struct */
MGEFF_EFFECTOROPS me5 = {
    MGEFF_MINOR_me5,
    MGEFF_FLOAT,
    effme5effector_init,
    effme5effector_finalize,
    effme5effector_ondraw,
    effme5effector_begindraw,
    NULL,
    NULL,
    NULL,
};
/*****************************************************************************/

/*****************************************************************************/
static void eff_get_rect (HDC hdc, RECT * rect)
{
    rect->left = 0;
    rect->top = 0;

    rect->right = GetGDCapability (hdc, GDCAP_MAXX) + 1;
    rect->bottom = GetGDCapability (hdc, GDCAP_MAXY) + 1;
}

static void eff_bitblt (HDC src1_dc, HDC src2_dc, HDC sink_dc,
                        PRECT prc, int reach, int direction, int cover)
{
    switch (direction) {
    case ME_DIRECTION_LEFT:
        BITBLT (src2_dc, prc->left + reach, prc->top,
                RECTWP (prc) - reach, RECTHP (prc), sink_dc, prc->left,
                prc->top, 0);

        if (!cover)
            BITBLT (src1_dc, RECTWP (prc) - reach, prc->top, reach,
                    RECTHP (prc), sink_dc, RECTWP (prc) - reach,
                    prc->top, 0);

        break;

    case ME_DIRECTION_TOP:
        BITBLT (src2_dc, prc->left, prc->top + reach, RECTWP (prc),
                RECTHP (prc) - reach, sink_dc, prc->left, prc->top, 0);

        if (!cover)
            BITBLT (src1_dc, prc->left, RECTHP (prc) - reach,
                    RECTWP (prc), reach, sink_dc, prc->left,
                    RECTHP (prc) - reach, 0);

        break;

    case ME_DIRECTION_RIGHT:
        BITBLT (src2_dc, prc->left, prc->top, RECTWP (prc) - reach,
                RECTHP (prc), sink_dc, prc->left + reach, prc->top, 0);

        if (!cover)
            BITBLT (src1_dc, prc->left, prc->top, reach, RECTHP (prc),
                    sink_dc, prc->left, prc->top, 0);

        break;

    case ME_DIRECTION_BOTTOM:
        BITBLT (src2_dc, prc->left, prc->top, RECTWP (prc),
                RECTHP (prc) - reach, sink_dc, prc->left,
                prc->top + reach, 0);

        if (!cover)
            BITBLT (src1_dc, prc->left, prc->top, RECTWP (prc), reach,
                    sink_dc, prc->left, prc->top, 0);

        break;
    }
}

/*****************************************************************************/
static MGEFF_EFFECTOR effme1effector_init (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) calloc (1, sizeof (EffMECtxt));

    context->prev_reach = 0;

    mGEffEffectorSetContext (effector, context);

    return effector;
}

static void effme1effector_finalize (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    free (context);
}

static void effme1effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value)
{
    EffMECtxt *context;
    MGEFF_SOURCE src1, src2;

    HDC src1_dc, src2_dc;
    RECT rc1, rc2, rc_sink;

    int reach1, reach2;


    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    src1 = mGEffEffectorGetSource (effector, 0);
    src2 = mGEffEffectorGetSource (effector, 1);

    src1_dc = mGEffGetSourceDC (src1);
    src2_dc = mGEffGetSourceDC (src2);

    eff_get_rect (src1_dc, &rc1);
    eff_get_rect (src2_dc, &rc2);
    eff_get_rect (sink_dc, &rc_sink);

    reach2 = RECTH (rc2) * (*(float *) value);

    BitBlt (src2_dc, rc2.left, reach2, RECTW (rc2), rc2.bottom, sink_dc,
            0, 0, 0);

    if (context->prev_reach < reach2) {
        reach1 = RECTH (rc1) - reach2;

        BitBlt (src1_dc, rc1.left, reach1, RECTW (rc1), rc1.bottom,
                sink_dc, 0, reach1, 0);
    }

    context->prev_reach = reach2;
}

static void effme1effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    float s = 1.0;
    float e = 0.0;


    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    context->prev_reach = s;

    mGEffAnimationSetStartValue (anim, &s);
    mGEffAnimationSetEndValue (anim, &e);

    mGEffAnimationSetCurve (anim, OutBounce);
}

/*****************************************************************************/
static MGEFF_EFFECTOR effme2effector_init (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) calloc (1, sizeof (EffMECtxt));

    context->prev_reach = 0;

    mGEffEffectorSetContext (effector, context);

    return effector;
}

static void effme2effector_finalize (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    free (context);
}

static void effme2effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value)
{
    EffMECtxt *context;
    MGEFF_SOURCE src1, src2;

    HDC src1_dc, src2_dc;
    RECT rc1, rc2, rc_sink;

    RECT rc = { 0, 0, 0, 0 };

    float reach;
    int cover;

    int half_w, half_h;
    int reach_w, reach_h;

    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    src1 = mGEffEffectorGetSource (effector, 0);
    src2 = mGEffEffectorGetSource (effector, 1);

    src1_dc = mGEffGetSourceDC (src1);
    src2_dc = mGEffGetSourceDC (src2);

    eff_get_rect (src1_dc, &rc1);
    eff_get_rect (src2_dc, &rc2);
    eff_get_rect (sink_dc, &rc_sink);

    reach = (*(float *) value);

    half_w = RECTW (rc_sink) / 2;
    half_h = RECTH (rc_sink) / 2;
    reach_w = half_w * reach;
    reach_h = half_h * reach;

    if (context->prev_reach < reach_w)
        cover = ME_COVER_NO;
    else
        cover = ME_COVER_YES;

    rc.left    = 0;
    rc.top     = 0;

    /* */
    rc.right   = half_w;
    rc.bottom  = half_h;

    eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach_w,
                ME_DIRECTION_LEFT, cover);

    /* */
    rc.left = half_w;
    rc.right = RECTW (rc_sink);

    eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach_h,
                ME_DIRECTION_TOP, cover);

    /* */
    rc.top = RECTH (rc_sink) - half_h;
    rc.bottom = RECTH (rc_sink);

    eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach_w,
                ME_DIRECTION_RIGHT, cover);

    /* */
    rc.left = 0;
    rc.right = RECTW (rc_sink) - half_w;

    eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach_h,
                ME_DIRECTION_BOTTOM, cover);

    context->prev_reach = reach_w;
}

static void effme2effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    float s = 1.0;
    float e = 0.0;


    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    context->prev_reach = e;

    mGEffAnimationSetStartValue (anim, &s);
    mGEffAnimationSetEndValue (anim, &e);

    mGEffAnimationSetCurve (anim, OutBounce);
}


/*****************************************************************************/
static void effme3effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value)
{
    //EffMECtxt *context;
    MGEFF_SOURCE src1, src2;

    HDC src1_dc, src2_dc;
    RECT rc1, rc2, rc_sink;
    RECT rc = { 0, 0, 0, 0 };

    int dv_w;
    int dv_h[3];

    int reach;
    int i;

    src1 = mGEffEffectorGetSource (effector, 0);
    src2 = mGEffEffectorGetSource (effector, 1);

    src1_dc = mGEffGetSourceDC (src1);
    src2_dc = mGEffGetSourceDC (src2);

    eff_get_rect (src1_dc, &rc1);
    eff_get_rect (src2_dc, &rc2);
    eff_get_rect (sink_dc, &rc_sink);

    dv_w = RECTW (rc2) / 3 + RECTW (rc2) % 3;

    dv_h[0] = RECTH (rc2) / 6;
    dv_h[1] = RECTH (rc2) / 12 * 5;
    dv_h[2] = RECTH (rc2) - dv_h[0] - dv_h[1] - dv_h[2];

    reach = dv_w * 2 * (*(float *) value);

    rc.right = dv_w * 2;

    for (i = 0; i < 3; ++i) {
        rc.top = rc.bottom;
        rc.bottom += dv_h[i];

        if ((i % 2))
            rc.right += dv_w;
        else
            rc.right -= dv_w;

        eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach,
                    ME_DIRECTION_LEFT, ME_COVER_YES);
    }

    memset (&rc, 0, sizeof (RECT));

    rc.left = dv_w * 2;
    rc.right = RECTW (rc2);

    for (i = 0; i < 3; ++i) {
        rc.top = rc.bottom;
        rc.bottom += dv_h[i];

        if ((i % 2))
            rc.left += dv_w;
        else
            rc.left -= dv_w;

        eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach,
                    ME_DIRECTION_RIGHT, ME_COVER_YES);
    }
}

static void effme3effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector)
{
    float s = 1.0;
    float e = 0.0;


    mGEffAnimationSetStartValue (anim, &s);
    mGEffAnimationSetEndValue (anim, &e);

    mGEffAnimationSetCurve (anim, OutQuint);
}


/*****************************************************************************/
static MGEFF_EFFECTOR effme4effector_init (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) calloc (1, sizeof (EffMECtxt));

    mGEffEffectorSetContext (effector, context);

    return effector;
}

static void effme4effector_finalize (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    free (context);
}

static void effme4effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value)
{
    EffMECtxt *context;
    MGEFF_SOURCE src1, src2;

    HDC src1_dc, src2_dc;
    RECT rc1, rc2, rc_sink;
    RECT rc[] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    };

    float reach_raw;
    int reach = 0;
    int dv_w, dv_h;

    float step_i;

    const int runners = 4;
    const float step = 1.0 / runners;
    int i;

    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    src1 = mGEffEffectorGetSource (effector, 0);
    src2 = mGEffEffectorGetSource (effector, 1);

    src1_dc = mGEffGetSourceDC (src1);
    src2_dc = mGEffGetSourceDC (src2);

    eff_get_rect (src1_dc, &rc1);
    eff_get_rect (src2_dc, &rc2);
    eff_get_rect (sink_dc, &rc_sink);

    reach_raw = *(float *) value;
    reach = 0;

    dv_w = RECTW (rc2) / 4;
    dv_h = RECTH (rc2) / 2;

    rc[0].bottom = dv_h;
    rc[1].bottom = RECTH (rc2);
    rc[1].top = dv_h;

    for (i = 0; i < runners; ++i) {
        rc[0].left = rc[0].right;
        rc[1].left = rc[1].right; rc[0].right += dv_w;
        rc[1].right += dv_w;

        step_i = step * i;

        if (reach_raw < step_i) {
            if (reach_raw == 0 || context->prev_reach > step_i) {
                reach = (reach_raw == 0) ? 0 : step_i;

                eff_bitblt (src1_dc, src2_dc, sink_dc, &rc[0], reach,
                            ME_DIRECTION_TOP, ME_COVER_YES);

                eff_bitblt (src1_dc, src2_dc, sink_dc, &rc[1], reach,
                            ME_DIRECTION_BOTTOM, ME_COVER_YES);
            }

            continue;
        }

        reach = RECTH (rc2) * (reach_raw - step_i) / (1.0 - step_i);

        eff_bitblt (src1_dc, src2_dc, sink_dc, &rc[0], reach,
                    ME_DIRECTION_TOP, ME_COVER_YES);

        eff_bitblt (src1_dc, src2_dc, sink_dc, &rc[1], reach,
                    ME_DIRECTION_BOTTOM, ME_COVER_YES);
    }

    context->prev_reach = reach_raw;
}

static void effme4effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    float s = 1.0;
    float e = 0.0;


    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    context->prev_reach = e;

    mGEffAnimationSetStartValue (anim, &s);
    mGEffAnimationSetEndValue (anim, &e);
}

/*****************************************************************************/
static MGEFF_EFFECTOR effme5effector_init (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    context = (EffMECtxt *) calloc (1, sizeof (EffMECtxt));

    mGEffEffectorSetContext (effector, context);

    return effector;
}

static void effme5effector_finalize (MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;
    
    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    free (context);
}

static void effme5effector_ondraw (MGEFF_ANIMATION anim,
                                   MGEFF_EFFECTOR effector, HDC sink_dc,
                                   intptr_t id, void *value)
{
    EffMECtxt *context;
    MGEFF_SOURCE src1, src2;

    HDC src1_dc, src2_dc;
    RECT rc1, rc2, rc_sink;

    float reach_raw;
    float step = 0.5;

    int reach;
    int prev_reach;

    int cover;

    int dv_h[3];


    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    src1 = mGEffEffectorGetSource (effector, 0);
    src2 = mGEffEffectorGetSource (effector, 1);

    src1_dc = mGEffGetSourceDC (src1);
    src2_dc = mGEffGetSourceDC (src2);

    eff_get_rect (src1_dc, &rc1);
    eff_get_rect (src2_dc, &rc2);
    eff_get_rect (sink_dc, &rc_sink);

    reach_raw = *(float *) value;
    step = 0.5;

    dv_h[0] = dv_h[1] = RECTH (rc2) / 3;
    dv_h[2] = RECTH (rc2) - dv_h[0] * 2;

    prev_reach = RECTW (rc2) * context->prev_reach;

    if (reach_raw < step || context->prev_reach < step) {
        if (reach_raw > step) {
            reach = RECTW (rc2);
        }
        else {
            reach = RECTW (rc2) * reach_raw;
        }

        BITBLT (src2_dc, prev_reach, 0, reach, dv_h[0], sink_dc,
                prev_reach, 0, 0);

        BITBLT (src2_dc, RECTW (rc2) - reach, RECTH (rc2) - dv_h[2],
                RECTW (rc2) - prev_reach, dv_h[0], sink_dc,
                RECTW (rc2) - reach, RECTH (rc2) - dv_h[2], 0);
    }
    else {
        cover = ME_COVER_YES;

        reach =
            RECTW (rc2) / 2 * (1.0 - (reach_raw - step) / (1.0 - step));
        if (context->prev_reach > reach_raw)
            cover = ME_COVER_NO;

        RECT rc;
        rc.left = 0;
        rc.top = dv_h[0];
        rc.right = RECTW (rc2) / 2;
        rc.bottom = dv_h[0] + dv_h[1];
        eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach,
                    ME_DIRECTION_LEFT, cover);
        rc.left = rc.right;
        rc.right = RECTW (rc2);
        eff_bitblt (src1_dc, src2_dc, sink_dc, &rc, reach,
                    ME_DIRECTION_RIGHT, cover);
    }

    context->prev_reach = reach_raw;
}

static void effme5effector_begindraw (MGEFF_ANIMATION anim,
                                      MGEFF_EFFECTOR effector)
{
    EffMECtxt *context;

    float s = 0.0;
    float e = 1.0;


    context = (EffMECtxt *) mGEffEffectorGetContext (effector);

    context->prev_reach = e;

    mGEffAnimationSetStartValue (anim, &s);
    mGEffAnimationSetEndValue (anim, &e);

    mGEffAnimationSetCurve (anim, OutBounce);
}

/*****************************************************************************/
static HDC createDCByPicture (HDC hdc, const char *path, int w, int h)
{
    HDC dc;
    BITMAP bmp;

    if (LoadBitmap (hdc, &bmp, path))
        return HDC_INVALID;

    if (w == 0)
        w = bmp.bmWidth;
    if (h == 0)
        h = bmp.bmHeight;

    dc = CreateCompatibleDCEx (hdc, w, h);

    FillBoxWithBitmap (dc, 0, 0, w, h, &bmp);

    UnloadBitmap (&bmp);

    return dc;
}

static int fillAnimation (HWND hWnd, HDC src1_dc, HDC src2_dc,
                          HDC dst_dc, int index, int duration)
{
    unsigned long key;
    MGEFF_ANIMATION anim;
    MGEFF_EFFECTOR effector;
    MGEFF_SOURCE src1, src2;
    MGEFF_SINK sink;

    switch (index) {
    case 0:
        key = MGEFF_EFFECTOR_ME1;
        break;
    case 1:
        key = MGEFF_EFFECTOR_ME2;
        break;
    case 2:
        key = MGEFF_EFFECTOR_ME3;
        break;
    case 3:
        key = MGEFF_EFFECTOR_ME4;
        break;
    case 4:
        key = MGEFF_EFFECTOR_ME5;
        break;
    default:
        key = MGEFF_EFFECTOR_ME1;
        break;
    }

    effector = mGEffEffectorCreate (key);

    src1 = mGEffCreateSource (src1_dc);
    src2 = mGEffCreateSource (src2_dc);

    sink = mGEffCreateHDCSink (dst_dc);

    mGEffEffectorAppendSource (effector, src1);
    mGEffEffectorAppendSource (effector, src2);

    mGEffSetBufferSink (sink, src1_dc);
    mGEffEffectorSetSink (effector, sink);

    anim = mGEffAnimationCreateWithEffector (effector);
    mGEffAnimationSetDuration (anim, duration);

#if 0
    mGEffAnimationAsyncRun (anim);
    mGEffAnimationWait (&hWnd, anim);
#else
    mGEffAnimationSyncRun (anim);
#endif

    mGEffAnimationDelete (anim);

    mGEffEffectorDelete (effector);

    return 0;
}

static LRESULT MEWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        {
            HDC hdc;
            hdc = GetDC (hWnd);

            g_hdc_pic[0] = createDCByPicture (hdc, "res/1.png", 0, 0);
            g_hdc_pic[1] = createDCByPicture (hdc, "res/2.png", 0, 0);

            if (g_hdc_pic[0] == HDC_INVALID
                || g_hdc_pic[1] == HDC_INVALID)
                return -1;

            ReleaseDC (hdc);

            mGEffInit ();

            mGEffEffectorRegister (&me1);
            mGEffEffectorRegister (&me2);
            mGEffEffectorRegister (&me3);
            mGEffEffectorRegister (&me4);
            mGEffEffectorRegister (&me5);
        }
        break;

    case MSG_KEYDOWN:
        switch (wParam) {
        case SCANCODE_CURSORBLOCKLEFT:
            InvalidateRect (hWnd, NULL, TRUE);
            break;

        case SCANCODE_CURSORBLOCKRIGHT:
            InvalidateRect (hWnd, NULL, TRUE);
            break;

        case SCANCODE_ESCAPE:
            SendMessage (hWnd, MSG_CLOSE, 0, 0);
            break;
        }
        return 0;
    case MSG_PAINT:
        {
            static unsigned ex = 0;
            HDC hdc = BeginPaint (hWnd);

            fillAnimation (hWnd, g_hdc_pic[ex % 2],
                           g_hdc_pic[(ex + 1) % 2], hdc, g_index, 1000);

            EndPaint (hWnd, hdc);

            ++ex;

            if (++g_index >= 5)
                g_index = 0;
        }

        return 0;

    case MSG_CLOSE:
        {
            mGEffEffectorUnRegister (&me1);
            mGEffEffectorUnRegister (&me2);
            mGEffEffectorUnRegister (&me3);
            mGEffEffectorUnRegister (&me4);
            mGEffEffectorUnRegister (&me5);

            DeleteMemDC (g_hdc_pic[0]);
            DeleteMemDC (g_hdc_pic[1]);

            mGEffDeinit ();

            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        }
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char **argv)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef  _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, "my effectors", 0, 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE;    // | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "my effectors";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = MEWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 320;
    CreateInfo.by = 240;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}
