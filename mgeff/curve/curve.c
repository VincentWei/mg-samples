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
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgeff/mgeff.h>

/*****************************************************************************/
#define CAPTION        "mGEff Demo (curve)"
#define BMP_PATH       "res/core.png"

#define DLG_W          640
#define DLG_H          480

#define BORDER         1

#define LISTBOX_X      0
#define LISTBOX_Y      0

#define LISTBOX_W      179
#define LISTBOX_H      454
//#define LISTBOX_H      372

#define STATIC_X       0
#define STATIC_Y       (LISTBOX_Y + LISTBOX_H + BORDER)

#define STATIC_W       LISTBOX_W
#define STATIC_H       80

#define FIXED_W        226
#define FIXED_H        226

#define ANIMATION_NUM  4

/*****************************************************************************/
#define S_CURVE_X      (LISTBOX_X + LISTBOX_W + BORDER)
#define S_CURVE_Y      0

#define S_CURVE_START  0
#define S_CURVE_END    100

#define S_CURVE_LEN    (S_CURVE_END - S_CURVE_START)

/*****************************************************************************/
#define S_ICON_X       (LISTBOX_X + LISTBOX_W + FIXED_W + 2 * BORDER)
#define S_ICON_Y       0

#define S_ICON_START   0
#define S_ICON_END     100

#define S_ICON_LEN     (S_CURVE_END - S_CURVE_START)

/*****************************************************************************/
#define S_BTN_X        (LISTBOX_X + LISTBOX_W + BORDER)
#define S_BTN_Y        (FIXED_H + BORDER)

#define S_BTN_START    50
#define S_BTN_END      150

/*****************************************************************************/
#define S_COLOR_X      (LISTBOX_X + LISTBOX_W + FIXED_W + 2 * BORDER)
#define S_COLOR_Y      (FIXED_H + BORDER)

#define S_COLOR_START  0
#define S_COLOR_END    255

#define S_COLOR_LEN    180

/* ID of controls */
#define IDC_LISTBOX      100
#define IDC_BUTTON       110

#define DURATION         3000
#define FRAME            (DURATION / 20)

/*****************************************************************************/
typedef struct _ANIMATION_PROPERTY {
    int duration;
    int start_val;
    int end_val;
} ANIMATION_PROPERTY;

typedef struct _CURVELIST {
    char name [32];
    enum EffMotionType type;
    int  show;
    int  flag;
} CURVELIST;

/*****************************************************************************/
static void listbox_notif_proc (HWND hwnd, LINT id, int nc, DWORD add_data);

/* draw a frame */
static void draw_frame (HWND hWnd);
/* callback function called when animation finished */
static void finished_callback (MGEFF_ANIMATION handle);
/* callback function called when animation loop count is changed */
static void curloopchanged_callback (MGEFF_ANIMATION handle);
/* callback function called when property change */
static void property_callback (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value);

/* create and run an animation */
MGEFF_ANIMATION do_animation (HWND hWnd, int curve);

/* draw */
static void draw_curve (HDC hdc);
static void draw_icon  (HDC hdc);
static void draw_btn   (HDC hdc);
static void draw_color (HDC hdc);

/*****************************************************************************/
static HWND g_hMainWnd = 0;
static MGEFF_ANIMATION g_animation = NULL;
static int g_curframe = 0;

/* for curve */
static int curve_new_x = S_CURVE_X + (FIXED_W - S_CURVE_LEN) / 2;
static int curve_new_y = S_CURVE_Y + (FIXED_H - S_CURVE_LEN) / 2;

static int curve_old_x = S_CURVE_X + (FIXED_W - S_CURVE_LEN) / 2;
static int curve_old_y = S_CURVE_X + (FIXED_H - S_CURVE_LEN) / 2;

/* for icon */
static int icon_new_x = S_ICON_X + (FIXED_W - S_ICON_LEN) / 2;
static int icon_new_y = S_ICON_Y + (FIXED_H - S_ICON_LEN) / 2;

/* for button */
static int bmp_new_x = S_BTN_X + (FIXED_W - S_BTN_START) / 2;
static int bmp_new_y = S_BTN_Y + (FIXED_H - S_BTN_START) / 2;

static int bmp_new_w = S_BTN_START;
static int bmp_new_h = S_BTN_START;

/* for color */
static int g_color = S_COLOR_START;

ANIMATION_PROPERTY animation_property[ANIMATION_NUM] = {
    { DURATION, S_CURVE_START,  S_CURVE_END },  /* curve */
    { DURATION, S_ICON_START,   S_ICON_END },   /* icon */
    { DURATION, S_BTN_START,    S_BTN_END },    /* button */
    { DURATION, S_COLOR_START,  S_COLOR_END }   /* color */
};

CURVELIST curvelist[NCurveTypes + 1] = {
    { "Linear",  Linear, 1, 0 },
    { "InQuad",  InQuad, 1, 0 },
    { "OutQuad",  OutQuad, 1, 0 },
    { "InOutQuad",  InOutQuad, 1, 0 },
    { "OutInQuad",  OutInQuad, 1, 0 },
    { "InCubic",  InCubic, 1, 0 },
    { "OutCubic",  OutCubic, 1, 0 },
    { "InOutCubic",  InOutCubic, 1, 0 },
    { "OutInCubic",  OutInCubic, 1, 0 },
    { "InQuart",  InQuart, 1, 0 },
    { "OutQuart",  OutQuart, 1, 0 },
    { "InOutQuart",  InOutQuart, 1, 0 },
    { "OutInQuart",  OutInQuart, 1, 0 },
    { "InQuint",  InQuint, 1, 0 },
    { "OutQuint",  OutQuint, 1, 0 },
    { "InOutQuint",  InOutQuint, 1, 0 },
    { "OutInQuint",  OutInQuint, 1, 0 },
    { "InSine",  InSine, 1, 0 },
    { "OutSine",  OutSine, 1, 0 },
    { "InOutSine",  InOutSine, 1, 0 },
    { "OutInSine",  OutInSine, 1, 0 },
    { "InExpo",  InExpo, 1, 0 },
    { "OutExpo",  OutExpo, 1, 0 },
    { "InOutExpo",  InOutExpo, 1, 0 },
    { "OutInExpo",  OutInExpo, 1, 0 },
    { "InCirc",  InCirc, 1, 0 },
    { "OutCirc",  OutCirc, 1, 0 },
    { "InOutCirc",  InOutCirc, 1, 0 },
    { "OutInCirc",  OutInCirc, 1, 0 },
    { "InElastic",  InElastic, 1, 0 },
    { "OutElastic",  OutElastic, 1, 0 },
    { "InOutElastic",  InOutElastic, 1, 0 },
    { "OutInElastic",  OutInElastic, 1, 0 },
    { "InBack",  InBack, 1, 0 },
    { "OutBack",  OutBack, 1, 0 },
    { "InOutBack",  InOutBack, 1, 0 },
    { "OutInBack",  OutInBack, 1, 0 },
    { "InBounce",  InBounce, 1, 0 },
    { "OutBounce",  OutBounce, 1, 0 },
    { "InOutBounce",  InOutBounce, 1, 0 },
    { "OutInBounce",  OutInBounce, 1, 0 },
    { "InCurve",  InCurve, 1, 0 },
    { "OutCurve",  OutCurve, 1, 0 },
    { "SineCurve",  SineCurve, 1, 0 },
    { "CosineCurve",  CosineCurve, 1, 0 },
    { "Custom",  Custom, 0, 0 },
    { "NCurveTypes",  NCurveTypes, 0, 0 }
};

static CTRLDATA CtrlBox[] =
{ 
    {
        CTRL_LISTBOX,
        WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY,
        LISTBOX_X, LISTBOX_Y, LISTBOX_W, LISTBOX_H,
        IDC_LISTBOX,
        "",
        0
    },
#if 0
    {
        CTRL_STATIC,
        WS_VISIBLE | SS_SIMPLE, 
        STATIC_X, STATIC_Y, STATIC_W, STATIC_H, 
        IDC_STATIC,
        "directories",
        0
    },
#endif
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON,
        S_BTN_X + (FIXED_W - S_BTN_START) / 2, S_BTN_Y + (FIXED_H - S_BTN_START) / 2,
        S_BTN_START, S_BTN_START,
        IDC_BUTTON,
        "this is a button!",
        0
    },
};

static DLGTEMPLATE DlgTmpl =
{
    WS_VISIBLE | WS_CAPTION | WS_MINIMIZE | WS_SYSMENU | WS_BORDER,
    //WS_EX_NONE,
    WS_EX_AUTOSECONDARYDC,
    0, 0, DLG_W, DLG_H,
    CAPTION,
    0, 0,
    //0, NULL,
    TABLESIZE(CtrlBox), CtrlBox,
    0,
};

/*****************************************************************************/
/* draw a frame */
static void draw_frame (HWND hWnd)
{
    HDC hdc;

    hdc = BeginPaint (hWnd);

    draw_curve (hdc);
    draw_icon  (hdc);
    draw_btn   (hdc);
    draw_color (hdc);

    EndPaint (hWnd, hdc);
}

/* init list box */
void InitListbox (HWND hDlg, int nListboxID)
{
    HWND hListbox;
    int i;

    hListbox = GetDlgItem (hDlg, nListboxID);

    SendMessage (hDlg, LB_SETCURSEL, 0, 0);

    SetNotificationCallback (hListbox, listbox_notif_proc);

    for (i = 0; i < TABLESIZE (curvelist); i++) {
        if (curvelist[i].show != 1) {
            continue;
        }

        SendDlgItemMessage (hDlg, nListboxID, LB_ADDSTRING, 0, (LPARAM)curvelist[i].name);
    }

}

/* draw curve */
static void draw_curve (HDC hdc)
{
    if (g_curframe == 0) {
        int x1, y1;
        int x2, y2;

        SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF, 0xFF, 0xFF));
        FillBox(hdc, S_CURVE_X, S_CURVE_Y, FIXED_W, FIXED_H);

        /* line */
        x1 = S_CURVE_X + (FIXED_W - S_CURVE_LEN) / 2;
        y1 = S_CURVE_Y;

        x2 = S_CURVE_X + (FIXED_W - S_CURVE_LEN) / 2;
        y2 = S_CURVE_Y + FIXED_H;

        SetPenColor(hdc, RGB2Pixel (hdc, 0x00, 0x00, 0xFF));
        MoveTo (hdc, x1, y1);
        LineTo (hdc, x2, y2);

        /* line */
        x1 = S_CURVE_X;
        y1 = S_CURVE_Y + FIXED_H - (FIXED_H - S_CURVE_LEN) / 2;

        x2 = S_CURVE_X + FIXED_W;
        y2 = S_CURVE_Y + FIXED_H - (FIXED_H - S_CURVE_LEN) / 2;

        SetPenColor(hdc, RGB2Pixel (hdc, 0x00, 0xFF, 0x00));
        MoveTo (hdc, x1, y1);
        LineTo (hdc, x2, y2);
    }
    else {
        SetPenColor(hdc, RGB2Pixel (hdc, 0xFF, 0x00, 0x00));

        MoveTo (hdc, curve_old_x, curve_old_y);
        LineTo (hdc, curve_new_x, curve_new_y);
    }
}

/* draw icon */
static void draw_icon (HDC hdc)
{
    BITMAP *pBmp;

    int fix_x;
    int fix_y;

    SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF, 0xFF, 0xFF));

    FillBox (hdc, S_ICON_X, S_ICON_Y, FIXED_W, FIXED_H);

    pBmp = LoadResource (BMP_PATH, RES_TYPE_IMAGE, (DWORD)HDC_SCREEN);

    fix_x = icon_new_x - pBmp->bmWidth / 2;
    fix_y = icon_new_y - pBmp->bmHeight / 2;

    FillBoxWithBitmap (hdc, fix_x, fix_y, 0, 0, pBmp);
}

/* draw a button */
static void draw_btn (HDC hdc)
{
#if 0
#else
    HWND hButton;
#endif

    SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF, 0xFF, 0xFF));

    FillBox(hdc, S_BTN_X, S_BTN_Y, FIXED_W, FIXED_H);

#if 0
    SetBrushColor (hdc, RGB2Pixel (hdc, 0x00, 0x00, 0xFF));

    FillBox(hdc, bmp_new_x, bmp_new_y, bmp_new_w, bmp_new_h);
#else
    hButton = GetDlgItem (g_hMainWnd, IDC_BUTTON);

    MoveWindow (hButton, bmp_new_x, bmp_new_y, bmp_new_w, bmp_new_h, TRUE);
#endif
}

/* draw color */
static void draw_color (HDC hdc)
{
    SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF, 0xFF, 0xFF));

    FillBox(hdc, S_COLOR_X, S_COLOR_Y, FIXED_W, FIXED_H);

#if 0
    /* red to blue */
    SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF - g_color, 0x00, g_color));

    FillBox(hdc,
            S_COLOR_X + (FIXED_W - S_COLOR_LEN) / 2, S_COLOR_Y + (FIXED_H - S_COLOR_LEN) / 2,
            S_COLOR_LEN, S_COLOR_LEN);
#else
    /* red to blue */
    SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF - g_color, 0x00, g_color));

    FillBox(hdc,
            S_COLOR_X + (FIXED_W - S_COLOR_LEN) / 2, S_COLOR_Y + (FIXED_H - S_COLOR_LEN) / 2,
            S_COLOR_LEN / 3, S_COLOR_LEN);

    /* blue to green */
    SetBrushColor (hdc, RGB2Pixel (hdc, 0x00, g_color, 0xFF - g_color));

    FillBox(hdc,
            S_COLOR_X + (FIXED_W - S_COLOR_LEN) / 2 + S_COLOR_LEN / 3, S_COLOR_Y + (FIXED_H - S_COLOR_LEN) / 2,
            S_COLOR_LEN / 3, S_COLOR_LEN);

    /* green to red */
    SetBrushColor (hdc, RGB2Pixel (hdc, g_color, 0xFF - g_color, 0x00));

    FillBox(hdc,
            S_COLOR_X + (FIXED_W - S_COLOR_LEN) / 2 + 2 * S_COLOR_LEN / 3, S_COLOR_Y + (FIXED_H - S_COLOR_LEN) / 2,
            S_COLOR_LEN / 3, S_COLOR_LEN);
#endif
}

static void listbox_notif_proc (HWND hWnd, LINT id, int nc, DWORD add_data)
{
    int curve;

    if (nc == LBN_CLICKED ) {
        curve = SendMessage (hWnd, LB_GETCURSEL, 0, 0);
        if (g_animation != NULL) {
            mGEffAnimationStop (g_animation);
            mGEffAnimationWait ((void *)&g_hMainWnd, g_animation);
            g_animation = NULL;
            g_curframe = 0;
        }

        g_animation = do_animation (g_hMainWnd, curve);
    }
}

/* callback */
static void finished_callback (MGEFF_ANIMATION handle)
{
    /* update */
    InvalidateRect (g_hMainWnd, NULL, TRUE);
}

/* callback */
static void curloopchanged_callback (MGEFF_ANIMATION handle)
{
    /* update */
    InvalidateRect (g_hMainWnd, NULL, TRUE);
}

/* callback */
static void property_callback (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    int val;
    int curframe;
    RECT rc;

    val = *(int *)value;

    curframe = mGEffAnimationGetProperty (handle, MGEFF_PROP_CURFRAME);

    g_curframe = curframe;

    switch (id) {
    case 0:
        /* for curve */
        if (curframe == 0) {
            curve_new_x = S_CURVE_X + (FIXED_W - S_CURVE_LEN) / 2 + curframe * S_CURVE_LEN / FRAME;
            curve_new_y = S_CURVE_Y + (FIXED_H - S_CURVE_LEN) / 2 + val;

            curve_old_x = curve_new_x;
            curve_old_y = curve_new_y;
        }
        else {
            curve_old_x = curve_new_x;
            curve_old_y = curve_new_y;

            curve_new_x = S_CURVE_X + (FIXED_W - S_CURVE_LEN) / 2 + curframe * S_CURVE_LEN / FRAME;
            curve_new_y = S_CURVE_Y + (FIXED_H - S_CURVE_LEN) / 2 + val;
        }

        SetRect((RECT *)&rc, S_CURVE_X, S_CURVE_Y, FIXED_W, FIXED_H);

        break;

    case 1:
        /* for icon */
        icon_new_x = S_ICON_X + (FIXED_W - S_ICON_LEN) / 2 + val;
        icon_new_y = S_ICON_Y + (FIXED_H - S_ICON_LEN) / 2 + val;

        SetRect((RECT *)&rc, S_ICON_X, S_ICON_Y, FIXED_W, FIXED_H);

        break;

    case 2:
        /* for button */
        bmp_new_w = val;
        bmp_new_h = val;

        bmp_new_x = S_BTN_X + (FIXED_W - bmp_new_w) / 2;
        bmp_new_y = S_BTN_Y + (FIXED_H - bmp_new_h) / 2;

        SetRect((RECT *)&rc, S_BTN_X, S_BTN_Y, FIXED_W, FIXED_H);

        break;

    case 3:
        /* for color */
        g_color = (val < 0) ? 0 : val;
        g_color = (val > 255) ? 255 : val;

        SetRect((RECT *)&rc, S_COLOR_X, S_COLOR_Y, FIXED_W, FIXED_H);

        break;

    default:
        printf ("error");
        break;
    }

    /* update window MSG_PAINT */
    UpdateWindow (hWnd, TRUE);
    //InvalidateRect (hWnd, NULL, TRUE);
}

/* do an animation with curve */
MGEFF_ANIMATION do_animation (HWND hWnd, int curve)
{
    MGEFF_ANIMATION animation[ANIMATION_NUM];
    MGEFF_ANIMATION group_animation;
    int id = 0;

    /* create a group animation */
    group_animation = mGEffAnimationCreateGroup (MGEFF_PARALLEL);

    /* set property */
    mGEffAnimationSetDuration (group_animation, DURATION);

    mGEffAnimationSetProperty (group_animation, MGEFF_PROP_KEEPALIVE, 0);
    mGEffAnimationSetProperty (group_animation, MGEFF_PROP_LOOPCOUNT, MGEFF_INFINITE);

    /* set callback function */
    mGEffAnimationSetFinishedCb (group_animation, finished_callback);
    mGEffAnimationSetCurLoopChangedCb (group_animation, curloopchanged_callback);

    /* create a curve animation */
    id = 0;

    animation[id] = mGEffAnimationCreate ((void *)hWnd, (void *) property_callback, id, MGEFF_INT);

    /* set property */
    mGEffAnimationSetDuration (animation[id], animation_property[id].duration);

    mGEffAnimationSetStartValue (animation[id], &(animation_property[id].start_val));
    mGEffAnimationSetEndValue (animation[id], &(animation_property[id].end_val));

    /* set curve */
    mGEffAnimationSetCurve (animation[id], curve);

    /* add it to group */
    mGEffAnimationAddToGroup (group_animation, animation[id]);

    /* create an icon animation */
    id = 1;

    animation[id] = mGEffAnimationCreate ((void *)hWnd, (void *) property_callback, id, MGEFF_INT);

    /* set property */
    mGEffAnimationSetDuration (animation[id], animation_property[id].duration);

    mGEffAnimationSetStartValue (animation[id], &(animation_property[id].start_val));
    mGEffAnimationSetEndValue (animation[id], &(animation_property[id].end_val));

    /* set curve */
    mGEffAnimationSetCurve (animation[id], curve);

    /* add it to group */
    mGEffAnimationAddToGroup (group_animation, animation[id]);

    /* create an button animation */
    id = 2;

    animation[id] = mGEffAnimationCreate ((void *)hWnd, (void *) property_callback, id, MGEFF_INT);

    /* set property */
    mGEffAnimationSetDuration (animation[id], animation_property[id].duration);

    mGEffAnimationSetStartValue (animation[id], &(animation_property[id].start_val));
    mGEffAnimationSetEndValue (animation[id], &(animation_property[id].end_val));

    /* set curve */
    mGEffAnimationSetCurve (animation[id], curve);

    /* add it to group */
    mGEffAnimationAddToGroup (group_animation, animation[id]);

    /* create a color animation */
    id = 3;

    animation[id] = mGEffAnimationCreate ((void *)hWnd, (void *) property_callback, id, MGEFF_INT);

    /* set property */
    mGEffAnimationSetDuration (animation[id], animation_property[id].duration);

    mGEffAnimationSetStartValue (animation[id], &(animation_property[id].start_val));
    mGEffAnimationSetEndValue (animation[id], &(animation_property[id].end_val));

    /* set curve */
    mGEffAnimationSetCurve (animation[id], curve);

    /* add it to group */
    mGEffAnimationAddToGroup (group_animation, animation[id]);


    /* running */
    mGEffAnimationAsyncRun (group_animation);

    return group_animation;
}

static LRESULT DlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITDIALOG:
        mGEffInit();

        g_hMainWnd = hDlg;

        LoadResource (BMP_PATH, RES_TYPE_IMAGE, (DWORD)HDC_SCREEN);

        InitListbox (hDlg, IDC_LISTBOX);

        g_animation = do_animation (g_hMainWnd, Linear);
        break;

    case MSG_PAINT:
        draw_frame (hDlg);
        break;

    case MSG_ERASEBKGND:
        return 0;
        break;
    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            if (g_animation != NULL) {
                mGEffAnimationStop (g_animation);
                mGEffAnimationWait ((void *)&g_hMainWnd, g_animation);
                g_animation = NULL;
                g_curframe = 0;
            }

            EndDialog (hDlg, wParam);

            mGEffDeinit();
            break;
        }

        break;

    case MSG_CLOSE:
        if (g_animation != NULL) {
            mGEffAnimationStop (g_animation);
            mGEffAnimationWait ((void *)&g_hMainWnd, g_animation);
            g_animation = NULL;
            g_curframe = 0;
        }

        EndDialog (hDlg, wParam);

        mGEffDeinit();
        break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char *argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "mGEff" , 0 , 0);
#endif

    DialogBoxIndirectParam (&DlgTmpl, HWND_DESKTOP, DlgProc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
