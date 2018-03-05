#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/minigui.h>

#include <mgeff/mgeff.h>

/*****************************************************************************/
#define CAPTION    "animation_callback"
#define BAR_HEIGHT 50
#define DURATION   (20 * 1000)
#define LOOPCOUNT  3
#define START_VAL  0x00
#define END_VAL    0xFF

/*****************************************************************************/
static char g_str[64];
static int g_value = 0x00;
static int g_loop = 0;

/*****************************************************************************/
/* main window proc */
static LRESULT mainWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
/* draw a frame */
static void draw_frame (HWND hWnd);
/* callback function called when property change */
static void property_callback (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value);
/* create and run an animation */
static int do_animation (HWND hWnd);

/* callback function called when animation finished */
static void finished_callback (MGEFF_ANIMATION handle);
/* callback function called when animation loop count is changed */
static void curloopchanged_callback (MGEFF_ANIMATION handle);
/* callback function called when animation direction changed */
static void dirchanged_callback (MGEFF_ANIMATION handle);
/* callback function called when animation state changed */
static void statechanged_callback (MGEFF_ANIMATION handle, enum EffState newEffState, enum EffState oldEffState);

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
/* main window proc */
static LRESULT mainWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        /* init mGEff library */
        mGEffInit ();

        sprintf (g_str, "you can press any key.");

        do_animation (hWnd);
        break;

    case MSG_KEYDOWN:
        sprintf (g_str, "you press key, keycode(%d)", (int)wParam);

        printf ("%s\n", g_str);

        InvalidateRect (hWnd, NULL, TRUE);
        break;

    case MSG_PAINT:
        draw_frame (hWnd);
        break;

    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);

        /* deinit mGEff library */
        mGEffDeinit ();
        break;

    default:
        break;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

/* draw a frame */
static void draw_frame (HWND hWnd)
{
    HDC dc;
    RECT rc;

    int client_w, client_h;
    char str[64];
    int color;

    /* begin draw */
    dc = BeginPaint (hWnd);

    /* get client rect */
    GetClientRect (hWnd, &rc);

    client_w = RECTW (rc);
    client_h = (RECTH (rc) - BAR_HEIGHT) * g_value / (END_VAL - START_VAL);

    color = g_value;

    /* set brush and draw area */
    SetBrushColor (dc, RGB2Pixel (dc, color, 0x00, 0x00));

    FillBox (dc, 0, BAR_HEIGHT, client_w, client_h);

    /* draw the text */
    sprintf (str, "current color: RGB(%d,0,0)", color);

    TextOut (dc, 0, 0, str);

    /* print information */
    TextOut (dc, 0, BAR_HEIGHT / 3, g_str);

    /* print curloop */
    sprintf (str, "current loop: (%d)", g_loop);

    TextOut (dc, 0, 2 * BAR_HEIGHT / 3, str);

    EndPaint (hWnd, dc);
    /* end draw */
}

/* callback function called when property change */
static void property_callback (MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    /* set animation property */
    g_value = *value;

    /* get current loop */
    g_loop = mGEffAnimationGetProperty (handle, MGEFF_PROP_CURLOOP);

    /* update */
    InvalidateRect (hWnd, NULL, TRUE);
}

/* create and run an animation */
static int do_animation (HWND hWnd)
{
    MGEFF_ANIMATION animation;
    int animation_id = 1;

    int duration;
    int start_val;
    int end_val;

    /* set value */
    duration = DURATION;
    start_val = START_VAL;
    end_val = END_VAL;

    /* create animation object */
    animation = mGEffAnimationCreate ((void *) hWnd, (void *) property_callback, animation_id, MGEFF_INT);

    /* set animation property */
    /* set loopcount */
    mGEffAnimationSetProperty (animation, MGEFF_PROP_LOOPCOUNT, LOOPCOUNT);

    /* duration */
    mGEffAnimationSetDuration (animation, duration);

    /* start value */
    mGEffAnimationSetStartValue (animation, &start_val);

    /* end value */
    mGEffAnimationSetEndValue (animation, &end_val);

    /* set callback */
    mGEffAnimationSetFinishedCb (animation, finished_callback);
    mGEffAnimationSetCurLoopChangedCb (animation, curloopchanged_callback);
    mGEffAnimationSetDirChangedCb (animation, dirchanged_callback);
    mGEffAnimationSetStateChangedCb (animation, statechanged_callback);

    /* running */
    mGEffAnimationAsyncRun (animation);

    /* wait animation stop */
    mGEffAnimationWait ((void *) &hWnd, animation);

    /* delete the animation object */
    mGEffAnimationDelete (animation);

    return 0;
}

/* callback function called when animation finished */
static void finished_callback (MGEFF_ANIMATION handle)
{
    printf ("finished_callback is called.\n");
}

/* callback function called when animation loop count is changed */
static void curloopchanged_callback (MGEFF_ANIMATION handle)
{
    int cur_loop;

    printf ("curloopchanged_callback is called.\n");

    cur_loop = mGEffAnimationGetProperty (handle, MGEFF_PROP_CURLOOP);

    printf ("current loop : %d\n", cur_loop);
}

/* callback function called when animation direction changed */
static void dirchanged_callback (MGEFF_ANIMATION handle)
{
    int direction;

    printf ("dirchanged_callback is called.\n");

    direction = mGEffAnimationGetProperty (handle, MGEFF_PROP_DIRECTION);

    printf ("current direction : %d\n", direction);
}

/* callback function called when animation state changed */
static void statechanged_callback (MGEFF_ANIMATION handle, enum EffState newEffState, enum EffState oldEffState)
{
    int state;

    printf ("statechanged_callback is called.\n");

    state = mGEffAnimationGetProperty (handle, MGEFF_PROP_STATE);

    printf ("current state : %d\n", state);
    printf ("new     state : %d\n", newEffState);
    printf ("old     state : %d\n", oldEffState);
}
