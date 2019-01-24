/*
** getglyphsbyrules.c: test code for GetGlyphsByRules of MiniGUI 3.4.0
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
#include <minigui/control.h>

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0)) \
        && defined(_MGCHARSET_UNICODE)

#define MAX_LINE_LEN        1023
#define MAX_UCHARS          64

#define TOKEN_HAVE_NO_BREAK_OPPORTUNITY   "×"
#define TOKEN_HAVE_BREAK_OPPORTUNITY      "÷"

static int parse_one_test(const char* line, Uchar32* uc, Uint8* bt)
{
    int nr_ucs = 0;
    int nr_bts = 0;
    const char* current;

    if (!line)
        return 0;

    current = line;
    while (*current) {
        while (*current == ' ' ||  *current == '\t')
            current++;

        if (*current == '#') {
            return nr_ucs;
        }
        else if (strncmp(current, TOKEN_HAVE_NO_BREAK_OPPORTUNITY,
                strlen(TOKEN_HAVE_NO_BREAK_OPPORTUNITY)) == 0) {

            current += strlen(TOKEN_HAVE_NO_BREAK_OPPORTUNITY);

            bt[nr_bts] = BOV_BEFORE_NOTALLOWED;
            nr_bts++;
        }
        else if (strncmp(current, TOKEN_HAVE_BREAK_OPPORTUNITY,
                strlen(TOKEN_HAVE_BREAK_OPPORTUNITY)) == 0) {

            current += strlen(TOKEN_HAVE_BREAK_OPPORTUNITY);

            bt[nr_bts] = BOV_BEFORE_ALLOWED;
            nr_bts++;
        }
        else if (isxdigit(*current)) {
            int tmp = sscanf(current, "%x", uc + nr_ucs);
            if (tmp == 1) {
                nr_ucs ++;
                while (isxdigit(*current))
                    current++;
            }
            else {
                _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                return 0;
            }
        }
    }

    return nr_ucs;
}

static int uc32_to_utf8(Uchar32 c, char* outbuf)
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

static void do_check(const char* buff, Uchar32* ucs, Uint8* bts, int n,
        Glyph32* my_gvs, Uint8* my_bts, int my_n)
{
    printf("CASE: \n%s", buff);

    printf("PARSED: \n");

    if (bts[0] & BOV_BEFORE_BREAK_FLAG) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < n; i++) {
        printf (" %04x ", REAL_GLYPH(my_gvs[i]));

        if (bts[i + 1] & BOV_BEFORE_BREAK_FLAG) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n");

    printf("MINIGUI: \n");
    for (int i = 0; i < my_n; i++) {
        if (my_bts[i] & BOV_BEFORE_BREAK_FLAG) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }

        printf (" %04x ", REAL_GLYPH(my_gvs[i]));

        if (my_bts[i] & BOV_AFTER_BREAK_FLAG) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n\n");

}

static void do_test(PLOGFONT lf, FILE* fp)
{
    char buff[MAX_LINE_LEN + 1];
    Uchar32 ucs[MAX_UCHARS];
    Uint8 bts[MAX_UCHARS + 1];
    int n;

    Glyph32* my_gvs;
    Uint8* my_bts;
    int my_n;
    int cosumed;
    int line = 0;

    while (TRUE) {
        if (!fgets(buff, MAX_LINE_LEN, fp)) {
            return;
        }

        n = parse_one_test (buff, ucs, bts);
        if (n == 0) {
            continue;
        }

        char utf8[MAX_UCHARS*6 + 1];
        int len_utf8 = 0;
        for (int i = 0; i < n; i++) {
            len_utf8 += uc32_to_utf8(ucs[i], utf8 + len_utf8);
        }

        if (len_utf8 < MAX_UCHARS*6) {
            utf8[len_utf8] = '\0';
        }
        else {
            _ERR_PRINTF("%s: UTF8 string overflowed: %s\n", __FUNCTION__, buff);
            break;
        }

        my_gvs = NULL;
        my_bts = NULL;
        cosumed = GetGlyphsByRules(lf, utf8, len_utf8,
                LANGCODE_en, UCHAR_SCRIPT_LATIN,
                WSR_PRE_WRAP, CTR_CAPITALIZE,
                &my_gvs, &my_bts, &my_n);
        if (cosumed > 0) {
            printf("LINE: %d\n", line);
            line++;

            do_check(buff, ucs, bts, n, my_gvs, my_bts, my_n);

            if (my_gvs) free (my_gvs);
            if (my_bts) free (my_bts);
        }
        else {
            _ERR_PRINTF("%s: GetGlyphsByRules failed\n", __FUNCTION__);
            break;
        }
    }
}

int MiniGUIMain (int argc, const char* argv[])
{
    PLOGFONT lf = NULL;
    FILE* fp = NULL;

    fp = fopen("res/LineBreakTest.txt", "r");
    if (fp == NULL) {
        _DBG_PRINTF ("%s: Failed to open LineBreakTest.txt file\n",
                __FUNCTION__);
        goto error;
    }

    lf = CreateLogFontEx ("ttf", "helvetica", "UTF-8",
            FONT_WEIGHT_REGULAR,
            FONT_SLANT_ROMAN,
            FONT_FLIP_NONE,
            FONT_OTHER_NONE,
            FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
            10, 0);
    if (lf == NULL) {
        _DBG_PRINTF ("%s: Failed to create logfont\n",
                __FUNCTION__);
        goto error;
    }

    do_test (lf, fp);

error:
    if (lf) DestroyLogFont (lf);
    if (fp) fclose (fp);

    exit(1);
    return 0;
}

#else
#error "To test GetGlyphsByRules, please use MiniGUI 3.4.0 and enable support for UNICODE"
#endif /* checking version and features */
