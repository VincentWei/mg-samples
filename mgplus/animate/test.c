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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#define VAL_DEBUG
#include <my_debug.h>

#include "animate.h"

ANIMATE* anim;

BITMAP img0;
BITMAP img1;
BITMAP img2;

 
typedef BOOL (* TIMERPROC)(HWND, int, DWORD);

static void draw_animte (HDC hdc, void* img_data, int x, int y,
        int w, int h, int alpha, void* param)
{
    TEST_VAL(x, %d);
    TEST_VAL(y, %d);

    if (img_data)
        FillBoxWithBitmap (hdc, x, y, w, h, (BITMAP*)img_data);
}

void get_outted_time_spec (struct timespec* tsp, int interval_usec)
{
    struct timeval now;
    gettimeofday (&now, NULL);
    tsp->tv_sec = now.tv_sec;
    tsp->tv_nsec = (now.tv_usec + interval_usec) * 1000;

    tsp->tv_sec += tsp->tv_nsec / 1000000000L;
    tsp->tv_nsec %= 1000000000L;
}

static void mono_animate (ANIMATE* animate, int all_sec, int interval_usec)
{
    int frame_num = 32 ;
    int i;
    struct timespec ts;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    anim = CreateAnimate (draw_animte, NULL, 0);

    AppendAnimateFrameInstruction (anim, 0, FALSE, &img0);
    AppendAnimateFrameInstruction (anim, 0, FALSE, &img1);
    AppendAnimateFrameInstruction (anim, 0, FALSE, &img2);
    AppendAnimateGotoInstruction (anim, 0, 0);

    AppendAnimateMoveInstruction (anim, 1, FALSE, 0, 0, 0, g_rcScr.bottom-30, frame_num>>2);
    AppendAnimateMoveInstruction (anim, 1, FALSE, VALUE_KEEP_OLD, VALUE_KEEP_OLD, 
           g_rcScr.right-30, g_rcScr.bottom-30, frame_num>>2);
    AppendAnimateMoveInstruction (anim, 1, FALSE, g_rcScr.right-30, g_rcScr.bottom-30, 
           g_rcScr.right-30, 0, frame_num>>2);
    AppendAnimateMoveInstruction (anim, 1, FALSE, g_rcScr.right-30, 0, 0, 0, frame_num - (frame_num >> 2)*3);
    AppendAnimateGotoInstruction (anim, 1, 0);

    pthread_mutex_lock (&mutex);

    //for (i=0; i<frame_num-1; i++)
    while (1)
    {
        if (ExecuteAnimateInstruction (anim, 4) == FALSE)
            goto end;
        FUN_STEP (1);
        anim->draw_animte (HDC_SCREEN, anim->img, fixtoi(anim->x), fixtoi(anim->y), 
                fixtoi(anim->w), fixtoi(anim->h), fixtoi(anim->alpha), NULL);
        FUN_STEP (1);
        get_outted_time_spec (&ts, interval_usec);
        FUN_STEP (1);
        pthread_cond_timedwait (&cond, &mutex, &ts);
        FUN_STEP (2);

    }

end:
    pthread_mutex_unlock (&mutex);
}

static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message) {
        case MSG_CREATE:
           //SetTimerEx (hWnd, 100, 100);
           return 0;

        case MSG_TIMER:
            if (wParam == 100);
            InvalidateRect (hWnd, NULL, TRUE);
            break;
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);

            EndPaint (hWnd, hdc);
            return 0;

        case MSG_LBUTTONDOWN:
            InvalidateRect (hWnd, NULL, FALSE);
            break;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            return 0;

        case MSG_DESTROY:
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}


int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif
    LoadBitmap (HDC_SCREEN, &img0, "menu6-50-l.png");
    LoadBitmap (HDC_SCREEN, &img1, "menu6-50.png");
    LoadBitmap (HDC_SCREEN, &img2, "menu6-50-r.png");


mono_animate (anim, 10, 50);
/*
    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "animate";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = HelloWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_green;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    */

    return 0;
}
