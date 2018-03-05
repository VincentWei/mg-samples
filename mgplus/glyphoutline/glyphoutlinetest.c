#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgutils/mgutils.h>

#define ID_MP_GLYPH_REN_NATIVE_MONO               101
#define ID_MP_GLYPH_REN_NATIVE_GRAY8                 102
#define ID_MP_GLYPH_REN_OUTLINE                 103
#define ID_MP_GLYPH_REN_AGG_MONO            104
#define ID_MP_GLYPH_REN_AGG_GRAY8            105
#define ID_MP_GLYPH_REN_METRICS              106

LRESULT glyphoutlineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static HGRAPHICS graphics;
    static HPATH fpath ;
    static HBRUSH brush;
    static HPEN pen;
    static ARGB p = 0xFF0000FF;
#ifdef _MGPLUS_FONT_FT2
    static HFONT font;
    static GLYPHMETRICS   metrics;
    int i = 0;
    char* text = "i i i i ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 !@#$%^&*()./<>?;':[]{} |-=_+`~.";
    int x = 10, y = 150;
    GLYPHDATA glyphdata = {0};
#endif
    switch (message) 
    {
        case MSG_CREATE:
            {
                HDC hdc = GetClientDC(hWnd);
                pen = MGPlusPenCreate(1, p);
                graphics = MGPlusGraphicCreateFromDC(hdc);
                brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
                MGPlusSetSolidBrushColor (brush, 0xFF0000FF);
#ifdef _MGPLUS_FONT_FT2
                font = MGPlusCreateFont ("./res/timesi.ttf", 0, MP_GLYPH_REN_OUTLINE, 32, 32, TRUE);
#endif
                ReleaseDC (hdc);
            }
            break;

        case MSG_COMMAND:
            hdc = GetClientDC(hWnd);
            MGPlusGraphicClear (graphics,0xFFFFFFFF);
#ifdef _MGPLUS_FONT_FT2
            switch (wParam)
            {
                case ID_MP_GLYPH_REN_OUTLINE:
                    for (i = 0; i < strlen(text); i++)
                    {
                        if (MGPlusGetGlyphOutline (font, text[i],
                                    &metrics, &glyphdata) != MP_OK)
                        {
                            printf("MGPlusGetGlyphOutline char %c failed!\n", text[i]);
                        }
                        fpath = MGPlusGetGlyphPath (x, y, &glyphdata);
                        MGPlusFillPath (graphics, brush, fpath);
                        MGPlusPathDelete(fpath);

                        x += metrics.adv_x;
                        y += metrics.adv_y;
                    }
                    break;
                case ID_MP_GLYPH_REN_AGG_MONO:
                    break;
                case ID_MP_GLYPH_REN_AGG_GRAY8:
                    break;
                case ID_MP_GLYPH_REN_NATIVE_MONO:
                    break;
                case ID_MP_GLYPH_REN_NATIVE_GRAY8:
                    break;
                default:
                    break;
            }
#endif
            ReleaseDC (hdc);
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

            TextOut (hdc, 150, 500, "Even-Odd rule");
            TextOut (hdc, 680, 500, "None-Zero rule");

            if (MGPlusGraphicSave (graphics, hdc, 0, 0, 0, 0, 0, 0) == MP_OK)
                printf ("save graphic ok! \n");

            EndPaint(hWnd, hdc);
            break;
        case MSG_CLOSE:
#ifdef _MGPLUS_FONT_FT2
            MGPlusDeleteFont (font);
#endif
            MGPlusPenDelete(pen);
            MGPlusGraphicDelete (graphics);
            MGPlusPathDelete(fpath);
            MGPlusBrushDelete (brush);

            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static HMENU Menu_Format (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_GLYPH_REN_NATIVE_MONO;
    mii.typedata    = (DWORD)"MP_GLYPH_REN_NATIVE_MONO";
    hmnu = CreatePopupMenu (&mii);
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_GLYPH_REN_NATIVE_GRAY8;
    mii.typedata    = (DWORD)"MP_GLYPH_REN_NATIVE_GRAY8";
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_GLYPH_REN_OUTLINE;
    mii.typedata    = (DWORD)"MP_GLYPH_REN_OUTLINE";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_GLYPH_REN_AGG_MONO;
    mii.typedata    = (DWORD)"MP_GLYPH_REN_AGG_MONO";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_GLYPH_REN_AGG_GRAY8;
    mii.typedata    = (DWORD)"MP_GLYPH_REN_AGG_GRAY8";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_MP_GLYPH_REN_METRICS;
    mii.typedata    = (DWORD)"MP_GLYPH_REN_METRICS";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_GlyphOutlineTest (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"Format";
    mii.hsubmenu    = Menu_Format();
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
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "ColorCompoiste test";
    CreateInfo.hMenu = Menu_GlyphOutlineTest();
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = glyphoutlineProc;
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

