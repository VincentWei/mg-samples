///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
** complexshapingengine.c:
**
**  Test code for Complex Shaping Engine of MiniGUI 3.4.0.
**  The following APIs are covered:
**
**      CreateLogFontForMChar2UChar
**      GetUCharsUntilParagraphBoundary
**      UStrGetBreaks
**      UBidiGetParagraphEmbeddingLevelsAlt
**      CreateTextRuns
**      InitComplexShapingEngine
**      CreateLayout
**      LayoutNextLine
**      DrawShapedGlyph
**      DrawLayoutLine
**      DestroyLayout
**      DestroyTextRuns
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

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0)) \
        && defined(_MGCHARSET_UNICODE)

#include "helpers.h"

static const char* _text_cases[] = {
    "file:res/en-iso8859-1.txt",

    "1234567890",

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

    "Source Han Serif is the serif-style typeface family companion to Source Han Sans. The Chinese glyphs, both simplified and traditional, were designed by partner type foundry Changzhou SinoType. The Simplified Chinese fonts support the GB 18030 standard, along with China’s list of 8,105 hanzi (Tōngyòng Guīfàn Hànzìbiǎo, which includes 199 hanzi that are outside the scope of the GB 18030 standard). The Traditional Chinese fonts support the Big 5 standard, and glyph shapes adhere to the Taiwan Ministry of Education standard. Learn more about how these fonts were created.",

    "An opening bracket or quote at the start of the line or a closing bracket or quote at the end line hangs:\n(12)\n'345'\n\"67890\"",

    "     (12) '345'\n \"67890\"  　　",

    "   12345678，\n123456789。",

    "这是一些汉字 and some Latin و کمی خط عربی และตัวอย่างการเขียนภาษาไทย\n"
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

    "file:res/ar-iso8859-6.txt",
    "file:res/he-iso8859-8.txt",
    "file:res/en-iso8859-15.txt",
    "file:res/zh-gb2312-0.txt",
    "file:res/zh-gbk.txt",
    "file:res/zh-big5.txt",
    "file:res/ko-euc-kr.txt",
    "file:res/ja-jisx0208-1.txt",
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

static RENDER_RULE _writing_mode_cases [] = {
    { GRF_WRITING_MODE_HORIZONTAL_TB,
        "GRF_WRITING_MODE_HORIZONTAL_TB" },
    { GRF_WRITING_MODE_HORIZONTAL_BT,
        "GRF_WRITING_MODE_HORIZONTAL_BT" },
    { GRF_WRITING_MODE_VERTICAL_RL,
        "GRF_WRITING_MODE_VERTICAL_RL" },
    { GRF_WRITING_MODE_VERTICAL_LR,
        "GRF_WRITING_MODE_VERTICAL_LR" },
};

static RENDER_RULE _text_ort_cases [] = {
    { GRF_TEXT_ORIENTATION_MIXED,
        "GRF_TEXT_ORIENTATION_MIXED" },
    { GRF_TEXT_ORIENTATION_UPRIGHT,
        "GRF_TEXT_ORIENTATION_UPRIGHT" },
    { GRF_TEXT_ORIENTATION_SIDEWAYS,
        "GRF_TEXT_ORIENTATION_SIDEWAYS" },
    { GRF_TEXT_ORIENTATION_UPSIDE_DOWN,
        "GRF_TEXT_ORIENTATION_UPSIDE_DOWN" },
    { GRF_TEXT_ORIENTATION_SIDEWAYS_LEFT,
        "GRF_TEXT_ORIENTATION_SIDEWAYS_LEFT" },
    { GRF_TEXT_ORIENTATION_AUTO,
        "GRF_TEXT_ORIENTATION_AUTO" },
};

static RENDER_RULE _indent_cases [] = {
    { GRF_INDENT_NONE,
        "GRF_INDENT_NONE" },
    { GRF_INDENT_FIRST_LINE,
        "GRF_INDENT_FIRST_LINE" },
    { GRF_INDENT_HANGING,
        "GRF_INDENT_HANGING" },
};

static RENDER_RULE _line_extent_cases [] = {
    { GRF_LINE_EXTENT_FIXED,
        "GRF_LINE_EXTENT_FIXED" },
    { GRF_LINE_EXTENT_VARIABLE,
        "GRF_LINE_EXTENT_VARIABLE" },
};

static RENDER_RULE _overflow_wrap_cases [] = {
    { GRF_OVERFLOW_WRAP_NORMAL,
        "GRF_OVERFLOW_WRAP_NORMAL" },
    { GRF_OVERFLOW_WRAP_BREAK_WORD,
        "GRF_OVERFLOW_WRAP_BREAK_WORD" },
    { GRF_OVERFLOW_WRAP_ANYWHERE,
        "GRF_OVERFLOW_WRAP_ANYWHERE" },
};

static RENDER_RULE _overflow_ellipsize_cases [] = {
    { GRF_OVERFLOW_ELLIPSIZE_NONE,
        "GRF_OVERFLOW_ELLIPSIZE_NONE" },
    { GRF_OVERFLOW_ELLIPSIZE_START,
        "GRF_OVERFLOW_ELLIPSIZE_START" },
    { GRF_OVERFLOW_ELLIPSIZE_MIDDLE,
        "GRF_OVERFLOW_ELLIPSIZE_MIDDLE" },
    { GRF_OVERFLOW_ELLIPSIZE_END,
        "GRF_OVERFLOW_ELLIPSIZE_END" },
};

static RENDER_RULE _align_cases [] = {
    { GRF_ALIGN_START,
        "GRF_ALIGN_START" },
    { GRF_ALIGN_END,
        "GRF_ALIGN_END" },
    { GRF_ALIGN_LEFT,
        "GRF_ALIGN_LEFT" },
    { GRF_ALIGN_RIGHT,
        "GRF_ALIGN_RIGHT" },
    { GRF_ALIGN_CENTER,
        "GRF_ALIGN_CENTER" },
    { GRF_ALIGN_JUSTIFY,
        "GRF_ALIGN_JUSTIFY" },
};

static RENDER_RULE _text_justify_cases [] = {
    { GRF_TEXT_JUSTIFY_AUTO,
        "GRF_TEXT_JUSTIFY_AUTO" },
    { GRF_TEXT_JUSTIFY_INTER_WORD,
        "GRF_TEXT_JUSTIFY_INTER_WORD" },
    { GRF_TEXT_JUSTIFY_INTER_CHAR,
        "GRF_TEXT_JUSTIFY_INTER_CHAR" },
};

static RENDER_RULE _hanging_punc_cases [] = {
    { GRF_HANGING_PUNC_NONE,
        "GRF_HANGING_PUNC_NONE" },
    { GRF_HANGING_PUNC_FORCE_END,
        "GRF_HANGING_PUNC_FORCE_END" },
    { GRF_HANGING_PUNC_ALLOW_END,
        "GRF_HANGING_PUNC_ALLOW_END" },
    { GRF_HANGING_PUNC_OPEN,
        "GRF_HANGING_PUNC_OPEN" },
    { GRF_HANGING_PUNC_FORCE_END | GRF_HANGING_PUNC_OPEN,
        "GRF_HANGING_PUNC_FORCE_END | GRF_HANGING_PUNC_OPEN" },
    { GRF_HANGING_PUNC_ALLOW_END | GRF_HANGING_PUNC_OPEN,
        "GRF_HANGING_PUNC_ALLOW_END | GRF_HANGING_PUNC_OPEN" },
    { GRF_HANGING_PUNC_CLOSE,
        "GRF_HANGING_PUNC_CLOSE" },
    { GRF_HANGING_PUNC_FORCE_END | GRF_HANGING_PUNC_CLOSE,
        "GRF_HANGING_PUNC_FORCE_END | GRF_HANGING_PUNC_CLOSE" },
    { GRF_HANGING_PUNC_ALLOW_END | GRF_HANGING_PUNC_CLOSE,
        "GRF_HANGING_PUNC_ALLOW_END | GRF_HANGING_PUNC_CLOSE" },
    { GRF_HANGING_PUNC_OPEN | GRF_HANGING_PUNC_CLOSE,
        "GRF_HANGING_PUNC_OPEN | GRF_HANGING_PUNC_CLOSE" },
    { GRF_HANGING_PUNC_FORCE_END | GRF_HANGING_PUNC_OPEN | GRF_HANGING_PUNC_CLOSE,
        "GRF_HANGING_PUNC_FORCE_END | GRF_HANGING_PUNC_OPEN | GRF_HANGING_PUNC_CLOSE" },
    { GRF_HANGING_PUNC_ALLOW_END | GRF_HANGING_PUNC_OPEN | GRF_HANGING_PUNC_CLOSE,
        "GRF_HANGING_PUNC_ALLOW_END | GRF_HANGING_PUNC_OPEN | GRF_HANGING_PUNC_CLOSE" },
};

static RENDER_RULE _spaces_cases [] = {
    { GRF_SPACES_KEEP,
        "GRF_SPACES_KEEP" },
    { GRF_SPACES_REMOVE_START,
        "GRF_SPACES_REMOVE_START" },
    { GRF_SPACES_REMOVE_END,
        "GRF_SPACES_REMOVE_END" },
    { GRF_SPACES_HANGE_END,
        "GRF_SPACES_HANGE_END" },
    { GRF_SPACES_REMOVE_START | GRF_SPACES_REMOVE_END,
        "GRF_SPACES_REMOVE_START | GRF_SPACES_REMOVE_END" },
    { GRF_SPACES_REMOVE_START | GRF_SPACES_HANGE_END,
        "GRF_SPACES_REMOVE_START | GRF_SPACES_HANGE_END" },
};

static BOOL _limited;
static int _curr_text;
static int _curr_wsr;
static int _curr_ctr;
static int _curr_wbr;
static int _curr_lbp;
static int _curr_writing_mode;
static int _curr_text_ort;
static int _curr_line_extent;
static int _curr_indent;
static int _curr_overflow_wrap;
static int _curr_overflow_ellipsize;
static int _curr_align;
static int _curr_text_justify;
static int _curr_hanging_punc;
static int _curr_spaces;
static int _letter_spacing = 0;
static int _word_spacing = 0;
static int _tab_size = 100;
static RECT _rc_output = {400, 5, 1024 - 5, 500};

typedef struct _TOGGLE_ITEM {
    int     scancode;
    int*    current;
    int     upper;
} TOGGLE_ITEM;

static TOGGLE_ITEM _toggle_items[] = {
    { SCANCODE_F1,      &_curr_text,                TABLESIZE(_text_cases) },
    { SCANCODE_F2,      &_curr_wsr,                 TABLESIZE(_wsr_cases) },
    { SCANCODE_F3,      &_curr_ctr,                 TABLESIZE(_ctr_cases) },
    { SCANCODE_F4,      &_curr_wbr,                 TABLESIZE(_wbr_cases) },
    { SCANCODE_F5,      &_curr_lbp,                 TABLESIZE(_lbp_cases) },
    { SCANCODE_F6,      &_curr_writing_mode,        TABLESIZE(_writing_mode_cases) },
    { SCANCODE_F7,      &_curr_text_ort,            TABLESIZE(_text_ort_cases) },
    { SCANCODE_F8,      &_curr_line_extent,         TABLESIZE(_line_extent_cases) },
    { SCANCODE_F9,      &_curr_indent,              TABLESIZE(_indent_cases) },
    { SCANCODE_F10,     &_curr_overflow_wrap,       TABLESIZE(_overflow_wrap_cases) },
    { SCANCODE_F11,     &_curr_overflow_ellipsize,  TABLESIZE(_overflow_ellipsize_cases) },
    { SCANCODE_F12,     &_curr_align,               TABLESIZE(_align_cases) },
    { SCANCODE_1,       &_curr_text_justify,        TABLESIZE(_text_justify_cases) },
    { SCANCODE_2,       &_curr_hanging_punc,        TABLESIZE(_hanging_punc_cases) },
    { SCANCODE_3,       &_curr_spaces,              TABLESIZE(_spaces_cases) },
};

static void randomize_items(void)
{
    int i;

    for (i = 0; i < TABLESIZE(_toggle_items); i++) {
        TOGGLE_ITEM* item = _toggle_items + i;
        *(item->current) = random() % item->upper;
    }

    _limited = !_limited;
}

static BOOL toggle_item(int scancode, DWORD keystatus, BOOL rdm)
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

static void output_rules(HDC hdc)
{
    char buf[64];
    int _text_y = 5;

    TextOut(hdc, 5,   _text_y,     "LIMITED");
    TextOut(hdc, 100, _text_y, _limited?"YES":"NO");
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "TEXT(F1)");
    TextOutLen(hdc, 100, _text_y, _text_cases[_curr_text], 10);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "WSR(F2)");
    TextOut(hdc, 100, _text_y, _wsr_cases[_curr_wsr].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "CTR(F3)");
    TextOut(hdc, 100, _text_y, _ctr_cases[_curr_ctr].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "WBR(F4)");
    TextOut(hdc, 100, _text_y, _wbr_cases[_curr_wbr].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "LBP(F5)");
    TextOut(hdc, 100, _text_y, _lbp_cases[_curr_lbp].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "WRT(F6)");
    TextOut(hdc, 100, _text_y, _writing_mode_cases[_curr_writing_mode].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "ORT(F7)");
    TextOut(hdc, 100, _text_y, _text_ort_cases[_curr_text_ort].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "EXT(F8)");
    TextOut(hdc, 100, _text_y, _line_extent_cases[_curr_line_extent].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "IDT(F9)");
    TextOut(hdc, 100, _text_y, _indent_cases[_curr_indent].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "OVF(F10)");
    TextOut(hdc, 100, _text_y, _overflow_wrap_cases[_curr_overflow_wrap].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "OVF(F11)");
    TextOut(hdc, 100, _text_y, _overflow_ellipsize_cases[_curr_overflow_ellipsize].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "ALG(F12)");
    TextOut(hdc, 100, _text_y, _align_cases[_curr_align].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "JST(1)");
    TextOut(hdc, 100, _text_y, _text_justify_cases[_curr_text_justify].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "HNG(2)");
    TextOut(hdc, 100, _text_y, _hanging_punc_cases[_curr_hanging_punc].desc);
    _text_y += 20;

    TextOut(hdc, 5,   _text_y,     "SPC(3)");
    TextOut(hdc, 100, _text_y, _spaces_cases[_curr_spaces].desc);
    _text_y += 20;

    snprintf(buf, 63, "%d", _letter_spacing);
    TextOut(hdc, 5,   _text_y,     "LTRSPC");
    TextOut(hdc, 100, _text_y, buf);
    _text_y += 20;

    snprintf(buf, 63, "%d", _word_spacing);
    TextOut(hdc, 5,   _text_y,     "WORDSPC");
    TextOut(hdc, 100, _text_y, buf);
    _text_y += 20;

    snprintf(buf, 63, "%d", _tab_size);
    TextOut(hdc, 5,   _text_y,     "TABSIZE");
    TextOut(hdc, 100, _text_y, buf);
    _text_y += 20;
}

static int _curr_font = 0;
static char* _font_cases [] = {
    "ttf-SansSerif,KacstScreen,Loma,Lohit,Saab-rrnnns-*-12-UTF-8",
    "ttf-SansSerif,KacstScreen,Loma,Lohit,Saab-rrnnns-*-14-UTF-8",
    "ttf-SansSerif,KacstScreen,Loma,Lohit,Saab-rrnnns-*-16-UTF-8",
    "ttf-SansSerif,KacstScreen,Loma,Lohit,Saab-rrnnns-*-20-UTF-8",
};

typedef struct _ParagraphInfo {
    Uchar32*        ucs;
    BreakOppo*      bos;
    TEXTRUNS*   textruns;
    LAYOUT*     layout;
    int             nr_ucs;
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

static void create_layout(ParagraphInfo* p)
{
    Uint32 render_flags;
    int max_extent;

    render_flags =
            _writing_mode_cases[_curr_writing_mode].rule |
            _text_ort_cases[_curr_text_ort].rule |
            _line_extent_cases[_curr_line_extent].rule |
            _indent_cases[_curr_indent].rule |
            _overflow_wrap_cases[_curr_overflow_wrap].rule |
            _overflow_ellipsize_cases[_curr_overflow_ellipsize].rule |
            _align_cases[_curr_align].rule |
            _text_justify_cases[_curr_text_justify].rule |
            _hanging_punc_cases[_curr_hanging_punc].rule |
            _spaces_cases[_curr_spaces].rule;

    if (_limited) {
        switch (_writing_mode_cases[_curr_writing_mode].rule) {
        case GRF_WRITING_MODE_HORIZONTAL_TB:
        case GRF_WRITING_MODE_HORIZONTAL_BT:
            max_extent = RECTW(_rc_output);
            break;
        default:
            max_extent = RECTH(_rc_output);
            break;
        }
    }
    else {
        max_extent = -1;
    }

    p->textruns = CreateTextRuns(p->ucs, p->nr_ucs,
            LANGCODE_unknown, BIDI_PGDIR_LTR,
            _font_cases[_curr_font], MakeRGB(0, 0, 0), 0, p->bos + 1);

    if (p->textruns) {
        if (!InitBasicShapingEngine(p->textruns)) {
            _ERR_PRINTF("%s: InitBasicShapingEngine returns FALSE\n",
                    __FUNCTION__);
            exit(1);
        }

        p->layout = CreateLayout(p->textruns,
                render_flags,
                p->bos + 1, TRUE, max_extent, 100, _letter_spacing, _word_spacing, _tab_size, NULL, 0);
        if (p->layout == NULL) {
            _ERR_PRINTF("%s: CreateLayout returns NULL\n", __FUNCTION__);
            exit(1);
        }

        LAYOUTLINE* line = NULL;
        int max_extent = 100;
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


static void create_paragraphs(void)
{
    char charset[100];
    PLOGFONT lf = NULL;
    const char* text;
    int left_len_text;

    text = get_text_case(_text_cases[_curr_text], _text_from_file, 4096);

    strcpy (charset, "utf-8");
    if (text == _text_from_file) {
        get_charset_from_filename(_text_cases[_curr_text], charset);
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
                (Uint8)_wsr_cases[_curr_wsr].rule,
                &ucs, &n);
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
                    (Uint8)_ctr_cases[_curr_ctr].rule,
                    (Uint8)_wbr_cases[_curr_wbr].rule,
                    (Uint8)_lbp_cases[_curr_lbp].rule,
                    ucs, n, &bos);

                if (len_bos > 0) {
                    //dump_glyphs_and_breaks(text, ucs, bos, n);
                    _paragraphs[_nr_parags - 1].bos = bos;
                    create_layout(_paragraphs + _nr_parags - 1);
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

static void render_paragraphs_draw_glphy(HDC hdc)
{
    switch (_writing_mode_cases[_curr_writing_mode].rule) {
    case GRF_WRITING_MODE_HORIZONTAL_BT:
        _text_x = _rc_output.left;
        _text_y = _rc_output.bottom;
        break;

    case GRF_WRITING_MODE_VERTICAL_RL:
        _text_x = _rc_output.right;
        _text_y = _rc_output.top;
        break;

    case GRF_WRITING_MODE_HORIZONTAL_TB:
    case GRF_WRITING_MODE_VERTICAL_LR:
    default:
        _text_x = _rc_output.left;
        _text_y = _rc_output.top;
        break;

    }

    SetMapMode(hdc, MM_ANISOTROPIC);

    POINT pt = {_text_x, _text_y};
    for (int i = 0; i < _nr_parags; i++) {
        LAYOUT* layout = _paragraphs[i].layout;
        LAYOUTLINE* line = NULL;
        int j = 0;

        _MG_PRINTF("%s: rendering paragraph: %d\n",
            __FUNCTION__, i);

        SetViewportOrg(hdc, &pt);
        while ((line = LayoutNextLine(layout, line, 0, 0,
                DrawShapedGlyph, (GHANDLE)hdc))) {

            SIZE sz;
            GetLayoutLineSize(line, &sz);
            sz.cx += 5;
            sz.cy += 5;

            _MG_PRINTF("%s: rendered line by calling DrawShapedGlyph: %d\n",
                __FUNCTION__, j);

            j++;

            switch (_writing_mode_cases[_curr_writing_mode].rule) {
            case GRF_WRITING_MODE_HORIZONTAL_TB:
                pt.y += sz.cy;
                break;

            case GRF_WRITING_MODE_HORIZONTAL_BT:
                pt.y -= sz.cy;
                break;

            case GRF_WRITING_MODE_VERTICAL_RL:
                pt.x -= sz.cy;
                break;

            case GRF_WRITING_MODE_VERTICAL_LR:
                pt.x += sz.cy;
                break;
            }

            SetViewportOrg(hdc, &pt);
        }

        switch (_writing_mode_cases[_curr_writing_mode].rule) {
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

static void render_paragraphs_draw_line(HDC hdc)
{
    switch (_writing_mode_cases[_curr_writing_mode].rule) {
    case GRF_WRITING_MODE_HORIZONTAL_BT:
        _text_x = _rc_output.left;
        _text_y = _rc_output.bottom;
        break;

    case GRF_WRITING_MODE_VERTICAL_RL:
        _text_x = _rc_output.right;
        _text_y = _rc_output.top;
        break;

    case GRF_WRITING_MODE_HORIZONTAL_TB:
    case GRF_WRITING_MODE_VERTICAL_LR:
    default:
        _text_x = _rc_output.left;
        _text_y = _rc_output.top;
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
            SetPenColor(hdc, PIXEL_blue);
            Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

            j++;
        }

        CalcLayoutBoundingRect(layout, 0, -1, line_height, parag_x, parag_y, &rc);
        SetPenColor(hdc, PIXEL_green);
        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

        switch (_writing_mode_cases[_curr_writing_mode].rule) {
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
    static int paint_count;

    switch (message) {
    case MSG_CREATE:
        create_paragraphs();
        break;

    case MSG_IDLE:
        if (_auto_test_runs > 0) {
            if (_nr_test_runs >= _auto_test_runs)
                exit(0);

            _nr_test_runs++;
            randomize_items();
            create_paragraphs();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case MSG_PAINT: {
        HDC hdc;
        hdc = BeginPaint(hWnd);
        output_rules(hdc);

        SetPenColor(hdc, PIXEL_red);
        Rectangle(hdc, _rc_output.left, _rc_output.top,
            _rc_output.right, _rc_output.bottom);
        if (paint_count & 1) {
            render_paragraphs_draw_glphy(hdc);
        }
        else {
            render_paragraphs_draw_line(hdc);
        }
        paint_count++;
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
            _limited = !_limited;
            repaint = TRUE;
            break;

        case SCANCODE_ENTER:
            _curr_font++;
            _curr_font %= TABLESIZE(_font_cases);
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKLEFT:
            _rc_output.right -= 5;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKRIGHT:
            _rc_output.right += 5;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKUP:
            _rc_output.bottom -= 20;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKDOWN:
            _rc_output.bottom += 20;
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
            if (toggle_item((int)wParam, (DWORD)lParam, FALSE))
                repaint = TRUE;
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
    pCreateInfo->spCaption = "SPACE: limit or not; F1~F12: change text/rules; CURSORS: change size";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = 0;
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MyMainWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 1024;
    pCreateInfo->by = 768;
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
    { FONTFILE_PATH "font/SourceHanSans-Regular.ttc",
        "ttf-Source Han Sans,SansSerif-rrncnn-0-0-UTF-8" },
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
#error "To test Complex Shaping Engine, please use MiniGUI 3.4.0 and enable support for UNICODE"
#endif /* checking version and features */
