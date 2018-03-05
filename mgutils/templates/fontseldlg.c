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

#ifdef _MGUTILS_DLGFONTSEL

#define DEFAULT_WIDTH       600
#define DEFAULT_HEIGHT      400

#define IDC_BUTTON_FONT1    100
#define IDC_BUTTON_FONT2    101

#define IDC_LABEL_INFO      100

static LRESULT FontHookProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
            {
                /* Set Font Size ComboBox */
                GetDlgItem(hDlg, IDC_FSD_SIZE);
                //SetWindowBkColor(tmp, PIXEL_red);


                /* to do other... */

                return 0;
            }


        case MSG_COLORSELOK:
            {
                // TODO
                // PFILEDLGDATA pfdd = (PFILEDLGDATA) lParam;
                
                return 0;
            }
    }

    return 0;
}

static void button_callback (HWND hwnd, LINT id, int nc, DWORD add_data)
{
    FONTDLGDATA pfdd = {0};
    pfdd.hook = FontHookProc;


    if (nc == BN_CLICKED && id == IDC_BUTTON_FONT1) {
        FontSelectDialog(NULL, hwnd, NULL, &pfdd);
    }
#if 0
    else if (nc == BN_CLICKED && id == IDC_BUTTON_FONT2) {
        FontSelectDialog((PDLGTEMPLATE) &DefSimpleFontDlg, hwnd, NULL, &pfdd);
    }
#endif
}

static LRESULT MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND button;

    switch (message) {
        case MSG_CREATE:
            button = CreateWindow(CTRL_BUTTON, "Select Font 1",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_FONT1,
                    10, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

#if 0
            button = CreateWindow(CTRL_BUTTON, "Select Font 2",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_FONT2,
                    160, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);
#endif
            return 0;

        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_VISIBLE | WS_BORDER;
    pCreateInfo->dwExStyle = 0;
    pCreateInfo->spCaption = "Font Select Dialog";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = 0;
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MainWndProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = pCreateInfo->lx + DEFAULT_WIDTH;
    pCreateInfo->by = pCreateInfo->ty + DEFAULT_HEIGHT;
    pCreateInfo->iBkColor = PIXEL_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain(int argc, const char* argv[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

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

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#else

int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please enable the support for Font Selection Dialog Box in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_DLGFONTSEL */
