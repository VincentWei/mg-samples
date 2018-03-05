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

#ifdef _MGUTILS_DLGOPENFILE

#define DEFAULT_WIDTH       600
#define DEFAULT_HEIGHT      400

#define IDC_BUTTON_OPEN1    100
#define IDC_BUTTON_OPEN2    101
#define IDC_BUTTON_SAVE1    102
#define IDC_BUTTON_SAVE2    103

#define IDC_LABEL_INFO      100

static HWND hStaticInfo;

static LRESULT OpenSaveHookProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
            {
                HWND hwnd;

                MoveWindow (hDlg, 130, 50, 320, 240, TRUE);
                hwnd = GetDlgItem(hDlg, IDC_FOSD_PATH_NOTE);
                SetWindowCaption(hwnd, "Find in..."); 

                hwnd = GetDlgItem(hDlg, IDC_FOSD_FILENAME);
                SetWindowBkColor(hwnd, PIXEL_lightgray);
                SetWindowText (hwnd, "<input name here>");

                return 0;
            }

        case MSG_FILESELOK:
            {
                PFILEDLGDATA pfdd = (PFILEDLGDATA) lParam;
                SetWindowText(hStaticInfo, pfdd->filefullname);
                return 0;
            }
    }

    return 0;
}

static void button_callback (HWND hwnd, LINT id, int nc, DWORD add_data)
{
    FILEDLGDATA pfdd = {0};

    pfdd.filterindex = 1;
    strcpy (pfdd.filepath, "./");
    strcpy (pfdd.filter,
            "All file(*.*)|C Source File(*.c)|C Header File(*.h)|Text file(*.txt)|Bitmap file(*.bmp)");
    pfdd.hook = OpenSaveHookProc;

    if (nc == BN_CLICKED && id == IDC_BUTTON_OPEN1) {
        pfdd.is_save = FALSE;
        FileOpenSaveDialog  (NULL, hwnd, NULL, &pfdd);
        return;
    }

    if (nc == BN_CLICKED && id == IDC_BUTTON_OPEN2) {
        pfdd.is_save = FALSE;
        FileOpenSaveDialog  ((PDLGTEMPLATE) &DefSimpleFileDlg, hwnd, NULL, &pfdd);
        return;
    }

    if (nc == BN_CLICKED && id == IDC_BUTTON_SAVE1) {
        pfdd.is_save = TRUE;
        FileOpenSaveDialog  (NULL, hwnd, NULL, &pfdd);
        return;
    }

    if (nc == BN_CLICKED && id == IDC_BUTTON_SAVE2) {
        pfdd.is_save = TRUE;
        FileOpenSaveDialog  ((PDLGTEMPLATE)&DefSimpleFileDlg, hwnd, NULL, &pfdd);
        return;
    }
}

static LRESULT MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND button;

    switch (message) {
        case MSG_CREATE:
            button = CreateWindow(CTRL_BUTTON, "Open Dialog1",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_OPEN1,
                    10, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

            button = CreateWindow(CTRL_BUTTON, "Open Dialog2",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_OPEN2,
                    160, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

            button = CreateWindow(CTRL_BUTTON, "Save Dialog1",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_SAVE1,
                    310, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

            button = CreateWindow(CTRL_BUTTON, "Save Dialog2",
                    WS_CHILD | WS_VISIBLE,
                    IDC_BUTTON_SAVE2,
                    460, 300, 100, 40,
                    hWnd, 0);
            SetNotificationCallback (button, button_callback);

            hStaticInfo = CreateWindow(CTRL_STATIC, "",
                    WS_CHILD | WS_VISIBLE,
                    IDC_STATIC,
                    10, 100, 400, 40,
                    hWnd, 0);
                    
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
    pCreateInfo->spCaption = "File Open Save Dialog";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = 0;
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MainWndProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = pCreateInfo->lx + DEFAULT_WIDTH;
    pCreateInfo->by = pCreateInfo->ty + DEFAULT_HEIGHT;
    pCreateInfo->iBkColor = PIXEL_lightgray; 
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
    fprintf (stderr, "Please enable the support for Open File Dialog Box in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_DLGOPENFILE */
