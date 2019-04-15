/*
** incore-data.c: Sample for using incore data to show image and load fonts.
**
** The following APIs of MiniGUI are covered:
**
**  LoadDevFontFromIncoreData
**  CreateLogFontByName
**  DestroyDynamicDevFont
**
**  AddInnerRes
**  LoadBitmapFromRes
**  ReleaseRes
**
** Copyright (C) 2019 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

static int cur_picture, cur_text;
static const char* hello_text[] = {
    "This is a sample using inner(incore) resource data.",
    "这是一个使用内嵌资源数据的示例。",
    "這是一个使用內部資源數據的示例。",
    "これは内部リソースデータを使用したサンプルです。",
    "내부 리소스 데이터를 사용하는 샘플입니다.",
    "Dies ist ein Beispiel, das innere Ressourcendaten verwendet.",
    "Questo è un esempio che utilizza i dati delle risorse interne.",
    "Это образец с использованием данных внутреннего ресурса.",
    "Ceci est un exemple utilisant des données de ressources internes.",
};

#define IDC_TIMER       100
#define NR_PICTURES     13
#define NR_TEXT         TABLESIZE(hello_text)

static void onPaint(HWND hwnd, HDC hdc)
{
    HDC text_dc;
    RECT rc_client;
    char png_file_name [256];
    BITMAP* bmp;

    snprintf(png_file_name, 255, "res/mguxdemo-%d.png", cur_picture + 1);
    bmp = LoadBitmapFromRes(hdc, png_file_name);
    if (bmp == NULL) {
        _ERR_PRINTF("Failed to loaded png picture: %s\n", png_file_name);
        return;
    }

    GetClientRect(hwnd, &rc_client);
    SetBitmapScalerType(hdc, BITMAP_SCALER_BILINEAR);
    FillBoxWithBitmap(hdc, 0, 0, RECTW(rc_client), RECTH(rc_client), bmp);

    // Do not call unloadBitmap for incore bitmap objects
    // Please use ReleaseRes instead.
    // UnloadBitmap(bmp);
    ReleaseRes(Str2Key(png_file_name));

    text_dc = CreateCompatibleDC(hdc);
    SetTextColor(text_dc, PIXEL_lightwhite);
    SetBkMode(text_dc, BM_TRANSPARENT);
    SetBrushColor(text_dc, PIXEL_black);
    FillBox(text_dc, 0, 0, RECTW(rc_client), RECTH(rc_client));

    SetMemDCAlpha(text_dc, MEMDC_FLAG_SRCALPHA, 0x80);

    InflateRect(&rc_client, -10, -5);
    DrawText(text_dc, hello_text[cur_text], -1, &rc_client, DT_CENTER | DT_WORDBREAK);

    BitBlt(text_dc, 0, 0, RECTW(rc_client) + 20, 60, hdc, 0, rc_client.bottom - 60, 0);

    DeleteCompatibleDC(text_dc);
}

static LRESULT MyWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE: {
        SetTimer(hWnd, IDC_TIMER, 300);
        return 0;
    }

    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        onPaint(hWnd, hdc);
        EndPaint(hWnd, hdc);
        return 0;
    }

    case MSG_TIMER: {
        cur_picture++;
        if (cur_picture >= NR_PICTURES) {
            cur_picture %= NR_PICTURES;
        }

        cur_text++;
        if (cur_text >= NR_TEXT) {
            cur_text %= NR_TEXT;
        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }

    case MSG_CLOSE:
        KillTimer(hWnd, IDC_TIMER);
        DestroyMainWindow(hWnd);
        PostQuitMessage(hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

/* defined in unifont_160_50.c file */
const void* get_incore_font_data_upf_unifont_160_50(void);

#define FONTNAME_UNIFONT_160_50     "upf-unifont-rrncnn-16-16-UTF-8"
static DEVFONT* devfont_unifont_160_50;

#include "png_pics.inc"

static INNER_RES incore_pngs [] = {
    { 0, _png__mguxdemo_1_data,  sizeof(_png__mguxdemo_1_data),
            "res/mguxdemo-1.png" },
    { 0, _png__mguxdemo_2_data,  sizeof(_png__mguxdemo_2_data),
            "res/mguxdemo-2.png" },
    { 0, _png__mguxdemo_3_data,  sizeof(_png__mguxdemo_3_data),
            "res/mguxdemo-3.png" },
    { 0, _png__mguxdemo_4_data,  sizeof(_png__mguxdemo_4_data),
            "res/mguxdemo-4.png" },
    { 0, _png__mguxdemo_5_data,  sizeof(_png__mguxdemo_5_data),
            "res/mguxdemo-5.png" },
    { 0, _png__mguxdemo_6_data,  sizeof(_png__mguxdemo_6_data),
            "res/mguxdemo-6.png" },
    { 0, _png__mguxdemo_7_data,  sizeof(_png__mguxdemo_7_data),
            "res/mguxdemo-7.png" },
    { 0, _png__mguxdemo_8_data,  sizeof(_png__mguxdemo_8_data),
            "res/mguxdemo-8.png" },
    { 0, _png__mguxdemo_9_data,  sizeof(_png__mguxdemo_9_data),
            "res/mguxdemo-9.png" },
    { 0, _png__mguxdemo_10_data, sizeof(_png__mguxdemo_10_data),
            "res/mguxdemo-10.png" },
    { 0, _png__mguxdemo_11_data, sizeof(_png__mguxdemo_11_data),
            "res/mguxdemo-11.png" },
    { 0, _png__mguxdemo_12_data, sizeof(_png__mguxdemo_12_data),
            "res/mguxdemo-12.png" },
    { 0, _png__mguxdemo_13_data, sizeof(_png__mguxdemo_13_data),
            "res/mguxdemo-13.png" },
};

static void load_incore_data(void)
{
    int i;

    if ((devfont_unifont_160_50 = LoadDevFontFromIncoreData(
            FONTNAME_UNIFONT_160_50,
            get_incore_font_data_upf_unifont_160_50())) == NULL) {
        _ERR_PRINTF("Failed to load incore UPF font.\n");
        exit (1);
    }

    for (i = 0; i < TABLESIZE(incore_pngs); i++) {
        incore_pngs[i].key = Str2Key(incore_pngs[i].additional);
        incore_pngs[i].additional = "png";
    }

    if (AddInnerRes(incore_pngs, TABLESIZE(incore_pngs), FALSE)
            != RES_RET_OK) {
        _ERR_PRINTF("Failed to add incore PNG files.\n");
        exit (1);
    }
}

static void unload_incore_data(void)
{
    int i;

    for (i = 0; i < TABLESIZE(incore_pngs); i++) {
        ReleaseRes(incore_pngs[i].key);
    }

    DestroyDynamicDevFont(&devfont_unifont_160_50);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;
    LOGFONT* logfont;

    load_incore_data();

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Usage of Incore Data";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = MyWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 240;
    CreateInfo.by = 400;
    CreateInfo.iBkColor = PIXEL_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);
    if (hMainWnd == HWND_INVALID) {
        _ERR_PRINTF("Failed to create main window.\n");
        exit (2);
    }

    logfont = CreateLogFontByName("upf-unifont-rrncnn-16-16-UTF-8");
    if (logfont == NULL) {
        _ERR_PRINTF("Failed to create logfont.\n");
        exit (2);
    }

    SetWindowFont(hMainWnd, logfont);
    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);

    DestroyLogFont(logfont);

    unload_incore_data();

    return 0;
}
