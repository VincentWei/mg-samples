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
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "animate.h"

static ANIMATE_SENCE as;

void init_animates()
{
	LoadAnimateFile("test/test.pcc");
}

void draw_animte(HDC hdc, ANIMATE* a)
{
//	printf("HDC=%p, SCREEN(%p); Animate: %d,%d,%d,%d\n",hdc, HDC_SCREEN, GetAnimateX(a), GetAnimateY(a), GetAnimateW(a), GetAnimateH(a));
	gal_pixel old = SetBrushColor(hdc, RGB2Pixel(hdc, 255,0,0));
	FillBox(hdc, GetAnimateX(a), GetAnimateY(a), GetAnimateW(a), GetAnimateH(a));
	SetBrushColor(hdc, old);
}

HWND InitWindow()
{
    MAINWINCREATE CreateInfo;
    CreateInfo.dwStyle = WS_VISIBLE ;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "test";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DefaultMainWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
   	return CreateMainWindow (&CreateInfo);
}

int MiniGUIMain(int argc, const char* argv[])
{
	ANIMATE a;
	TIME_LINE* tl;
	PCODE_METHOD *pm;
	HWND hMainWindow;
	HDC hdc;
	int* args = NULL;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "animate_test" , 0 , 0);
#endif

	if(argc <= 1){
		printf("usage: %s [p-code method name] [ arg1 [arg2 ...]]\n",argv[0]);
		return -1;
	}

	init_animates();

	pm = GetTimeLineMethod(argv[1]);


	if(pm == NULL){
		printf("cannot get method :%s\n", argv[1]);
		return -2;
	}
	
	if(argc > 2)
	{
		int i;
		if(pm->argc > (argc-2)+1){
			fprintf(stderr," method \"%s\" need %d args\n", argv[1], pm->argc-1);
			return -3;
		}

		args = (int*)alloca(sizeof(int)*pm->argc);
		args[0] = (int)&a;
		for(i=1; i< pm->argc; i++)
			args[i] = atoi(argv[i+1]);
	}

	hMainWindow = InitWindow();

	ShowWindow(hMainWindow, SW_SHOWNORMAL);

	hdc = GetClientDC(hMainWindow);

	InitAnimateSence(&as, 200, -1, hdc, draw_animte, NULL, NULL, &g_rcScr, NULL);
	InsertAnimate(&as, &a, FALSE);	
	tl = CreateTimeLine();

	SetAnimateW(&a,40);
	SetAnimateH(&a,40);
	
	StartTimeLine(&as, tl, pm, 1024, args);

	StartAnimateSence(&as);

	return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

