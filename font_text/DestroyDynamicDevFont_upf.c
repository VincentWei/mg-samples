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
** $Id$
**
** The demo of font.
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


#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

static int WinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	const char* devfont_name1 = "upf-unifont-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312-0,GBK,BIG5,UTF-8";
	const char* file_name1 = "/usr/local/lib/minigui/res/font/unifont_160_50.upf";
	const char* devfont_name2 = "upf-times-rrncnn-5-10-ISO8859-1,ISO8859-15";
	const char* file_name2 = "/usr/local/lib/minigui/res/font/smoothtimes_100_50.upf";
	const char* devfont_name3 = "upf-helvetica-rrncnn-5-10-ISO8859-1,ISO8859-15";
	const char* file_name3 = "/usr/local/lib/minigui/res/font/helvetica_100_50.upf";
	const char* devfont_name4 = "upf-micro-rrncnn-4-4-ISO8859-1,ISO8859-15";
	const char* file_name4 = "/usr/local/lib/minigui/res/font/micro_40_50.upf";
	static DEVFONT *devfont1, *devfont2, *devfont3, *devfont4;

	switch (message) {
		case MSG_CREATE:

			return 0;

		case MSG_LBUTTONDOWN:
		        printf ("left mouse down\n");
		        devfont1 = LoadDevFontFromFile (devfont_name1, file_name1);
			printf("Load ok\n");
			if(!devfont1)
			    printf("devfont1 is NULL\n");
			DestroyDynamicDevFont (devfont1);
			printf("Destroy ok\n");
			break;

		case MSG_RBUTTONDOWN:
		        printf ("right mouse down\n");
		        devfont3 = LoadDevFontFromFile (devfont_name3, file_name3);
			DestroyDynamicDevFont (devfont3);
			break;

		case MSG_KEYDOWN:
			printf ("keydown1\n");
		        devfont2 = LoadDevFontFromFile (devfont_name2, file_name2);
			DestroyDynamicDevFont (devfont2);
			break;

		case MSG_KEYUP:
			printf ("keyup1\n");
		        devfont4 = LoadDevFontFromFile (devfont_name4, file_name4);
			DestroyDynamicDevFont (devfont4);
			break;

		case MSG_CLOSE:
			DestroyAllControls (hWnd);
			DestroyMainWindow (hWnd);
			PostQuitMessage (hWnd);
			return 0;
	}

	return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int args, const char* arg[])
{
	MSG Msg;
	HWND hMainWnd;
	MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
	JoinLayer(NAME_DEF_LAYER , "bitblt" , 0 , 0);
	if (InitVectorialFonts() == TRUE)
		printf(" initvectorialfonts ok\n");
	else 
		printf(" initvectorialfonts false\n");

#endif
	CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
	CreateInfo.dwExStyle = WS_EX_NONE;
	CreateInfo.spCaption = "LoadDev-upfFont demo";
	CreateInfo.hMenu = 0;
	CreateInfo.hCursor = GetSystemCursor(0);
	CreateInfo.hIcon = 0;
	CreateInfo.MainWindowProc = WinProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = 900;
	CreateInfo.by = 700;
	CreateInfo.iBkColor = COLOR_lightwhite;
	CreateInfo.dwAddData = 0;
	CreateInfo.hHosting = HWND_DESKTOP;

	hMainWnd = CreateMainWindow (&CreateInfo);

	if (hMainWnd == HWND_INVALID)
		return -1;

	ShowWindow(hMainWnd, SW_SHOWNORMAL);
	//	InitVectorialFonts();

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	MainWindowThreadCleanup (hMainWnd);

	return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

