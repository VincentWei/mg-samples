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
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "common_animates.h"

int MiniGUIMain (int argc, const char* argv[])
{
    RECT rcScr =  {0, 0, 240, 320 };

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

	BITMAP bmpPush;
	BITMAP bmpPull;
	
	LoadBitmapFromFile(HDC_SCREEN, &bmpPush, argv[1]);
	LoadBitmapFromFile(HDC_SCREEN, &bmpPull, argv[2]);

	while(1){
		RunPushPullBitmapAnimate(HDC_SCREEN, &rcScr, &bmpPush, &bmpPull, 5, FALSE);

		getchar();
		RunPushPullBitmapAnimate(HDC_SCREEN, &rcScr, &bmpPush, &bmpPull, 5, TRUE);
		getchar();
	}

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

