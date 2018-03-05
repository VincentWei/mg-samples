/*
** $Id$
**
** Listing P2C1.2
**
** helloworld_dlgbox.c: Sample program for mGNCS Programming Guide.
**      The mGNCS helloworld using mDialogBox.
**
** Copyright (C) 2009 Feynman Software.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgncs/mgncs.h>

#ifdef _MGNCSCTRL_DIALOGBOX

static void mymain_onPaint (mWidget *_this, HDC hdc, const CLIPRGN* inv)
{
    RECT rt;
    GetClientRect (_this->hwnd, &rt);
    DrawText (hdc, "Hello, world!", -1, &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}

static BOOL mymain_onClose (mWidget* _this, int message)
{
    DestroyMainWindow (_this->hwnd);
    return TRUE;
}

static NCS_EVENT_HANDLER mymain_handlers [] = {
    {MSG_PAINT, mymain_onPaint},
    {MSG_CLOSE, mymain_onClose},
    {0, NULL}
};

// START_OF_MINIGUIMAIN
int MiniGUIMain (int argc, const char* argv[])
{
    ncsInitialize ();

    mDialogBox* mymain = (mDialogBox*) ncsCreateMainWindow (
        NCSCTRL_DIALOGBOX, "Hello, world!",
        WS_CAPTION | WS_BORDER | WS_VISIBLE,
        WS_EX_NONE,
        1,
        0, 0, 300, 200,
        HWND_DESKTOP,
        0, 0,
        NULL,
        NULL,
        mymain_handlers,
        0);

    _c(mymain)->doModal (mymain, TRUE);

    ncsUninitialize ();

    return 0;
}
// END_OF_MINIGUIMAIN

#else //_MGNCSCTRL_DIALOGBOX

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the dialogbox contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-dialogbox ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_DIALOGBOX
