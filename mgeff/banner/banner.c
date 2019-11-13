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
/*
 *   This file is part of mGEff, a component for MiniGUI.
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
#define dbg() printf("F:%s L:%d\n", __FUNCTION__, __LINE__)

#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <assert.h>
#include <string.h>

#include <mgeff/mgeff.h>

#define BLK_SIZE 5

typedef struct {
    int x;
    int y;
    int color;
} star_t;

typedef struct {
    int n;
    star_t *star;
} letter_t;

static letter_t *s_letters;
static int s_nrLetters;
static star_t *s_allstar;
static int s_nrStars;
static HWND s_hwnd;

static int star_cmp(star_t *s1, star_t *s2) {
    if (s1->y != s2->y) {
        return (-s1->y) - (-s2->y);
    }
    else{
        return s1->x - s2->x;
    }
}

static void gen_letters(void) {
    int line;
    int i;
    extern const char *g_lines[];

    // count letters
    for (line=0; g_lines[line]; line++) {
        if (strcmp(g_lines[line], "----") == 0) {
            s_nrLetters ++;
        }
    }
    s_letters = (letter_t *)calloc(s_nrLetters, sizeof(letter_t));

    // count stars
    {
        int index=0;
        for (line=0; g_lines[line]; line++) {
            int i;
            if (strcmp(g_lines[line], "----") == 0) {
                index++;
                continue;
            }
            for (i=0; g_lines[line][i]; i++) {
                if (g_lines[line][i] == '#'){
                    s_letters[index].n++;
                }
            }
        }
    }

    // record star
    {
        int index=0;
        int nStar;
        // malloc
        for (index=0; index<s_nrLetters; index++) {
            s_letters[index].star = (star_t *)calloc(s_letters[index].n, sizeof(*s_letters[index].star));
            s_nrStars += s_letters[index].n;
        }
        s_allstar = (star_t *)calloc(s_nrStars, sizeof(*s_allstar));

        index = 0;
        nStar = 0;
        for (line=0; g_lines[line]; line++) {
            if (strcmp(g_lines[line], "----") == 0) {
                index++;
                nStar=0;
                continue;
            }
            for (i=0; g_lines[line][i]; i++) {
                if (g_lines[line][i] == '#'){
                    assert(nStar < s_letters[index].n);
                    s_letters[index].star[nStar].x =
                        120 + line * BLK_SIZE;
                    s_letters[index].star[nStar].y =
                        550 - i*BLK_SIZE;
                    s_letters[index].star[nStar].color = MakeRGBA(
                            0xf0, 0xf0, 0xf0, 0xff);
                            // 0xe8, 0xd1, 0x06, 0xff);
                            // 0x40+(rand() & 0x7f), 0x40+(rand() & 0x7f), 0x40+(rand() & 0x7f), 0xff);
                    nStar++;
                }
            }
        }
    }

    // sort
    for (i=0; i<s_nrLetters; i++) {
        qsort(s_letters[i].star, s_letters[i].n, sizeof(s_letters[i].star[0]), (void *)star_cmp);
    }

#if 0
    // print
    for (i=0; i<s_nrLetters; i++) {
        int j;
        for (j=0; j<s_letters[i].n; j++) {
            printf("%d, %d\n",
                    s_letters[i].star[j].x, s_letters[i].star[j].y);
        }
    }
#endif
}

static void paint_all_star(HDC win_dc) {
    HDC memdc;
    HDC dc;
    RECT rc;
    int i;
    GetClientRect(s_hwnd, &rc);
    memdc = CreateCompatibleDC(win_dc);
    dc = memdc;

    SetBrushColor(dc, RGB2Pixel(dc, 0x01, 0x1d, 0x2e));
    FillBox(dc, rc.left, rc.top, RECTW(rc), RECTH(rc));

    for (i=0; i<s_nrStars; i++) {
        star_t *star = &s_allstar[i];
        int color;
        if (star->x >= rc.left && star->x < rc.right
                && star->y >= rc.top && star->y < rc.bottom) {
            color = star->color;

            SetBrushColor(dc, RGB2Pixel(dc,
                        GetRValue(color),
                        GetGValue(color),
                        GetBValue(color)));
            FillBox(dc, star->x, star->y, BLK_SIZE-1, BLK_SIZE-1);
        }
    }

    BitBlt(memdc, 0, 0, 0, 0, win_dc, 0, 0, -1);

    DeleteMemDC(memdc);
}

static void moveStar(MGEFF_ANIMATION animation, star_t *star, int id, POINT *point)
{
    int real_id = id;

    if (real_id < 0) {
        real_id = -real_id;
    }

    s_allstar[real_id].x = point->x;
    s_allstar[real_id].y = point->y;

    if (id < 0) {
        HDC win_dc = GetClientDC(s_hwnd);
        paint_all_star(win_dc);
        ReleaseDC(win_dc);
    }
}

static void setup_animations(void)
{
    MGEFF_ANIMATION sequence_group;
    int i;
    int id=0;
    int w, h;
    RECT win_rc;

    GetClientRect(s_hwnd, &win_rc);

    w = RECTW(win_rc);
    h = RECTH(win_rc);

    sequence_group = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL);

    for (i=0; i<s_nrLetters; i++) {
        int j;
        letter_t *letter = &s_letters[i];
        MGEFF_ANIMATION parallel_group;

        parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);

        for (j=0; j<letter->n; j++) {
            MGEFF_ANIMATION animation;
            POINT point;
            animation = mGEffAnimationCreate(&letter->star[j], (void *)moveStar,
                    j<letter->n-1 ? id : -id, MGEFF_POINT);
            // duration
            switch (i) {
                case 0: // m
                    mGEffAnimationSetDuration(animation, 600);
                    break;
                case 1: // G
                case 2: // E
                case 3: // f
                case 5: // _
                case 6: // 0
                    mGEffAnimationSetDuration(animation, 1000-i*50);
                    break;
                case 4: // f
                    mGEffAnimationSetDuration(animation, 600);
                    break;
                case 7: // .
                    mGEffAnimationSetDuration(animation, 1000 + j*60);
                    break;
                case 8: // 4
                default:
                    mGEffAnimationSetDuration(animation, 500 + j*6);
                    break;
            }

            // start value
            switch (i) {
                case 0: // m
                    point.x = win_rc.left = 5;
                    point.y = letter->star[j].y;
                    break;
                case 1: // G
                case 2: // E
                case 3: // f
                case 5: // _
                case 6: // 0
                case 7: // .
                    point.x = letter->star[j].x;
                    point.y = letter->star[j].y - 600;
                    break;
                case 4: // f
                    point.x = letter->star[j].x;
                    point.y = letter->star[j].y + 350;
                    break;
                case 8: // 4
                default:
                    {
                        int x, y;
                        x = (rand() % (2*w)) - w;
                        y = (rand() % (2*h)) - h;
                        if (x > 0 && y > 0) {
                            point.x = win_rc.right;
                            point.y = y;
                        }else if (x > 0 && y < 0) {
                            point.x = x;
                            point.y = win_rc.top - BLK_SIZE;
                        }else if (x < 0 && y > 0) {
                            point.x = win_rc.left - BLK_SIZE - 1;
                            point.y = y;
                        }else{
                            point.x = -x;
                            point.y = win_rc.bottom;
                        }
                    }
                    break;
            }

            // curve
            switch (i) {
                case 0: // m
                    mGEffAnimationSetCurve(animation, InCubic);
                    break;
                case 1: // G
                    mGEffAnimationSetCurve(animation, OutBounce);
                    break;
                case 6: // 0
                    mGEffAnimationSetCurve(animation, OutInBounce);
                    break;
                case 5: // _
                    mGEffAnimationSetCurve(animation, InQuad);
                    break;
                case 2: // E
                    mGEffAnimationSetCurve(animation, OutQuad);
                    break;
                case 3: // f
                    mGEffAnimationSetCurve(animation, InBounce);
                    break;
                case 7: // .
                    mGEffAnimationSetCurve(animation, InBounce);
                    break;
                case 4: // f
                    mGEffAnimationSetCurve(animation, OutCubic);
                    break;
                case 8: // 4
                default:
                    mGEffAnimationSetCurve(animation, OutBounce);
                    break;
            }

            // color
            if (i < s_nrLetters-1) {
                s_allstar[id].color = letter->star[j].color;
            }else{
                s_allstar[id].color = MakeRGBA(
                        0xe8, 0xd1, 0x06, 0xff);
            }
            
            s_allstar[id].x = point.x;
            s_allstar[id].y = point.y;
            mGEffAnimationSetStartValue(animation, &point);
            point.x = letter->star[j].x;
            point.y = letter->star[j].y;
            mGEffAnimationSetEndValue(animation, &point);
            mGEffAnimationAddToGroup(parallel_group, animation);

            id ++;
        }
        mGEffAnimationAddToGroup(sequence_group, parallel_group);
    }
    // mGEffAnimationSetProperty(sequence_group, MGEFF_PROP_LOOPCOUNT, 999);
    // mGEffAnimationSetProperty(sequence_group, MGEFF_PROP_KEEPALIVE, 0);
    mGEffAnimationSyncRun(sequence_group);
    // mGEffAnimationWait ((void *)&s_hwnd, sequence_group);
    mGEffAnimationDelete (sequence_group);
}

static LRESULT mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
            {
                srand(time(NULL));
                s_hwnd = hwnd;

                gen_letters();

                mGEffInit();

                setup_animations();
            }
            break;
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint(hwnd);
                paint_all_star(hdc);
                EndPaint(hwnd, hdc);
            }
            break;
        case MSG_ERASEBKGND:
            {
                HDC dc;
                RECT rcBg;
                GetClientRect(hwnd, &rcBg);
                dc = GetClientDC(hwnd);
                SetBrushColor(dc, RGB2Pixel(dc, 0x2e, 0x1d, 0x01));
                FillBox(dc, rcBg.left, rcBg.top, RECTW(rcBg), RECTH(rcBg));
                ReleaseDC(dc);
            }
            return 0;
        case MSG_CLOSE:
            mGEffDeinit();
            DestroyMainWindow (hwnd);
            PostQuitMessage (hwnd);
            break;
        default:
            break;
    }
    return DefaultMainWinProc(hwnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG msg;
    HWND hMainHwnd;
    MAINWINCREATE createInfo;

    dbg();
#ifdef _MGRM_PROCESSES   
    JoinLayer(NAME_DEF_LAYER, "banner", 0, 0);
#endif

    createInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    createInfo.dwExStyle = WS_EX_NONE;
    createInfo.spCaption = "banner";
    createInfo.hMenu = 0;
    createInfo.hCursor = GetSystemCursor(0);
    createInfo.hIcon = 0;
    createInfo.MainWindowProc = mainWindowProc;
    createInfo.lx = 0;  
    createInfo.ty = 0;  
    createInfo.rx = g_rcScr.right; 
    createInfo.by = g_rcScr.bottom; 
    createInfo.iBkColor = MakeRGBA(0x2e, 0x1d, 0x01, 0xff);
    createInfo.dwAddData = 0;
    createInfo.hHosting = HWND_DESKTOP;
    hMainHwnd = CreateMainWindow(&createInfo);
    if (hMainHwnd == HWND_INVALID)
    {
        return -1;
    }

    ShowWindow(hMainHwnd, SW_SHOWNORMAL);

    while (GetMessage(&msg, hMainHwnd))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    MainWindowThreadCleanup(hMainHwnd);

    return 0;
}
