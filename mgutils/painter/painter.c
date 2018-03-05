/*
 *   This file is part of mGUtils, a component for MiniGUI.
 * 
 *   Copyright (C) 2003~2018, Beijing FMSoft Technologies Co., Ltd.
 *   Copyright (C) 1998~2002, WEI Yongming
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "painter.h"

#ifdef _LANG_ZHCN
#include "painter_res_cn.h"
#elif defined _LANG_ZHTW
#include "painter_res_tw.h"
#else
#include "painter_res_en.h"
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#define DEFAULT_WIDTH   600
#define DEFAULT_HEIGHT  400

static HMENU createpmenuabout (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)PT_ST_ABOUT;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT_THIS;
    mii.typedata    = (DWORD)PT_ST_ABOUTPAINTER;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT;
    mii.typedata    = (DWORD)PT_ST_ABOUTMINIGUI;
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenufile (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)PT_ST_FILE;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_NEW;
    mii.typedata    = (DWORD)PT_ST_NEW;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_OPEN;
    mii.typedata    = (DWORD)PT_ST_OPEN;
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SAVE;
    mii.typedata    = (DWORD)PT_ST_SAVE;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SAVEAS;
    mii.typedata    = (DWORD)PT_ST_SAVEAS;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_CLOSE;
    mii.typedata    = (DWORD)PT_ST_CLOSE;
    InsertMenuItem(hmnu, 4, TRUE, &mii);
    
    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_EXIT;
    mii.typedata    = (DWORD)PT_ST_EXIT;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenuedit (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)PT_ST_EDIT;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_COPY;
    mii.typedata    = (DWORD)PT_ST_COPYSC;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_PASTE;
    mii.typedata    = (DWORD)PT_ST_PASTE;
    InsertMenuItem(hmnu, 1, TRUE, &mii);  
    
    return hmnu;
}

static HMENU createpmenuobject (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)PT_ST_DRAW;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING;
    mii.state       = MF_CHECKED;
    mii.id          = IDM_LINE;
    mii.typedata    = (DWORD)PT_ST_LINE;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_ELLIPSE;
    mii.typedata    = (DWORD)PT_ST_CIRCLE;
    InsertMenuItem(hmnu, 1, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_RECT;
    mii.typedata    = (DWORD)PT_ST_RECT;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_ARC;
    mii.typedata    = (DWORD)PT_ST_CIRCLEARC;
    InsertMenuItem(hmnu, 3, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SPLINE;
    mii.typedata    = (DWORD)PT_ST_SPLINE;
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_FILLED_RECT;
    mii.typedata    = (DWORD)PT_ST_FILLRECT;
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_FILLED_ELLIPSE;
    mii.typedata    = (DWORD)PT_ST_FILLCIRCLE;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_FILLED_POLYGON;
    mii.typedata    = (DWORD)PT_ST_FILLPOLYGON;
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_FLOOD_FILL;
    mii.typedata    = (DWORD)PT_ST_FLOODFILL;
    InsertMenuItem(hmnu, 8, TRUE, &mii);
    
    return hmnu;
}

static HMENU createmenu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)PT_ST_FILE;
    mii.hsubmenu    = createpmenufile ();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)PT_ST_EDIT;
    mii.hsubmenu    = createpmenuedit ();
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)PT_ST_OBJECT;
    mii.hsubmenu    = createpmenuobject ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 130;
    mii.typedata    = (DWORD)PT_ST_ABOUT;
    mii.hsubmenu    = createpmenuabout ();
    InsertMenuItem(hmnu, 3, TRUE, &mii);
                   
    return hmnu;
}

#define MAX_POINTS  40

static int obj_type = IDM_LINE;
static POINT pts [MAX_POINTS];
static int cur_pt = 0;
static int oldx = -1, oldy;

static void DrawObject (HDC hdc)
{
    switch (obj_type) {
    case IDM_LINE:
        if (cur_pt == 2) {
            MoveTo (hdc, pts[0].x, pts[0].y);
            LineTo (hdc, pts[1].x, pts[1].y);
            cur_pt = 0; oldx = -1;
        }
        break;

    case IDM_ELLIPSE:
        if (cur_pt == 2) {
            int rx = ABS (pts[1].x - pts[0].x);
            int ry = ABS (pts[1].y - pts[0].y);

            if (rx == ry)
                Circle (hdc, pts[0].x, pts[0].y, rx);
            else
                Ellipse (hdc, pts[0].x, pts[0].y, rx, ry);
            cur_pt = 0; oldx = -1;
        }
        break;

    case IDM_RECT:
        if (cur_pt == 2) {
            Rectangle (hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y);
            cur_pt = 0; oldx = -1;
        }
        break;

    case IDM_ARC:
        if (cur_pt == 3) {
            int sx = pts [0].x, sy = pts [0].y;
            int dx = pts [1].x - sx, dy = pts [1].y - sy;
            double r = sqrt (dx * dx + dy * dy);
            double cos_d = dx * 1.0 / r;
            double ang1 = acos (cos_d);
            int r2;
            double ang2;
            int a1, a2;

            if (dy > 0) {
                ang1 = -ang1;
            }

            dx = pts [2].x - sx;
            dy = pts [2].y - sy;
            r2 = sqrt (dx * dx * 1.0 + dy * dy * 1.0);
            cos_d = dx * 1.0 / r2;
            ang2 = acos (cos_d);
            if (dy > 0) {
                ang2 = M_PI*2 - ang2;
            }

            a1 = (int)(ang1*180.0*64/M_PI);
            a2 = (int)(ang2*180.0*64/M_PI);
            CircleArc (hdc, sx, sy, r, a1, a2 - a1); 
            cur_pt = 0; oldx = -1;
        }
        else {
            int x = pts [cur_pt - 1].x;
            int y = pts [cur_pt - 1].y;

            MoveTo (hdc, x - 2, y - 2);
            LineTo (hdc, x + 2, y + 2);
            MoveTo (hdc, x - 2, y + 2);
            LineTo (hdc, x + 2, y - 2);
        }
        break;

    case IDM_SPLINE:
        if (cur_pt == 4) {
            SplineTo (hdc, pts);
            cur_pt = 0; oldx = -1;
        }
        else {
            int x = pts [cur_pt - 1].x;
            int y = pts [cur_pt - 1].y;

            MoveTo (hdc, x - 2, y - 2);
            LineTo (hdc, x + 2, y + 2);
            MoveTo (hdc, x - 2, y + 2);
            LineTo (hdc, x + 2, y - 2);
        }
        break;

    case IDM_FILLED_RECT:
        if (cur_pt == 2) {
            int w = ABS (pts[1].x - pts[0].x) + 1;
            int h = ABS (pts[1].y - pts[0].y) + 1;
            FillBox (hdc, MIN (pts[0].x, pts[1].x), 
                          MIN (pts[0].y, pts[1].y), w, h);
            cur_pt = 0; oldx = -1;
        }
        break;

    case IDM_FILLED_ELLIPSE:
        if (cur_pt == 2) {
            int rx = ABS (pts[1].x - pts[0].x);
            int ry = ABS (pts[1].y - pts[0].y);

            if (rx == ry)
                FillCircle (hdc, pts[0].x, pts[0].y, rx);
            else
                FillEllipse (hdc, pts[0].x, pts[0].y, rx, ry);
            cur_pt = 0; oldx = -1;
        }
        break;

    case IDM_FILLED_POLYGON:
        { /* draw vertice */
            int x = pts [cur_pt - 1].x;
            int y = pts [cur_pt - 1].y;

            MoveTo (hdc, x - 2, y - 2);
            LineTo (hdc, x + 2, y + 2);
            MoveTo (hdc, x - 2, y + 2);
            LineTo (hdc, x + 2, y - 2);
        }
        break;

    case IDM_FLOOD_FILL:
        FloodFill (hdc, pts[0].x, pts[0].y);
        cur_pt = 0;
        break;
    }
}

static void FinishObject (HDC hdc)
{
    if (obj_type == IDM_FILLED_POLYGON) {
        if (cur_pt > 2)
            FillPolygon (hdc, pts, cur_pt);
        cur_pt = 0; oldx = -1;
    }
}

static void ObjectOnMouseMove (HDC hdc, int x, int y)
{
    switch (obj_type) {
    case IDM_LINE:
        if (cur_pt > 0) {
            if (oldx >= 0) {
                MoveTo (hdc, pts[0].x, pts[0].y);
                LineTo (hdc, oldx, oldy);
            }
            MoveTo (hdc, pts[0].x, pts[0].y);
            LineTo (hdc, x, y);
            oldx = x; oldy = y;
        }
        break;

    case IDM_ELLIPSE:
    case IDM_FILLED_ELLIPSE:
        if (cur_pt > 0) {
            int rx;
            int ry;

            if (oldx >= 0) {
                rx = ABS (oldx - pts[0].x);
                ry = ABS (oldy - pts[0].y);
                Ellipse (hdc, pts[0].x, pts[0].y, rx, ry);
            }

            rx = ABS (x - pts[0].x);
            ry = ABS (y - pts[0].y);
            Ellipse (hdc, pts[0].x, pts[0].y, rx, ry);
            oldx = x; oldy = y;
        }
        break;

    case IDM_RECT:
    case IDM_FILLED_RECT:
        if (cur_pt > 0) {
            if (oldx >= 0) {
                Rectangle (hdc, pts[0].x, pts[0].y, oldx, oldy);
            }

            Rectangle (hdc, pts[0].x, pts[0].y, x, y);
            oldx = x; oldy = y;
        }
        break;

    case IDM_ARC:
        break;

    case IDM_SPLINE:
        break;

    case IDM_FILLED_POLYGON:
        break;

    case IDM_FLOOD_FILL:
        break;
    }
}

static LRESULT PainterWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message) {
        case MSG_CREATE:
            srand (time (NULL));
            break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            TabbedTextOut (hdc, 0, 0, 
                                 PT_ST_STARTPROMPT);
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_ACTIVEMENU:
            if (wParam == 2) {
                CheckMenuRadioItem ((HMENU)lParam, 
                    IDM_LINE, IDM_FLOOD_FILL, 
                    obj_type, MF_BYCOMMAND);
            }
        break;
        
        case MSG_LBUTTONDOWN:
            if (cur_pt >= MAX_POINTS)
                break;

            pts [cur_pt].x = LOWORD (lParam);
            pts [cur_pt].y = HIWORD (lParam);
            cur_pt ++;
            hdc = GetClientDC (hWnd);
            SetPenColor (hdc, 
                RGB2Pixel (hdc, rand()%256, rand()%256, rand()%256));
            SetBrushColor (hdc, 
                RGB2Pixel (hdc, rand()%256, rand()%256, rand()%256));
            DrawObject (hdc);
            ReleaseDC (hdc);
        break;

        case MSG_RBUTTONDOWN:
            hdc = GetClientDC (hWnd);
            srand (time (NULL));
            SetPenColor (hdc, 
                RGB2Pixel (hdc, rand()%256, rand()%256, rand()%256));
            SetBrushColor (hdc, 
                RGB2Pixel (hdc, rand()%256, rand()%256, rand()%256));
            FinishObject (hdc);
            ReleaseDC (hdc);
        break;

        case MSG_MOUSEMOVE:
            hdc = GetClientDC (hWnd);
            SetPenColor (hdc, PIXEL_lightwhite);
            SetBrushColor (hdc, PIXEL_lightwhite);
            SetRasterOperation (hdc, ROP_XOR);
            ObjectOnMouseMove (hdc, LOWORD(lParam), HIWORD(lParam));
            ReleaseDC (hdc);
        break;

        case MSG_COMMAND:
        switch (wParam) 
        {
            case IDM_NEW:
            case IDM_OPEN:
            case IDM_SAVE:
            case IDM_SAVEAS:
            case IDM_CLOSE:
            break;

            case IDM_COPY:
            break;

            case IDM_PASTE:
            break;
            
            case IDM_EXIT:
                SendMessage (hWnd, MSG_CLOSE, 0, 0L);
            break;

            case IDM_LINE ... IDM_FLOOD_FILL:
                cur_pt = 0;
                obj_type = wParam;
                oldx = -1;
            break;
            
            case IDM_ABOUT_THIS:
                MessageBox (hWnd, 
                    PT_ST_ABOUTTHIS, PT_ST_ABOUTPAINTER,
                    MB_OK | MB_ICONINFORMATION | MB_BASEDONPARENT);
            break;
            
            case IDM_ABOUT:
#ifdef _MGMISC_ABOUTDLG
#ifndef _MGRM_THREADS
                OpenAboutDialog (hWnd);
#else
                OpenAboutDialog ();
#endif
#endif
            break;
        }
        break;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_VISIBLE | WS_BORDER;
    pCreateInfo->dwExStyle = 0;
    pCreateInfo->spCaption = PT_ST_CAPTION;
    pCreateInfo->hMenu = createmenu();
    pCreateInfo->hCursor = GetSystemCursor (IDC_PENCIL);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = PainterWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = pCreateInfo->lx + DEFAULT_WIDTH;
    pCreateInfo->by = pCreateInfo->ty + DEFAULT_HEIGHT;
    pCreateInfo->iBkColor = PIXEL_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

