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

enum _TestMode {
    TM_MANUAL_SINGLE_MBC,
    TM_MANUAL_MULTI_MBC,
    TM_RANDOM,
    TM_RANDOM_RUN,
};

static const char* _test_modes[] = {
    "TM_MANUAL_SINGLE_MBC",
    "TM_MANUAL_MULTI_MBC",
    "TM_RANDOM",
    "TM_RANDOM_RUN",
};

static const char* _text_cases[] = {
    "1234567890",

    "Source Han Serif is the serif-style typeface family companion to Source Han Sans. The Chinese glyphs, both simplified and traditional, were designed by partner type foundry Changzhou SinoType. The Simplified Chinese fonts support the GB 18030 standard, along with China’s list of 8,105 hanzi (Tōngyòng Guīfàn Hànzìbiǎo, which includes 199 hanzi that are outside the scope of the GB 18030 standard). The Traditional Chinese fonts support the Big 5 standard, and glyph shapes adhere to the Taiwan Ministry of Education standard. Learn more about how these fonts were created.",

    "An opening bracket or quote at the start of the line or a closing bracket or quote at the end line hangs:\n(12)\n'345'\n\"67890\"",

    "     (12) '345'\n \"67890\"  　　",

    "   12345678，\n123456789。",

    " some Latin letters.",

    "这是一些汉字; some Latin letters; \n"
    "$89.00 (￥50.00); 80,000.00; 90.2%\n"
    "窓ぎわのトットちゃん\n"
    "각 줄의 마지막에 한글이 올 때 줄 나눔 기준을 “글자” 또는 “어절” 단위로 한다.",

    "　登鹳雀楼　\n"
    "\n"
    "      作者：王之涣 年代：唐\n"
    "白日依山尽，黄河入海流。\n"
    "欲穷千里目，更上一层楼。\n"
    "\n"
    "\n"
    "其中，前两句写所见。“白日依山尽”写远景，写山，写的是登楼望见的景色，“黄河入海流”写近景，写水写得景象壮观，气势磅礴。这里，诗人运用极其朴素、极其浅显的语言，既高度形象又高度概括地把进入广大视野的万里河山，收入短短十个字中；而后人在千载之下读到这十个字时，也如临其地，如见其景，感到胸襟为之一开。",

    "Grapheme clusters formed with an Enclosing Mark (Me) of the Common script are considered to be Other Symbols (So) in the Common script. They are assumed to have the same Unicode properties as the Replacement Character U+FFFD.",

    "ぁ\tU+3041\tあ\tU+3042\n"
    "ぃ\tU+3043\tい\tU+3044\n"
    "ぅ\tU+3045\tう\tU+3046\n"
    "ぇ\tU+3047\tえ\tU+3048\n"
    "ぉ\tU+3049\tお\tU+304A\n"
    "ゕ\tU+3095\tか\tU+304B\n"
    "ゖ\tU+3096\tけ\tU+3051\n"
    "っ\tU+3063\tつ\tU+3064\n"
    "ゃ\tU+3083\tや\tU+3084\n"
    "ゅ\tU+3085\tゆ\tU+3086\n"
    "ょ\tU+3087\tよ\tU+3088\n"
    "ゎ\tU+308E\tわ\tU+308F",

    "If the content language is Chinese and the writing system is unspecified, or for any content language if the writing system to specified to be one of the ‘Hant’, ‘Hans’, ‘Hani’, ‘Hanb’, or ‘Bopo’ [ISO15924] codes, then the writing system is Chinese.",

    "    if (outbuf) {                      \n"
    "    \tfor (i = len - 1; i > 0; --i) {  \n"
    "    \t\toutbuf[i] = (c & 0x3f) | 0x80; \n"
    "    \t\tc >>= 6;                       \n"
    "    \t}                                \n"
    "    outbuf[0] = c | first;\n"
    "    }\n",

    "        　",

    "Amazon Will Pay a Whopping $0 in Federal Taxes on $11.2 Billion Profits\n"
    "\n"
    "Those wondering how many zeros Amazon, which is valued at nearly $800 billion, has to pay in federal taxes might be surprised to learn that its check to the IRS will read exactly $0.00.\n"
    "\n"
    "According to a report published by the Institute on Taxation and Economic (ITEP) policy Wednesday, the e-tail/retail/tech/entertainment/everything giant won’t have to pay a cent in federal taxes for the second year in a row.\n"
    "\n"
    "This tax-free break comes even though Amazon almost doubled its U.S. profits from $5.6 billion to $11.2 billion between 2017 and 2018.\n"
    "\n"
    "To top it off, Amazon actually reported a $129 million 2018 federal income tax rebate—making its tax rate -1%.\n"
    "\n"
    "Amazon’s low (to non-existent) tax rate has been chided by politicians ranging from Senator Bernie Sanders to President Donald Trump.\n",

    "file:res/iso8859-1.txt",
    "file:res/iso8859-6.txt",
    "file:res/iso8859-8.txt",
    "file:res/iso8859-15.txt",
    "file:res/gb2312.txt",
    "file:res/gbk.txt",
    "file:res/big5.txt",
    "file:res/euc-kr.txt",
    "file:res/jisx0208-1.txt",
};

static const char* _type_cases [] = {
    FONT_TYPE_NAME_ANY,
    FONT_TYPE_NAME_BITMAP_RAW,
    FONT_TYPE_NAME_BITMAP_VAR,
    FONT_TYPE_NAME_BITMAP_QPF,
    FONT_TYPE_NAME_BITMAP_UPF,
    FONT_TYPE_NAME_BITMAP_BMP,
    FONT_TYPE_NAME_SCALE_TTF,
};

static const char* _family_cases [] = {
    "fixed",
    "monospace",
    "serif",
    "sans serif",
    "times",
    "Courier",
    "Helvetica",
    "lucida",
    "song",
    "fangsong",
    "kai",
    "hei",
    "宋体",
    "仿宋",
    "黑体",
    "ming",
    "Source Han Sans",
    "思源黑体",
    "思源黑體",
    "源ノ角ゴシック",
    "본고딕",
    "Source Han Serif",
    "思源宋体",
    "思源宋體",
    "源ノ明朝",
    "본명조",
    "Source Sans Pro",
    "Source Serif Pro",
    "Source Code",
    "mingchao",
    "batang",
    "gothic",
    "naskhi",
};

static const char* _charset_cases[] = {
    "ISO8859-1",
    "ISO8859-6",
    "ISO8859-8",
    "ISO8859-15",
    "GB2312",
    "GBK",
    "BIG5",
    "EUC-KR",
    "SHIFT-JIS",
    "UTF-8",
};

typedef struct _LOGFONT_STYLE {
    char        style_ch;
    DWORD32     style;
    const char* desc;
} LOGFONT_STYLE;

static LOGFONT_STYLE _weight_style_cases [] = {
    { FONT_WEIGHT_THIN,
        FS_WEIGHT_THIN,
        "FS_WEIGHT_THIN" },
    { FONT_WEIGHT_EXTRA_LIGHT,
        FS_WEIGHT_EXTRA_LIGHT,
        "FONT_WEIGHT_EXTRA_LIGHT" },
    { FONT_WEIGHT_LIGHT,
        FS_WEIGHT_LIGHT,
        "FONT_WEIGHT_LIGHT" },
    { FONT_WEIGHT_NORMAL,
        FS_WEIGHT_NORMAL,
        "FONT_WEIGHT_NORMAL" },
    { FONT_WEIGHT_REGULAR,
        FS_WEIGHT_REGULAR,
        "FONT_WEIGHT_REGULAR" },
    { FONT_WEIGHT_MEDIUM,
        FS_WEIGHT_MEDIUM,
        "FONT_WEIGHT_MEDIUM" },
    { FONT_WEIGHT_DEMIBOLD,
        FS_WEIGHT_DEMIBOLD,
        "FONT_WEIGHT_DEMIBOLD" },
    { FONT_WEIGHT_BOLD,
        FS_WEIGHT_BOLD,
        "FONT_WEIGHT_BOLD" },
    { FONT_WEIGHT_EXTRA_BOLD,
        FS_WEIGHT_EXTRA_BOLD,
        "FONT_WEIGHT_EXTRA_BOLD" },
    { FONT_WEIGHT_BLACK,
        FS_WEIGHT_BLACK,
        "FONT_WEIGHT_BLACK" },
};

static LOGFONT_STYLE _slant_style_cases [] = {
    { FONT_SLANT_ALL,
        FS_SLANT_ALL,
        "FONT_SLANT_ALL" },
    { FONT_SLANT_ITALIC,
        FS_SLANT_ITALIC,
        "FONT_SLANT_ITALIC" },
    { FONT_SLANT_OBLIQUE,
        FS_SLANT_OBLIQUE,
        "FONT_SLANT_OBLIQUE" },
    { FONT_SLANT_ROMAN,
        FS_SLANT_ROMAN,
        "FONT_SLANT_ROMAN" },
};

static LOGFONT_STYLE _flip_style_cases [] = {
    { FONT_FLIP_HORZ,
        FS_FLIP_HORZ,
        "FONT_FLIP_HORZ" },
    { FONT_FLIP_VERT,
        FS_FLIP_VERT,
        "FONT_FLIP_VERT" },
    { FONT_FLIP_HORZVERT,
        FS_FLIP_HORZVERT,
        "FONT_FLIP_HORZVERT" },
    { FONT_FLIP_NONE,
        FS_FLIP_NONE,
        "FONT_FLIP_NONE" },
};

static LOGFONT_STYLE _other_style_cases [] = {
    { FONT_OTHER_NONE,
        FS_OTHER_NONE,
        "FONT_OTHER_NONE" },
    { FONT_OTHER_AUTOSCALE,
        FS_OTHER_AUTOSCALE,
        "FONT_OTHER_AUTOSCALE" },
    { FONT_OTHER_TTFNOCACHE,
        FS_OTHER_TTFNOCACHE,
        "FONT_OTHER_TTFNOCACHE" },
    { FONT_OTHER_TTFKERN,
        FS_OTHER_TTFKERN,
        "FONT_OTHER_TTFKERN" },
    { FONT_OTHER_TTFNOCACHEKERN,
        FS_OTHER_TTFNOCACHEKERN,
        "FONT_OTHER_TTFNOCACHEKERN" },
};

static LOGFONT_STYLE _decorate_style_cases [] = {
    { FONT_DECORATE_NONE,
        FS_DECORATE_NONE,
        "FONT_DECORATE_NONE" },
    { FONT_DECORATE_UNDERLINE,
        FS_DECORATE_UNDERLINE,
        "FONT_DECORATE_UNDERLINE" },
    { FONT_DECORATE_STRUCKOUT,
        FS_DECORATE_STRUCKOUT,
        "FONT_DECORATE_STRUCKOUT" },
    { FONT_DECORATE_REVERSE,
        FS_DECORATE_REVERSE,
        "FONT_DECORATE_REVERSE" },
    { FONT_DECORATE_OUTLINE,
        FS_DECORATE_OUTLINE,
        "FONT_DECORATE_OUTLINE" },
    { FONT_DECORATE_US,
        FS_DECORATE_US,
        "FONT_DECORATE_US" },
};

static LOGFONT_STYLE _render_style_cases [] = {
    { FONT_RENDER_ANY,
        FS_RENDER_ANY,
        "FONT_RENDER_ANY" },
    { FONT_RENDER_MONO,
        FS_RENDER_MONO,
        "FONT_RENDER_MONO" },
    { FONT_RENDER_GREY,
        FS_RENDER_GREY,
        "FONT_RENDER_GREY" },
    { FONT_RENDER_SUBPIXEL,
        FS_RENDER_SUBPIXEL,
        "FONT_RENDER_SUBPIXEL" },
};

static int _curr_mode;
static int _curr_text;

static int _curr_mode;
static int _curr_type;
static int _curr_family;
static int _curr_charset;

static int _curr_weight_style;
static int _curr_slant_style;
static int _curr_flip_style;
static int _curr_other_style;
static int _curr_decorate_style;
static int _curr_render_style;

typedef struct _TOGGLE_ITEM {
    int     scancode;
    int*    current;
    int     upper;
} TOGGLE_ITEM;

static TOGGLE_ITEM _toggle_items[] = {
    { SCANCODE_SPACE,   &_curr_mode,            TABLESIZE(_test_modes) },
    { SCANCODE_F1,      &_curr_text,            TABLESIZE(_text_cases) },
    { SCANCODE_F2,      &_curr_type,            TABLESIZE(_type_cases) },
    { SCANCODE_F3,      &_curr_family,          TABLESIZE(_family_cases) },
    { SCANCODE_F4,      &_curr_charset,         TABLESIZE(_charset_cases) },
    { SCANCODE_F5,      &_curr_weight_style,    TABLESIZE(_weight_style_cases) },
    { SCANCODE_F6,      &_curr_slant_style,     TABLESIZE(_slant_style_cases) },
    { SCANCODE_F7,      &_curr_flip_style,      TABLESIZE(_flip_style_cases) },
    { SCANCODE_F8,      &_curr_other_style,     TABLESIZE(_other_style_cases) },
    { SCANCODE_F9,      &_curr_decorate_style,  TABLESIZE(_decorate_style_cases) },
    { SCANCODE_F10,     &_curr_render_style,    TABLESIZE(_render_style_cases) },
};

static BOOL toggle_item (int scancode, DWORD keystatus, BOOL rdm)
{
    int i;
    TOGGLE_ITEM* item = NULL;

    for (i = 0; i < TABLESIZE(_toggle_items); i++) {
        if (_toggle_items[i].scancode == scancode) {
            item = _toggle_items + i;
            break;
        }
    }

    if (item) {
        int current;

        if (rdm) {
            current = random() % item->upper;
        }
        else {
            current = *(item->current);
            if (keystatus & KS_SHIFT) {
                current--;
                if (current < 0)
                    current = item->upper - 1;
            }
            else {
                current++;
                current %= item->upper;
            }
        }

        if (*(item->current) != current) {
            *(item->current) = current;
            return TRUE;
        }
    }

    return FALSE;
}

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
        if (toggle_item((int)wParam, (DWORD)lParam, _curr_mode == TM_RANDOM))
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
         "upf-fmsong,宋体,sans serif,monospace-rrncnn-12-12-GB2312-0,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/fmsong-latin-16.upf",
         "upf-fmsong,宋体,sans serif,monospace-rrncnn-16-16-GB2312-0,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/fmfsong-latin-12.upf",
         "upf-fmfsong,仿宋,serif,monospace-rrncnn-12-12-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmfsong-latin-16.upf",
         "upf-fmfsong,仿宋,serif,monospace-rrncnn-16-16-GB2312-0,UTF-8" },
    { FONTFILE_PATH "font/fmhei-latin-16.upf",
         "upf-fmhei,黑体,sans serif,monospace-rrncnn-16-16-GB2312-0,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/fmhei-latin-20.upf",
         "upf-fmhei,黑体,sans serif,monospace-rrncnn-20-20-GB2312-0,GBK,BIG5,UTF-8" },
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
        "rbf-mincho,明朝,sans serif,monospace-rrncnn-12-12-JISX0208-1" },
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
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-erncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Light.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-lrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Normal.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-nrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Regular.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-rrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Medium.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-mrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Bold.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-brncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Heavy.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,sans serif-crncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },

    { FONTFILE_PATH "font/SourceHanSerif-ExtraLight.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-erncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Light.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-lrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Regular.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-rrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Medium.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-mrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-SemiBold.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-drncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Bold.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-brncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSerif-Heavy.ttc",
        "ttf-Source Han Serif,思源宋体,思源宋體,源ノ明朝,본명조,serif-crncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },

    { FONTFILE_PATH "font/SourceSansPro-BlackIt.ttf",
        "ttf-Source Sans Pro,sans serif-cincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Black.ttf",
        "ttf-Source Sans Pro,sans serif-crncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-BoldIt.ttf",
        "ttf-Source Sans Pro,sans serif-bincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Bold.ttf",
        "ttf-Source Sans Pro,sans serif-brncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-ExtraLightIt.ttf",
        "ttf-Source Sans Pro,sans serif-eincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-ExtraLight.ttf",
        "ttf-Source Sans Pro,sans serif-erncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-It.ttf",
        "ttf-Source Sans Pro,sans serif-rincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-LightIt.ttf",
        "ttf-Source Sans Pro,sans serif-lincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Light.ttf",
        "ttf-Source Sans Pro,sans serif-lrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-SemiboldIt.ttf",
        "ttf-Source Sans Pro,sans serif-dincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Semibold.ttf",
        "ttf-Source Sans Pro,sans serif-drncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },

    { FONTFILE_PATH "font/SourceSerifPro-BlackIt.ttf",
        "ttf-Source Serif Pro,serif-cincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Black.ttf",
        "ttf-Source Serif Pro,serif-crncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-BoldIt.ttf",
        "ttf-Source Serif Pro,serif-bincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Bold.ttf",
        "ttf-Source Serif Pro,serif-brncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-ExtraLightIt.ttf",
        "ttf-Source Serif Pro,serif-eincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-ExtraLight.ttf",
        "ttf-Source Serif Pro,serif-erncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-It.ttf",
        "ttf-Source Serif Pro,serif-rincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-LightIt.ttf",
        "ttf-Source Serif Pro,serif-lincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Light.ttf",
        "ttf-Source Serif Pro,serif-lrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-SemiboldIt.ttf",
        "ttf-Source Serif Pro,serif-dincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Semibold.ttf",
        "ttf-Source Serif Pro,serif-drncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },

    { FONTFILE_PATH "font/SourceCodeVariable-Italic.ttf",
        "ttf-Source Code,monospace-rincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },

    { FONTFILE_PATH "font/SourceSansPro-Regular.ttf",
        "ttf-Source Sans Pro,sans serif-rrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSerifPro-Regular.ttf",
        "ttf-Source Serif Pro,serif-rrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceCodeVariable-Roman.ttf",
        "ttf-Source Code,monospace-rrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
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

    srandom(time(NULL));

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
