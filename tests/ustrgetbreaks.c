/*
** ustrgetbreaks.c:
**
**  Test code for UStrGetBreaks of MiniGUI 3.4.0
**  The following APIs are covered:
**
**      UStrGetBreaks
**      UCharGetCategory
**      UCharGetBreakType
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

#include "helpers.h"

#define MAX_LINE_LEN        4096
#define MAX_UCHARS          128

#define TOKEN_HAVE_NO_BREAK_OPPORTUNITY "×"
#define TOKEN_HAVE_BREAK_OPPORTUNITY    "÷"

#define BREAK_NOTALLOWED                0
#define BREAK_ALLOWED                   1

static int parse_one_case(const char* line, Uchar32* ucs, Uint8* bos)
{
    int nr_ucs = 0;
    int nr_bos = 0;
    const char* current;

    if (!line)
        return 0;

    current = line;
    while (*current) {
        while (*current == ' ' ||  *current == '\t')
            current++;

        if (*current == '#' || *current == '\n' || *current == '\r') {
            return nr_ucs;
        }
        else if (strncmp(current, TOKEN_HAVE_NO_BREAK_OPPORTUNITY,
                strlen(TOKEN_HAVE_NO_BREAK_OPPORTUNITY)) == 0) {

            current += strlen(TOKEN_HAVE_NO_BREAK_OPPORTUNITY);

            bos[nr_bos] = BREAK_NOTALLOWED;
            nr_bos++;
        }
        else if (strncmp(current, TOKEN_HAVE_BREAK_OPPORTUNITY,
                strlen(TOKEN_HAVE_BREAK_OPPORTUNITY)) == 0) {

            current += strlen(TOKEN_HAVE_BREAK_OPPORTUNITY);

            bos[nr_bos] = BREAK_ALLOWED;
            nr_bos++;
        }
        else if (isxdigit(*current)) {
            int tmp = sscanf(current, "%X", ucs + nr_ucs);
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

typedef void (* CB_CHECK_RESULT) (const Uchar32* ucs, const Uint8* bos, int n,
        const Uchar32* my_ucs, const Uint16* my_bos, int my_n);

static CB_CHECK_RESULT _cb_check_result;

static void check_result_lb(const Uchar32* ucs, const Uint8* bos, int n,
        const Uchar32* my_ucs, const Uint16* my_bos, int my_n)
{
    printf("TEST CASE: \n");

    if (bos[0] == BREAK_ALLOWED) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < n; i++) {
        printf (" %04X ", ucs[i]);

        if (bos[i + 1] == BREAK_ALLOWED) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n");

    printf("MINIGUI: \n");
    if (my_bos[0] & BOV_LB_BREAK_FLAG) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < my_n; i++) {
        printf (" %04X ", my_ucs[i]);

        if (my_bos[i + 1] & BOV_LB_BREAK_FLAG) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n\n");

    BOOL ok = TRUE;
    if (n != my_n) {
        ok = FALSE;
    }
    else {
        for (int i = 0; i < n; i++) {
            if (((bos[i] == BREAK_ALLOWED) && !(my_bos[i] & BOV_LB_BREAK_FLAG)) ||
                    ((bos[i] == BREAK_NOTALLOWED) && (my_bos[i] & BOV_LB_BREAK_FLAG))) {
                ok = FALSE;
                break;
            }
        }
    }

    if (!ok) {
        _ERR_PRINTF("Bad implmentation for line breaking!\n");
        exit(1);
    }
}

static void check_result_gb(const Uchar32* ucs, const Uint8* bos, int n,
        const Uchar32* my_ucs, const Uint16* my_bos, int my_n)
{
    printf("TEST CASE: \n");

    if (bos[0] == BREAK_ALLOWED) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < n; i++) {
        printf (" %04X ", ucs[i]);

        if (bos[i + 1] == BREAK_ALLOWED) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n");

    printf("MINIGUI: \n");
    if (my_bos[0] & BOV_GB_CHAR_BREAK) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < my_n; i++) {
        printf (" %04X ", my_ucs[i]);

        if (my_bos[i + 1] & BOV_GB_CHAR_BREAK) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n\n");

    BOOL ok = TRUE;
    if (n != my_n) {
        ok = FALSE;
    }
    else {
        for (int i = 0; i < n; i++) {
            if (((bos[i] == BREAK_ALLOWED) && !(my_bos[i] & BOV_GB_CHAR_BREAK)) ||
                    ((bos[i] == BREAK_NOTALLOWED) && (my_bos[i] & BOV_GB_CHAR_BREAK))) {
                ok = FALSE;
                break;
            }
        }
    }

    if (!ok) {
        _ERR_PRINTF("Bad implmentation for grapheme breaking!\n");
        exit(1);
    }
}

static void check_result_wb(const Uchar32* ucs, const Uint8* bos, int n,
        const Uchar32* my_ucs, const Uint16* my_bos, int my_n)
{
    printf("TEST CASE: \n");

    if (bos[0] == BREAK_ALLOWED) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < n; i++) {
        printf (" %04X ", ucs[i]);

        if (bos[i + 1] == BREAK_ALLOWED) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n");

    printf("MINIGUI: \n");
    if (my_bos[0] & BOV_WB_WORD_BOUNDARY) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < my_n; i++) {
        printf (" %04X ", my_ucs[i]);

        if (my_bos[i + 1] & BOV_WB_WORD_BOUNDARY) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n\n");

    BOOL ok = TRUE;
    if (n != my_n) {
        ok = FALSE;
    }
    else {
        for (int i = 0; i < n; i++) {
            if (((bos[i] == BREAK_ALLOWED) && !(my_bos[i] & BOV_WB_WORD_BOUNDARY)) ||
                    ((bos[i] == BREAK_NOTALLOWED) && (my_bos[i] & BOV_WB_WORD_BOUNDARY))) {
                ok = FALSE;
                break;
            }
        }
    }

    if (!ok) {
        _ERR_PRINTF("Bad implmentation for word breaking!\n");
        exit(1);
    }
}

static void check_result_sb(const Uchar32* ucs, const Uint8* bos, int n,
        const Uchar32* my_ucs, const Uint16* my_bos, int my_n)
{
    printf("TEST CASE: \n");

    if (bos[0] == BREAK_ALLOWED) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < n; i++) {
        printf (" %04X ", ucs[i]);

        if (bos[i + 1] == BREAK_ALLOWED) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n");

    printf("MINIGUI: \n");
    if (my_bos[0] & BOV_SB_SENTENCE_BOUNDARY) {
        printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
    }
    else {
        printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
    }

    for (int i = 0; i < my_n; i++) {
        printf (" %04X ", my_ucs[i]);

        if (my_bos[i + 1] & BOV_SB_SENTENCE_BOUNDARY) {
            printf (TOKEN_HAVE_BREAK_OPPORTUNITY);
        }
        else {
            printf (TOKEN_HAVE_NO_BREAK_OPPORTUNITY);
        }
    }
    printf("\n\n");

    BOOL ok = TRUE;
    if (n != my_n) {
        ok = FALSE;
    }
    else {
        for (int i = 0; i < n; i++) {
            if (((bos[i] == BREAK_ALLOWED) && !(my_bos[i] & BOV_SB_SENTENCE_BOUNDARY)) ||
                    ((bos[i] == BREAK_NOTALLOWED) && (my_bos[i] & BOV_SB_SENTENCE_BOUNDARY))) {
                ok = FALSE;
                break;
            }
        }
    }

    if (!ok) {
        _ERR_PRINTF("Bad implmentation for sentence breaking!\n");
        exit(1);
    }
}

static int do_test(PLOGFONT lf, FILE* fp, Uint8 lbp)
{
    char buff[MAX_LINE_LEN + 1];
    Uchar32 ucs[MAX_UCHARS];
    Uint8 bos[MAX_UCHARS + 1];
    int n;

    Uint16* my_bos;
    int consumed;
    int line = 0;

    while (TRUE) {
        if (!fgets(buff, MAX_LINE_LEN, fp)) {
            return 0;
        }

        line++;
        printf("==== LINE %d ====\n", line);
        printf("CASE: \n%s", buff);

        n = parse_one_case(buff, ucs, bos);
        if (n == 0) {
            continue;
        }

        printf("CHARS: ");
        for (int i = 0; i < n; i++) {
            printf("%04X(%s, %s) ", ucs[i],
                get_general_category_name(UCharGetCategory(ucs[i])),
                get_break_type_name(UCharGetBreakType(ucs[i])));
        }
        printf("\n");

        my_bos = NULL;
        consumed = UStrGetBreaks(UCHAR_SCRIPT_LATIN,
                CTR_CAPITALIZE, WBR_NORMAL, lbp,
                ucs, n, &my_bos);
        if (consumed > 0) {
            _cb_check_result(ucs, bos, n, ucs, my_bos, consumed);

            if (my_bos) free (my_bos);
        }
        else {
            _ERR_PRINTF("%s: GetUCharsAndBreaks failed\n", __FUNCTION__);
            return 1;
        }
    }
}

static int test_from_file(PLOGFONT lf, const char* filename, Uint8 lbp)
{
    FILE* fp = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        _DBG_PRINTF("%s: Failed to open %s file\n",
                __FUNCTION__, filename);
        return 1;
    }

    do_test(lf, fp, lbp);
    fclose(fp);
    return 0;
}

int MiniGUIMain (int argc, const char* argv[])
{
    PLOGFONT lf = NULL;
    lf = CreateLogFontEx("ttf", "helvetica", "UTF-8",
            FONT_WEIGHT_REGULAR,
            FONT_SLANT_ROMAN,
            FONT_FLIP_NONE,
            FONT_OTHER_NONE,
            FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
            10, 0);
    if (lf == NULL) {
        _DBG_PRINTF("%s: Failed to create logfont\n",
                __FUNCTION__);
        goto error;
    }

    _MG_PRINTF ("========= TEST GRAPHEME BREAKS\n");
    _cb_check_result = check_result_gb;
    if (test_from_file(lf, "ucd/auxiliary/GraphemeBreakTest.txt", LBP_NORMAL))
        exit (1);
    _MG_PRINTF ("========= END OF TEST FOR GRAPHEME BREAKS\n");

    _MG_PRINTF ("========= TEST WORD BREAKS\n");
    _cb_check_result = check_result_wb;
    if (test_from_file(lf, "ucd/auxiliary/WordBreakTest.txt", LBP_NORMAL))
        exit (1);
    _MG_PRINTF ("========= END OF TEST FOR WORD BREAKS\n");

    _MG_PRINTF ("========= TEST SENTENCE BREAKS\n");
    _cb_check_result = check_result_sb;
    if (test_from_file(lf, "ucd/auxiliary/SentenceBreakTest.txt", LBP_NORMAL))
        exit (1);
    _MG_PRINTF ("========= END OF TEST FOR SENTENCE BREAKS\n");

    _cb_check_result = check_result_lb;
    _MG_PRINTF ("========= TEST LINE BREAKS WITH LBP_NORMAL\n");
    if (test_from_file(lf, "ucd/auxiliary/LineBreakTest.txt", LBP_NORMAL))
        exit (1);
    _MG_PRINTF ("========= END OF TEST FOR LINE BREAKS WITH LBP_NORMAL\n");

    _MG_PRINTF ("========= TEST LINE BREAKS WITH LBP_STRICT\n");
    if (test_from_file(lf, "res/LineBreakTest-strict.txt", LBP_STRICT))
        exit (1);
    _MG_PRINTF ("========= END OF TEST FOR LINE BREAKS WITH LBP_STRICT\n");

    DestroyLogFont(lf);
    exit(0);
    return 0;

error:
    if (lf) DestroyLogFont(lf);
    exit(1);
    return 0;
}

#else
#error "To test GetUCharsAndBreaks, please use MiniGUI 3.4.0 and enable support for UNICODE"
#endif /* checking version and features */
