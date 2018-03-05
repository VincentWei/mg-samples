#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgplus/mgplus.h>

#define ID_Curve            101
#define ID_AddPath            103
#define ID_Line            104
#define ID_Arc            106
#define ID_Bezier            107
#define ID_Rectangle            108
#define ID_Ellipse            109
#define ID_DrawImageWithPoints 111
#define ID_DrawImageWithPath 112
#define ID_RoundRect 113

#define TEST

LRESULT PathOddProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HGRAPHICS graphics;
    static HPATH  oddpath, zeropath, oldpathodd, oldpathzero, addpathodd;
    static HBRUSH brush ;
    static ARGB a = 0xFF00FF00;
    static HPEN pen;
    static BITMAP Bitmap;
    HDC hdc;

    switch (message) 
    {
        case MSG_CREATE:
            hdc = GetClientDC (hWnd);
#ifndef TEST
            graphics= MGPlusGraphicCreateFromDC(hdc);
            //graphics = MGPlusGraphicCreate( 1024, 768);
#else
            graphics = MGPlusGraphicCreateWithoutCanvas(hdc);
#endif

            brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
            MGPlusSetSolidBrushColor (brush, 0xFF00FFFF);

            pen = MGPlusPenCreate(9, a);

            oddpath = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);
            zeropath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
            oldpathodd = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);
            oldpathzero = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
            addpathodd = MGPlusPathCreate (MP_PATH_FILL_MODE_ALTERNATE);

            LoadBitmap (HDC_SCREEN, &Bitmap, "./res/wede.bmp");
            break;
        case MSG_COMMAND:
            {
                MGPlusPathReset(oddpath);
                MGPlusPathReset(zeropath);
                MGPlusGraphicClear (graphics,0);

                hdc = GetClientDC (hWnd);
                RECT rc = {0};
                GetClientRect(hWnd, &rc);
                FillBox(hdc, 0, 0, RECTW(rc), RECTH(rc));
                ReleaseDC(hdc);
                MGPlusGraphicCopyFromDC (graphics, hdc, 0, 0, 0, 0, 0, 0);
            }
            switch (wParam)
            {
                case ID_AddPath:
                    {
                        MPPOINT oddpoint_curve [4];

                        oddpoint_curve[0].x = 25.6;
                        oddpoint_curve[0].y = 128.0;
                        oddpoint_curve[1].x = 102.4; 
                        oddpoint_curve[1].y = 230.4;
                        oddpoint_curve[2].x = 153.6;
                        oddpoint_curve[2].y = 25.6;
                        oddpoint_curve[3].x = 230.4;
                        oddpoint_curve[3].y = 128;

                        MGPlusPathAddCurve ( oldpathodd, oddpoint_curve, 4);

                        MPPOINT zeropoint_curve [4];

                        zeropoint_curve[0].x = 625.6;
                        zeropoint_curve[0].y = 128.0;
                        zeropoint_curve[1].x = 702.4; 
                        zeropoint_curve[1].y = 230.4;
                        zeropoint_curve[2].x = 753.6;
                        zeropoint_curve[2].y = 25.6;
                        zeropoint_curve[3].x = 830.4;
                        zeropoint_curve[3].y = 128;

                        MGPlusPathAddCurve ( oldpathzero, zeropoint_curve, 4);

                        MGPlusPathAddEllipse( oldpathodd, 10, 400, 200, 200, TRUE);
                        MGPlusPathAddEllipse( oldpathodd, 100, 400, 200, 200, TRUE);
                        MGPlusPathAddArc (oldpathodd, 100,50,200,200,45,180);
                        MGPlusPathAddLine (oldpathodd, 50, 50, 50, 300);
                        MGPlusPathAddBezier(oldpathodd, 11,11, 88,333, 99,0, 222,111);
                        MGPlusPathAddRectangle( oldpathodd, 100, 200, 100, 100);

                        MGPlusPathAddEllipse( oldpathzero, 700, 400, 200, 200, FALSE);
                        MGPlusPathAddEllipse( oldpathzero, 800, 400, 200, 200, FALSE);
                        MGPlusPathAddArc (oldpathzero, 700,50,200,200,45,180);
                        MGPlusPathAddLine (oldpathzero, 650, 50, 650, 300);
                        MGPlusPathAddBezier(oldpathzero, 611,11, 688,333, 699,0, 822,111);
                        MGPlusPathAddRectangle( oldpathzero, 650, 100, 100, 100);


                        MGPlusPathAddPath (addpathodd, oldpathodd);
                        MGPlusPathAddPath (addpathodd, oldpathzero);

                        MGPlusDrawPath (graphics, pen, addpathodd);
                        MGPlusDrawPath (graphics, pen, oldpathodd);
                        MGPlusDrawPath (graphics, pen, oldpathzero);
                    }
                    break;

                case ID_RoundRect:
                    {
                        MGPlusPathAddRoundRect (oddpath, 100, 100, 300, 300, 20);
                        MGPlusPathAddRoundRect (oddpath, 125, 125, 250, 250, 20);
                        MGPlusPathAddRoundRect (oddpath, 150, 150, 200, 200, 20);

                        MGPlusPathAddRoundRect (zeropath, 600, 100, 300 ,300, 20);
                        MGPlusPathAddRoundRect (zeropath, 625, 125, 250, 250, 20);
                        MGPlusPathAddRoundRect (zeropath, 650, 150, 200, 200, 20);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                    }
                    break;

                case ID_Arc:
                    {
                        MPPOINT oddpoint [] ={{250,0},{25,375},{475,375},{250,0}};
                        MGPlusPathAddLines (oddpath, oddpoint,4);
                        MGPlusPathAddEllipse (oddpath, 250, 250, 25, 25, TRUE);

                        MPPOINT zeropoint [] ={{850,0},{625,375},{1075,375},{850,0}};
                        MGPlusPathAddLines (zeropath, zeropoint,4);
                        MGPlusPathAddEllipse (zeropath, 850, 850, 25, 25, TRUE);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                        hdc = GetClientDC (hWnd);
                        MGPlusGraphicSave (graphics, hdc, 0, 0, 0, 0, 0, 0);
                        MGPlusPathReset(oddpath);
                        MGPlusPathReset(zeropath);
                        ReleaseDC(hdc);

                        MGPlusSetSolidBrushColor (brush, 0xFF000000);

                        MGPlusPathAddArc (oddpath , 250,250,0,0,0,0);
                        MGPlusPathAddArc (oddpath , 250,250,100,100,0, 60);
                        MGPlusPathAddArc (oddpath , 251,250,0,0,0,0);
                        MGPlusPathAddArc (oddpath , 251,250,100,100,180, -60);
                        MGPlusPathAddArc (oddpath , 251,251,0,0,0,0);
                        MGPlusPathAddArc (oddpath , 251,251,100,100,-60, -60);
                        MGPlusPathAddEllipse (oddpath, 250, 250, 12, 12, TRUE);

                        MGPlusPathAddArc (zeropath , 850,250,0,0,0,0);
                        MGPlusPathAddArc (zeropath , 850,250,100,100,0, 60);
                        MGPlusPathAddArc (zeropath , 851,250,0,0,0,0);
                        MGPlusPathAddArc (zeropath , 851,250,100,100,180, -60);
                        MGPlusPathAddArc (zeropath , 851,251,0,0,0,0);
                        MGPlusPathAddArc (zeropath , 851,251,100,100,-60, -60);
                        MGPlusPathAddEllipse (zeropath, 850, 850, 12, 12, TRUE);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                        MGPlusSetSolidBrushColor (brush, 0xFF00FFFF);
                    }
                    break;

                case ID_Line:
                    {
                        MGPlusPathAddLine (oddpath, 50, 50, 50, 300);
                        MGPlusPathAddLine (oddpath, 300, 300, 300, 50);
                        //MGPlusPathAddLine (oddpath, 300, 50, 300, 300);

                        MGPlusPathAddLine (zeropath, 650, 50, 650, 300);
                        MGPlusPathAddLine (zeropath, 900, 300, 900, 50);
                        //MGPlusPathAddLine (zeropath, 900, 50, 900, 300);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                    }
                    break;

                case ID_Bezier: 
                    {
                        MGPlusPathAddBezier(oddpath, 11,11, 88,333, 99,0, 222,111);
                        MGPlusPathAddBezier(zeropath, 611,11, 688,333, 699,0, 822,111);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                    }
                    break;

                case ID_Rectangle:
                    {
                        MGPlusPathAddRectangle( oddpath, 50, 100, 100, 100);
                        MGPlusPathAddRectangle( oddpath, 75, 150, 100, 100);
                        MGPlusPathAddRectangle( oddpath, 100, 175, 100, 100);

                        MGPlusPathAddRectangle( zeropath, 650, 100, 100, 100);
                        MGPlusPathAddRectangle( zeropath, 675, 150, 100, 100);
                        //MPPOINT zeropoint_rectangle [] = {{675,150},{775,150},{775,250},{675,250}};
                        //MGPlusPathAddLines ( zeropath, zeropoint_rectangle, 4);
                        MGPlusPathAddRectangle( zeropath, 700, 175, 100, 100);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                    }
                    break;

                case ID_Ellipse: 
                    {
                        //odd
                        MGPlusPathAddRectangle( oddpath, 25, 150, 200, 75);
                        MGPlusPathAddEllipse( oddpath, 75, 225, 25, 25, TRUE );
                        MGPlusPathAddEllipse( oddpath,175, 225, 25 , 25, FALSE);

                        MGPlusPathAddRectangle( oddpath, 25, 350, 200, 75);
                        MGPlusPathAddEllipse( oddpath, 75, 425, 25,25, TRUE );
                        MGPlusPathAddEllipse( oddpath,175, 425,25 ,25, FALSE);
                        //zero
                        MGPlusPathAddRectangle( zeropath, 625, 150, 200, 75);
                        MGPlusPathAddEllipse( zeropath, 675, 225,25 ,25, TRUE);
                        MGPlusPathAddEllipse( zeropath,775, 225,25 ,25, FALSE );

                        MGPlusPathAddRectangle( zeropath, 625, 350, 200, 75);
                        MGPlusPathAddEllipse( zeropath, 675, 425,25 ,25, FALSE );
                        MGPlusPathAddEllipse( zeropath,775, 425,25 ,25, TRUE);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                    }
                    break;

                case ID_Curve:
                    {
                        MPPOINT oddpoint_curve [4];

                        oddpoint_curve[0].x = 25.6;
                        oddpoint_curve[0].y = 128.0;
                        oddpoint_curve[1].x = 102.4; 
                        oddpoint_curve[1].y = 230.4;
                        oddpoint_curve[2].x = 153.6;
                        oddpoint_curve[2].y = 25.6;
                        oddpoint_curve[3].x = 230.4;
                        oddpoint_curve[3].y = 128;

                        MGPlusPathAddCurve ( oddpath, oddpoint_curve, 4);

                        MPPOINT zeropoint_curve [4];

                        zeropoint_curve[0].x = 625.6;
                        zeropoint_curve[0].y = 128.0;
                        zeropoint_curve[1].x = 702.4; 
                        zeropoint_curve[1].y = 230.4;
                        zeropoint_curve[2].x = 753.6;
                        zeropoint_curve[2].y = 25.6;
                        zeropoint_curve[3].x = 830.4;
                        zeropoint_curve[3].y = 128;

                        MGPlusPathAddCurve ( zeropath, zeropoint_curve, 4);

                        MPPOINT oddpoint_lines[] = {{25.6,128},{25.6,300},{230.4,300},{230.4,128}};
                        MGPlusPathAddLines ( oddpath, oddpoint_lines, 4);

                        MPPOINT zeropoint_lines[] = {{625.6,128},{625.6,300},{830.4,300},{830.4,128}};
                        MGPlusPathAddLines ( zeropath, zeropoint_lines, 4);

                        MGPlusFillPath (graphics, brush, oddpath);
                        MGPlusFillPath (graphics, brush, zeropath);
                    }
                    break;

                case ID_DrawImageWithPath:
                    {
                        //RECT rect = {50 , 50, 600,600};
                        //MGPlusPathAddBezier(oddpath, 11,11, 188, 933, 99, 0, 222,211);
                        //MGPlusPathAddBezier(zeropath, 611,11, 788,933, 699,0, 822,211);
                        MGPlusPathAddRectangle( oddpath, 0, 0, 1024, 765);

                        MGPlusGraphicLoadBitmap (graphics, 0, &Bitmap);
                        MGPlusDrawImageWithPath (graphics, 0, oddpath);
                        MGPlusDrawImageWithPath (graphics, 0, zeropath);
                    }
                    break;

                case ID_DrawImageWithPoints:
                    {
                        MPPOINT points [4];

                        points [0].x = 100;
                        points [0].y = 50;

                        points [1].x = 400;
                        points [1].y = 50;

                        points [2].x = 500;
                        points [2].y = 400;

                        points [3].x = 0;
                        points [3].y = 400;

                        MGPlusGraphicLoadBitmap (graphics, 1, &Bitmap);
                        MGPlusDrawImageWithPoints (graphics, 1, points, 4);
                    }
                    break;
                default:
                    break;
            }
            InvalidateRect(hWnd, NULL, FALSE);
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

            //MGPlusGraphicSave (graphics, hdc, 0, 0, 0, 0, 0, 0);
    if (MGPlusGraphicSave(graphics, hdc, 0, 0, 0, 0, 0, 0) != MP_OK)
        printf("save graphics ok!\n");

            EndPaint(hWnd, hdc);
            break;
        case MSG_CLOSE:
            MGPlusPenDelete (pen);

            MGPlusBrushDelete (brush);

            MGPlusGraphicDelete (graphics);

            MGPlusPathDelete(oddpath);
            MGPlusPathDelete(zeropath);
            MGPlusPathDelete(addpathodd);
            MGPlusPathDelete(oldpathzero);
            MGPlusPathDelete(oldpathodd);

            UnloadBitmap(&Bitmap);

            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static HMENU Menu_Command (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_AddPath;
    mii.typedata    = (DWORD)"Path Add NewPath";
    hmnu = CreatePopupMenu (&mii);
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    return hmnu;
}

static HMENU Menu_Figure(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_Line;
    mii.typedata    = (DWORD)"Line";
    hmnu = CreatePopupMenu (&mii);
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_Arc;
    mii.typedata    = (DWORD)"Arc";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_Bezier;
    mii.typedata    = (DWORD)"Bezier";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_Rectangle;
    mii.typedata    = (DWORD)"Rectangle";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_Ellipse;
    mii.typedata    = (DWORD)"Ellipse";
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_Curve;
    mii.typedata    = (DWORD)"Curve";
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_DrawImageWithPoints;
    mii.typedata    = (DWORD)"DrawImageWithPoints";
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_DrawImageWithPath;
    mii.typedata    = (DWORD)"DrawImageWithPath";
    InsertMenuItem(hmnu, 8, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_RoundRect;
    mii.typedata    = (DWORD)"RoundRect";
    InsertMenuItem(hmnu, 9, TRUE, &mii);
    return hmnu;
}

static HMENU Menu_PathTest (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    hmnu = CreateMenu();


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"Command";
    mii.hsubmenu    = Menu_Command ();
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"Figure";
    mii.hsubmenu    = Menu_Figure ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);

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

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "path test";
    CreateInfo.hMenu = Menu_PathTest();
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = PathOddProc;
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

