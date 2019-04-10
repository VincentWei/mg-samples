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
    const char* header;
    const char* text;
    const char* footer;
    const char* header_font;
    const char* text_font;
    const char* footer_font;
    Uint32      common_flags;
    Uint32      header_flags;
    Uint32      text_flags;
    Uint32      footer_flags;
} NewsInfo;

static int _curr_news;

static NewsInfo _news_cases[] = {
    {
        "习近平：推动国土绿化不断取得实实在在的成效",
        "file:res/zh_CN-utf-8.txt",
        "https://news.sina.com.cn/c/xl/2019-04-08/doc-ihvhiewr4147778.shtml",
        "ttf-思源黑体-mrnnns-*-20-UTF-8",
        "ttf-思源黑体-ernnns-*-14-UTF-8",
        "ttf-Source Sans Pro-lrnnus-*-10-UTF-8",
        GRF_WRITING_MODE_HORIZONTAL_TB | GRF_TEXT_ORIENTATION_AUTO,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_MIDDLE | GRF_ALIGN_CENTER | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_NONE | GRF_ALIGN_LEFT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_END | GRF_ALIGN_RIGHT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
    },
    {
        "第一回 甄士隐梦幻识通灵　贾雨村风尘怀闺秀",
        "file:res/zh-红楼梦-utf-8.txt",
        "http://www.purepen.com/hlm/001.htm",
        "ttf-思源黑体-mrnnns-*-20-UTF-8",
        "ttf-思源黑体-ernnns-*-14-UTF-8",
        "ttf-Source Sans Pro-lrnnus-*-10-UTF-8",
        GRF_WRITING_MODE_VERTICAL_RL | GRF_TEXT_ORIENTATION_MIXED,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_MIDDLE | GRF_ALIGN_CENTER | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_NONE | GRF_ALIGN_LEFT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_END | GRF_ALIGN_RIGHT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
    },
    {
        "反對《與臺灣關係法》鬥爭——中美建交後圍繞臺灣問題的第一場較量",
        "file:res/zh_TW-utf-8.txt",
        "http://big5.taiwan.cn/plzhx/zhjzhl/zhjlw/201904/t20190409_12154867.htm",
        "ttf-思源黑體-mrnnns-*-20-UTF-8",
        "ttf-思源黑體-ernnns-*-14-UTF-8",
        "ttf-Source Sans Pro-lrnnus-*-10-UTF-8",
        GRF_WRITING_MODE_HORIZONTAL_TB | GRF_TEXT_ORIENTATION_AUTO,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_MIDDLE | GRF_ALIGN_CENTER | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_NONE | GRF_ALIGN_LEFT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_END | GRF_ALIGN_RIGHT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
    },
    {
        "Japanese F-35 fighter crashes into the Pacific",
        "file:res/en-utf-8.txt",
        "https://edition.cnn.com/2019/04/09/asia/japan-f-35-stealth-fighter-missing-intl/index.html",
        "ttf-Source Sans Pro-mrnnns-*-20-UTF-8",
        "ttf-Source Sans Pro-rrnnns-*-14-UTF-8",
        "ttf-Source Sans Pro-lrnnus-*-10-UTF-8",
        GRF_WRITING_MODE_HORIZONTAL_TB | GRF_TEXT_ORIENTATION_AUTO,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_MIDDLE | GRF_ALIGN_CENTER | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_NONE | GRF_ALIGN_LEFT | GRF_TEXT_JUSTIFY_INTER_WORD | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
        GRF_INDENT_NONE | GRF_LINE_EXTENT_FIXED | GRF_OVERFLOW_WRAP_NORMAL | GRF_OVERFLOW_ELLIPSIZE_END | GRF_ALIGN_RIGHT | GRF_TEXT_JUSTIFY_NONE | GRF_HANGING_PUNC_NONE | GRF_SPACES_KEEP,
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

typedef struct _ParagraphInfo {
    Uchar32*    ucs;
    BreakOppo*  bos;
    TEXTRUNS*   textruns;
    LAYOUT*     layout;
    int         nr_ucs;
} ParagraphInfo;

static RECT _rc_text;
static RECT _rc_header;
static RECT _rc_footer;
static int _max_extent_text;
static int _max_extent_header;
static int _max_extent_footer;

static int _width_inc, _height_inc;

static void set_rectangles(HWND hwnd)
{
    GetClientRect(hwnd, &_rc_text);
    InflateRect(&_rc_text, -10, -10);
    InflateRect(&_rc_text, _width_inc, _height_inc);

    CopyRect(&_rc_header, &_rc_text);
    CopyRect(&_rc_footer, &_rc_text);

    switch (_news_cases[_curr_news].common_flags & GRF_WRITING_MODE_MASK) {
    default:
    case GRF_WRITING_MODE_HORIZONTAL_TB:
        InflateRect(&_rc_text, 0, -50);
        _rc_header.bottom = _rc_text.top;
        _rc_footer.top = _rc_text.bottom;
        _max_extent_text = RECTW(_rc_text);
        _max_extent_header = RECTW(_rc_header);
        _max_extent_footer = RECTW(_rc_footer);
        break;

    case GRF_WRITING_MODE_HORIZONTAL_BT:
        InflateRect(&_rc_text, 0, -50);
        _rc_header.top = _rc_text.bottom;
        _rc_footer.bottom = _rc_text.top;
        _max_extent_text = RECTW(_rc_text);
        _max_extent_header = RECTW(_rc_header);
        _max_extent_footer = RECTW(_rc_footer);
        break;

    case GRF_WRITING_MODE_VERTICAL_LR:
        InflateRect(&_rc_text, -50, 0);
        _rc_header.right = _rc_text.left;
        _rc_footer.left = _rc_text.right;
        _max_extent_text = RECTH(_rc_text);
        _max_extent_header = RECTH(_rc_header);
        _max_extent_footer = RECTH(_rc_footer);
        break;

    case GRF_WRITING_MODE_VERTICAL_RL:
        InflateRect(&_rc_text, -50, 0);
        _rc_header.left = _rc_text.right;
        _rc_footer.right = _rc_text.left;
        _max_extent_text = RECTH(_rc_text);
        _max_extent_header = RECTH(_rc_header);
        _max_extent_footer = RECTH(_rc_footer);
        break;
    }
}

static void create_layout(ParagraphInfo* p, Uint32 render_flags, const char* fontname,
        int max_extent, BOOL oneline)
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
                max_extent, 0, _letter_spacing, _word_spacing, _tab_size, NULL, 0);
        if (p->layout == NULL) {
            _ERR_PRINTF("%s: CreateLayout returns NULL\n", __FUNCTION__);
            exit(1);
        }

        LAYOUTLINE* line = NULL;
        while ((line = LayoutNextLine(p->layout, line, 0, oneline, NULL, 0))) {
        }
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }
}

#define TOKEN_HAVE_NO_BREAK_OPPORTUNITY "×"
#define TOKEN_HAVE_BREAK_OPPORTUNITY    "÷"

static void do_dump(const Uchar32* ucs, const Uint16* bos, int n,
        Uint16 bo_flag)
{
    int i;

    if (bos[0] & bo_flag) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (i = 0; i < n; i++) {
        char utf8[16];
        int len;
        Uchar32 uc = ucs[i];

        len = uc32_to_utf8(uc, utf8);
        utf8[len] = 0;
        printf(" %s ", utf8);

        if (bos[i + 1] & bo_flag) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
}

static char _utf8_str [5000];
static void dump_glyphs_and_breaks(const char* text,
        const Uchar32* ucs, const Uint16* bos, int n)
{
    int i;
    char* tmp = _utf8_str;

    printf("START OF DUMPING GLYPHS AND BREAKS\n");
    printf("==================================\n");

    memset(_utf8_str, 0, 5000);
    for (i = 0; i < n; i++) {
        int len;
        Uchar32 uc = ucs[i];

        if ((tmp - _utf8_str) < 4990) {
            len = uc32_to_utf8(uc, tmp);
            tmp += len;
        }

        printf("%04X(%s, %s)\n", uc,
            get_general_category_name(UCharGetCategory(uc)),
            get_break_type_name(UCharGetBreakType(uc)));
    }
    printf("\n");

    printf("TEXT IN UTF-8\n");
    printf("==================================\n");
    printf("\n");
    puts(_utf8_str);
    printf("\n");

    printf("\tBOV_LB_BREAK_FLAG\n");
    do_dump(ucs, bos, n, BOV_LB_BREAK_FLAG);
    printf("\n\n");

    printf("\tBOV_WHITESPACE\n");
    do_dump(ucs, bos, n, BOV_WHITESPACE);
    printf("\n\n");

    printf("\tBOV_EXPANDABLE_SPACE\n");
    do_dump(ucs, bos, n, BOV_EXPANDABLE_SPACE);
    printf("\n\n");

    printf("\tBOV_ZERO_WIDTH\n");
    do_dump(ucs, bos, n, BOV_ZERO_WIDTH);
    printf("\n\n");

    printf("\tBOV_GB_CHAR_BREAK\n");
    do_dump(ucs, bos, n, BOV_GB_CHAR_BREAK);
    printf("\n\n");

    printf("\tBOV_GB_CURSOR_POS\n");
    do_dump(ucs, bos, n, BOV_GB_CURSOR_POS);
    printf("\n\n");

    printf("\tBOV_GB_BACKSPACE_DEL_CH\n");
    do_dump(ucs, bos, n, BOV_GB_BACKSPACE_DEL_CH);
    printf("\n\n");

    printf("\tBOV_WB_WORD_BOUNDARY\n");
    do_dump(ucs, bos, n, BOV_WB_WORD_BOUNDARY);
    printf("\n\n");

    printf("\tBOV_WB_WORD_START\n");
    do_dump(ucs, bos, n, BOV_WB_WORD_START);
    printf("\n\n");

    printf("\tBOV_WB_WORD_END\n");
    do_dump(ucs, bos, n, BOV_WB_WORD_END);
    printf("\n\n");

    printf("\tBOV_SB_SENTENCE_BOUNDARY\n");
    do_dump(ucs, bos, n, BOV_SB_SENTENCE_BOUNDARY);
    printf("\n\n");

    printf("\tBOV_SB_SENTENCE_START\n");
    do_dump(ucs, bos, n, BOV_SB_SENTENCE_START);
    printf("\n\n");

    printf("\tBOV_SB_SENTENCE_END\n");
    do_dump(ucs, bos, n, BOV_SB_SENTENCE_END);
    printf("\n\n");

    printf("================================\n");
    printf("END OF DUMPING GLYPHS AND BREAKS\n");
}


static ParagraphInfo  _header;
static ParagraphInfo  _footer;
static ParagraphInfo* _paragraphs;
static int            _nr_parags;

static void create_header(void)
{
    PLOGFONT lf = NULL;
    const char* text;
    int left_len_text;

    text = _news_cases[_curr_news].header;

    if (!(lf = CreateLogFontForMChar2UChar("utf-8"))) {
        _ERR_PRINTF("%s: failed to create logfont for utf-8 charset\n",
                __FUNCTION__);
        exit(1);
    }

    left_len_text = strlen(text);
    while (left_len_text > 0) {
        Uchar32* ucs;
        Uint16* bos;
        int consumed;
        int n;

        ucs = NULL;
        consumed = GetUCharsUntilParagraphBoundary(lf, text, left_len_text,
                WSR_NOWRAP, &ucs, &n);
        if (consumed > 0) {

            _DBG_PRINTF("%s: GetUCharsUntilParagraphBoundary: bytes: %d, glyphs: %d\n",
                __FUNCTION__, consumed, n);

            if (n > 0) {
                _header.ucs = ucs;
                _header.nr_ucs = n;

                int len_bos;
                bos = NULL;
                len_bos = UStrGetBreaks (LANGCODE_unknown, CTR_CAPITALIZE, WBR_NORMAL, LBP_NORMAL,
                    ucs, n, &bos);

                if (len_bos > 0) {
                    _header.bos = bos;
                    create_layout(&_header,
                        _news_cases[_curr_news].common_flags | _news_cases[_curr_news].header_flags,
                        _news_cases[_curr_news].header_font, _max_extent_header, TRUE);
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

            // keep header has only one paragraph
            break;
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

static void create_footer(void)
{
    PLOGFONT lf = NULL;
    const char* text;
    int left_len_text;

    text = _news_cases[_curr_news].footer;

    if (!(lf = CreateLogFontForMChar2UChar("utf-8"))) {
        _ERR_PRINTF("%s: failed to create logfont for utf-8 charset\n",
                __FUNCTION__);
        exit(1);
    }

    left_len_text = strlen(text);
    while (left_len_text > 0) {
        Uchar32* ucs;
        Uint16* bos;
        int consumed;
        int n;

        ucs = NULL;
        consumed = GetUCharsUntilParagraphBoundary(lf, text, left_len_text,
                WSR_NOWRAP, &ucs, &n);
        if (consumed > 0) {

            _DBG_PRINTF("%s: GetUCharsUntilParagraphBoundary: bytes: %d, glyphs: %d\n",
                __FUNCTION__, consumed, n);

            if (n > 0) {
                _footer.ucs = ucs;
                _footer.nr_ucs = n;

                int len_bos;
                bos = NULL;
                len_bos = UStrGetBreaks (LANGCODE_unknown, CTR_NONE, WBR_NORMAL, LBP_NORMAL,
                    ucs, n, &bos);

                if (len_bos > 0) {
                    _footer.bos = bos;
                    create_layout(&_footer,
                        _news_cases[_curr_news].common_flags | _news_cases[_curr_news].footer_flags,
                        _news_cases[_curr_news].footer_font, _max_extent_footer, TRUE);
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

            // keep footer has only one paragraph
            break;
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

#define MAX_LEN_TEXT        65535

static char _text_from_file[MAX_LEN_TEXT + 1];

static void create_paragraphs(void)
{
    char charset[100];
    PLOGFONT lf = NULL;
    const char* text;
    int left_len_text;

    text = get_text_case(_news_cases[_curr_news].text, _text_from_file, MAX_LEN_TEXT);

    strcpy (charset, "utf-8");
    if (text == _text_from_file) {
        get_charset_from_filename(_news_cases[_curr_news].text, charset);
    }

    if (!(lf = CreateLogFontForMChar2UChar(charset))) {
        _ERR_PRINTF("%s: failed to create logfont for charset: %s\n",
                __FUNCTION__, charset);
        exit(1);
    }

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

            _DBG_PRINTF("%s: GetUCharsUntilParagraphBoundary: bytes: %d, glyphs: %d\n",
                __FUNCTION__, consumed, n);

            if (n > 0) {
                _nr_parags++;
                _paragraphs = realloc(_paragraphs,
                        sizeof(ParagraphInfo) * _nr_parags);
                _paragraphs[_nr_parags - 1].ucs = ucs;
                _paragraphs[_nr_parags - 1].nr_ucs = n;

                int len_bos;
                bos = NULL;
                len_bos = UStrGetBreaks (LANGCODE_unknown,
                    CTR_NONE, WBR_NORMAL, LBP_STRICT,
                    ucs, n, &bos);

                if (len_bos > 0) {
                    //if (_nr_parags == 1)
                    //    dump_glyphs_and_breaks(text, ucs, bos, n);
                    _paragraphs[_nr_parags - 1].bos = bos;
                    create_layout(_paragraphs + _nr_parags - 1,
                        _news_cases[_curr_news].common_flags | _news_cases[_curr_news].text_flags,
                        _news_cases[_curr_news].text_font, _max_extent_text, FALSE);
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

static void destroy_news(void)
{
    if (_header.layout) {
        DestroyLayout(_header.layout);
        DestroyTextRuns(_header.textruns);
        free(_header.bos);
        free(_header.ucs);
        memset(&_header, 0, sizeof(ParagraphInfo));
    }

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

    if (_footer.layout) {
        DestroyLayout(_footer.layout);
        DestroyTextRuns(_footer.textruns);
        free(_footer.bos);
        free(_footer.ucs);
        memset(&_footer, 0, sizeof(ParagraphInfo));
    }
}

static void create_news(HWND hwnd)
{
    set_rectangles(hwnd);

    destroy_news();
    create_header();
    create_paragraphs();
    create_footer();
}

static int _header_x, _header_y;
static void render_header(HDC hdc)
{
    LAYOUTLINE* line = NULL;

    switch (_news_cases[_curr_news].common_flags & GRF_WRITING_MODE_MASK) {
    case GRF_WRITING_MODE_HORIZONTAL_BT:
        _header_x = _rc_header.left;
        _header_y = _rc_header.bottom;
        break;

    case GRF_WRITING_MODE_VERTICAL_RL:
        _header_x = _rc_header.right;
        _header_y = _rc_header.top;
        break;

    case GRF_WRITING_MODE_HORIZONTAL_TB:
    case GRF_WRITING_MODE_VERTICAL_LR:
    default:
        _header_x = _rc_header.left;
        _header_y = _rc_header.top;
        break;
    }

    SetTextColorInTextRuns(_header.textruns, 0, 4096, MakeRGB(255, 0, 0));

    // always draw only one line
    if ((line = LayoutNextLine(_header.layout, line, 0, 0, NULL, 0))) {
        DrawLayoutLine(hdc, line, _header_x, _header_y);
    }
}

static int _footer_x, _footer_y;
static void render_footer(HDC hdc)
{
    LAYOUTLINE* line = NULL;

    switch (_news_cases[_curr_news].common_flags & GRF_WRITING_MODE_MASK) {
    case GRF_WRITING_MODE_HORIZONTAL_BT:
        _footer_x = _rc_footer.left;
        _footer_y = _rc_footer.bottom;
        break;

    case GRF_WRITING_MODE_VERTICAL_RL:
        _footer_x = _rc_footer.right;
        _footer_y = _rc_footer.top;
        break;

    case GRF_WRITING_MODE_HORIZONTAL_TB:
    case GRF_WRITING_MODE_VERTICAL_LR:
    default:
        _footer_x = _rc_footer.left;
        _footer_y = _rc_footer.top;
        break;
    }

    SetTextColorInTextRuns(_footer.textruns, 0, 4096, MakeRGB(92, 92, 92));

    // always draw only one line
    if ((line = LayoutNextLine(_footer.layout, line, 0, 0, NULL, 0))) {
        DrawLayoutLine(hdc, line, _footer_x, _footer_y);
    }
}

static int _text_x, _text_y;
static void render_paragraphs_draw_line(HDC hdc)
{
    switch (_news_cases[_curr_news].common_flags & GRF_WRITING_MODE_MASK) {
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

    POINT pt = {_text_x, _text_y};
    int parag_x, parag_y;
    for (int i = 0; i < _nr_parags; i++) {
        LAYOUT* layout = _paragraphs[i].layout;
        LAYOUTLINE* line = NULL;
        RECT rc;
        int line_height = 0;
        int j = 0;

        SetTextColorInTextRuns(_paragraphs[i].textruns, 0, 4096, MakeRGB(32, 32, 32));

        parag_x = pt.x;
        parag_y = pt.y;

        while ((line = LayoutNextLine(layout, line, 0, 0, NULL, 0))) {
            SIZE sz;
            GetLayoutLineSize(line, &sz);
            sz.cx += 5;
            sz.cy += 5;
            if (sz.cy > line_height)
                line_height = sz.cy;

            DrawLayoutLine(hdc, line, pt.x, pt.y);

            GetLayoutLineRect(line, &pt.x, &pt.y, sz.cy, &rc);
            //SetPenColor(hdc, PIXEL_blue);
            //Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

            j++;
        }

        CalcLayoutBoundingRect(layout, 0, -1, line_height, parag_x, parag_y, &rc);

        switch (_news_cases[_curr_news].common_flags & GRF_WRITING_MODE_MASK) {
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
        create_news(hWnd);
        break;

    case MSG_PAINT: {
        HDC hdc;
        hdc = BeginPaint(hWnd);

        SelectClipRect(hdc, &_rc_header);
        render_header(hdc);

        SelectClipRect(hdc, &_rc_text);
        SetPenColor(hdc, PIXEL_green);
        Rectangle(hdc, _rc_text.left, _rc_text.top,
                _rc_text.right - 1, _rc_text.bottom - 1);
        render_paragraphs_draw_line(hdc);

        SelectClipRect(hdc, &_rc_footer);
        render_footer(hdc);

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
            _width_inc -= 2;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKRIGHT:
            _width_inc += 2;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKUP:
            _height_inc -= 10;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKDOWN:
            _height_inc += 10;
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
            create_news(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }

        return 0;
    }

    case MSG_CLOSE:
        destroy_news();
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
    // Free TrueType or OpenType fonts
    // download location:
    // https://github.com/adobe-fonts/
    { FONTFILE_PATH "font/SourceSansPro-BlackIt.ttf",
        "ttf-Source Sans Pro,SansSerif-cincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Black.ttf",
        "ttf-Source Sans Pro,SansSerif-crncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-BoldIt.ttf",
        "ttf-Source Sans Pro,SansSerif-bincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Bold.ttf",
        "ttf-Source Sans Pro,SansSerif-brncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-ExtraLightIt.ttf",
        "ttf-Source Sans Pro,SansSerif-eincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-ExtraLight.ttf",
        "ttf-Source Sans Pro,SansSerif-erncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Regular.ttf",
        "ttf-Source Sans Pro,SansSerif-rrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-It.ttf",
        "ttf-Source Sans Pro,SansSerif-rincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-LightIt.ttf",
        "ttf-Source Sans Pro,SansSerif-lincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Light.ttf",
        "ttf-Source Sans Pro,SansSerif-lrncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-SemiboldIt.ttf",
        "ttf-Source Sans Pro,SansSerif-dincnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },
    { FONTFILE_PATH "font/SourceSansPro-Semibold.ttf",
        "ttf-Source Sans Pro,SansSerif-drncnn-0-0-ISO8859-1,ISO8859-15,UTF-8" },

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
