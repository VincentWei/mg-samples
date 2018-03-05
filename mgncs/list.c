/**
 * $Id: list.c 1683 2017-10-26 06:52:09Z weiym $
 *
 * Listing P2C18.1
 *
 * list.c: Sample program for mGNCS Programming Guide
 *      The demo application for List.
 *
 * Copyright (C) 2010 Feynman Software.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// START_OF_INCS
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
// END_OF_INCS

#ifdef _MGNCSCTRL_LIST

#define IDL_DIR     100

static char caption[] = "address";
// START_OF_LISTDATA
static BITMAP demoBmp;

static NCS_MENU_DATA child_data_1[] = {
    {0, NULL, 0, "phone: 15200000000", NULL, -1, 0, NULL},
    {0, NULL, 0, "location: Beijing HaiDian", NULL, -1, 0, NULL},
    {0, NULL, 0, "e-mail: support@minigui.org", NULL, -1, 0, NULL},
};

static NCS_MENU_DATA child_data_2[] = {
    {0, NULL, 0, "home:  010-82000000", NULL, -1, 0, NULL},
    {0, NULL, 0, "office: 010-82650000", NULL, -1, 0, NULL},
    {0, NULL, 0, "phone: 13400000000", NULL, -1, 0, NULL},
};
//nr_child, child, style, text, image, height, addData, parent
static NCS_MENU_DATA data[] = {
    {3, child_data_2, NCSS_NODE_LTEXTBS, "Zeus", &demoBmp, -1, 0, NULL},
    {3, child_data_2, NCSS_NODE_LTEXTBS, "Pan", &demoBmp, -1, 0, NULL},
    {3, child_data_2, NCSS_NODE_LTEXTBS, "Apollo", &demoBmp, -1, 0, NULL},
    {3, child_data_2, NCSS_NODE_LTEXTBS, "Heracles", &demoBmp, -1, 0, NULL},
    {3, child_data_1, NCSS_NODE_LTEXTBS, "Achilles", &demoBmp, -1, 0, NULL},
    {3, child_data_1, NCSS_NODE_LTEXTBS, "Jason", &demoBmp, -1, 0, NULL},
    {3, child_data_1, NCSS_NODE_LTEXTBS, "Theseus", &demoBmp, -1, 0, NULL},
};
// END_OF_LISTDATA

static void list_init (mDialogBox *dialog, PBITMAP bmp)
{
    mList *dirObj = (mList*)ncsGetChildObj(dialog->hwnd, IDL_DIR);

    if (!dirObj)
        return;

// START_OF_ADDNODES
    _c(dirObj)->freeze(dirObj, TRUE);

    _c(dirObj)->setProperty(dirObj, NCSP_LIST_LAYOUT, NCS_LIST_LICONV);
    _c(dirObj)->setNodeDefSize(dirObj, 52, 52);

    ncsSetMenuData((mObject*)dirObj, data, sizeof(data)/sizeof(NCS_MENU_DATA));
    _c(dirObj)->setCurSel(dirObj, 0);

    _c(dirObj)->freeze(dirObj, FALSE);
// END_OF_ADDNODES
}

static NCS_PROP_ENTRY static_props [] = {
	{NCSP_STATIC_ALIGN, NCS_ALIGN_LEFT},
	{0, 0}
};

static void list_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    mStatic *staticObj = (mStatic*)ncsGetChildObj(GetParent(self->hwnd), IDC_STATIC);
    mList *lstObj = (mList *)self;

    if (nc == NCSN_LIST_ENTERSUBLIST) {
// START_OF_CHANGE_LAYOUT
        mNode *selNode = (mNode*)add_data;
        _c(lstObj)->setProperty(lstObj, NCSP_LIST_LAYOUT, NCS_LIST_LLIST);
// END_OF_CHANGE_LAYOUT

        if (selNode && staticObj) {
            SetWindowText(staticObj->hwnd, _c(selNode)->getText(selNode));
        }
    }
    else if (nc == NCSN_LIST_BACKUPLIST) {
        _c(lstObj)->setProperty(lstObj, NCSP_LIST_LAYOUT, NCS_LIST_LICONV);
        _c(lstObj)->setNodeDefSize(lstObj, 52, 52);

        SetWindowText(staticObj->hwnd, caption);
    }
}

static NCS_EVENT_HANDLER list_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_LIST_ENTERSUBLIST, list_notify),
    NCS_MAP_NOTIFY(NCSN_LIST_BACKUPLIST, list_notify),
	{0, NULL}
};

static NCS_WND_TEMPLATE _ctrl_tmpl[] = {
	{
		NCSCTRL_STATIC,
		IDC_STATIC,
		0, 0, 160, 15,
		WS_VISIBLE,
		WS_EX_NONE,
		caption,
		static_props,
		NULL,
		NULL,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_LIST,
		IDL_DIR,
		0, 20, 166, 188,
		WS_BORDER | WS_TABSTOP | WS_VISIBLE
            | NCSS_NOTIFY | NCSS_LIST_LOOP | NCSS_ASTLST_AUTOSORT,
		WS_EX_NONE,
		"",
		NULL,
		NULL,
		list_handlers,
		NULL,
		0,
		0
	}
};

static NCS_MNWND_TEMPLATE mainwnd_tmpl = {
	NCSCTRL_DIALOGBOX,
	7,
	100, 100, 176, 250,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "List Demo",
	NULL,
	NULL,
	NULL,
	_ctrl_tmpl,
	sizeof(_ctrl_tmpl)/sizeof(NCS_WND_TEMPLATE),
	0,
	0,
    0,
};

int MiniGUIMain(int argc, const char* argv[])
{
	ncsInitialize();

	mDialogBox* dialog =
        (mDialogBox *)ncsCreateMainWindowIndirect (&mainwnd_tmpl, HWND_DESKTOP);

    LoadBitmap(HDC_SCREEN, &demoBmp, "./res/listfolder.png");
    list_init (dialog, &demoBmp);

	_c(dialog)->doModal(dialog, TRUE);

    UnloadBitmap(&demoBmp);
	ncsUninitialize();

	return 0;
}
#else //_MGNCSCTRL_LIST

int main (void)
{
	printf("\n==========================================================\n");
	printf("======== You haven't enable the list contorl =====\n");
	printf("==========================================================\n");
	printf("============== ./configure --enable-list ==========\n");
	printf("==========================================================\n\n");
	return 0;
}
#endif	//_MGNCSCTRL_LIST
