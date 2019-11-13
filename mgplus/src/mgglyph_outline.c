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
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgplus/mgplus.h>
#include "stdio.h"
#include "string.h"

#ifdef _MGPLUS_FONT_FT2

//#define HGRAPHICS_MARTIX 1

void TestDrawGlyphByOutlineCNText(HGRAPHICS hgs, int x, int y, int angle)
{
    int i = 0;
    HPATH  hpath   = 0;
    HBRUSH hbrush  = 0;
    GLYPHMETRICS   metrics;
    char* text = "\x98\xde\x6f\x2b\x4f\x60\x59\x7d\xff\x0c\x8f\xd9\x66\x2f\x4e\x2d\x65\x87\x6d\x4b\x8b\xd5\xff\x01\x8c\x22\x8c\x22\xff\x01\xfe\xff\x0\x0";
    GLYPHDATA glyph_data = {0};
    int orig_x = x, orig_y = y;

    HFONT hfont = MGPlusCreateFont ("./res/simhei.ttf", 0, MP_GLYPH_REN_OUTLINE,
            18, 18, TRUE);

    hbrush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR); 
    MGPlusSetSolidBrushColor (hbrush, 0xFF009000);
    if (!hbrush){
        printf("create brush failed!\n");
    }

    for (i = 0; i < strlen(text); i += 2) {
        unsigned short glyph_code = MAKEWORD16(text[i+1], text[i]);
        if (MGPlusGetGlyphOutline (hfont, glyph_code, 
                    &metrics, &glyph_data) != MP_OK)
        {
            printf("MGPlusGetGlyphOutline char %0x failed!\n", glyph_code);
        }

        hpath = MGPlusGetGlyphPath (x, y, &glyph_data);

        /* Transform used path Martix.*/
        MGPlusPathTranslate (hpath, -orig_x, -orig_y);
        MGPlusPathRotate (hpath, angle);
        MGPlusPathTranslate (hpath, orig_x, orig_y);
        MGPlusPathTransform (hpath);

        MGPlusFillPath(hgs, hbrush, hpath); 
        MGPlusPathReset(hpath);
        MGPlusPathDelete(hpath);

        x += metrics.adv_x;
        y += metrics.adv_y;
    }

    MGPlusBrushDelete(hbrush);
    MGPlusDeleteFont(hfont);
}

void TestDrawGlyphByOutline(HGRAPHICS hgs, int x, int y, int angle)
{
    int i = 0;
    HPATH  hpath   = 0;
    HBRUSH hbrush  = 0;
    GLYPHMETRICS   metrics;
    char* text = "GLYPH OUTLINE: ABCDEFG HIJKLMNOPQRSTUVWXYZ,abcdefghijklmnopqrstuvwxyz.";
    GLYPHDATA glyph_data = {0};
    int orig_x = x, orig_y = y;

    HFONT hfont = MGPlusCreateFont ("./res/timesi.ttf", 0, MP_GLYPH_REN_OUTLINE,
            18, 18, TRUE);

    hbrush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR); 
    MGPlusSetSolidBrushColor (hbrush, 0xFF009000);
    if (!hbrush){
        printf("create brush failed!\n");
    }

    /* Transform used HGRAPHICS Martix.*/
#ifdef HGRAPHICS_MARTIX 
    if (angle) {
        MGPlusResetWorldTransform (hgs);
        MGPlusTranslateWorldTransform (hgs, -x, -y);
        MGPlusRotateWorldTransform (hgs, angle);
        MGPlusTranslateWorldTransform (hgs, x, y);
    }
#endif

#ifdef PATH_TEST
    hpath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    MGPlusPathAddRectangle (hpath, 10, 100, 200, 200);

    MGPlusPathTranslate (hpath, -(10+100), -(100+100));
    MGPlusPathRotate (hpath, 70);
    MGPlusPathTranslate (hpath, (10+100), (100+100));
    MGPlusPathTransform (hpath);

    MGPlusFillPath(hgs, hbrush, hpath); 
    return;
#endif

    for (i = 0; i < strlen(text); i++) {
        if (MGPlusGetGlyphOutline (hfont, text[i], 
                    &metrics, &glyph_data) != MP_OK)
        {
            printf("MGPlusGetGlyphOutline char %c failed!\n", text[i]);
        }

        hpath = MGPlusGetGlyphPath (x, y, &glyph_data);

        /* Transform used path Martix.*/
        MGPlusPathTranslate (hpath, -orig_x, -orig_y);
        MGPlusPathRotate (hpath, angle);
        MGPlusPathTranslate (hpath, orig_x, orig_y);

        MGPlusPathTransform (hpath);

        MGPlusFillPath(hgs, hbrush, hpath); 
        MGPlusPathReset(hpath);
        MGPlusPathDelete(hpath);

        x += metrics.adv_x;
        y += metrics.adv_y;
        //printf("x=%d, y=%d,adv_x=%d, adv_y=%d\n", x, y, metrics.adv_x, metrics.adv_y);
    }

    MGPlusBrushDelete(hbrush);
    MGPlusDeleteFont(hfont);
}

void TestDrawGlyphByDataCNText(HGRAPHICS hgs, int x, int y, int angle, MPGlyphRendering ren)
{
    int i = 0;
    GLYPHMETRICS   metrics;
    char* text = "\x98\xde\x6f\x2b\x4f\x60\x59\x7d\xff\x0c\x8f\xd9\x66\x2f\x4e\x2d\x65\x87\x6d\x4b\x8b\xd5\xff\x01\x8c\x22\x8c\x22\xff\x01\xfe\xff\x0\x0";
    GLYPHDATA glyph_data = {0};

    HFONT hfont = MGPlusCreateFont ("./res/simhei.ttf", 0, ren,
            18, 18, TRUE);

    /* Transform used HGRAPHICS Martix.*/
    if (angle) {
        MGPlusResetWorldTransform (hgs);
        MGPlusTranslateWorldTransform (hgs, -x, -y);
        MGPlusRotateWorldTransform (hgs, angle);
        MGPlusTranslateWorldTransform (hgs, x, y);
    }

    for (i = 0; i < strlen(text); i += 2) {
        unsigned short glyph_code = MAKEWORD16(text[i+1], text[i]);
        if (MGPlusGetGlyphOutline (hfont, glyph_code, 
                    &metrics, &glyph_data) != MP_OK)
        {
            printf("MGPlusGetGlyphOutline char %0x failed!\n", glyph_code);
        }

        if (MGPlusDrawGlyph (hgs, hfont, x, y, 
                &glyph_data, 0xff809070) != MP_OK) 
        {
            printf("MGPlusDrawGlyph char %0x failed!\n", glyph_code);
        }

        x += metrics.adv_x;
        y += metrics.adv_y;
    }

    MGPlusDeleteFont(hfont);
}

void TestDrawGlyphByData(HGRAPHICS hgs, int x, int y, MPGlyphRendering ren)
{
    int i = 0;
    GLYPHMETRICS   metrics;
    char* test_text = ": ABCDEFG HIJKLMNOPQRSTUVWXYZ,abcdefghijklmnopqrstuvwxyz.";
    GLYPHDATA glyph_data = {0};
    char* ren_str[] = {" FT2_MONO", "FT2_GRAY8", "       OUTLINE", "    MGPLUS_MONO", "   MGPLUS_GRAY8"};
    char text[512] = {0};

    HFONT hfont = MGPlusCreateFont ("./res/timesi.ttf", 0, ren,
            18, 18, TRUE);

    strcpy (text, ren_str[ren]);
    strcat (text, test_text);


    for (i = 0; i < strlen(text); i++) {
        if (MGPlusGetGlyphOutline (hfont, text[i], 
                    &metrics, &glyph_data) != MP_OK)
        {
            printf("MGPlusGetGlyphOutline char %c failed!\n", text[i]);
        }

        if (MGPlusDrawGlyph (hgs, hfont, x, y, 
                &glyph_data, 0xff809070) != MP_OK) 
        {
            printf("MGPlusDrawGlyph char %c failed!\n", text[i]);
        }

        x += metrics.adv_x;
        y += metrics.adv_y;
    }

    MGPlusDeleteFont(hfont);
}

#endif

static LRESULT mGPlusWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int angle = 0;
    switch (message)
    {
        case MSG_CREATE:
            break;
        case MSG_RBUTTONDOWN:
            {
                HDC hdc = GetClientDC(hWnd);
                FillBox (hdc, 0, 0, g_rcScr.right, g_rcScr.bottom); 
                ReleaseDC(hdc);
            }
            break;
        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            switch(scancode)
            {
                case SCANCODE_F1:
                {
                    angle = 0;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                }
                case SCANCODE_F2:
                {
                    if (angle < 360)
                        angle += 5;
                    else angle = 1;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            break;
        }
        case MSG_PAINT:
        {
            HDC hdc = BeginPaint(hWnd);
            HGRAPHICS hgs = MGPlusGraphicCreateFromDC(hdc);

            if (hgs == MP_GENERIC_ERROR) {
                printf("create graphics failed\n");
                return 0;
            }

#ifdef _MGPLUS_FONT_FT2
            if (angle) {
                TestDrawGlyphByOutline(hgs, 200, 110, angle);
                TestDrawGlyphByOutlineCNText(hgs, 200, 150, angle);
                TestDrawGlyphByDataCNText(hgs, 200, 180, angle, MP_GLYPH_REN_NATIVE_MONO);
                TestDrawGlyphByDataCNText(hgs, 200, 210, angle, MP_GLYPH_REN_NATIVE_GRAY8);
                TestDrawGlyphByDataCNText(hgs, 200, 240, angle, MP_GLYPH_REN_AGG_MONO);
                TestDrawGlyphByDataCNText(hgs, 200, 270, angle, MP_GLYPH_REN_AGG_GRAY8);
            }
            else {
                /* 1.show english text.*/
                TestDrawGlyphByOutline(hgs, 10, 110, 0);
                TestDrawGlyphByData(hgs, 10, 130, MP_GLYPH_REN_NATIVE_MONO);
                TestDrawGlyphByData(hgs, 10, 150, MP_GLYPH_REN_NATIVE_GRAY8);
                TestDrawGlyphByData(hgs, 10, 170, MP_GLYPH_REN_AGG_MONO);
                TestDrawGlyphByData(hgs, 10, 190, MP_GLYPH_REN_AGG_GRAY8);
                TestDrawGlyphByData(hgs, 10, 210, MP_GLYPH_REN_OUTLINE);
                /* 2.show chinese text.*/
                TestDrawGlyphByOutlineCNText(hgs, 150, 260, 0);
                TestDrawGlyphByDataCNText(hgs, 150, 280, 0, MP_GLYPH_REN_NATIVE_MONO);
                TestDrawGlyphByDataCNText(hgs, 150, 300, 0, MP_GLYPH_REN_NATIVE_GRAY8);
                TestDrawGlyphByDataCNText(hgs, 150, 320, 0, MP_GLYPH_REN_AGG_MONO);
                TestDrawGlyphByDataCNText(hgs, 150, 340, 0, MP_GLYPH_REN_AGG_GRAY8);
            } 
#endif
            if (MGPlusGraphicSave(hgs, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
                printf("save graphic failed!");

            MGPlusGraphicDelete(hgs);
            EndPaint (hWnd, hdc);
            break;
        }
        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
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

    CreateInfo.dwStyle   = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_USEPARENTFONT | WS_EX_AUTOSECONDARYDC; 
    CreateInfo.spCaption = "mgplus glyphout(Please press F1,F2(glyph rotate))...";
    CreateInfo.hMenu     = 0;
    CreateInfo.hCursor   = 0;
    CreateInfo.hCursor   = GetSystemCursor(0);
    CreateInfo.hIcon     = 0;
    CreateInfo.MainWindowProc = mGPlusWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor  = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting  = HWND_DESKTOP;

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
