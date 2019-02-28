/*
** getglyphsbyrules.c:
**  test code for GetGlyphsAndBreaks of MiniGUI 3.4.0
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

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0)) \
        && defined(_MGCHARSET_UNICODE)

#include "helpers.h"

int uc32_to_utf8(Uchar32 c, char* outbuf)
{
    int len = 0;
    int first;
    int i;

    if (c < 0x80) {
        first = 0;
        len = 1;
    }
    else if (c < 0x800) {
        first = 0xc0;
        len = 2;
    }
    else if (c < 0x10000) {
        first = 0xe0;
        len = 3;
    }
    else if (c < 0x200000) {
        first = 0xf0;
        len = 4;
    }
    else if (c < 0x4000000) {
        first = 0xf8;
        len = 5;
    }
    else {
        first = 0xfc;
        len = 6;
    }

    if (outbuf) {
        for (i = len - 1; i > 0; --i) {
            outbuf[i] = (c & 0x3f) | 0x80;
            c >>= 6;
        }
        outbuf[0] = c | first;
    }

    return len;
}

static const char* _gc_names[] = {
    "UCHAR_CATEGORY_CONTROL",
    "UCHAR_CATEGORY_FORMAT",
    "UCHAR_CATEGORY_UNASSIGNED",
    "UCHAR_CATEGORY_PRIVATE_USE",
    "UCHAR_CATEGORY_SURROGATE",
    "UCHAR_CATEGORY_LOWERCASE_LETTER",
    "UCHAR_CATEGORY_MODIFIER_LETTER",
    "UCHAR_CATEGORY_OTHER_LETTER",
    "UCHAR_CATEGORY_TITLECASE_LETTER",
    "UCHAR_CATEGORY_UPPERCASE_LETTER",
    "UCHAR_CATEGORY_SPACING_MARK",
    "UCHAR_CATEGORY_ENCLOSING_MARK",
    "UCHAR_CATEGORY_NON_SPACING_MARK",
    "UCHAR_CATEGORY_DECIMAL_NUMBER",
    "UCHAR_CATEGORY_LETTER_NUMBER",
    "UCHAR_CATEGORY_OTHER_NUMBER",
    "UCHAR_CATEGORY_CONNECT_PUNCTUATION",
    "UCHAR_CATEGORY_DASH_PUNCTUATION",
    "UCHAR_CATEGORY_CLOSE_PUNCTUATION",
    "UCHAR_CATEGORY_FINAL_PUNCTUATION",
    "UCHAR_CATEGORY_INITIAL_PUNCTUATION",
    "UCHAR_CATEGORY_OTHER_PUNCTUATION",
    "UCHAR_CATEGORY_OPEN_PUNCTUATION",
    "UCHAR_CATEGORY_CURRENCY_SYMBOL",
    "UCHAR_CATEGORY_MODIFIER_SYMBOL",
    "UCHAR_CATEGORY_MATH_SYMBOL",
    "UCHAR_CATEGORY_OTHER_SYMBOL",
    "UCHAR_CATEGORY_LINE_SEPARATOR",
    "UCHAR_CATEGORY_PARAGRAPH_SEPARATOR",
    "UCHAR_CATEGORY_SPACE_SEPARATOR",
};

const char* get_general_category_name(UCharGeneralCategory gc)
{
    return _gc_names[gc];
}

static const char* _bt_names[] = {
    "UCHAR_BREAK_MANDATORY",
    "UCHAR_BREAK_CARRIAGE_RETURN",
    "UCHAR_BREAK_LINE_FEED",
    "UCHAR_BREAK_COMBINING_MARK",
    "UCHAR_BREAK_SURROGATE",
    "UCHAR_BREAK_ZERO_WIDTH_SPACE",
    "UCHAR_BREAK_INSEPARABLE",
    "UCHAR_BREAK_NON_BREAKING_GLUE",
    "UCHAR_BREAK_CONTINGENT",
    "UCHAR_BREAK_SPACE",
    "UCHAR_BREAK_AFTER",
    "UCHAR_BREAK_BEFORE",
    "UCHAR_BREAK_BEFORE_AND_AFTER",
    "UCHAR_BREAK_HYPHEN",
    "UCHAR_BREAK_NON_STARTER",
    "UCHAR_BREAK_OPEN_PUNCTUATION",
    "UCHAR_BREAK_CLOSE_PUNCTUATION",
    "UCHAR_BREAK_QUOTATION",
    "UCHAR_BREAK_EXCLAMATION",
    "UCHAR_BREAK_IDEOGRAPHIC",
    "UCHAR_BREAK_NUMERIC",
    "UCHAR_BREAK_INFIX_SEPARATOR",
    "UCHAR_BREAK_SYMBOL",
    "UCHAR_BREAK_ALPHABETIC",
    "UCHAR_BREAK_PREFIX",
    "UCHAR_BREAK_POSTFIX",
    "UCHAR_BREAK_COMPLEX_CONTEXT",
    "UCHAR_BREAK_AMBIGUOUS",
    "UCHAR_BREAK_UNKNOWN",
    "UCHAR_BREAK_NEXT_LINE",
    "UCHAR_BREAK_WORD_JOINER",
    "UCHAR_BREAK_HANGUL_L_JAMO",
    "UCHAR_BREAK_HANGUL_V_JAMO",
    "UCHAR_BREAK_HANGUL_T_JAMO",
    "UCHAR_BREAK_HANGUL_LV_SYLLABLE",
    "UCHAR_BREAK_HANGUL_LVT_SYLLABLE",
    "UCHAR_BREAK_CLOSE_PARANTHESIS",
    "UCHAR_BREAK_CONDITIONAL_JAPANESE_STARTER",
    "UCHAR_BREAK_HEBREW_LETTER",
    "UCHAR_BREAK_REGIONAL_INDICATOR",
    "UCHAR_BREAK_EMOJI_BASE",
    "UCHAR_BREAK_EMOJI_MODIFIER",
    "UCHAR_BREAK_ZERO_WIDTH_JOINER"
};

const char* get_break_type_name(UCharBreakType bt)
{
    return _bt_names[bt];
}

#else
#error "To build this file, please use MiniGUI 3.4.0 and enable support for UNICODE"
#endif /* checking version and features */