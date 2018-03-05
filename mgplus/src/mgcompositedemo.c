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

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgplus/mgplus.h>

#define COMP_CLEAR      310
#define COMP_SRC        311
#define COMP_DST        312
#define COMP_SRC_OVER   313
#define COMP_DST_OVER   314
#define COMP_SRC_IN     315
#define COMP_DST_IN     316
#define COMP_SRC_OUT    317
#define COMP_DST_OUT    318
#define COMP_SRC_ATOP   319
#define COMP_DST_ATOP   320
#define COMP_XOR        321


static BITMAP bmp_comp;
static BITMAP bmp_bkgnd;
static BITMAP bmp_butterfly;
static int comp_mode = MP_COMP_OP_SRC_OVER;

static void GetCompMode(HWND hWnd, WPARAM wParam)
{

    switch (wParam) {
        case COMP_CLEAR:
            comp_mode = MP_COMP_OP_CLEAR;
            break;
        case COMP_SRC:
            comp_mode = MP_COMP_OP_SRC;
            break;
        case COMP_DST:
            comp_mode = MP_COMP_OP_DST;
            break;
        case COMP_SRC_OVER:
            comp_mode = MP_COMP_OP_SRC_OVER;
            break;
        case COMP_DST_OVER:
            comp_mode = MP_COMP_OP_DST_OVER;
            break;
        case COMP_SRC_IN:
            comp_mode = MP_COMP_OP_SRC_IN;
            break;
        case COMP_DST_IN:
            comp_mode = MP_COMP_OP_DST_IN;
            break;
        case COMP_SRC_OUT:
            comp_mode = MP_COMP_OP_SRC_OUT;
            break;
        case COMP_DST_OUT:
            comp_mode = MP_COMP_OP_DST_OUT;
            break;
        case COMP_SRC_ATOP: 
            comp_mode = MP_COMP_OP_SRC_ATOP;
            break;
        case COMP_DST_ATOP:
            comp_mode = MP_COMP_OP_DST_ATOP;
            break;
        case COMP_XOR: 
            comp_mode = MP_COMP_OP_XOR;
            break;
        default:
            return;
    }

}

#define C_POS 600
#define C_W   170

static CTRLDATA CtrlYourTaste[] =
{ 
    {
        "static",
        WS_VISIBLE | SS_GROUPBOX, 
        16+C_POS, 0, 200, 160+200,
        IDC_STATIC,
        "Composite_Mode",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP,
        36+C_POS, 38, C_W, 20,
        COMP_CLEAR,
        "Composite_Clear",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 64, C_W, 20,
        COMP_SRC,
        "Composite_Source",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON, 
        36+C_POS, 90, C_W, 20, 
        COMP_DST, 
        "Composite_Dst",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON| BS_CHECKED ,
        36+C_POS, 116, C_W, 20,
        COMP_SRC_OVER,
        "Composite_SourceOver",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142, C_W, 20,
        COMP_DST_OVER,
        "Composite_DstOver",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142+1*26, C_W, 20,
        COMP_SRC_IN,
        "Composite_SourceIn",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142+2*26, C_W, 20,
        COMP_DST_IN,
        "Composite_DstIn",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON, 
        36+C_POS, 142+3*26, C_W, 20, 
        COMP_SRC_OUT, 
        "Composite_SourceOut",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142+4*26, C_W, 20,
        COMP_DST_OUT,
        "Composite_DstOut",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142+5*26, C_W, 20,
        COMP_SRC_ATOP,
        "Composite_SourceAtop",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142+6*26, C_W, 20,
        COMP_DST_ATOP,
        "Composite_DestAtop",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        36+C_POS, 142+7*26, C_W, 20,
        COMP_XOR,
        "Composite_Xor",
        0
    },

    {
        "static",
        WS_VISIBLE | SS_GROUPBOX, 
        16+C_POS, 50+142+7*26, 200, 235,
        IDC_STATIC,
        "Composite_Alpha",
        0
    },
};

static DLGTEMPLATE DlgYourTaste =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_AUTOSECONDARYDC,
    0, 0, 370+420, 300+290,
    "CompositeDemo(press F1,F2,F3...)",
    0, 0,
    TABLESIZE(CtrlYourTaste),
    NULL,
    0
};

void CompositeDrawBmpBlend(HWND hDlg)
{
    RECT rect = {20, 20, 200, 200};
    HDC  hdc = BeginPaint(hDlg);
    HPATH  hpath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    HBRUSH hbrush = 0;
    HGRAPHICS hgs_comp;
    HGRAPHICS hgs;
    RECT cl_rc = {0};

    /* first Clear the output_area that have draw last time.*/
    GetClientRect(hDlg, &cl_rc);
    //FillBox (hdc, 0, 0, 400, 200); 
    FillBox (hdc, 0, 0, RECTW(cl_rc), RECTH(cl_rc)); 

    hgs_comp = MGPlusGraphicCreateFromDC(hdc);
    hgs = MGPlusGraphicCreateFromDC(hdc);
    if (hgs_comp == MP_GENERIC_ERROR || hgs == MP_GENERIC_ERROR) {
        printf("create graphics failed\n");
        return;
    }

    hbrush = MGPlusBrushCreate(MP_BRUSH_TYPE_SOLIDCOLOR); 
    if (!hbrush){
        printf("create brush failed!\n");
    }

    MGPlusPathAddArc (hpath, rect.left+RECTW(rect)/2, 
            rect.top+RECTH(rect)/2, RECTW(rect)/2, RECTH(rect)/2, 0, 360);

    MGPlusGraphicClear(hgs_comp, 0x00000000);
    MGPlusGraphicClear(hgs, 0x00ffffff);

    MGPlusGraphicLoadBitmap (hgs_comp, 1, &bmp_butterfly);
    MGPlusDrawImage(hgs_comp, 1, 220, 35, -1, -1);
    MGPlusSetSolidBrushColor (hbrush, 0xDF6FF0FD);
    MGPlusSetCompositingMode (hgs_comp, (MPCompositingMode)MP_COMP_OP_SRC);
    MGPlusFillPath(hgs_comp, hbrush, hpath); 

    MGPlusPathReset(hpath);
    OffsetRect(&rect, 80, 0);
    MGPlusPathAddRoundRect(hpath, rect.left, rect.top, RECTW(rect), RECTH(rect), 50);
    MGPlusSetSolidBrushColor (hbrush, 0xBFFFC17F);
    MGPlusSetCompositingMode (hgs_comp, (MPCompositingMode)comp_mode);
    MGPlusFillPath(hgs_comp, hbrush, hpath); 
    MGPlusGraphicBlend(hgs_comp, hgs);

    if (MGPlusGraphicSave(hgs, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
        printf("save graphic failed!\n");

    MGPlusPathDelete (hpath);
    MGPlusGraphicDelete(hgs_comp);
    MGPlusGraphicDelete(hgs);
    EndPaint(hDlg, hdc);

}

void CompositeDrawBlend(HWND hDlg)
{
    RECT rect = {20, 20, 200, 200};
    HDC  hdc = BeginPaint(hDlg);
    HPATH  hpath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    HBRUSH hbrush = 0;
    HGRAPHICS hgs_comp;
    HGRAPHICS hgs;
    RECT cl_rc = {0};

    /* first Clear the output_area that have draw last time.*/
    GetClientRect(hDlg, &cl_rc);
    //FillBox (hdc, 0, 0, 400, 200); 
    FillBox (hdc, 0, 0, RECTW(cl_rc), RECTH(cl_rc)); 

    hgs_comp = MGPlusGraphicCreateFromDC(hdc);
    hgs = MGPlusGraphicCreateFromDC(hdc);
    if (hgs_comp == MP_GENERIC_ERROR || hgs == MP_GENERIC_ERROR) {
        printf("create graphics failed\n");
        return;
    }

    hbrush = MGPlusBrushCreate(MP_BRUSH_TYPE_SOLIDCOLOR); 
    if (!hbrush){
        printf("create brush failed!\n");
    }

    MGPlusPathAddArc (hpath, rect.left+RECTW(rect)/2, 
            rect.top+RECTH(rect)/2, RECTW(rect)/2, RECTH(rect)/2, 0, 360);

#if 0
    MGPlusGraphicClear(hgs_comp, 0x00000000);
    MGPlusGraphicClear(hgs, 0x00000000);
#else
    MGPlusGraphicClear(hgs_comp, 0x00000000);
    MGPlusGraphicClear(hgs, 0x00ffffff);
#endif

    MGPlusSetSolidBrushColor (hbrush, 0xDF6FF0FD);
    MGPlusSetCompositingMode (hgs_comp, (MPCompositingMode)MP_COMP_OP_SRC);
    MGPlusFillPath(hgs_comp, hbrush, hpath); 

    MGPlusPathReset(hpath);
    OffsetRect(&rect, 80, 0);
    MGPlusPathAddRoundRect(hpath, rect.left, rect.top, RECTW(rect), RECTH(rect), 50);
    MGPlusSetSolidBrushColor (hbrush, 0xBFFFC17F);
    MGPlusSetCompositingMode (hgs_comp, (MPCompositingMode)comp_mode);
    MGPlusFillPath(hgs_comp, hbrush, hpath); 
    MGPlusGraphicBlend(hgs_comp, hgs);

    if (MGPlusGraphicSave(hgs, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
        printf("save graphic failed!\n");

    MGPlusPathDelete (hpath);
    MGPlusGraphicDelete(hgs_comp);
    MGPlusGraphicDelete(hgs);
    EndPaint(hDlg, hdc);

}

void CompositeDrawCopy(HWND hDlg)
{
    RECT rect = {20, 20, 200, 200};
    HDC hdc = BeginPaint(hDlg);
    HPATH hpath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    HBRUSH hbrush = 0;
    HGRAPHICS hgs;
    RECT cl_rc = {0};

    /* first Clear the output_area that have draw last time.*/
    GetClientRect(hDlg, &cl_rc);
    //FillBox (hdc, 0, 0, 400, 200); 
    FillBox (hdc, 0, 0, RECTW(cl_rc), RECTH(cl_rc)); 

    hgs = MGPlusGraphicCreateFromDC(hdc);

    if (hgs == MP_GENERIC_ERROR) {
        printf("create graphics failed\n");
        return;
    }

    hbrush = MGPlusBrushCreate(MP_BRUSH_TYPE_SOLIDCOLOR); 
    if (!hbrush){
        printf("create brush failed!\n");
    }

    MGPlusPathAddArc (hpath, rect.left+RECTW(rect)/2, 
            rect.top+RECTH(rect)/2, RECTW(rect)/2, RECTH(rect)/2, 0, 360);
    MGPlusSetSolidBrushColor (hbrush, 0xDF6FF0FD);
    MGPlusSetCompositingMode (hgs, (MPCompositingMode)MP_COMP_OP_SRC_OVER);
    MGPlusFillPath(hgs, hbrush, hpath); 

    MGPlusPathReset(hpath);
    OffsetRect(&rect, 80, 0);
    MGPlusPathAddRoundRect(hpath, rect.left, rect.top, RECTW(rect), RECTH(rect), 50);
    MGPlusSetSolidBrushColor (hbrush, 0xBFFFC17F);
    MGPlusSetCompositingMode (hgs, (MPCompositingMode)comp_mode);
    MGPlusFillPath(hgs, hbrush, hpath); 

    if (MGPlusGraphicSave(hgs, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
        printf("save graphic failed!");

    MGPlusPathDelete (hpath);
    MGPlusGraphicDelete(hgs);
    EndPaint(hDlg, hdc);
}

static LRESULT DialogBoxProc2 (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int disp_mode = 3;
    switch (message) {
        case MSG_INITDIALOG:
            SetWindowBkColor(hDlg, COLOR_lightwhite);
            return 1;
        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            //RECT rc = {0, 0, 400, 300};
            switch(scancode)
            {
                case SCANCODE_F1:
                    disp_mode = 1;
                    InvalidateRect(hDlg, NULL, FALSE);
                    break;
                case SCANCODE_F2:
                    disp_mode = 2;
                    InvalidateRect(hDlg, NULL, FALSE);
                    break;
                case SCANCODE_F3:
                    disp_mode = 3;
                    InvalidateRect(hDlg, NULL, FALSE);
                    break;
            }
            break;
        }
        case MSG_COMMAND:
            GetCompMode(hDlg, wParam);
            if(wParam >= COMP_CLEAR && wParam <= COMP_XOR) {
                RECT rc = {0, 0, 400, 300};
                InvalidateRect(hDlg, &rc, FALSE);
            }
            switch (wParam) {
                case IDOK:
                case IDCANCEL:
                    EndDialog (hDlg, wParam);
                    break;
            }
            break;
        case MSG_PAINT:
            if (disp_mode == 1)
                CompositeDrawCopy(hDlg);
            else if (disp_mode == 2)
                CompositeDrawBlend(hDlg);
            else 
                CompositeDrawBmpBlend(hDlg);
            break;
        case MSG_CLOSE:
            break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "composite" , 0 , 0);
#endif
    
    if (LoadBitmap (HDC_SCREEN, &bmp_comp, "res/compositing.bmp"))
        return 1;
    if (LoadBitmap (HDC_SCREEN, &bmp_butterfly, "res/butterfly.png"))
        return 1;
    if (LoadBitmap (HDC_SCREEN, &bmp_bkgnd, "res/flower_2.png"))
        return 1;

    DlgYourTaste.controls = CtrlYourTaste;
    
    DialogBoxIndirectParam (&DlgYourTaste, HWND_DESKTOP, DialogBoxProc2, 0L);

    UnloadBitmap (&bmp_butterfly);
    UnloadBitmap (&bmp_bkgnd);
    UnloadBitmap (&bmp_comp);

    return 0;
}

#ifndef _MGRM_PROCESSES
#include <minigui/dti.c>
#endif

