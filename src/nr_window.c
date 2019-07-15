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
** $Id: nr_window.c 444 2008-08-25 03:11:06Z gelei $
**
** Listing 9.2
**
** nr_window.c: Sample program for MiniGUI Programming Guide
**      Demo of non-rectangle window
**
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

static LRESULT NonRegularWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
        case MSG_CREATE:
            return 0;

        case MSG_CLOSE:
            {
                DestroyMainWindow (hWnd);
                PostQuitMessage (hWnd);
                return 0;
            }
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    int i = 0;
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    MYBITMAP mybmp;
    RGB *pal;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "non regular" , 0 , 0);
#endif

    pal = (RGB *)calloc (1,256 * sizeof (RGB));
    LoadMyBitmap (&mybmp, pal, "res/map.bmp");

    for(i = 0; i < 256; ++i)
    {
        if(pal[i].r == 0xFC && pal[i].g == 0xFF &&  pal[i].b == 0xFB)
        {
            mybmp.transparent = i;
            break;
        }
    }

    CreateInfo.dwStyle = WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE ;
    CreateInfo.spCaption = "non-regular window ...";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(1);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = NonRegularWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = mybmp.w;
    CreateInfo.by = mybmp.h;
    CreateInfo.iBkColor = PIXEL_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (!SetWindowMask(hMainWnd, &mybmp))
    {
        printf ("SetWindowMask error \n ");
        free (pal);
        return -1;
    }

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);
    UnloadMyBitmap(&mybmp);
    free(pal);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

