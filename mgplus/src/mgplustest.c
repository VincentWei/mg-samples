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

static inline void radio_gradient_draw(HGRAPHICS hgs, int cx, int cy, int r,
        gal_pixel *pixels, int pixel_num)
{
    HPATH path;
    HBRUSH brush;

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_PATHGRADIENT);
    if (!brush){
        return;
    }
    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (!path) {
        MGPlusBrushDelete(brush);
        return;
    }

    MPPOINT pt = {cx, cy};
    RECT rc={cx-r, cy-r, cx+r, cy+r};
    MGPlusSetPathGradientBrushSurroundRect (brush, &rc);
    MGPlusSetPathGradientBrushCenterPoint(brush, &pt);
    MGPlusSetPathGradientBrushCenterColor (brush, pixels[0]);
    MGPlusSetPathGradientBrushSurroundColors(brush, (ARGB *)&pixels[1], 1);

    MGPlusPathAddEllipseI(path, cx, cy, r, r, TRUE);

    MGPlusFillPath(hgs, brush, path);

    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
}

static inline void linear_gradient_draw_round(HGRAPHICS hgs, MPLinearGradientMode mode,
        RECT *rc, gal_pixel *pixel, int pixel_num, BOOL up)
{
    HPATH path;
    HBRUSH brush;

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (!brush){
        MGPlusGraphicDelete(hgs);
        return;
    }
    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (!path) {
        MGPlusBrushDelete(brush);
        return;
    }

    MGPlusSetLinearGradientBrushMode(brush, mode);
    MGPlusSetLinearGradientBrushRect(brush, rc);
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)pixel, pixel_num);

    if(up) {
        MGPlusPathAddLineI(path, rc->left+2, rc->top, rc->right-2, rc->top);
        MGPlusPathAddLineI(path, rc->right-2, rc->top, rc->right, rc->top+2);
        MGPlusPathAddLineI(path, rc->right, rc->top+2, rc->right, rc->bottom);
        MGPlusPathAddLineI(path, rc->right, rc->bottom, rc->left, rc->bottom);
        MGPlusPathAddLineI(path, rc->left, rc->bottom, rc->left, rc->top+2);
        MGPlusPathAddLineI(path, rc->left, rc->top+2, rc->left+2, rc->top);
    }
    else {
        MGPlusPathAddLineI(path, rc->left, rc->top, rc->right, rc->top);
        MGPlusPathAddLineI(path, rc->right, rc->top, rc->right, rc->bottom-2);
        MGPlusPathAddLineI(path, rc->right, rc->bottom-2, rc->right-2, rc->bottom);
        MGPlusPathAddLineI(path, rc->right-2, rc->bottom, rc->left+2, rc->bottom);
        MGPlusPathAddLineI(path, rc->left+2, rc->bottom, rc->left, rc->bottom-2);
        MGPlusPathAddLineI(path, rc->left, rc->bottom-2, rc->left, rc->top);
    }

    MGPlusFillPath(hgs, brush, path);

    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
}

static inline void linear_gradient_draw(HGRAPHICS hgs, MPLinearGradientMode mode,
        RECT *rc, gal_pixel *pixel, int pixel_num)
{
    HPATH path;
    HBRUSH brush;

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (!brush){
        MGPlusGraphicDelete(hgs);
        return;
    }
    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (!path) {
        MGPlusBrushDelete(brush);
        return;
    }

    MGPlusSetLinearGradientBrushMode(brush, mode);
    MGPlusSetLinearGradientBrushRect(brush, rc);
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)pixel, pixel_num);

    MGPlusPathAddRectangleI(path, rc->left, rc->top, RECTWP(rc), RECTHP(rc));
    MGPlusFillPath(hgs, brush, path);

    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
}

static BITMAP g_stTestBitmap;


void draw_path(HGRAPHICS hgs, HPEN pen)
{
    unsigned char dash [] = {13, 9, 8, 3};
    HPEN pen_dash = MGPlusPenCreate (1, 0xFF505050);

    MGPlusSetSmoothingMode (hgs, MP_SMOOTHING_QUALITY);
    //MGPlusSetPathRenderingHint (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);
    MGPlusDrawEllipse (hgs, pen, 0, 0, 200, 100);
#if 1
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
#endif
}

static void test_add_path(HGRAPHICS hgs, HPEN pen, HBRUSH brush)
{
    MPPOINT points [5]= {{450, 150},{450,250},{400, 350},{450,50},{350,150}};

    //ARGB argb = 0x7fff00ff;
    //ARGB argb = 0x99ff0000;
    //ARGB argb = 0xffff0000;
    ARGB argb = 0xff0000ff;
    //.awt.Color[r=255,g=0,b=0]alpha153

    HPATH path = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    HPATH path_second = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);

    //MGPlusSetSolidBrushColor (brush, 0xFF505050);
    MGPlusSetSolidBrushColor (brush, argb);
    MGPlusSetSmoothingMode (hgs, MP_PATH_RENDER_HINT_ANTIALIAS_OFF);
    MGPlusPathAddLine (path, 100, 10, 200, 50);

    MGPlusPathAddArc (path, 100, 100, 50, 100, 0, 160);

    MGPlusPathAddBezier (path, 120, 350, 145, 312, 200, 380, 220, 350);
    MGPlusPathAddRectangle (path, 180, 100, 150, 50);
    MGPlusPathAddEllipse (path, 180, 200, 100, 50, TRUE);
    MGPlusPathAddCurve (path, points, 5);
    MGPlusPathAddPath (path_second, path);
    MGPlusFillPath (hgs, brush, path_second);

    MGPlusPathDelete (path);
    MGPlusPathDelete (path_second);
}

static void CleanClient(HWND hWnd)
{
    HDC hdc = GetClientDC(hWnd);
    FillBox (hdc, 0, 0, g_rcScr.right, g_rcScr.bottom);
    ReleaseDC(hdc);
}

#define CANVAS      0
#define NO_CANVAS   1
//#define TYPE        CANVAS
//#define TYPE        NO_CANVAS
static int canvas_mode = CANVAS;

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
#if 0
        if (MGPlusGraphicSave (hgs, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
            printf ("save hgs failed!");
#else
        if (hgs) {
            HDC mem_dc = MGPlusGetGraphicDC (hgs);
            //SetMemDCAlpha (mem_dc, MEMDC_FLAG_SRCALPHA, 30);
            //            //SetMemDCColorKey (mem_dc, MEMDC_FLAG_SRCCOLORKEY, 0x3343121);
            BitBlt(mem_dc, 0, 0, 0, 0, hdc, 0, 0, 0);
        }

#endif
    }
    else
    {
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
    static HGRAPHICS hgs = 0;
    static HPEN pen = 0;
    static HBRUSH brush = 0;
    static HDC private_dc = 0;

    switch (message) {
        case MSG_CREATE:
            {
                CleanClient(hWnd);
                private_dc = CreatePrivateClientDC (hWnd);
                hgs = GetGraphics(private_dc, canvas_mode);
                pen = MGPlusPenCreate (1, 0xFF505050);
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
                draw_path(hgs, pen);
                LoadBitmap (MGPlusGetGraphicDC(hgs), &g_stTestBitmap, "./res/gundam.bmp");
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
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint (hWnd);
                //.MGPlusGraphicClear (hgs, 0xffffffff);
                test_add_path(hgs, pen, brush);
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
                switch(scancode) {
                    case SCANCODE_F1:
                        draw_path(hgs, pen);
                        break;
                    case SCANCODE_F2:
                        {
                            int i = 0, count = 0;
                            MPPOINT* pt = NULL;
                            MPPOINT  center = {100.0, 50.0};
                            HPATH path = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);
                            MGPlusPathAddRectangleI (path, 0, 0, 200, 100);
                            MGPlusPathGetPoints (path, &count, &pt);
                            for (i = 0; i < count; i++) {
                                //printf("p:x=%f,y=%f\n", pt[i].x, pt[i].y);
                            }
                            free(pt);
                            {
                                double x = 0;
                                double y = 0;
                                int cmd = 0;
                                MGPlusPathGetVertex (path, 2, &x, &y, &cmd);
                                x = x;
                            }
                            /* rotate.*/
                            MGPlusPathRotateAroundPoint (path, &center, -10);
                            MGPlusPathTransform (path);
                            MGPlusPathGetPoints (path, &count, &pt);
                            for (i = 0; i < count; i++) {
                                //printf("rotate p:x=%f,y=%f\n", pt[i].x, pt[i].y);
                            }
                            MGPlusFillPath(hgs, brush, path);

                            {
                                HPEN pen = MGPlusPenCreate (1, 0xFF505050);
                                MGPlusPathReset (path);
                                MGPlusPathMovetoI (path, 330, 200);
                                MGPlusPathLinetoI (path, 350, 300);
                                MGPlusPathLinetoI (path, 250, 420);
                                MGPlusPathLinetoI (path, 150, 290);
                                MGPlusPathCloseFigure (path);
                                MGPlusPenSetJoinStyle (pen, JOIN_ROUND);
                                MGPlusPenSetCapStyle (pen, CAP_ROUND);
                                MGPlusDrawPath(hgs, pen, path);
                                MGPlusPenDelete (pen);
                            }

                            MGPlusPathDelete(path);
                            free(pt);
                        }
                        break;
                    case SCANCODE_F3:
                        test_add_path(hgs, pen, brush);
                        break;
                    case SCANCODE_F4:
                        {
                            HPATH path_solid = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                            MGPlusPathAddRectangle (path_solid, 180, 50, 150, 50);
                            MGPlusSetSolidBrushColor (brush, 0xFF505050);
                            MGPlusFillPath (hgs, brush, path_solid);

                            HBRUSH brush_path_gradient = MGPlusBrushCreate (MP_BRUSH_TYPE_PATHGRADIENT);
                            HPATH path_path_gradient = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                            MPPOINT center_point = {255, 175};
                            MGPlusPathAddRectangle (path_path_gradient, 180, 150, 350, 150);
                            MGPlusSetPathGradientBrushCenterPoint (brush_path_gradient,
                                    &center_point);
                            MGPlusSetPathGradientBrushCenterColor (brush_path_gradient, 0xFF505050);

                            ARGB rect_rgba [4] = {0xFFABCDEF, 0xFFBCDEFA, 0xFFCDEFAB, 0xFFDEFABC};
                            MGPlusSetPathGradientBrushSurroundColors (brush_path_gradient,
                                    rect_rgba, 4);
                            MGPlusFillPath (hgs, brush_path_gradient, path_path_gradient);

                            MGPlusPathDelete (path_solid);
                            MGPlusBrushDelete (brush_path_gradient);
                            MGPlusPathDelete (path_path_gradient);
                            break;
                        }

                    case SCANCODE_F5:
                        {
                            //x
                            HBRUSH brush_x_gradient = MGPlusBrushCreate (MP_BRUSH_TYPE_LINEARGRADIENT);
                            HPATH path_x_gradient = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                            RECT rect = {10, 10, 200, 100};
                            ARGB colors [3] = {0xFFFF0000, 0xFF00FF00, 0xFFFF0000};
                            MGPlusPathAddRectangle (path_x_gradient, rect.left, rect.top,
                                    rect.right - rect.left, rect.bottom - rect.top);
                            MGPlusSetLinearGradientBrushMode (brush_x_gradient,
                                    MP_LINEAR_GRADIENT_MODE_HORIZONTAL);
                            MGPlusSetLinearGradientBrushRect (brush_x_gradient, &rect);
                            MGPlusSetLinearGradientBrushColors (brush_x_gradient, colors, 3);
                            MGPlusFillPath (hgs, brush_x_gradient, path_x_gradient);

                            //y
                            HBRUSH brush_y_gradient = MGPlusBrushCreate (MP_BRUSH_TYPE_LINEARGRADIENT);
                            HPATH path_y_gradient = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                            rect.top = 110;
                            rect.bottom = 200;
                            MGPlusPathAddRectangle (path_y_gradient, rect.left, rect.top,
                                    rect.right - rect.left, rect.bottom - rect.top);
                            MGPlusSetLinearGradientBrushMode (brush_y_gradient,
                                    MP_LINEAR_GRADIENT_MODE_VERTICAL);
                            MGPlusSetLinearGradientBrushRect (brush_y_gradient, &rect);
                            MGPlusSetLinearGradientBrushColors (brush_y_gradient, colors, 3);
                            MGPlusFillPath (hgs, brush_y_gradient, path_y_gradient);

                            //forward
                            HBRUSH brush_xy_gradient = MGPlusBrushCreate (MP_BRUSH_TYPE_LINEARGRADIENT);
                            HPATH path_xy_gradient = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                            rect.top = 210;
                            rect.bottom = 300;
                            MGPlusPathAddRectangle (path_xy_gradient, rect.left, rect.top,
                                    rect.right - rect.left, rect.bottom - rect.top);
                            MGPlusSetLinearGradientBrushMode (brush_xy_gradient,
                                    MP_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL);
                            MGPlusSetLinearGradientBrushRect (brush_xy_gradient, &rect);
                            MGPlusSetLinearGradientBrushColors (brush_xy_gradient, colors, 3);
                            MGPlusFillPath (hgs, brush_xy_gradient, path_xy_gradient);

                            //backward
                            HBRUSH brush_yx_gradient = MGPlusBrushCreate (MP_BRUSH_TYPE_LINEARGRADIENT);
                            HPATH path_yx_gradient = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                            rect.top = 310;
                            rect.bottom = 400;
                            MGPlusPathAddRectangle (path_yx_gradient, rect.left, rect.top,
                                    rect.right - rect.left, rect.bottom - rect.top);
                            MGPlusSetLinearGradientBrushMode (brush_yx_gradient,
                                    MP_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL);
                            MGPlusSetLinearGradientBrushRect (brush_yx_gradient, &rect);
                            MGPlusSetLinearGradientBrushColors (brush_yx_gradient, colors, 3);
                            MGPlusFillPath (hgs, brush_yx_gradient, path_yx_gradient);

                            MGPlusBrushDelete (brush_xy_gradient);
                            MGPlusPathDelete (path_xy_gradient);
                            MGPlusBrushDelete (brush_yx_gradient);
                            MGPlusPathDelete (path_yx_gradient);
                            MGPlusBrushDelete (brush_y_gradient);
                            MGPlusPathDelete (path_y_gradient);
                            MGPlusBrushDelete (brush_x_gradient);
                            MGPlusPathDelete (path_x_gradient);
                            break;
                        }
                    case SCANCODE_F6:
                        {
                            MPPOINT points [4] = {{40,40},{440,-40},{340,840},{90,440}};
                            MGPlusGraphicLoadBitmap (hgs, 0, &g_stTestBitmap);
                            MGPlusDrawImageWithPoints (hgs, 0, points, 4);
                            break;
                        }
                    case SCANCODE_F7:
                        {
                            RECT rc = {0, 0, 1024, 768};
                            gal_pixel color[2]= {0xFF00FF00, 0xFF0000FF};
                            linear_gradient_draw(hgs, MP_LINEAR_GRADIENT_MODE_VERTICAL,
                                    &rc, color, 2);
                            break;
                        }
                    case SCANCODE_F8:
                        {
                            MPPOINT pts [4]= {
                                {0,   0},
                                {640, 0},
                                {640, 480},
                                {0,   480}
                            };
                            MPPOINT points [4]={
                                {0,   0},
                                {100, 0},
                                {100, 100},
                                {0,   100}
                            };

                            MGPlusGraphicLoadBitmap (hgs, 0, &g_stTestBitmap);
                            MGPlusDrawImageWithPoints (hgs, 0, pts, 4);

                            MGPlusGraphicLoadBitmapFromFile (hgs, 2, "res/butterfly.png");
                            MGPlusDrawImageWithPoints (hgs, 2, points, 4);
                            break;
                        }
                    case SCANCODE_F9:
                        {
                            HPATH path;
                            HPATH path_wind;

                            MGPlusSetSolidBrushColor(brush, 0xFF9314FF);

                            path = MGPlusPathCreate(MP_PATH_FILL_MODE_ALTERNATE);
                            path_wind = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);

                            if (!path) {
                                break;
                            }

                            MGPlusPathAddRectangleI (path_wind, 12, 12, 232, 70);
                            MGPlusPathAddEllipseI (path_wind, 64, 64, 40, 40, TRUE);
                            MGPlusPathAddEllipseI (path_wind, 192, 64, 40, 40, FALSE);

                            MGPlusPathMoveto (path_wind, 12, 132);
                            MGPlusPathAddRectangleI (path_wind, 12, 132, 232, 70);
                            MGPlusPathAddEllipseI (path_wind, 64, 184, 40, 40, TRUE);
                            MGPlusPathAddEllipseI (path_wind, 192, 184, 40, 40, TRUE);

                            MGPlusPathMoveto (path_wind, 12, 252);
                            MGPlusPathAddRectangleI (path_wind, 12, 252, 232, 70);
                            MGPlusPathAddEllipseI (path_wind, 64, 304, 40, 40, FALSE);
                            MGPlusPathAddEllipseI (path_wind, 192, 304, 40, 40, FALSE);

                            MGPlusFillPath (hgs, brush, path_wind);

                            MGPlusPathAddRectangleI(path, 262, 12, 232, 70);
                            MGPlusPathAddEllipseI(path, 314, 64, 40, 40, TRUE);
                            MGPlusPathAddEllipseI(path, 442, 64, 40, 40, FALSE);

                            MGPlusPathMoveto (path, 262, 132);
                            MGPlusPathAddRectangleI(path, 262, 132, 232, 70);
                            MGPlusPathAddEllipseI(path, 314, 184, 40, 40, TRUE);
                            MGPlusPathAddEllipseI(path, 442, 184, 40, 40, TRUE);

                            MGPlusPathMoveto (path, 262, 252);
                            MGPlusPathAddRectangleI (path, 262, 252, 232, 70);
                            MGPlusPathAddEllipseI (path, 314, 304, 40, 40, FALSE);
                            MGPlusPathAddEllipseI (path, 442, 304, 40, 40, FALSE);

                            MGPlusFillPath(hgs, brush, path);

                            MGPlusPathDelete(path);
                            MGPlusPathDelete(path_wind);
                            break;
                        }
                    case SCANCODE_F10:
                        {
                            HPATH path;

                            // MP_PATH_FILL_MODE_ALTERNATE, MP_PATH_FILL_MODE_WINDING
                            path = MGPlusPathCreate(MP_PATH_FILL_MODE_ALTERNATE);
                            if (!path) break;

                            MGPlusSetSolidBrushColor(brush, 0xFFFF0000);
                            MGPlusPathAddRoundRect(path, 10, 10, 100, 100, 20);
                            MGPlusPathAddRoundRect(path, 30, 30, 50,  50,  20);
                            MGPlusFillPath(hgs, brush, path);
                            MGPlusPathAddEllipseI(path, 192, 64, 100, 40, TRUE);
                            // rotate.
                            MGPlusTranslateWorldTransform (hgs, -192, -64);
                            MGPlusRotateWorldTransform (hgs, 120);
                            MGPlusTranslateWorldTransform (hgs, 192, 64);
                            MGPlusFillPath(hgs, brush, path);

                            MGPlusPathDelete(path);
                            break;
                        }

                    case SCANCODE_F11:
                        {
                            gal_pixel color[2]= {
                                0xFF00FF00, 0xFF0000FF
                            };
                            HDC hdc_com = CreateCompatibleDC (HDC_SCREEN);
                            //radio_gradient_draw(hdc_com, 200, 300, 100, color, 2);
                            radio_gradient_draw(hgs, 200, 300, 100, color, 2);
                            DeleteCompatibleDC (hdc_com);
                            break;
                        }
                    case SCANCODE_F12:
                    {
                        int i;
                        HDC hdc;
                        MPPOINT pts [4]= {
                                {0,   0},
                                {1024, 0},
                                {1024, 768},
                                {0,   768}
                        };
                        BITMAP bmp;
                        int count =1;
                        int begin, end;

                        //MGPlusSetInterpolationMode (hgs, MP_INTERPOLATION_MODE_PERSPECTIVE);

                        MGPlusGraphicLoadBitmapFromFile(hgs, 0, "res/pic1.jpg");
                        begin = GetTickCount();
                        for(i=0;i<count;i++)
                            MGPlusDrawImageWithPoints (hgs, 0, pts, 4);
                        end = GetTickCount();

                        printf("draw picture %d with mgplus: speed %d\n", count, end-begin);

                        hdc = GetClientDC(hWnd);
                        LoadBitmapFromFile(hdc, &bmp, "res/pic1.jpg");
                        begin = GetTickCount();
                        for(i=0;i<count;i++)
                            FillBoxWithBitmap(hdc, 0, 0, 0, 0, &bmp);
                        end = GetTickCount();

                        printf("draw picture %d with minigui: speed %d\n", count, end-begin);

                        ReleaseDC(hdc);
                        UnloadBitmap(&bmp);
                        break;
                    }
                    default:
                        break;
                }
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
