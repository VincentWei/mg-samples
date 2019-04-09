/*
** complexscripts.c:
**
**  Sample code for to render complex scripts in MiniGUI 4.0.0.
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

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(4,0,0)) \
        && defined(_MGCHARSET_UNICODE)

#include "helpers.h"

typedef struct _NewsInfo {
    const char* title;
    const char* text;
    const char* url;
    const char* title_font;
    const char* text_font;
    Uint32      title_flags;
    Uint32      text_flags;
} NewsInfo;

static const char* link_font =  "upf-unifont-rrnnun-*-16-UTF-8";

static int _curr_news;

static NewsInfo _news_cases[] = {
    {
        "习近平：推动国土绿化不断取得实实在在的成效",
        "file:res/zh-utf-8.txt",
        "https://news.sina.com.cn/c/xl/2019-04-08/doc-ihvhiewr4147778.shtml",
        "ttf-思源黑体-mrnnns-*-20-UTF-8",
        "ttf-思源黑体-rrnnns-*-14-UTF-8",
        GRF_WRITING_MODE_HORIZONTAL_TB | GRF_TEXT_ORIENTATION_AUTO | GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_END | GRF_ALIGN_CENTER | GRF_TEXT_JUSTIFY_AUTO | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_WRITING_MODE_HORIZONTAL_TB | GRF_TEXT_ORIENTATION_AUTO | GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_NONE | GRF_ALIGN_LEFT | GRF_TEXT_JUSTIFY_AUTO | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
    },
};

typedef struct _RENDER_RULE {
    Uint32 rule;
    const char* desc;
} RENDER_RULE;

static RENDER_RULE _wsr_cases [] = {
    { WSR_NORMAL,
        "WSR_NORMAL" },
    { WSR_PRE,
        "WSR_PRE" },
    { WSR_NOWRAP,
        "WSR_NOWRAP" },
    { WSR_PRE_WRAP,
        "WSR_PRE_WRAP" },
    { WSR_BREAK_SPACES,
        "WSR_BREAK_SPACES" },
    { WSR_PRE_LINE,
        "WSR_PRE_LINE" },
};

static RENDER_RULE _ctr_cases [] = {
    { CTR_NONE,
        "CTR_NONE" },
    { CTR_CAPITALIZE,
        "CTR_CAPITALIZE" },
    { CTR_UPPERCASE,
        "CTR_UPPERCASE" },
    { CTR_LOWERCASE,
        "CTR_LOWERCASE" },
    { CTR_FULL_WIDTH,
        "CTR_FULL_WIDTH" },
    { CTR_FULL_SIZE_KANA,
        "CTR_FULL_SIZE_KANA" },
    { CTR_CAPITALIZE | CTR_FULL_WIDTH,
        "CTR_CAPITALIZE | CTR_FULL_WIDTH" },
    { CTR_UPPERCASE | CTR_FULL_WIDTH,
        "CTR_UPPERCASE | CTR_FULL_WIDTH" },
    { CTR_LOWERCASE | CTR_FULL_WIDTH,
        "CTR_LOWERCASE | CTR_FULL_WIDTH" },
    { CTR_CAPITALIZE | CTR_FULL_SIZE_KANA,
        "CTR_CAPITALIZE | CTR_FULL_SIZE_KANA" },
    { CTR_UPPERCASE | CTR_FULL_SIZE_KANA,
        "CTR_UPPERCASE | CTR_FULL_SIZE_KANA" },
    { CTR_LOWERCASE | CTR_FULL_SIZE_KANA,
        "CTR_LOWERCASE | CTR_FULL_SIZE_KANA" },
    { CTR_CAPITALIZE | CTR_FULL_WIDTH | CTR_FULL_SIZE_KANA,
        "CTR_CAPITALIZE | CTR_FULL_WIDTH | CTR_FULL_SIZE_KANA" },
    { CTR_UPPERCASE | CTR_FULL_WIDTH | CTR_FULL_SIZE_KANA,
        "CTR_UPPERCASE | CTR_FULL_WIDTH | CTR_FULL_SIZE_KANA" },
    { CTR_LOWERCASE | CTR_FULL_WIDTH | CTR_FULL_SIZE_KANA,
        "CTR_LOWERCASE | CTR_FULL_WIDTH | CTR_FULL_SIZE_KANA" },
};

static RENDER_RULE _wbr_cases [] = {
    { WBR_NORMAL,
        "WBR_NORMAL" },
    { WBR_BREAK_ALL,
        "WBR_BREAK_ALL" },
    { WBR_KEEP_ALL,
        "WBR_KEEP_ALL" },
};

static RENDER_RULE _lbp_cases [] = {
    { LBP_NORMAL,
        "LBP_NORMAL" },
    { LBP_LOOSE,
        "LBP_LOOSE" },
    { LBP_STRICT,
        "LBP_STRICT" },
    { LBP_ANYWHERE,
        "LBP_ANYWHERE" },
};

static int _letter_spacing = 0;
static int _word_spacing = 0;
static int _tab_size = 100;
static RECT _rc_text = {10, 10, 1024 - 20, 600 - 20};

typedef struct _ParagraphInfo {
    Uchar32*    ucs;
    BreakOppo*  bos;
    TEXTRUNS*   textruns;
    LAYOUT*     layout;
    int         nr_ucs;
} ParagraphInfo;

static ParagraphInfo* _paragraphs;
static int            _nr_parags;

static void destroy_paragraphs(void)
{
    for (int i = 0; i < _nr_parags; i++) {
        DestroyLayout(_paragraphs[i].layout);
        DestroyTextRuns(_paragraphs[i].textruns);
        free(_paragraphs[i].bos);
        free(_paragraphs[i].ucs);
    }

    if (_paragraphs)
        free(_paragraphs);
    _paragraphs = NULL;
    _nr_parags = 0;
}

static void create_layout(ParagraphInfo* p, Uint32 render_flags, const char* fontname)
{
    p->textruns = CreateTextRuns(p->ucs, p->nr_ucs,
            LANGCODE_unknown, BIDI_PGDIR_LTR,
            fontname, MakeRGB(0, 0, 0), 0, p->bos + 1);

    if (p->textruns) {
        if (!InitComplexShapingEngine(p->textruns)) {
            _ERR_PRINTF("%s: InitComplexShapingEngine returns FALSE\n",
                    __FUNCTION__);
            exit(1);
        }

        p->layout = CreateLayout(p->textruns,
                render_flags, p->bos + 1, TRUE,
                RECTW(_rc_text), 0,
                _letter_spacing, _word_spacing, _tab_size, NULL, 0);
        if (p->layout == NULL) {
            _ERR_PRINTF("%s: CreateLayout returns NULL\n", __FUNCTION__);
            exit(1);
        }

        LAYOUTLINE* line = NULL;
        int i = 1;
        while ((line = LayoutNextLine(p->layout, line, 100 * i, 0, NULL, 0))) {
            i++;
        }
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }
}

static char _text_from_file[4096];
static char _utf8_str [5000];

static void create_paragraphs(void)
{
    char charset[100];
    PLOGFONT lf = NULL;
    const char* text;
    int left_len_text;

    text = get_text_case(_news_cases[_curr_news].text, _text_from_file, 4096);

    strcpy (charset, "utf-8");
    if (text == _text_from_file) {
        get_charset_from_filename(_news_cases[_curr_news].text, charset);
    }

    if (!(lf = CreateLogFontForMChar2UChar(charset))) {
        _ERR_PRINTF("%s: failed to create logfont for charset: %s\n",
                __FUNCTION__, charset);
        exit(1);
    }

    destroy_paragraphs();

    left_len_text = strlen(text);
    while (left_len_text > 0) {
        Uchar32* ucs;
        Uint16* bos;
        int consumed;
        int n;

        ucs = NULL;
        consumed = GetUCharsUntilParagraphBoundary(lf, text, left_len_text,
                WSR_PRE_WRAP, &ucs, &n);
        if (consumed > 0) {

            _MG_PRINTF("%s: GetUCharsUntilParagraphBoundary: bytes: %d, glyphs: %d\n",
                __FUNCTION__, consumed, n);

            if (n > 0) {
                _nr_parags++;
                _paragraphs = realloc(_paragraphs,
                        sizeof(ParagraphInfo) * _nr_parags);
                _paragraphs[_nr_parags - 1].ucs = ucs;
                _paragraphs[_nr_parags - 1].nr_ucs = n;

                int len_bos;
                bos = NULL;
                len_bos = UStrGetBreaks (SCRIPT_LATIN,
                    CTR_NONE, WBR_NORMAL, LBP_NORMAL,
                    ucs, n, &bos);

                if (len_bos > 0) {
                    _paragraphs[_nr_parags - 1].bos = bos;
                    create_layout(_paragraphs + _nr_parags - 1,
                        _news_cases[_curr_news].text_flags, _news_cases[_curr_news].text_font);
                }
                else {
                    _ERR_PRINTF("%s: UStrGetBreaks failed.\n",
                        __FUNCTION__);
                    goto error;
                }
            }
            else {
                _ERR_PRINTF("%s: GetUCharsUntilParagraphBoundary did not generate any uchar\n",
                    __FUNCTION__);
                goto error;
            }
        }
        else {
            _ERR_PRINTF("%s: GetUCharsUntilParagraphBoundary failed\n", __FUNCTION__);
            goto error;
        }

        left_len_text -= consumed;
        text += consumed;
    }

    DestroyLogFont(lf);
    return;

error:
    exit(1);
}

static int _text_x, _text_y;

static void render_paragraphs_draw_line(HDC hdc)
{
    switch (_news_cases[_curr_news].text_flags & GRF_WRITING_MODE_MASK) {
    case GRF_WRITING_MODE_HORIZONTAL_BT:
        _text_x = _rc_text.left;
        _text_y = _rc_text.bottom;
        break;

    case GRF_WRITING_MODE_VERTICAL_RL:
        _text_x = _rc_text.right;
        _text_y = _rc_text.top;
        break;

    case GRF_WRITING_MODE_HORIZONTAL_TB:
    case GRF_WRITING_MODE_VERTICAL_LR:
    default:
        _text_x = _rc_text.left;
        _text_y = _rc_text.top;
        break;
    }

    SetMapMode(hdc, MM_TEXT);

    POINT pt = {_text_x, _text_y};
    int parag_x, parag_y;
    for (int i = 0; i < _nr_parags; i++) {
        LAYOUT* layout = _paragraphs[i].layout;
        LAYOUTLINE* line = NULL;
        RECT rc;
        int line_height = 0;
        int j = 0;

        _MG_PRINTF("%s: rendering paragraph: %d\n",
            __FUNCTION__, i);

        SetTextColorInTextRuns(_paragraphs[i].textruns, 0, 4096, MakeRGB(255, 0, 0));

        parag_x = pt.x;
        parag_y = pt.y;

        while ((line = LayoutNextLine(layout, line, 0, 0, NULL, 0))) {
            SIZE sz;
            GetLayoutLineSize(line, &sz);
            sz.cx += 5;
            sz.cy += 5;
            if (sz.cy > line_height)
                line_height = sz.cy;

            _MG_PRINTF("%s: rendered line by calling DrawShapedGlyph: %d\n",
                __FUNCTION__, j);

            DrawLayoutLine(hdc, line, pt.x, pt.y);

            GetLayoutLineRect(line, &pt.x, &pt.y, sz.cy, &rc);
            //SetPenColor(hdc, PIXEL_blue);
            //Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

            j++;
        }

        CalcLayoutBoundingRect(layout, 0, -1, line_height, parag_x, parag_y, &rc);
        //SetPenColor(hdc, PIXEL_green);
        //Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

        switch (_news_cases[_curr_news].text_flags & GRF_WRITING_MODE_MASK) {
        case GRF_WRITING_MODE_HORIZONTAL_TB:
            pt.y += 10;
            break;

        case GRF_WRITING_MODE_HORIZONTAL_BT:
            pt.y -= 10;
            break;

        case GRF_WRITING_MODE_VERTICAL_RL:
            pt.x -= 10;
            break;

        case GRF_WRITING_MODE_VERTICAL_LR:
            pt.x += 10;
            break;
        }
    }
}

static int _auto_test_runs = 0;
static int _nr_test_runs = 0;

static
LRESULT MyMainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        GetClientRect(hWnd, &_rc_text);
        InflateRect(&_rc_text, -10, -10);
        create_paragraphs();
        break;

    case MSG_PAINT: {
        HDC hdc;
        hdc = BeginPaint(hWnd);
        SetPenColor(hdc, PIXEL_green);
        Rectangle(hdc, _rc_text.left, _rc_text.top,
            _rc_text.right, _rc_text.bottom);
        render_paragraphs_draw_line(hdc);
        EndPaint(hWnd, hdc);
        return 0;
    }

    case MSG_KEYDOWN: {
        BOOL repaint = FALSE;

        switch (wParam) {
        case SCANCODE_ESCAPE:
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case SCANCODE_SPACE:
            repaint = TRUE;
            break;

        case SCANCODE_ENTER:
            _curr_news++;
            _curr_news %= TABLESIZE(_news_cases);
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKLEFT:
            _rc_text.right -= 5;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKRIGHT:
            _rc_text.right += 5;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKUP:
            _rc_text.bottom -= 20;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKDOWN:
            _rc_text.bottom += 20;
            repaint = TRUE;
            break;

        case SCANCODE_INSERT:
            _letter_spacing += 1;
            repaint = TRUE;
            break;

        case SCANCODE_REMOVE:
            _letter_spacing -= 1;
            repaint = TRUE;
            break;

        case SCANCODE_HOME:
            _word_spacing += 2;
            repaint = TRUE;
            break;

        case SCANCODE_END:
            _word_spacing -= 2;
            repaint = TRUE;
            break;

        case SCANCODE_PAGEUP:
            _tab_size += 2;
            repaint = TRUE;
            break;

        case SCANCODE_PAGEDOWN:
            _tab_size -= 2;
            repaint = TRUE;
            break;

        default:
            break;
        }

        if (repaint) {
            create_paragraphs();
            InvalidateRect(hWnd, NULL, TRUE);
        }

        return 0;
    }

    case MSG_CLOSE:
        destroy_paragraphs();
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = "Sample for Complex/Mixed Scripts";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = 0;
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MyMainWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = GetGDCapability(HDC_SCREEN, GDCAP_HPIXEL);
    pCreateInfo->by = GetGDCapability(HDC_SCREEN, GDCAP_VPIXEL);;
    pCreateInfo->iBkColor = COLOR_lightwhite;
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

#define  FONTFILE_PATH   "/usr/local/share/minigui/res/"

typedef struct _DEVFONTINFO {
    const char* filename;
    const char* fontname;
    DEVFONT*    devfont;
} DEVFONTINFO;

static DEVFONTINFO _devfontinfo[] = {
    { FONTFILE_PATH "font/SourceHanSans-ExtraLight.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-erncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Light.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-lrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Normal.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-nrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Regular.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-rrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Medium.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-mrncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Bold.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-brncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },
    { FONTFILE_PATH "font/SourceHanSans-Heavy.ttc",
        "ttf-Source Han Sans,思源黑体,思源黑體,源ノ角ゴシック,본고딕,SansSerif-crncnn-0-0-ISO8859-1,GBK,BIG5,UTF-8" },

    { FONTFILE_PATH "font/unifont_160_50.upf",
        "upf-unifont,SansSerif,monospace-rrncnn-8-16-ISO8859-1,ISO8859-6,ISO8859-8,UTF-8" },

    { "/usr/share/fonts/truetype/fonts-guru-extra/Saab.ttf",
        "ttf-Saab-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/lohit-punjabi/Lohit-Punjabi.ttf",
        "ttf-Lohit-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/kacst/KacstArt.ttf",
        "ttf-KacstArt-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/kacst/KacstBook.ttf",
        "ttf-KacstBook-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/kacst/KacstDecorative.ttf",
        "ttf-KacstDecorative-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/kacst/KacstLetter.ttf",
        "ttf-KacstLetter-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/kacst/KacstNaskh.ttf",
        "ttf-KacstNaskh-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/kacst/KacstScreen.ttf",
        "ttf-KacstScreen-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Garuda.ttf",
        "ttf-Garuda-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Kinnari.ttf",
        "ttf-Kinnari-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Laksaman.ttf",
        "ttf-Laksaman-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Loma.ttf",
        "ttf-Loma-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Norasi.ttf",
        "ttf-Norasi-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Purisa.ttf",
        "ttf-Purisa-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Sawasdee.ttf",
        "ttf-Sawasdee-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/TlwgMono.ttf",
        "ttf-TlwgMono-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/TlwgTypewriter.ttf",
        "ttf-TlwgTypewriter-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/TlwgTypist.ttf",
        "ttf-TlwgTypist-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/TlwgTypo.ttf",
        "ttf-TlwgTypo-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Umpush.ttf",
        "ttf-Umpush-rrncnn-0-0-UTF-8" },
    { "/usr/share/fonts/truetype/tlwg/Waree.ttf",
        "ttf-Waree-rrncnn-0-0-UTF-8" },
};

int MiniGUIMain (int argc, const char* argv[])
{
    int i;
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

    srandom(time(NULL));
    if (argc > 1)
        _auto_test_runs = atoi(argv[1]);

#ifdef _MGRM_PROCESSES
    int i;
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

#error "To build this sample, please use MiniGUI 4.0.0 and enable support for UNICODE"

#endif /* checking version and features */
