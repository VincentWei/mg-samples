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

static int comp_mode = MP_COMP_OP_SRC_OVER;
BITMAP bitmap;

void CompositeDraw(HWND hWnd, HGRAPHICS graphics)
{
    HPATH path_bk, path;
    HBRUSH brush, Brush;
    HDC hdc = BeginPaint(hWnd);
    MPPOINT bmpoint []= {{600,50},{400,50},{400,250},{600,250}};

    FillBox (hdc, 0, 0, 1024, 768);

    Brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
    MGPlusSetSolidBrushColor (Brush, 0xFF00FF00);
    brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
    MGPlusSetSolidBrushColor (brush, 0x8F0000FF);

    path = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
    path_bk = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);

    MGPlusGraphicCopyFromDC (graphics, hdc, 0, 0, 0, 0, 0, 0);

    MGPlusPathReset(path);
    MGPlusPathAddEllipse( path, 325, 150, 150, 150, TRUE);
    MGPlusGraphicLoadBitmap (graphics, 0, &bitmap); 
    MGPlusDrawImageWithPoints (graphics, 0, bmpoint, 4);
    MGPlusSetCompositingMode (graphics, MP_COMP_OP_SRC);
    MGPlusFillPath (graphics, brush, path);

    MGPlusPathReset(path);
    MGPlusPathAddRoundRect (path, 325, 40, 200, 220, 25);
    MGPlusSetCompositingMode(graphics , comp_mode);
    MGPlusFillPath (graphics, Brush, path);

    MGPlusGraphicSave (graphics, hdc, 0, 0, 0, 0, 0, 0);

    MGPlusPathDelete(path_bk);
    MGPlusPathDelete(path);
    MGPlusBrushDelete (brush);
    MGPlusBrushDelete (Brush);

    EndPaint(hWnd, hdc);
}

LRESULT ColorCompositeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HGRAPHICS graphics;

    switch (message) 
    {
        case MSG_CREATE:
            {
                LoadBitmap(HDC_SCREEN, &bitmap, "res/wede.bmp");
                graphics = MGPlusGraphicCreate( 1024, 768);
            }
            break;

        case MSG_COMMAND:
            switch (wParam)
            {
                case ID_MP_COMP_OP_CLEAR:
                    comp_mode = MP_COMP_OP_CLEAR;
                    break;
                case ID_MP_COMP_OP_SRC:
                    comp_mode = MP_COMP_OP_SRC;
                    break;
                case ID_MP_COMP_OP_DST:
                    comp_mode = MP_COMP_OP_DST;
                    break;
                case ID_MP_COMP_OP_SRC_OVER:
                    comp_mode = MP_COMP_OP_SRC_OVER;
                    break;
                case ID_MP_COMP_OP_DST_OVER:
                    comp_mode = MP_COMP_OP_DST_OVER;
                    break;
                case ID_MP_COMP_OP_SRC_IN:
                    comp_mode = MP_COMP_OP_SRC_IN;
                    break;
                case ID_MP_COMP_OP_DST_IN:
                    comp_mode = MP_COMP_OP_DST_IN;
                    break;
                case ID_MP_COMP_OP_SRC_OUT:
                    comp_mode = MP_COMP_OP_SRC_OUT;
                    break;
                case ID_MP_COMP_OP_DST_OUT:
                    comp_mode = MP_COMP_OP_DST_OUT;
                    break;
                case ID_MP_COMP_OP_SRC_ATOP:
                    comp_mode = MP_COMP_OP_SRC_ATOP;
                    break;
                case ID_MP_COMP_OP_DST_ATOP:
                    comp_mode = MP_COMP_OP_DST_ATOP;
                    break;
                case ID_MP_COMP_OP_XOR:
                    comp_mode = MP_COMP_OP_XOR;
                    break;
                default:
                    break;
            }
            if (comp_mode >= MP_COMP_OP_CLEAR && comp_mode <= MP_COMP_OP_XOR)
                InvalidateRect (hWnd, NULL, FALSE);
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
            CompositeDraw(hWnd, graphics);
            break;
        case MSG_CLOSE:
            MGPlusGraphicDelete (graphics);
            UnloadBitmap(&bitmap);
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static HMENU Menu_Mode (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_CLEAR;
    mii.typedata    = (DWORD)"MP_COMP_OP_CLEAR";
    hmnu = CreatePopupMenu (&mii);
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_SRC;
    mii.typedata    = (DWORD)"MP_COMP_OP_SRC";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_DST;
    mii.typedata    = (DWORD)"MP_COMP_OP_DST";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_SRC_OVER;
    mii.typedata    = (DWORD)"MP_COMP_OP_SRC_OVER";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_DST_OVER;
    mii.typedata    = (DWORD)"MP_COMP_OP_DST_OVER";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_SRC_IN;
    mii.typedata    = (DWORD)"MP_COMP_OP_SRC_IN";
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_DST_IN;
    mii.typedata    = (DWORD)"MP_COMP_OP_DST_IN";
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_SRC_OUT;
    mii.typedata    = (DWORD)"MP_COMP_OP_SRC_OUT";
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_DST_OUT;
    mii.typedata    = (DWORD)"MP_COMP_OP_DST_OUT";
    InsertMenuItem(hmnu, 8, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_SRC_ATOP;
    mii.typedata    = (DWORD)"MP_COMP_OP_SRC_ATOP";
    InsertMenuItem(hmnu, 9, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_DST_ATOP;
    mii.typedata    = (DWORD)"MP_COMP_OP_DST_ATOP";
    InsertMenuItem(hmnu, 10, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_COMP_OP_XOR;
    mii.typedata    = (DWORD)"MP_COMP_OP_XOR";
    InsertMenuItem(hmnu, 11, TRUE, &mii);
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
    mii.typedata    = (DWORD)"CompositingMode";
    mii.hsubmenu    = Menu_Mode ();
    InsertMenuItem(hmnu, 0, TRUE, &mii);

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
    CreateInfo.dwExStyle = WS_EX_AUTOSECONDARYDC;
    CreateInfo.spCaption = "ColorCompoiste test";
    CreateInfo.hMenu = Menu_CompositeTest();
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ColorCompositeProc;
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

