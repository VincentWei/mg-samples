#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

static int TestDemoProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

HWND CreateTestDemoProc(int color)
{
    MAINWINCREATE CreateInfo;
    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "test";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc =TestDemoProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = color;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    HWND hWnd = CreateMainWindow (&CreateInfo);
    
    if (hWnd == HWND_INVALID)
        return -1;

    ShowWindow(hWnd, SW_SHOWNORMAL);
	
	return hWnd;
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
	HWND hwndPush, hwndPull;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

	hwndPull = CreateTestDemoProc(COLOR_black);
	hwndPush = CreateTestDemoProc(COLOR_lightwhite);

	getchar();

	while(1){
		RunPushPullWindowAnimate(&g_rcScr, hwndPush, hwndPull, 5, FALSE);
		getchar();
		RunPushPullWindowAnimate(&g_rcScr, hwndPush, hwndPull, 5, TRUE);
		getchar();
	}


    while (GetMessage(&Msg, HWND_DESKTOP)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (HWND_DESKTOP);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

