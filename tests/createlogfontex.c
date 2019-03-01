/*
** createlogfontex.c:
**
**  Test code for CreateLogFontEx of MiniGUI 3.4.0.
**  The following APIs are covered:
**
**      LoadDevFontFromFile
**      CreateBMPDevFont
**      CreateLogFontEx
**      DestroyLogFont
**      DestroyBMPFont
**      DestroyDynamicDevFont
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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0))

#include "helpers.h"

static void output_test_case(HDC hdc)
{
}

static LRESULT MyMainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        break;

    case MSG_PAINT: {
        HDC hdc;
        hdc = BeginPaint(hWnd);
        output_test_case(hdc);
        EndPaint(hWnd, hdc);
        return 0;
    }

    case MSG_KEYDOWN: {
        switch (wParam) {
        case SCANCODE_SPACE:
            break;

        case SCANCODE_F1:
            break;

        case SCANCODE_F2:
            break;

        case SCANCODE_F3:
            break;

        case SCANCODE_F4:
            break;

        case SCANCODE_F5:
            break;

        case SCANCODE_F6:
            break;

        case SCANCODE_F7:
            break;

        case SCANCODE_F8:
            break;

        case SCANCODE_F9:
            break;

        case SCANCODE_F10:
            break;

        case SCANCODE_F11:
            break;

        case SCANCODE_F12:
            break;

        case SCANCODE_CURSORBLOCKLEFT:
            break;

        case SCANCODE_CURSORBLOCKRIGHT:
            break;

        case SCANCODE_CURSORBLOCKUP:
            break;

        case SCANCODE_CURSORBLOCKDOWN:
            break;

        case SCANCODE_INSERT:
            break;

        case SCANCODE_REMOVE:
            break;

        case SCANCODE_HOME:
            break;

        case SCANCODE_END:
            break;

        case SCANCODE_PAGEUP:
            break;

        case SCANCODE_PAGEDOWN:
            break;

        default:
            return 0;
        }

        InvalidateRect (hWnd, NULL, TRUE);
        return 0;
    }

    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

#define  FONTFILE_PATH   "/usr/local/share/minigui/res/"

typedef struct _DEVFONTINFO {
    const char* filename;
    const char* fontname;
    DEVFONT*    devfont;
} DEVFONTINFO;

static DEVFONTINFO _devfontinfo[] = {
    { FONTFILE_PATH "font/6x12-iso8859-1.bin",
        "rbf-fixed,sans serif,monospace-rrncnn-6-12-ISO8859-1" },
    { FONTFILE_PATH "font/8x16-iso8859-1.bin",
        "rbf-fixed,sans serif,monospace-rrncnn-8-16-ISO8859-1" },
    { FONTFILE_PATH "font/song-12-gb2312.bin",
        "rbf-song,宋体,sans serif,monospace-rrncnn-12-12-GB2312-0" },
    { FONTFILE_PATH "font/song-16-gb2312.bin",
        "rbf-song,宋体,sans serif,monospace-rrncnn-16-16-GB2312-0" },
    { FONTFILE_PATH "font/song-24-gb2312.bin",
        "rbf-song,宋体,sans serif,monospace-rrncnn-24-24-GB2312-0" },
    { FONTFILE_PATH "font/fmfsong-12-gb2312.bin",
        "rbf-fmfsong,fangsong,仿宋,serif,monospace-rrncnn-12-12-GB2312-0" },
    { FONTFILE_PATH "font/fmfsong-16-gb2312.bin",
        "rbf-fmfsong,fangsong,仿宋,serif,monospace-rrncnn-16-16-GB2312-0" },
    { FONTFILE_PATH "font/fmfsong-24-gb2312.bin",
        "rbf-fmfsong,fangsong,仿宋,serif,monospace-rrncnn-24-24-GB2312-0" },
    { FONTFILE_PATH "font/fmhei-16-gb2312.bin",
        "rbf-fmhei,hei,黑体,sans serif,monospace-rrncnn-16-16-GB2312-0" },
    { FONTFILE_PATH "font/fmhei-20-gb2312.bin",
        "rbf-fmhei,hei,黑体,sans serif,monospace-rrncnn-20-20-GB2312-0" },
    { FONTFILE_PATH "font/fmhei-24-gb2312.bin",
        "rbf-fmhei,hei,黑体,sans serif,monospace-rrncnn-24-24-GB2312-0" },
    { FONTFILE_PATH "font/ming-12-big5.bin",
        "rbf-ming,明体,sans serif,monospace-rrncnn-12-12-BIG5" },
    { FONTFILE_PATH "font/ming-16-big5.bin",
        "rbf-ming,明体,sans serif,monospace-rrncnn-16-16-BIG5" },
    { FONTFILE_PATH "font/ming-24-big5.bin",
        "rbf-ming,明体,sans serif,monospace-rrncnn-24-24-BIG5" },
    { FONTFILE_PATH "font/Courier-rr-8-13.vbf",
        "vbf-Courier,sans serif-rrncnn-8-13-ISO8859-1" },
    { FONTFILE_PATH "font/Courier-rr-10-15.vbf",
        "vbf-Courier,sans serif-rrncnn-10-15-ISO8859-1" },
    { FONTFILE_PATH "font/Helvetica-rr-11-12.vbf",
        "vbf-Helvetica,sans serif-rrncnn-11-12-ISO8859-1" },
    { FONTFILE_PATH "font/Helvetica-rr-15-16.vbf",
        "vbf-Helvetica,sans serif-rrncnn-15-16-ISO8859-1" },
    { FONTFILE_PATH "font/Times-rr-10-12.vbf",
         "vbf-Times,serif-rrncnn-10-12-ISO8859-1" },
    { FONTFILE_PATH "font/Times-rr-13-15.vbf",
         "vbf-Times,serif-rrncnn-13-15-ISO8859-1" },
    { FONTFILE_PATH "font/courier_120_50.upf",
        "upf-Courier,sans serif-rrncnn-12-12-ISO8859-1" },
    { FONTFILE_PATH "font/courier_180_50.upf",
        "upf-Courier,sans serif-rrncnn-18-18-ISO8859-1" },
    { FONTFILE_PATH "font/courier_240_50.upf",
        "upf-Courier,sans serif-rrncnn-24-24-ISO8859-1" },
    { FONTFILE_PATH "font/helvetica_120_50.upf",
        "upf-Helvetica,sans serif-rrncnn-12-12-ISO8859-1" },
    { FONTFILE_PATH "font/helvetica_180_50.upf",
        "upf-Helvetica,sans serif-rrncnn-18-18-ISO8859-1" },
    { FONTFILE_PATH "font/helvetica_240_50.upf",
        "upf-Helvetica,sans serif-rrncnn-24-24-ISO8859-1" },
    { FONTFILE_PATH "font/times_120_50.upf",
         "upf-Times,serif-rrncnn-12-12-ISO8859-1" },
    { FONTFILE_PATH "font/times_180_50.upf",
         "upf-Times,serif-rrncnn-18-18-ISO8859-1" },
    { FONTFILE_PATH "font/times_240_50.upf",
         "upf-Times,serif-rrncnn-24-24-ISO8859-1" },
    { FONTFILE_PATH "font/fmsong-latin-12.upf",
         "upf-fmsong,宋体,sans serif,monospace-rrncnn-12-12-GB2312-0,BIG5,UTF-8" },
    { FONTFILE_PATH "font/fmsong-latin-16.upf",
         "upf-fmsong,宋体,sans serif,monospace-rrncnn-16-16-GB2312-0,BIG5,UTF-8" },
    { FONTFILE_PATH "font/fmfsong-latin-12.upf",
         "upf-fmfsong,仿宋,serif,monospace-rrncnn-12-12-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmfsong-latin-16.upf",
         "upf-fmfsong,仿宋,serif,monospace-rrncnn-16-16-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmhei-latin-16.upf",
         "upf-fmhei,黑体,sans serif,monospace-rrncnn-16-16-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmhei-latin-20.upf",
         "upf-fmhei,黑体,sans serif,monospace-rrncnn-20-20-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmkai-latin-12.upf",
         "upf-fmkai,楷体,monospace-rrncnn-12-12-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmkai-latin-16.upf",
         "upf-fmkai,楷体,monospace-rrncnn-16-16-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/naskhi-18-21-iso8859-6.vbf",
         "vbf-naskhi-rrncnn-18-21-ISO8859-6" },
    { FONTFILE_PATH "font/fixed-10-20-iso8859-8.vbf",
         "vbf-fixed-mrncnn-10-20-ISO8859-8" },
    { FONTFILE_PATH "font/gothic-6x12-jisx0201.bin",
         "rbf-gothic,sans serif,monospace-rrncnn-12-12-JISX0201-1" },
    { FONTFILE_PATH "font/batang-12-ksc5601.bin",
         "rbf-batang,sans serif,monospace-rrncnn-12-12-EUC-KR" },
    { FONTFILE_PATH "font/gothic-12-jiskan.bin",
        "rbf-gothic,sans serif,monospace-rrncnn-12-12-JISX0208-1" },
    { FONTFILE_PATH "font/fixed-16-jiskan.bin",
        "rbf-fixed,sans serif,monospace-rrncnn-16-16-JISX0208-1" },
    { FONTFILE_PATH "font/gothic-12-jiskan.bin",
        "rbf-gothic,sans serif,monospace-rrncnn-12-12-JISX0208-1" },
    { FONTFILE_PATH "font/mincho-12-jiskan.bin",
        "rbf-mincho,sans serif,monospace-rrncnn-12-12-JISX0208-1" },
    { FONTFILE_PATH "font/gothic-6x12rk.bin",
        "rbf-gothic,sans serif,monospace-rrncnn-12-12-JISX0201-1" },
    { FONTFILE_PATH "font/courier_140_50.qpf",
        "qpf-courier,sans serif-rrncnn-14-14-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/courier_180_50.qpf",
        "qpf-courier,sans serif-rrncnn-18-18-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/courier_240_50.qpf",
        "qpf-courier,sans serif-rrncnn-24-24-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/lucida_140_50.qpf",
        "qpf-lucida,sans serif-rrncnn-14-14-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/lucida_180_50.qpf",
        "qpf-lucida,sans serif-rrncnn-18-18-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/lucida_190_50.qpf",
        "qpf-lucida,sans serif-rrncnn-19-19-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/lucida_240_50.qpf",
        "qpf-lucida,sans serif-rrncnn-24-24-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/times_140_50.qpf",
        "qpf-times,serif-rrncnn-14-14-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/times_180_50.qpf",
        "qpf-times,serif-rrncnn-18-18-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/times_240_50.qpf",
        "qpf-times,serif-rrncnn-24-24-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/helvetica_140_50.qpf",
        "qpf-helvetica,sans serif-rrncnn-14-14-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/helvetica_180_50.qpf",
        "qpf-helvetica,sans serif-rrncnn-18-18-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/helvetica_240_50.qpf",
        "qpf-helvetica,sans serif-rrncnn-24-24-ISO8859-1,ISO8859-15" },
    { FONTFILE_PATH "font/unifont_160_50.upf",
         "upf-unifont,sans serif,monospace-rrncnn-8-16-ISO8859-1,ISO8859-6,ISO8859-8,UTF-8" },
    { FONTFILE_PATH "font/fmfsong-16.qpf",
        "qpf-fmfsong,fangsong,仿宋,serif,monospace-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmfsong-18.qpf",
        "qpf-fmfsong,fangsong,仿宋,serif,monospace-rrncnn-18-18-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmfsong-20.qpf",
        "qpf-fmfsong,fangsong,仿宋,serif,monospace-rrncnn-20-20-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmsong-15.qpf",
        "qpf-fmsong,song,宋体,sans serif,monospace-rrncnn-15-15-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmsong-16.qpf",
        "qpf-fmsong,song,宋体,sans serif,monospace-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmsong-18.qpf",
        "qpf-fmsong,song,宋体,sans serif,monospace-rrncnn-18-18-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmkai-16.qpf",
        "qpf-fmkai,kai,楷体,monospace-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmkai-18.qpf",
        "qpf-fmkai,kai,楷体,monospace-rrncnn-18-18-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },
    { FONTFILE_PATH "font/fmkai-20.qpf",
        "qpf-fmkai,kai,楷体,monospace-rrncnn-20-20-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE" },

    // TrueType or OpenType fonts
    // download location:
    // https://github.com/adobe-fonts/
    { FONTFILE_PATH "font/SourceHanSans-ExtraLight.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-erncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Light.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-lrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Normal.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-nrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Regular.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-rrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Medium.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-mrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Bold.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-brncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Heavy.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-crncnn-0-0-ISO8859-1,UTF-8" },

    { FONTFILE_PATH "font/SourceHanSerif-ExtraLight.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-erncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Light.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-lrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Regular.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-rrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Medium.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-mrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-SemiBold.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-drncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Bold.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-brncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Heavy.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-crncnn-0-0-ISO8859-1,UTF-8" },

    { FONTFILE_PATH "font/SourceSansPro-BlackIt.ttf",
        "ttf-Source Sans Pro,sans serif-cincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Black.ttf",
        "ttf-Source Sans Pro,sans serif-crncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-BoldIt.ttf",
        "ttf-Source Sans Pro,sans serif-bincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Bold.ttf",
        "ttf-Source Sans Pro,sans serif-brncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-ExtraLightIt.ttf",
        "ttf-Source Sans Pro,sans serif-eincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-ExtraLight.ttf",
        "ttf-Source Sans Pro,sans serif-erncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-It.ttf",
        "ttf-Source Sans Pro,sans serif-rincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-LightIt.ttf",
        "ttf-Source Sans Pro,sans serif-lincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Light.ttf",
        "ttf-Source Sans Pro,sans serif-lrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-SemiboldIt.ttf",
        "ttf-Source Sans Pro,sans serif-dincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Semibold.ttf",
        "ttf-Source Sans Pro,sans serif-drncnn-0-0-ISO8859-1,UTF-8" },

    { FONTFILE_PATH "font/SourceSerifPro-BlackIt.ttf",
        "ttf-Source Serif Pro,serif-cincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Black.ttf",
        "ttf-Source Serif Pro,serif-crncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-BoldIt.ttf",
        "ttf-Source Serif Pro,serif-bincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Bold.ttf",
        "ttf-Source Serif Pro,serif-brncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-ExtraLightIt.ttf",
        "ttf-Source Serif Pro,serif-eincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-ExtraLight.ttf",
        "ttf-Source Serif Pro,serif-erncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-It.ttf",
        "ttf-Source Serif Pro,serif-rincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-LightIt.ttf",
        "ttf-Source Serif Pro,serif-lincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Light.ttf",
        "ttf-Source Serif Pro,serif-lrncnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-SemiboldIt.ttf",
        "ttf-Source Serif Pro,serif-dincnn-0-0-ISO8859-1,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Semibold.ttf",
        "ttf-Source Serif Pro,serif-drncnn-0-0-ISO8859-1,UTF-8" },

    { FONTFILE_PATH "font/SourceCodeVariable-Italic.ttf",
        "ttf-Source Code,monospace-rincnn-0-0-ISO8859-1,UTF-8" },
};

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = "Space: functions; F1~F12: change family/styles; CURSORS: change size";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = 0;
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MyMainWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 1024;
    pCreateInfo->by = 530;
    pCreateInfo->iBkColor = COLOR_lightwhite;
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain (int args, const char* arg[])
{
    int i;
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }

    if (!InitVectorialFonts ()) {
        printf ("InitVectorialFonts: error.\n");
        exit (2);
    }
#endif

    for (i = 0; i < TABLESIZE(_devfontinfo); i++) {
        _devfontinfo[i].devfont = LoadDevFontFromFile (_devfontinfo[i].fontname,
                _devfontinfo[i].filename);
        if (_devfontinfo[i].devfont == NULL) {
            _ERR_PRINTF("%s: Failed to load devfont(%s) from %s\n",
                __FUNCTION__, _devfontinfo[i].fontname, _devfontinfo[i].filename);
        }
    }

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        exit (3);

    ShowWindow (hMainWnd, SW_SHOWNORMAL);
    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    for (i = 0; i < TABLESIZE(_devfontinfo); i++) {
        if (_devfontinfo[i].devfont) {
             DestroyDynamicDevFont (&_devfontinfo[i].devfont);
        }
    }

#ifndef _MGRM_THREADS
    TermVectorialFonts ();
#endif
    return 0;
}


#else
#error "To test CreateLogFontEx, please use MiniGUI 3.4.0"
#endif /* checking version and features */
