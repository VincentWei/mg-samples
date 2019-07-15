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
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/fixedmath.h>
#include "p-code.h"
#include "animate.h"


typedef struct RAD_DATA {
    BITMAP* bmp;
    int x;
    int y;
    int w;
    int h;
} RAD_DATA;

typedef struct _SCALE_DATA {
    BITMAP* bmp;
    int w0;
    int h0;
    int w1;
    int h1;
} SCALE_DATA;

static void draw_animate (HDC hdc, ANIMATE* ani)
{
    int x = GetAnimateX (ani);
    int y = GetAnimateY (ani);
    int w = GetAnimateW (ani);
    int h = GetAnimateH (ani);

    if (w == 0 || h == 0)
        return;

	FillBoxWithBitmap (hdc, x-(w>>1), y-(h>>1), w, h, 
            (BITMAP*)ani->img);
}

#if 0
static unsigned short long_sqrt(unsigned long a){
  unsigned long rem = 0;
  unsigned long root = 0;
  unsigned long divisor = 0;
  int i;
  for(i=0; i<16; i++){
    root <<= 1;
    rem = ((rem << 2) + (a >> 30));
    a <<= 2;
    divisor = (root<<1) + 1;
    if(divisor <= rem){
      rem -= divisor;
      root++;
    }
  }
  return (unsigned short)(root);
}
#endif

/*(x2, y2)--(x0, y0) is times of (x1, y1)--(x0, y0)
 * times is 16.16 fixed decimal*/
static void inline get_rad_target (int x0, int y0, int x1, int y1, 
        int* x2, int* y2, int times)
{
#if 0
    int diff_x10 = x1 - x0;
    int diff_y10 = y1 - y0;

    unsigned short len_10 = long_sqrt (diff_x10*diff_x10 + diff_y10*diff_y10);

    if (len_10 == 0)
    {
        *x2 = x1 + times;
        *y2 = y1;
    }
    else
    {
        *x2 = x1 + times * diff_x10 / len_10;
        *y2 = y1 + times * diff_y10 / len_10;
    }
#else
    *x2 = x0 + ((x1 - x0) * times >> 16);
    *y2 = y0 + ((y1 - y0) * times >> 16);
#endif
}

static void draw_bkgnd (HDC hdc, const RECT* rc, void* param)
{
    SetBrushType (hdc, BT_SOLID);
    SetBrushColor (hdc, COLOR_black);
    FillBox (hdc, rc->left, rc->top, RECTWP(rc), RECTHP(rc));
}

static void on_finished (ANIMATE_SENCE* as)
{
    ANIMATE* a;
#define REMOVE_ALL_ANIMATE(first) \
    while (first) \
    { \
        a = first; \
        RemoveAnimate (as, first); \
        free (a); \
    } 

    REMOVE_ALL_ANIMATE (as->normal);
    REMOVE_ALL_ANIMATE (as->topmost);
}

#define GET_FRAME_NUM(as) ((as)->total_time / (as)->interval - 3)

void AddRadAnimate (ANIMATE_SENCE* as, BITMAP* bmp, int x, int y, int w, int h,
        int cent_x, int cent_y, int len_times, BOOL is_convs)
{
    int target_x;
    int target_y;
    int frame_num = GET_FRAME_NUM(as);

    ANIMATE* a = (ANIMATE*) calloc (1, sizeof(ANIMATE));;
    TIME_LINE* tl = CreateTimeLine((int)a);;
    
    a->img = bmp;

    get_rad_target (cent_x, cent_y, x, y, &target_x, &target_y, len_times);

    SetAnimateW (a, w);
    SetAnimateH (a, h);

    if (!is_convs)
    {
        SetAnimateX (a, x);
        SetAnimateY (a, y);
        TLMoveTo(tl, a, target_x, target_y, frame_num);
    }
    else
    {
        SetAnimateX (a, target_x);
        SetAnimateY (a, target_y);
        TLMoveTo(tl, a, x, y, frame_num);
    }

    TLRun (tl, frame_num);
    InsertAnimate(as, a, FALSE);	
    StartTimeLine(as, tl, NULL, 0, NULL);
}

void AddScaleAnimate (ANIMATE_SENCE* as, BITMAP* bmp, int x, int y, int w0, int h0,
        int w1, int h1, BOOL is_convs)
{
    int frame_num = GET_FRAME_NUM(as);
    ANIMATE* a = (ANIMATE*) calloc (1, sizeof(ANIMATE));;
    TIME_LINE* tl = CreateTimeLine((int)a);;
    
    a->img = bmp;

    SetAnimateX (a, x);
    SetAnimateY (a, y);

    if (!is_convs)
    {
        SetAnimateW (a, w0);
        SetAnimateH (a, h0);
        TLScaleTo(tl, a, w1, h1, frame_num);
    }
    else
    {
        SetAnimateW (a, w1);
        SetAnimateH (a, h1);
        TLScaleTo(tl, a, w0, h0, frame_num);
    }

    TLRun (tl, frame_num);
    InsertAnimate (as, a, FALSE);	
    StartTimeLine(as, tl, NULL, 0, NULL);

}

ANIMATE_SENCE* CreateRadAnimateSence (int interval, int all_times, HDC hdc, RECT* rc)
{
    return CreateAnimateSence (interval, all_times,
            hdc, draw_animate, draw_bkgnd, on_finished, rc, NULL);
}

static void run_zoom_animate (BOOL is_out)
{
#define WW  20
#define HH  20
#if 1
#define ROW (RECTW(g_rcScr)/WW/2)
#define COL (RECTW(g_rcScr)/HH/2)
#else
#define ROW 2
#define COL 2
#endif

    int row;
    int col;
    int cent_x = g_rcScr.right >> 1;
    int cent_y = g_rcScr.bottom >> 1;
    int i;
    BITMAP rad_bmp;
    BITMAP zoom_bmp;
    LoadBitmap (HDC_SCREEN, &rad_bmp, "fall_night.jpg");
    LoadBitmap (HDC_SCREEN, &zoom_bmp, "plain_tree.jpg");
    ANIMATE_SENCE* as;
    as = CreateRadAnimateSence (50, 1000, HDC_SCREEN, &g_rcScr);
    for (row=0; row<ROW; row++)
        for (col=0; col<COL; col++)
        {
            AddRadAnimate (as, &rad_bmp, 
                    g_rcScr.right/ROW * row + g_rcScr.right/ROW/2,
                    g_rcScr.bottom/COL * col + g_rcScr.bottom/COL/2 , 
                    WW, HH, cent_x, cent_y, COL<<16, is_out);
        }
    AddScaleAnimate (as, &zoom_bmp, cent_x, cent_y, 
            0, 0, g_rcScr.right, g_rcScr.bottom, is_out);

    StartAnimateSence (as);
    DeleteAnimateSence (as, NULL); 

}
static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
            {

                while (1)
                {
                    run_zoom_animate (FALSE);
                    run_zoom_animate (TRUE);
                }
            }
            return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "alpha animate" , 0 , 0);
#endif

#if 1
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "alpha animate";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = HelloWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 0;
    CreateInfo.by = 0;
    CreateInfo.iBkColor = COLOR_lightwhite;
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

    return 0;
#endif
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif


