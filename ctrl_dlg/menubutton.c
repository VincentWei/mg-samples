/*
** $Id: menubutton.c 554 2008-03-03 09:09:48Z malin $
**
** menubutton.c: The MenuButton control demo program.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
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
#include <stdlib.h>
#ifndef __NOUNIX__
#include <unistd.h>
#include <dirent.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_MENUBUTTON

static HWND hMainWnd = HWND_INVALID;

#define IDC_MENUBUTTON 100

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

static void create_normal_menubutton (HWND hparent)
{
    HWND menubtn;
    MENUBUTTONITEM mbi;

    DIR*    dir;
    struct  dirent* dir_ent;
    struct  stat my_stat;

    menubtn = CreateWindowEx (CTRL_MENUBTN,
                       Files_in_the_current_directory , 
#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 5, 3)
                        WS_CHILD | WS_VISIBLE | MBS_ALIGNCENTER, 
#else
                        WS_CHILD | WS_VISIBLE, 
#endif
                        WS_EX_NONE,
                        IDC_MENUBUTTON,
                        10, 20, 310, 24, hparent, 0);

    mbi.text = NULL;
    mbi.bmp = NULL;
    mbi.data = 0;

    dir = opendir ("./");
    while ( (dir_ent = readdir ( dir )) != NULL ) {

        /* Assemble full path name. */
        if (stat (dir_ent->d_name, &my_stat) < 0 ){
           continue;
        }

        if (!S_ISDIR (my_stat.st_mode)) {
            mbi.text = dir_ent->d_name;
            SendMessage (menubtn, MBM_ADDITEM, 0, (LPARAM) &mbi);
        }
    }

    closedir (dir);
}

static void create_sorted_menubutton (HWND hparent)
{
    HWND menubtn;
    MENUBUTTONITEM mbi;

    DIR*    dir;
    struct  dirent* dir_ent;
    struct  stat my_stat;

    menubtn = CreateWindowEx (CTRL_MENUBTN,
                        Files_and_directories_in_the_current_directory_sorted, 
#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 5, 3)
                        WS_CHILD | WS_VISIBLE | MBS_SORT | MBS_NOBUTTON | MBS_LEFTARROW, 
#else
                        WS_CHILD | WS_VISIBLE | MBS_SORT,
#endif
                        WS_EX_NONE,
                        IDC_MENUBUTTON + 1,
                        10, 60, 310, 24, hparent, 0);

    mbi.text = NULL;
    mbi.bmp = NULL;
    mbi.data = 0;

    dir = opendir ("./");
    while ( (dir_ent = readdir ( dir )) != NULL ) {

        /* Assemble full path name. */
        if (stat (dir_ent->d_name, &my_stat) < 0 ){
           continue;
        }

        mbi.text = dir_ent->d_name;
        if (S_ISDIR (my_stat.st_mode))
            SendMessage (menubtn, MBM_ADDITEM, 0, (LPARAM) &mbi);
        else
            SendMessage (menubtn, MBM_ADDITEM, -1, (LPARAM) &mbi);
    }

    closedir (dir);
}

static BITMAP bmp_folder;

static void create_bitmap_menubutton (HWND hparent)
{
    HWND menubtn;
    MENUBUTTONITEM mbi;

    DIR*    dir;
    struct  dirent* dir_ent;
    struct  stat my_stat;

    menubtn = CreateWindowEx (CTRL_MENUBTN,
                        Files_and_directories_in_the_current_directory, 
#if _MINIGUI_VERSION_CODE >= _VERSION_CODE (1, 5, 3)
                        WS_CHILD | WS_VISIBLE | MBS_SORT | MBS_LEFTARROW | MBS_ALIGNLEFT, 
#else
                        WS_CHILD | WS_VISIBLE | MBS_SORT,
#endif
                        WS_EX_NONE,
                        IDC_MENUBUTTON + 2,
                        10, 100, 310, 24, hparent, 0);

    mbi.text = NULL;
    mbi.bmp = NULL;
    mbi.data = 0;

    dir = opendir ("./");
    while ( (dir_ent = readdir ( dir )) != NULL ) {

        /* Assemble full path name. */
        if (stat (dir_ent->d_name, &my_stat) < 0 ){
           continue;
        }

        mbi.text = dir_ent->d_name;
        if (S_ISDIR (my_stat.st_mode)) {
            mbi.bmp = &bmp_folder;
            SendMessage (menubtn, MBM_ADDITEM, 0, (LPARAM) &mbi);
        }
        else {
            mbi.bmp = NULL;
            SendMessage (menubtn, MBM_ADDITEM, -1, (LPARAM) &mbi);
        }
    }

    closedir (dir);
}

static LRESULT ControlTestWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
            create_normal_menubutton (hWnd);
            create_sorted_menubutton (hWnd);
            create_bitmap_menubutton (hWnd);

            CreateWindow (CTRL_BUTTON, 
                                Close, 
                                WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 
                                IDCANCEL, 
                                260, 140, 60, 24, hWnd, 0);
            LoadBitmapFromFile (HDC_SCREEN, &bmp_folder, "./res/folder.bmp");
        break;

        case MSG_COMMAND:
        {
            int id = LOWORD(wParam);
            int code = HIWORD(wParam);

            switch (id) {
            case IDC_MENUBUTTON:
            case IDC_MENUBUTTON + 1:
            case IDC_MENUBUTTON + 2:
                if (code == MBN_SELECTED) {
                    //printf ("An item in the control %d is selected.\n", id);
                }
                else if (code == MBN_CHANGED) {
                    //printf ("Selected item in the control %d has changed.\n", id);
                }
                break;
            
            case IDCANCEL:
                PostMessage (hWnd, MSG_CLOSE, 0, 0);
                break;
                
            default:
                break;
            }
        }
        break;
       
        case MSG_DESTROY:
            UnloadBitmap (&bmp_folder);
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
	    return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = MenuButton_controls;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ControlTestWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 340;
    pCreateInfo->by = 200;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void menubutton_demo (HWND hwnd)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hwnd;

    hMainWnd = CreateMainWindow (&CreateInfo);
}

#else

void menubutton_demo (HWND hwnd)
{
    MessageBox (hwnd, "WARNING", "Please enable MENUBUTTON control support in MiniGUI.", MB_OK);
}


#endif /* _MGCTRL_MENUBUTTON */
