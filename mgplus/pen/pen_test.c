#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgutils/mgutils.h>
#include <mgplus/mgplus.h>
#include <minigui/control.h>


#define ID_SelectColor            103
#define ID_SelectWind            105

//#define TEST

static DLGTEMPLATE DlgBoxInputChar =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    0, 0, 400, 230, 
    "WindSelect",
    0, 0,
    4, NULL,
    0
};

#define IDC_CHAR        100

static CTRLDATA CtrlInputChar [] =
{ 
    {
        CTRL_STATIC,
        WS_VISIBLE,
        10, 10, 380, 18, 
        IDC_STATIC, 
        "please input a number",
        0
    },
    {
        CTRL_SLEDIT,
        WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER,
        10, 40, 80, 25,
        IDC_CHAR,
        NULL,
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        170, 160, 60, 25,
        IDOK, 
        "OK",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        270, 160, 60, 25,
        IDCANCEL, 
        "cancel",
        0
    }
};


static char buffer[24] = "16";
static LRESULT SelectWindProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CLOSE:
            EndDialog (hDlg, IDCANCEL);
            break;

        case MSG_COMMAND:
            switch (wParam) {
                case IDOK:
                    SendDlgItemMessage (hDlg, IDC_CHAR, MSG_GETTEXT, sizeof(buffer), (LPARAM)buffer);
                case IDCANCEL:
                    EndDialog (hDlg, wParam);
                    break;
            }
            break;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static void SelectWind (HWND hWnd)
{

    DlgBoxInputChar.controls = CtrlInputChar;
    DialogBoxIndirectParam (&DlgBoxInputChar, HWND_DESKTOP, SelectWindProc, 0L);
}


LRESULT PenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static COLORDLGDATA pcdd = {0};

    switch (message) 
    {
        case MSG_CREATE:
            {
                pcdd.pixel = 0xFF000000;
            }
            break;

        case MSG_COMMAND:
            switch (wParam)
            {
                case ID_SelectColor:
                    ColorSelectDialog(NULL, hWnd, NULL, &pcdd);
                    InvalidateRect(hWnd, NULL, TRUE);
                    printf("fillbox");
                    break;
                case ID_SelectWind:
                    SelectWind ( hWnd);
                    InvalidateRect(hWnd, NULL, TRUE);
                    printf("fillbox");
                    break;
                default:
                    break;
            }
            break;

        case MSG_KEYDOWN:
            {
                int scancode = (int)wParam;
                switch(scancode)
                {

                }
                break;
            }
            break;
        case MSG_PAINT:
            {
                gal_pixel color;
                int lResult = atoi(buffer);
                DWORD r, g, b;
                hdc = BeginPaint (hWnd);
                color = 0xff000000;
                r = g = b = 0x00000000; 
                r = (pcdd.b&0xFF)<<16;
                g = (pcdd.g&0xFF)<<8;
                b = (pcdd.r&0xFF);

                //color = 0xFF000000 |(DWORD)(pcdd.r&0xFF)<<16 | (DWORD)(pcdd.g&0xFF)<<8 | (DWORD)(pcdd.b&0xFF);
                //color = RGB2Pixel(hdc, pcdd.r, pcdd.g, pcdd.b);
                color = color | r | g | b;
                printf ("color %x\n", color);
                HPEN pen = MGPlusPenCreate (lResult,color);
                //HPEN pen = MGPlusPenCreate (lResult, (ARGB*)&pcdd.pixel);
                if (pen == MP_GENERIC_ERROR)
                {
                    printf("create pen fialed\n");
                    return 0;
                }

#ifndef TEST
                HGRAPHICS gpc = MGPlusGraphicCreateFromDC(hdc);
#else
                HGRAPHICS gpc = MGPlusGraphicCreateWithoutCanvas(hdc);
#endif
                if (gpc == MP_GENERIC_ERROR)
                    printf("create gpc failed\n");

                HPATH path= MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                HBRUSH brush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
                MGPlusSetSolidBrushColor (brush, 0xFF00FFFF);

                MGPlusPathAddRectangle( path, 0, 0, 2000,2000 );
                MGPlusFillPath (gpc, brush, path);
                MGPlusDrawEllipse(gpc, pen, 500, 500, 200, 200);
                MGPlusDrawEllipse(gpc, pen, 500, 500, 900, 900);
                MGPlusDrawRectangle(gpc, pen, 300, 300, 400, 400);
                MGPlusDrawLine(gpc, pen, 500, 150, 500, 500);
                MGPlusDrawArc(gpc, pen, 500, 200, 600, 50, 45, 180);
                MGPlusDrawBezier(gpc, pen , 500, 150, 145, 312, 200, 530, 220, 500);
                MGPlusDrawRoundRectEx (gpc, pen, 550, 300, 100, 100.55, 10, 50);

                if (MGPlusGraphicSave (gpc, hdc, 0, 0, 0, 0, 0, 0) == MP_OK)
                    printf ("save graphic ok!\n");

                MGPlusPathDelete(path);
                MGPlusPenDelete (pen);
                MGPlusGraphicDelete (gpc);
                EndPaint (hWnd, hdc);
            }
            break;
        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static HMENU Menu_SelectColor (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"Select";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SelectColor;
    mii.typedata    = (DWORD)"SelectColor";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = ID_SelectWind;
    mii.typedata    = (DWORD)"SelectWind";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    return hmnu;
} 

static HMENU Menu_PenTest (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"Select";
    mii.hsubmenu    = Menu_SelectColor ();
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
} 

int MiniGUIMain(int argc, const char *argv[]) 
{
    MSG Msg;
    HWND hwnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

    CreateInfo.dwStyle = 
        WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "pen test";
    CreateInfo.hMenu = Menu_PenTest();
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = PenProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_cyan;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hwnd = CreateMainWindow (&CreateInfo);

    if (hwnd == HWND_INVALID)
        return -1;

    ShowWindow(hwnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hwnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hwnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

