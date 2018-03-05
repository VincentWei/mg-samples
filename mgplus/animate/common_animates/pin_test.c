#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "common_animates.h"

int MiniGUIMain (int argc, const char* argv[])
{

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

	BITMAP bmpbk;
	BITMAP bmpPin;
	
	LoadBitmapFromFile(HDC_SCREEN, &bmpbk, argv[1]);
	LoadBitmapFromFile(HDC_SCREEN, &bmpPin, argv[2]);

	while(1){
		RunPinAnimate(HDC_SCREEN, &bmpbk, &bmpPin, 0, 0, 0, 0, 100, 0);

		getchar();
		RunPinAnimate(HDC_SCREEN, &bmpbk, &bmpPin, 0, 0, 100, 0, 0, 0);
		getchar();
	}

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif


