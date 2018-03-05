/*
 *   This file is part of mGPlus, a component for MiniGUI.
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
#include <mgplus/mgplus.h>
#include <unistd.h>

#define CANVAS      0
#define NO_CANVAS   1
//#define TYPE        CANVAS
//#define TYPE        NO_CANVAS

static HGRAPHICS hgs = 0;
static HPEN pen = 0;
static HBRUSH brush = 0;
static HDC private_dc = 0;
static int canvas_mode = CANVAS;
static void SaveGraphics(HGRAPHICS hgs, HDC hdc, HPEN pen, BOOL type);

static BITMAP g_stTestBitmap;

static int test_count = 1000;

static int r(void)
{
    return rand()%(RECTW(g_rcScr)) + 10;
}

void test_func_draw_line(HGRAPHICS hgs, HPEN pen)
{
    int i = 0;
    HDC hdc = MGPlusGetGraphicDC (hgs);
    DWORD s_tick = GetTickCount();

    MGPlusSaveHG(hgs);
    //MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);
    //MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);

    for(i = 0; i < test_count; i++) {
        int x1 = r() + 10;
        int y1 = r() + 10;
        int x2 = 100;
        int y2 = 100;
        //printf("x1=%d,y1=%d,x2=%d,y2=%d\n", x1, y1, x2, y2);
        MGPlusDrawLine(hgs, pen, x1, y1, x2, y2);
    }
    printf("test_func_draw_line: mgplus used time %ld\n", GetTickCount() - s_tick);
    SaveGraphics(hgs, private_dc, 0, canvas_mode);
    sleep(1);
    s_tick = GetTickCount();
    SetPenWidth(hdc, 2);
    SetPenColor(hdc, RGB2Pixel(HDC_SCREEN, 0xa0, 20, 88));
    for(i = 0; i < test_count; i++) {
        int x1 = r() + 10;
        int y1 = r() + 10;
        int x2 = 100;
        int y2 = 100;
        //MoveTo(hdc, x1, y1);
        //LineTo(hdc, x2, y2);
        LineEx(hdc, x1, y1, x2, y2);
    }
    MGPlusRestoreHG(hgs, 1);
    printf("test_func_draw_line: minigui used time %ld\n", GetTickCount() - s_tick);
    SaveGraphics(hgs, hdc, 0, canvas_mode);
}

void test_func_fill_rect(HGRAPHICS hgs, HBRUSH hbrush)
{
    int i = 0;
    HDC hdc = MGPlusGetGraphicDC (hgs);
    DWORD s_tick = GetTickCount();

    MGPlusSaveHG(hgs);
    //MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);
    //MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);

    for(i = 0; i < test_count; i++) {
        int x1 = r() + 10;
        int y1 = r() + 10;
        int x2 = 100;
        int y2 = 100;
        MGPlusFillRectangle(hgs, hbrush, x1, y1, x2, y2);
    }
    printf("test_func_fill_rect: mgplus used time %ld\n", GetTickCount() - s_tick);
    SaveGraphics(hgs, private_dc, 0, canvas_mode);
    sleep(1);
    s_tick = GetTickCount();
    for(i = 0; i < test_count; i++) {
        int x1 = r() + 10;
        int y1 = r() + 10;
        int x2 = 100;
        int y2 = 100;
        DWORD color = RGB2Pixel(HDC_SCREEN, x1%255, y1%255, x2%255);
        SetBrushColor(hdc, color);
        FillBox(hdc, x1, y1, x2, y2);
    }
    MGPlusRestoreHG(hgs, 1);
    printf("test_func_fill_rect: minigui used time %ld\n", GetTickCount() - s_tick);
}

void test_func_draw_rect(HGRAPHICS hgs, HPEN hpen)
{
    int i = 0;
    HDC hdc = MGPlusGetGraphicDC (hgs);
    DWORD s_tick = GetTickCount();

    MGPlusSaveHG(hgs);
    //MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);
    //MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);

    for(i = 0; i < test_count; i++) {
        int x1 = r() + 10;
        int y1 = r() + 10;
        int x2 = 100;
        int y2 = 100;
        MGPlusDrawRectangle(hgs, hpen, x1, y1, x2, y2);
    }
    printf("test_func_draw_rect: mgplus used time %ld\n", GetTickCount() - s_tick);
    SaveGraphics(hgs, private_dc, 0, canvas_mode);
    sleep(1);
    s_tick = GetTickCount();
    for(i = 0; i < test_count; i++) {
        int x1 = r() + 10;
        int y1 = r() + 10;
        int x2 = 100;
        int y2 = 100;
        Rectangle(hdc, x1, y1, x2, y2);
    }
    MGPlusRestoreHG(hgs, 1);
    printf("test_func_draw_rect: minigui used time %ld\n", GetTickCount() - s_tick);
}

void test_func_fill_bitmap(HGRAPHICS hgs)
{
    int i = 0;
    HDC hdc = MGPlusGetGraphicDC (hgs);
    DWORD s_tick = GetTickCount();

    MGPlusSaveHG(hgs);
    //MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);
    //MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);

    MPPOINT points [4] = {{40,40},{440,-40},{340,840},{90,440}};
    MGPlusGraphicLoadBitmap (hgs, 0, &g_stTestBitmap);

    s_tick = GetTickCount();
    //SetBrushColor(hdc, COLOR.blue);
    LoadBitmap (hdc, &g_stTestBitmap, "./res/gundam.bmp");
    for(i = 0; i < test_count; i++) {
        MGPlusDrawImageWithPoints (hgs, 0, points, 4);
    }
    printf("test_func_fill_bitmap: minigui used time %ld\n", GetTickCount() - s_tick);
    SaveGraphics(hgs, private_dc, 0, canvas_mode);
    sleep(1);

    s_tick = GetTickCount();
    for(i = 0; i < test_count; i++) {
        FillBoxWithBitmap(hdc, 100, 100, -1, -1, &g_stTestBitmap);
    }
    printf("test_func_fill_bitmap: mgplus used time %ld\n", GetTickCount() - s_tick);
    MGPlusRestoreHG(hgs, 1);
}



void draw_path(HGRAPHICS hgs, HPEN pen)
{
    unsigned char dash [] = {13, 9, 8, 3};
    HPEN pen_dash = MGPlusPenCreate (3, 0xFF505050);

    MGPlusSaveHG(hgs);
    //MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);

    MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);
    MGPlusDrawEllipse (hgs, pen, 0, 0, 200, 100);
    MGPlusPenSetJoinStyle (pen, JOIN_ROUND);
    MGPlusPenSetCapStyle (pen, CAP_ROUND);

    MGPlusDrawLine(hgs, pen, 210, 10, 310, 10);
    MGPlusDrawLine(hgs, pen, 310, 10, 310, 50);
    MGPlusDrawLine(hgs, pen, 310, 50, 210, 50);
    MGPlusDrawLine(hgs, pen, 210, 50, 210, 10);

    MGPlusPenSetDashes (pen_dash, 1, dash, TABLESIZE(dash));
    MGPlusDrawRectangle(hgs, pen_dash, 320, 10, 100, 60);
    MGPlusPenDelete (pen_dash);

    MGPlusDrawArc(hgs, pen, 10, 320, 100, 50, 0, 300);
    
    MGPlusDrawEllipseI(hgs, pen, 0, 0, 200, 100);
    MGPlusDrawBezier(hgs, pen , 120, 350, 145, 312, 200, 380, 220, 350);
}

void draw_on_mouse(HGRAPHICS hgs, HPEN pen, int x1, int y1, int x2, int y2)
{
    int distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    //MGPlusSaveHG(hgs);
    //MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);

    MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);
    //MGPlusPenSetJoinStyle (pen, JOIN_ROUND);
    //MGPlusPenSetCapStyle (pen, CAP_ROUND);

    //MGPlusDrawArc(hgs, pen, 10, 320, 100, 50, 0, 300);
    //MGPlusDrawRectangle(hgs, hpen, 10, 10, x2, y2);

    MGPlusDrawEllipse (hgs, pen, x1, y1, distance, distance);
}


static void CleanClient(HWND hWnd)
{
    HDC hdc = GetClientDC(hWnd);
    FillBox (hdc, 0, 0, g_rcScr.right, g_rcScr.bottom); 
    ReleaseDC(hdc);
}


static HGRAPHICS GetGraphics(HDC hdc, BOOL type)
{
    HGRAPHICS hgs = 0;
    if (type == CANVAS)
        hgs = MGPlusGraphicCreateFromDC(hdc);
    else
        hgs = MGPlusGraphicCreateWithoutCanvas(hdc);

    if (!hgs) {
        printf("create hgs failed!\n");
        return 0;
    }
    return hgs;
}

static void SaveGraphics(HGRAPHICS hgs, HDC hdc, HPEN pen, BOOL type)
{
    if (type == CANVAS) {
#if 1
        if (MGPlusGraphicSave (hgs, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
            printf ("save hgs failed!");
#else
        if (hgs) {
            HDC mem_dc = MGPlusGetGraphicDC (hgs);
            BitBlt(mem_dc, 0, 0, 0, 0, hdc, 0, 0, 0);
        }
#endif
    }
    else {
        MGPlusGraphicClear(hgs, 0xffffffff);
        draw_path(hgs, pen);
    }
}

#define ID_NOCANVAS               101
#define ID_CANVAS                 102

#include "stdio.h"
#include "string.h"
static HMENU Menu_Mode (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = (DWORD)"Mode";

    hmnu = CreatePopupMenu (&mii);
    //InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_NOCANVAS;
    mii.typedata    = (DWORD)"WithOutCanvas";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_CANVAS;
    mii.typedata    = (DWORD)"WithCanvas";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_CompositeTest (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"Canvas Mode";
    mii.hsubmenu    = Menu_Mode ();
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}

static void SetCaption(HWND hWnd)
{
    char buf[512];
    const char* caption = "Please use F1,F2,F3,F4,F5,F6,F7,F8,F9,F10 =======";
    strcpy (buf, caption);
    if (canvas_mode == CANVAS)
        strcat(buf, " CANVAS: =======");
    else
        strcat(buf, " WITHOUT_CANVAS: =========");
    SetWindowCaption(hWnd, buf);
}

static HGRAPHICS ResetGraphics(HWND hWnd, HDC* hdc)
{
    HGRAPHICS hgs = 0;
    DeletePrivateDC (*hdc);
    MGPlusGraphicDelete (hgs);
    *hdc = CreatePrivateClientDC (hWnd);
    hgs  = GetGraphics(*hdc, canvas_mode);
    SetCaption(hWnd);
    return hgs;
}

static LRESULT mGPlusWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x_s = 0, y_s = 0;
    switch (message) {
        case MSG_CREATE:
            {
                UpdateWindow(hWnd, TRUE);
                private_dc = CreatePrivateClientDC (hWnd);
                hgs = GetGraphics(private_dc, canvas_mode);
                pen = MGPlusPenCreate (2, 0xFF505050);
                if (!pen) {
                    printf("create pen fialed!\n");
                    break;
                }
                brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
                if (!brush) {
                    printf("create brush fialed!\n");
                    break;
                }
                MGPlusSetSolidBrushColor (brush, 0xFF505050);
#if 0
                /* set graphics clip.*/
                HPATH clip_path = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                //HPATH clip_path = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);
                //MGPlusPathAddBezier (path, 120, 350, 145, 312, 200, 380, 220, 350);
                //MGPlusPathAddRectangle (path, 180, 100, 150, 50);
                //MGPlusPathAddEllipse (clip_path, 0, 0, 400,500, TRUE);

                MGPlusPathAddRectangle (clip_path, 10, 10, 420, 420);
                //MGPlusPathAddRectangle (clip_path, 80, 90, 150, 150);

                //MGPlusPathAddArc(clip_path, 100, 120, 100, 100, 0, 360);
                //MGPlusPathAddArc(clip_path, 150, 120, 100, 100, 0, 360);
                MGPlusSetClipPath(hgs, clip_path);
                //MGPlusFillPath(hgs, brush, clip_path); 
#endif
                //draw_path(hgs, pen);
                LoadBitmap (HDC_SCREEN, &g_stTestBitmap, "./res/gundam.bmp");
                SetCaption(hWnd);
                break;
            }
        case MSG_COMMAND:
            switch (wParam)
            {
                case ID_NOCANVAS:
                    canvas_mode = NO_CANVAS;
                    hgs = ResetGraphics(hWnd, &private_dc);
                    draw_path(hgs, pen);
                    break;
                case ID_CANVAS:
                    canvas_mode = CANVAS;
                    hgs = ResetGraphics(hWnd, &private_dc);
                    draw_path(hgs, pen);
                    break;
            }
            break;
        case MSG_RBUTTONDOWN:
            CleanClient(hWnd);
            break;
        case MSG_LBUTTONDOWN:
            x_s = LOWORD(lParam); 
            y_s = HIWORD(lParam); 
            return 0;
        case MSG_MOUSEMOVE:
            {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                MGPlusGraphicClear (hgs, 0xffffffff);
                printf("x1=%d,y1=%d, x2=%d,y2=%d\n", x_s, y_s, x, y);
                draw_on_mouse(hgs, pen, x_s, y_s, x, y);
                SaveGraphics(hgs, private_dc, 0, canvas_mode);
                break;
            }
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint (hWnd);
                SaveGraphics(hgs, hdc, pen, canvas_mode);
                EndPaint(hWnd, hdc);
                return 0;
            }
        case MSG_KEYUP:
            {
                int scancode = (int)wParam;
                if ((scancode >= SCANCODE_F1 && scancode <= SCANCODE_F10)
                        || scancode == SCANCODE_F11) {
                    MGPlusGraphicClear (hgs, 0xffffffff);
                    MGPlusResetWorldTransform (hgs);
                }
                printf("*****************************\n");
                MGPlusGraphicClear(hgs, 0xffffffff);
                switch(scancode) {
                    case SCANCODE_F1:
                        //MGPlusSetCompositingMode (hgs, 
                        //        (MPCompositingMode)MP_COMP_OP_SRC_OVER);
                        test_func_draw_line(hgs, pen);
                        break;
                    case SCANCODE_F2:
                        test_func_draw_rect(hgs, pen);
                        break;
                    case SCANCODE_F3:
                        test_func_fill_rect(hgs, brush);
                        break;
                    case SCANCODE_F4:
                        test_func_fill_bitmap(hgs);
                        break;
                    case SCANCODE_F5:
                        break;
                }
                printf("*****************************\n");
                if ((scancode >= SCANCODE_F1 && scancode <= SCANCODE_F10)
                        || scancode == SCANCODE_F11) {
                    if (canvas_mode == CANVAS)
                        InvalidateRect (hWnd, NULL, TRUE);
                }
                break;
            }
        case MSG_CLOSE:
            MGPlusPenDelete (pen);
            MGPlusBrushDelete (brush);
            MGPlusGraphicDelete (hgs);
            DeletePrivateDC (private_dc);
            UnloadBitmap(&g_stTestBitmap);
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
        default:
            break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

#ifdef _MISC_MOUSECALIBRATE
static void
mouse_calibrate(void)
{
    POINT   src_pts[5] =
        { {5, 10}, {600, 20}, {620, 450}, {20, 470}, {310, 234} };
    POINT   dst_pts[5] = { {0, 0}, {639, 0}, {639, 479}, {0, 479}, {320, 240} };

    SetMouseCalibrationParameters(src_pts, dst_pts);
}
#endif /* !_MISC_MOUSECALIBRATE */

int
MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "mgplustest", 0, 0);
#endif

#ifdef _MISC_MOUSECALIBRATE
    mouse_calibrate();
#endif
    srand(500);

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_USEPARENTFONT;
    CreateInfo.spCaption = "Please use F1,F2,F3,F4,F5,F6,F7,F8,F9,F10";
    //CreateInfo.hMenu = 0;
    CreateInfo.hMenu = Menu_CompositeTest();
    CreateInfo.hCursor = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = mGPlusWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right-50;
    CreateInfo.by = g_rcScr.bottom-50;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif
