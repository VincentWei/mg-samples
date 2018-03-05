#include <stdlib.h>
#include <string.h>

#include <minigui/common.h> 
#include <minigui/minigui.h> 
#include <minigui/gdi.h> 
#include <minigui/window.h> 
#include <minigui/control.h>

#ifdef __MGUTILS_LIB__
#include "../../include/mgutils.h"
#else
#include <mgutils/mgutils.h>
#endif

#ifdef _MGUTILS_DLGSHOWINFO

static LRESULT InfoHookProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
            {
                MoveWindow (hDlg, 30, 50, 320, 240, TRUE);
                return 0;
            }
    }

    return 0;
}

int MiniGUIMain(int argc, const char* argv[])
{
    INFODLGDATA idd;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp (argv[i], "-layer") == 0) {
            layer = argv[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, argv[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    idd.msg =  "1 Informaiton Dialog Sample!\n"
                "2 Informaiton Dialog Sample!\n"
                "3 Informaiton Dialog Sample!\n"
                "4 Informaiton Dialog Sample!\n"
                "5 Informaiton Dialog Sample!\n"
                "6 Informaiton Dialog Sample!\n"
                "7 Informaiton Dialog Sample!\n"
                "8 Informaiton Dialog Sample!\n"
                "9 Informaiton Dialog Sample!\n"
                "10 Informaiton Dialog Sample!\n"
                "11 Informaiton Dialog Sample!\n"
                "12 Informaiton Dialog Sample!\n"
                "13 Informaiton Dialog Sample!\n"
                "14 Informaiton Dialog Sample!\n"
                "15 Informaiton Dialog Sample!\n"
                "16 Informaiton Dialog Sample!\n"
                "17 Informaiton Dialog Sample!";
    idd.hook = InfoHookProc;

    InfoShowDialog (NULL, HWND_DESKTOP, NULL, &idd);
    return 0;
}

#else

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for Show Info Dialog Box in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_DLGSHOWINFO */
