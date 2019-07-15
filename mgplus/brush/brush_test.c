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
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgutils/mgutils.h>

#include <mgplus/mgplus.h>

#define ID_AddPath            103

#define ID_SetSolidBrushColor            101
#define ID_SetHatchBrushColor            102
#define ID_SetPathGradientBrushCenterColor            110
#define ID_PathGradientBrush           125
#define ID_SetPathGradientBrushSurroundColor            111

#define ID_SetTextureBrushImage 123

#define ID_HATCH_STYLE_HORIZONTAL            104
#define ID_HATCH_STYLE_VERTICAL            105
#define ID_HATCH_STYLE_FORWARDDIAGONAL            106
#define ID_HATCH_STYLE_BACKWARDDIAGONAL            107
#define ID_HATCH_STYLE_CROSS            108
#define ID_HATCH_STYLE_DIAGONALCROSS            109

#define ID_WRAP_MODE_TILE 113
#define ID_WRAP_MODE_TILE_FLIP_X            114
#define ID_WRAP_MODE_TILE_FLIP_Y            115
#define ID_WRAP_MODE_TILE_FLIP_XY            116
#define ID_WRAP_MODE_TILE_FLIP             117
#define ID_WRAP_MODE_CLAMP            118

#define ID_LINEAR_GRADIENT_MODE_HORIZONTAL            119
#define ID_LINEAR_GRADIENT_MODE_VERTICAL            120
#define ID_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL            121
#define ID_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL            122

//#define TEST

LRESULT BrushProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HGRAPHICS  graphics_L, graphics, graphics_T, graphics_H;
    static HPATH  pathodd,pathzero;
    static HBRUSH brush_T, brush_S, brush_L, brush_H, brush_P ;
    static HPEN pen;
    static ARGB p = 0xFF000000;
    static COLORDLGDATA pcdd = {0};
    HDC hdc;
    static RECT oddrect_L = {25,150,325,250};
    static RECT zerorect_L = {625,150,925,250};
    static ARGB a [3] = {0xFFFF00FF, 0xFF00FF00, 0xFFFF0000};
    static ARGB b [3] = {0xFFFF0000, 0xFF00FF00,0xFFFF00FF };
    static ARGB color_L [6] = {0xFFFF0000, 0xFF0000FF,0xFFFF00FF, 0xff00FFFF,0xff00FF00, 0xFF00FF00};
    switch (message) 
    {
#if 1
        case MSG_CREATE:

            hdc = GetClientDC (hWnd);
            pathodd = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);
            MGPlusPathAddRectangle (pathodd, 25, 150, 300, 100);
            MGPlusPathAddEllipse (pathodd, 100, 250, 50, 50, TRUE);
            MGPlusPathAddEllipse (pathodd,250, 250, 50, 50, TRUE);

            pathzero = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
            MGPlusPathAddRectangle (pathzero, 625, 150, 300, 100);
            MGPlusPathAddEllipse (pathzero, 700, 250, 50, 50, TRUE);
            MGPlusPathAddEllipse (pathzero, 850, 250, 50, 50, TRUE);

#ifndef TEST
            graphics = MGPlusGraphicCreate( 1024, 768);
#else
            graphics = MGPlusGraphicCreateWithoutCanvas(hdc);
#endif
            pen = MGPlusPenCreate(9, p);

            brush_T = MGPlusBrushCreate (MP_BRUSH_TYPE_TEXTUREFILL);
            brush_H = MGPlusBrushCreate (MP_BRUSH_TYPE_HATCHFILL);
            brush_S = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);

            brush_L = MGPlusBrushCreate (MP_BRUSH_TYPE_LINEARGRADIENT);

            brush_P = MGPlusBrushCreate (MP_BRUSH_TYPE_PATHGRADIENT);
#if 0
            graphics_H = MGPlusGraphicCreate( 1024, 768);
            MGPlusBrushSetHatchBrushColor (brus_H, ARGB fore_rgba, ARGB back_rgba);
            MGPlusBrushGetHatchBrushColor (brus_H, ARGB* fore_rgba, ARGB* back_rgba);
            MGPlusBrushSetHatchBrushStyle (brus_H, MGPlusHatchStyle type);
            MGPlusBrushGetHatchBrushStyle (brus_H, MGPlusHatchStyle* type);

            MGPlusBrushSetTextureBrushWrapMode (brus_T, MGPlusWrapMode wrapmode)
                MGPlusBrushGetTextureBrushWrapMode (brus_T, MGPlusWrapMode* wrapmode)

                graphics_L = MGPlusGraphicCreate( 1024, 768);
            MGPlusBrushSetLinearGradientBrushMode (brus_L, MGPlusLinearGradientMode mode);
            MGPlusBrushGetLinearGradientBrushMode (brus_L, MGPlusLinearGradientMode* mode);
            MGPlusBrushSetLinearGradientBrushRect (brus_L, RECT* rect);
            MGPlusBrushSetLinearGradientBrushColor (brus_L,ARGB* start_color, ARGB* end_color);
#endif
            MGPlusSetSolidBrushColor (brush_S, 0xFFFF0000);
            MGPlusFillPath (graphics, brush_S, pathodd);
            MGPlusFillPath (graphics, brush_S, pathzero);
            break;

        case MSG_RBUTTONDOWN:
            {
                HDC hdc = GetClientDC(hWnd);
                FillBox (hdc, 0, 0, g_rcScr.right, g_rcScr.bottom); 
                ReleaseDC(hdc);
            }
            break;
    
        case MSG_COMMAND:
            MGPlusGraphicClear (graphics, 0);
            hdc = GetClientDC (hWnd);
            FillBox (hdc, 0, 0, 1024, 768);
            MGPlusGraphicCopyFromDC (graphics, hdc, 0, 0, 0, 0, 0, 0);
            ReleaseDC (hdc);
            switch (wParam)
            {
                case ID_SetSolidBrushColor:
                    {
                        ColorSelectDialog(NULL, hWnd, NULL, &pcdd);

                        gal_pixel color;
                        DWORD r, g, b;
                        color = 0xff000000;
                        r = g = b = 0x00000000; 
                        r = (pcdd.b&0xFF)<<16;
                        g = (pcdd.g&0xFF)<<8;
                        b = (pcdd.r&0xFF);
                        color = color | r | g | b;
                        MGPlusSetSolidBrushColor (brush_S, color);

                        MGPlusFillPath (graphics, brush_S, pathodd);
                        MGPlusFillPath (graphics, brush_S, pathzero);
                    }
                    break;
                case ID_PathGradientBrush:
                    {
                        MPPOINT oddpoint = {170,225};
                        MPPOINT zeropoint = {770,225};

                        RECT oddrect = {25,150,325,250};
                        RECT zerorect = {625,150,925,250};
                        
                        MGPlusSetPathGradientBrushCenterPoint (brush_P, &oddpoint);
                        MGPlusSetPathGradientBrushCenterColor (brush_P, 0xFF0000FF);
                        MGPlusSetPathGradientBrushSurroundColors (brush_P, a,3);
                        MGPlusSetPathGradientBrushSurroundRect (brush_P, &oddrect);

                        MGPlusFillPath (graphics, brush_P, pathodd);

                        MGPlusSetPathGradientBrushCenterPoint (brush_P, &zeropoint);
                        MGPlusSetPathGradientBrushCenterColor (brush_P, 0xFF0000FF);
                        MGPlusSetPathGradientBrushSurroundColors (brush_P, b,3);
                        MGPlusSetPathGradientBrushSurroundRect (brush_P, &zerorect);

                        MGPlusFillPath (graphics, brush_P, pathzero);
                    }
                    break;

                case ID_LINEAR_GRADIENT_MODE_HORIZONTAL:     
                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_HORIZONTAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &oddrect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathodd);

                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_HORIZONTAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &zerorect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathzero);
                    break;
                case ID_LINEAR_GRADIENT_MODE_VERTICAL:     
                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_VERTICAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &oddrect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathodd);

                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_VERTICAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &zerorect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathzero);
                    break;
                case ID_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL:
                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &oddrect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathodd);

                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &zerorect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathzero);
                    break;
                case ID_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL:
                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &oddrect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathodd);
                    
                    MGPlusSetLinearGradientBrushMode (brush_L, MP_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL);
                    MGPlusSetLinearGradientBrushRect (brush_L, &zerorect_L);
                    MGPlusSetLinearGradientBrushColors (brush_L, color_L, 6);

                    MGPlusFillPath (graphics, brush_L, pathzero);
                    break;

                case ID_SetHatchBrushColor:
                    break;
                case ID_SetTextureBrushImage:
#if 0
                    {
                        static BITMAP bitmap;
                        LoadBitmap (HDC_SCREEN, &bitmap, "./res/aifousen.jpg");

                        MGPlusSetTextureBrushImage (brush_T, &bitmap);
                    }
                    MGPlusFillPath (graphics, brush_T, pathodd);
                    MGPlusFillPath (graphics, brush_T, pathzero);
#endif
                    break;

                case ID_HATCH_STYLE_HORIZONTAL:
                    break;
                case ID_HATCH_STYLE_VERTICAL:
                    break;
                case ID_HATCH_STYLE_FORWARDDIAGONAL:
                    break;
                case ID_HATCH_STYLE_BACKWARDDIAGONAL: 
                    break;
                case ID_HATCH_STYLE_CROSS:
                    break;
                case ID_HATCH_STYLE_DIAGONALCROSS:   
                    break;

                case ID_WRAP_MODE_TILE:
                    break;
                case ID_WRAP_MODE_TILE_FLIP_X:      
                    break;
                case ID_WRAP_MODE_TILE_FLIP_Y:
                    break;
                case ID_WRAP_MODE_TILE_FLIP_XY:
                    break;
                case ID_WRAP_MODE_TILE_FLIP :
                    break;
                case ID_WRAP_MODE_CLAMP:
                    break;

                default:
                    break;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case MSG_KEYDOWN:
            {
                int scancode = (int)wParam;
                switch(scancode){
                    default:
                        break;
                }
                break;
            }
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);

            MGPlusDrawLine(graphics, pen, 500, 0,500 ,500 );
            TextOut (hdc, 150, 500, "Even-Odd rule");
            TextOut (hdc, 680, 500, "None-Zero rule");

            MGPlusGraphicSave (graphics, hdc, 0, 0, 0, 0, 0, 0);
            MGPlusGraphicSave (graphics_T, hdc, 0, 0, 0, 0, 0, 0);

            EndPaint(hWnd, hdc);
            break;
        default:
            break;
        case MSG_CLOSE:
            MGPlusGraphicDelete (graphics);
            MGPlusPenDelete(pen);
            MGPlusPathDelete(pathodd);
            MGPlusPathDelete(pathzero);

            MGPlusBrushDelete (brush_T);
            MGPlusBrushDelete (brush_H);
            MGPlusBrushDelete (brush_S);
            MGPlusBrushDelete (brush_P);
            MGPlusBrushDelete (brush_L);

            MGPlusGraphicDelete (graphics_T);
            MGPlusGraphicDelete (graphics_L);
            MGPlusGraphicDelete (graphics_H);

            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }
#endif

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static HMENU Menu_SolidColor (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"Set color";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SetSolidBrushColor;
    mii.typedata    = (DWORD)"SetSolidBrushColor";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_HatchFill(void)
{
    HMENU hmnu;

    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"Style";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_HATCH_STYLE_HORIZONTAL;
    mii.typedata    = (DWORD)"HORIZONTAL";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_HATCH_STYLE_VERTICAL;
    mii.typedata    = (DWORD)"VERTICAL";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_HATCH_STYLE_FORWARDDIAGONAL;
    mii.typedata    = (DWORD)"FORWARDDIAGONAL";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_HATCH_STYLE_BACKWARDDIAGONAL;
    mii.typedata    = (DWORD)"BACKWARDDIAGONAL";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_HATCH_STYLE_CROSS;
    mii.typedata    = (DWORD)"CROSS";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_HATCH_STYLE_DIAGONALCROSS;
    mii.typedata    = (DWORD)"DIAGONALCROSS";
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 1;
    mii.typedata    = (DWORD)"SetColor";

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SetHatchBrushColor;
    mii.typedata    = (DWORD)"SetHatchBrushColor";
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_TextureFill(void)
{
    HMENU hmnu;

    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"WrapMode";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_WRAP_MODE_TILE;
    mii.typedata    = (DWORD)"TILE";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_WRAP_MODE_TILE_FLIP_X;
    mii.typedata    = (DWORD)"TILE_FLIP_X";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_WRAP_MODE_TILE_FLIP_Y;
    mii.typedata    = (DWORD)"TILE_FLIP_Y";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_WRAP_MODE_TILE_FLIP_XY;
    mii.typedata    = (DWORD)"TILE_FLIP_XY";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_WRAP_MODE_TILE_FLIP ;
    mii.typedata    = (DWORD)"TILE_FLIP =";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_WRAP_MODE_CLAMP;
    mii.typedata    = (DWORD)"CLAMP";
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 1;
    mii.typedata    = (DWORD)"SetImage";

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SetTextureBrushImage ;
    mii.typedata    = (DWORD)"SetTextureBrushImage ";
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_PathGradient(void)
{
    HMENU hmnu;

    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_PathGradientBrush;
    mii.typedata    = (DWORD)"PathGradientBrush";
    hmnu = CreatePopupMenu (&mii);
    InsertMenuItem(hmnu, 0, TRUE, &mii);
#if 0
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SetPathGradientBrushCenterColor;
    mii.typedata    = (DWORD)"SetPathGradientBrushCenterColor";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SetPathGradientBrushSurroundColor;
    mii.typedata    = (DWORD)"SetPathGradientBrushSurroundColor";
    InsertMenuItem(hmnu, 2, TRUE, &mii);
#endif

    return hmnu;
}

static HMENU Menu_LinearGradient(void)
{
    HMENU hmnu;

    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"LinearGradientMode";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_LINEAR_GRADIENT_MODE_HORIZONTAL;
    mii.typedata    = (DWORD)"HORIZONTAL";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_LINEAR_GRADIENT_MODE_VERTICAL;
    mii.typedata    = (DWORD)"VERTICAL";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL;
    mii.typedata    = (DWORD)"FORWARDDIAGONAL";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL;
    mii.typedata    = (DWORD)"BACKWARDDIAGONAL";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_BrushTest (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"SolidColor";
    mii.hsubmenu    = Menu_SolidColor ();
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"HatchFill";
    mii.hsubmenu    = Menu_HatchFill ();
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"TextureFill";
    mii.hsubmenu    = Menu_TextureFill ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"PathGradient";
    mii.hsubmenu    = Menu_PathGradient ();
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"LinearGradient";
    mii.hsubmenu    = Menu_LinearGradient ();
    InsertMenuItem(hmnu, 4, TRUE, &mii);
    return hmnu;
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
    CreateInfo.spCaption = "brush test";
    CreateInfo.hMenu = Menu_BrushTest();
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = BrushProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_cyan;
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

