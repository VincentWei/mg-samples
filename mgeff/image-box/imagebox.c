/*! ============================================================================
 * \file imagebox.c 
 * \brief  
 * \author DongKai
 * \version 1.0
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgeff/mgeff.h>

#include "image.h"

#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__)

#define WINDOW_WIDTH  482
#define WINDOW_HEIGHT 382

extern Image *g_image[IMAGE_TOTAL];

static LRESULT ImageBoxWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        {
            mGEffInit ();

            if (0 != ImagesInitialize ()) {
                fprintf (stderr, "Images initialize failed!\n");
                return -1;
            }
        }
        break;
    case MSG_LBUTTONUP:
        {
            int index = 0;
            int x = LOWORD (lParam);
            int y = HIWORD (lParam);

            if (CursorInImage (x, y, &index)) {
                if (g_image[index]->is_expanded) {
                    g_image[index]->is_expanded = FALSE;

                    ShrinkImageWithAnimation (hWnd, index);
                }
                else {
                    g_image[index]->is_expanded = TRUE;

                    ExpandImageWithAnimation (hWnd, index);
                }
            }
        }

        break;

    case MSG_PAINT:
        {
            HDC hdc = BeginPaint (hWnd);

            ImagePaint (hdc);

            EndPaint (hWnd, hdc);

            return 0;
        }

    case MSG_CLOSE:
        {
            mGEffDeinit ();

            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        }

        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char *argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, "mgeff test", 0, 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE;// | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Image box";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ImageBoxWinProc;
    CreateInfo.lx = 10;
    CreateInfo.ty = 10;
    CreateInfo.rx = CreateInfo.lx + WINDOW_WIDTH;
    CreateInfo.by = CreateInfo.ty + WINDOW_HEIGHT;
    CreateInfo.iBkColor = COLOR_blue;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}
