#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgplus/mgplus.h>
#include <mgutils/mgutils.h>

#define ID_MP_COMP_OP_CLEAR               101
#define ID_MP_COMP_OP_SRC                 102
#define ID_MP_COMP_OP_DST                 103
#define ID_MP_COMP_OP_SRC_OVER            104
#define ID_MP_COMP_OP_DST_OVER            105
#define ID_MP_COMP_OP_SRC_IN              106
#define ID_MP_COMP_OP_DST_IN              107
#define ID_MP_COMP_OP_SRC_OUT             108
#define ID_MP_COMP_OP_DST_OUT             109
#define ID_MP_COMP_OP_SRC_ATOP            111
#define ID_MP_COMP_OP_DST_ATOP            112
#define ID_MP_COMP_OP_XOR                 113

LRESULT worldTransformProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static HGRAPHICS graphics;
    static HPATH oddpath, zeropath;
    static HBRUSH brush ;
    static HPEN pen;
    static ARGB p = 0xFF00FFFF;
    int i = 0, count = 0;
    MPPOINT* pt;

    switch (message) 
    {
        case MSG_CREATE:
            {
                HDC hdc = GetClientDC(hWnd);

                pen = MGPlusPenCreate(9, p);
                graphics = MGPlusGraphicCreate( 1024, 768);

                brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
                MGPlusSetSolidBrushColor (brush, 0xFFFF0000);

                oddpath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                zeropath= MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);

                ReleaseDC (hdc);
            }
            break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);

            MGPlusGraphicCopyFromDC (graphics, hdc, 0, 0, 0, 0, 0, 0);
            MGPlusDrawLine(graphics, pen, 500, 0,500 ,500 );
            TextOut (hdc, 150, 500, "Even-Odd rule");
            TextOut (hdc, 680, 500, "None-Zero rule");

            MGPlusPathAddRectangleI(oddpath, 50, 50, 200, 100);
            //MGPlusPathAddRoundRect (oddpath, 125, 60, 200, 100, 25);
            //MGPlusPathAddEllipse( oddpath, 225, 60, 200, 100, TRUE);

            MGPlusPathGetPoints (oddpath, &count, &pt);
            for (i = 0; i < count; i++) {
                printf("rotate p:x=%f,y=%f\n", pt[i].x, pt[i].y);
            }

            //MPMatrix matrix = {10, 10, 10, 10, 10, 10};
            //MGPlusSetWorldTransform(graphics,  &matrix);

            MGPlusTranslateWorldTransform (graphics, -150, -100);
            MGPlusScaleWorldTransform(graphics, 1, 2);
            MGPlusRotateWorldTransform (graphics, 45);
            MGPlusTranslateWorldTransform (graphics, 150, 100);
            MGPlusPathTransform(oddpath);
            MGPlusFillPath (graphics, brush, oddpath);
            MGPlusGraphicSave (graphics, hdc, 0, 0, 0, 0, 0, 0);


            //MGPlusTransformPath (graphics, oddpath);
            //MGPlusResetWorldTransform(graphics);
            free(pt);

            //MGPlusPathAddRectangleI(zeropath, 600, 600, 200, 100);
            //MGPlusPathAddRoundRect (zeropath, 725, 60, 200, 100, 25);
            MGPlusPathAddEllipse( zeropath, 825, 60, 200, 100, TRUE);

            MPPOINT  center = {825, 110};

            MGPlusPathResetTransform(zeropath);
            MGPlusPathRotateAroundPoint(zeropath, &center, -45);

            MGPlusFillPath(graphics, brush, zeropath); 
            MGPlusGraphicSave(graphics, hdc, 0, 0, 0, 0, 0, 0);

            EndPaint(hWnd, hdc);
            break;
        case MSG_CLOSE:
            MGPlusPenDelete(pen);
            MGPlusGraphicDelete (graphics);
            MGPlusPathDelete(oddpath);
            MGPlusPathDelete(zeropath);
            MGPlusBrushDelete (brush);

            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG Msg;
    HWND hwnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "ColorCompoiste test";
    CreateInfo.hMenu = 0; 
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = worldTransformProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hwnd = CreateMainWindow (&CreateInfo);

    if (hwnd == HWND_INVALID)
        return -1;

    ShowWindow(hwnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hwnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hwnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

