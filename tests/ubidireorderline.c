/*
** ubidireorderline.c
**
**  Test code for UBidiReorderLine of MiniGUI 3.4.0
**  The following APIs are covered:
**
**  For Unicode charset:
**
**      UBidiGetParagraphDir
**      UStrGetBidiTypes
**      UStrGetBracketTypes
**      UBidiGetParagraphEmbeddingLevels
**      UBidiReorderLine
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

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0))

#include "helpers.h"

#define MAX_LINE_LEN        4096
#define CHAR_REMOVED        -1

struct test_case {
    int         nr_ucs; // number of characters.
    int         len_indics; // The length of ovi
    int         pd;     // The paragraph direction:
                        //  - 0 represents left-to-right
                        //  - 1 represents right-to-left
                        //  - 2 represents auto-LTR according to rules P2 and P3 of the algorithm
    int         pel;    // The resolved paragraph embedding level
    Uchar32*    ucs;    // The Unicode character sequence
    int*        rel;    // The resolved embedding levels.
    int*        ovi;    // A list of indices showing the resulting visual ordering from left to right.
};

static void init_test_case(struct test_case* tc)
{
    tc->nr_ucs = 0;
    tc->len_indics = 0;
    tc->ucs = NULL;
    tc->rel = NULL;
    tc->ovi = NULL;
}

static void destroy_test_case(struct test_case* tc)
{
    if (tc->ucs) {
        free(tc->ucs);
        tc->ucs = NULL;
    }

    if (tc->rel) {
        free(tc->rel);
        tc->rel = NULL;
    }

    if (tc->ovi) {
        free(tc->ovi);
        tc->ovi = NULL;
    }

    tc->nr_ucs = 0;
    tc->len_indics = 0;
}

static void test_case_push_back(struct test_case* tc, Uchar32 uc)
{
    tc->nr_ucs++;

    if (tc->ucs == NULL) {
        tc->ucs = (Uchar32*)malloc(sizeof(Uchar32));
        tc->rel = (int*)malloc(sizeof(int));
        tc->ovi = (int*)malloc(sizeof(int));
    }
    else {
        tc->ucs = (Uchar32*)realloc(tc->ucs, sizeof(Uchar32) * tc->nr_ucs);
        tc->rel = (int*)realloc(tc->rel, sizeof(int) * tc->nr_ucs);
        tc->ovi = (int*)realloc(tc->ovi, sizeof(int) * tc->nr_ucs);
    }

    if (tc->ucs == NULL || tc->rel == NULL || tc->ovi == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for new character!\n",
            __FUNCTION__);
        exit(1);
    }

    tc->ucs[tc->nr_ucs - 1] = uc;
}

static int parse_one_case(const char* line, struct test_case* tc)
{
    int field_idx = 0;
    int ch_idx = 0;
    const char* current;

    if (!line)
        return 0;

    current = line;
    while (*current) {

        while (*current == ' ' ||  *current == '\t')
            current++;

        if (*current == '#' || *current == '\n' || *current == '\r') {
            return tc->nr_ucs;
        }
        else if (*current == ';') {
            field_idx++;
            ch_idx = 0;

            current++;
        }
        else if (*current == 'x') {
            if (field_idx == 3) {
                tc->rel[ch_idx] = CHAR_REMOVED;
                ch_idx++;
            }
#if 0
            else if (field_idx == 4) {
                tc->ovi[ch_idx] = CHAR_REMOVED;
                ch_idx++;
            }
#endif
            else {
                _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                return 0;
            }

            current++;
        }
        else if (isxdigit(*current)) {
            switch (field_idx) {
                case 0: {
                    Uchar32 uc;
                    int n = sscanf(current, "%X", &uc);
                    if (n == 1) {
                        test_case_push_back(tc, uc);
                    }
                    else {
                        _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                        return 0;
                    }
                    break;
                }

                case 1: {
                    int tmp;
                    int n = sscanf(current, "%d", &tmp);
                    if (n == 1) {
                        tc->pd = tmp;
                    }
                    else {
                        _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                        return 0;
                    }
                    break;
                }

                case 2: {
                    int tmp;
                    int n = sscanf(current, "%d", &tmp);
                    if (n == 1) {
                        tc->pel = tmp;
                    }
                    else {
                        _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                        return 0;
                    }
                    break;
                }

                case 3: {
                    int tmp;
                    int n = sscanf(current, "%d", &tmp);
                    if (n == 1) {
                        tc->rel[ch_idx] = tmp;
                        ch_idx++;
                    }
                    else {
                        _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                        return 0;
                    }
                    break;
                }

                case 4: {
                    int tmp;
                    int n = sscanf(current, "%d", &tmp);
                    if (n == 1) {
                        tc->ovi[ch_idx] = tmp;
                        ch_idx++;
                        tc->len_indics = ch_idx;
                    }
                    else {
                        _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                        return 0;
                    }
                    break;
                }
            }

            while (isxdigit(*current))
                current++;
        }
    }

    return tc->nr_ucs;
}

static void check_tc(const struct test_case* tc, const char* line)
{
    int i;
    char buff[MAX_LINE_LEN + 1];
    char* tmp = buff;

    for (i = 0; i < tc->nr_ucs; i++) {
        int l = sprintf(tmp, "%04X", tc->ucs[i]);
        tmp += l;

        if (i < tc->nr_ucs - 1) {
            *tmp = ' ';
            tmp++;
        }
    }

    *tmp = ';';
    tmp++;

    sprintf(tmp, "%d", tc->pd);
    tmp++;

    *tmp = ';';
    tmp++;

    sprintf(tmp, "%d", tc->pel);
    tmp++;

    *tmp = ';';
    tmp++;

    for (i = 0; i < tc->nr_ucs; i++) {
        if (tc->rel[i] >= 0) {
            int l = sprintf(tmp, "%d", tc->rel[i]);
            tmp += l;
        }
        else {
            *tmp = 'x';
            tmp++;
        }

        if (i < tc->nr_ucs - 1) {
            *tmp = ' ';
            tmp++;
        }
    }

    *tmp = ';';
    tmp++;

    for (i = 0; i < tc->len_indics; i++) {
        if (tc->ovi[i] >= 0) {
            int l = sprintf(tmp, "%d", tc->ovi[i]);
            tmp += l;
        }
        else {
            *tmp = 'x';
            tmp++;
        }

        if (i < tc->len_indics - 1) {
            *tmp = ' ';
            tmp++;
        }
    }

    if (strncmp(buff, line, tmp - buff)) {
        _ERR_PRINTF("%s: Failed to check read test case: \n%s\n",
                __FUNCTION__, buff);
        exit(1);
    }
}

#if 0
static void reverse_indics (void* extra, int len, int pos)
{
    int* indics = (int*)extra + pos;
    int i;

    _ERR_PRINTF("%s: reverse %d characters at position %d\n",
            __FUNCTION__, len, pos);

    for (i = 0; i < len / 2; i++) {
        int tmp = indics[i];
        indics[i] = indics[len - 1 - i];
        indics[len - 1 - i] = tmp;
    }
}
#endif

static void check_levels(const struct test_case* tc, const BidiLevel* levels)
{
    for (int i = 0; i < tc->nr_ucs; i++) {
        int level = (int)levels[i];
        if (tc->rel[i] != -1 && level != tc->rel[i]) {
            _ERR_PRINTF("%s failed: %d vs %d at index %d\n",
                    __FUNCTION__, tc->rel[i], level, i);
            goto failed;
        }
    }

    return;

failed:
    for (int i = 0; i < tc->nr_ucs; i++) {
        int level = (int)levels[i];
        _ERR_PRINTF("%d ", level);
    }

    _ERR_PRINTF("\n");
    exit(1);
}

static void check_indics(const struct test_case* tc, int* indics, int check_len)
{
    for (int i = 0; i < check_len; i++) {
        if (tc->ovi[i] != indics[i]) {
            _ERR_PRINTF("%s failed: %d vs %d at index %d\n",
                    __FUNCTION__, tc->ovi[i], indics[i], i);
            exit(1);
        }
    }
}

static void do_test(PLOGFONT lf, const struct test_case* tc)
{
    BidiType* bidi_types;
    BidiBracketType* bracket_types;
    BidiLevel* levels;
    BidiType base_dir;
    int* indics;
    int i;

    bidi_types = (BidiType*)malloc(sizeof(BidiType) * tc->nr_ucs);
    bracket_types = (BidiBracketType*)malloc(sizeof(BidiBracketType) * tc->nr_ucs);
    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    indics = (int*)malloc(sizeof(int) * tc->nr_ucs);

    if (bidi_types == NULL || bracket_types == NULL ||
            levels == NULL || indics == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for types, levels, and indics\n",
                __FUNCTION__);
        exit(1);
    }

    UStrGetBidiTypes(tc->ucs, tc->nr_ucs, bidi_types);

    for (i = 0; i < tc->nr_ucs; i++) {
        /* Note the optimization that a bracket is always
           of type neutral */
        if (bidi_types[i] == BIDI_TYPE_ON)
            bracket_types[i] = UCharGetBracketType(tc->ucs[i]);
        else
            bracket_types[i] = BIDI_BRACKET_NONE;
    }

    switch (tc->pd) {
    case 0:
        base_dir = BIDI_PGDIR_LTR;
        break;
    case 1:
        base_dir = BIDI_PGDIR_RTL;
        break;
    case 2:
    default:
        base_dir = BIDI_PGDIR_ON;
        break;
    }

    if (UBidiGetParagraphEmbeddingLevels(bidi_types,
                bracket_types, tc->nr_ucs,
                &base_dir, levels) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiGetParagraphEmbeddingLevels\n",
                __FUNCTION__);
        exit(1);
    }

    switch (base_dir) {
    case BIDI_PGDIR_LTR:
        base_dir = 0;
        break;
    case BIDI_PGDIR_RTL:
        base_dir = 1;
        break;
    default:
        _ERR_PRINTF("%s: UBidiGetParagraphEmbeddingLevels returns a bad resolved paragraph direction. (%d vs 0x%04x)\n",
                __FUNCTION__, tc->pel, base_dir);
        exit(1);
        break;
    }

    if (base_dir != tc->pel) {
        _ERR_PRINTF("%s: The resolved paragraph embedding level does not matched (%d vs %d)\n",
                __FUNCTION__, tc->pel, base_dir);
        exit(1);
    }

    for (i = 0; i < tc->nr_ucs; i++) {
        indics[i] = i;
    }

    if (UBidiReorderLine(0, bidi_types, tc->nr_ucs,
                0, base_dir, levels, NULL, indics, NULL, NULL) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiReorderLine\n",
                __FUNCTION__);
        exit(1);
    }

    check_levels(tc, levels);

    int j = 0;
    int nr_reordered;
    for (i = 0; i < tc->nr_ucs; i++) {
        if (!BIDI_IS_EXPLICIT_OR_BN (bidi_types[indics[i]]))
            indics[j++] = indics[i];
    }
    nr_reordered = j;

    if (nr_reordered != tc->len_indics) {
        _ERR_PRINTF("%s: the reordered string length does not matched: %d vs %d\n",
                __FUNCTION__, tc->len_indics, nr_reordered);
        exit(1);
    }

    check_indics(tc, indics, nr_reordered);

    free(indics);
    free(levels);
    free(bracket_types);
    free(bidi_types);
}

static int test_from_file(PLOGFONT lf, const char* filename)
{
    FILE* fp = NULL;
    int line = 0;
    char buff[MAX_LINE_LEN + 1];
    struct test_case tc;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        _ERR_PRINTF("%s: Failed to open %s file\n",
                __FUNCTION__, filename);
        return 1;
    }

    while (TRUE) {

        if (!fgets(buff, MAX_LINE_LEN, fp)) {
            return 0;
        }

        line++;
        printf("==== LINE %d ====\n", line);
        printf("CASE: \n%s", buff);

        init_test_case(&tc);
        int n = parse_one_case(buff, &tc);
        if (n == 0) {
            destroy_test_case(&tc);
            continue;
        }

        check_tc(&tc, buff);

        // true test here
        do_test(lf, &tc);

        destroy_test_case(&tc);
    }

    fclose(fp);
    return 0;
}

#ifdef _MGCHARSET_UNICODE
static void test_for_unicode(void)
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
        _ERR_PRINTF("%s: Failed to create logfont\n",
            __FUNCTION__);
        goto error;
    }

    if (test_from_file(lf, "ucd/BidiCharacterTest.txt"))
        goto error;

    DestroyLogFont(lf);
    return;

error:
    if (lf) DestroyLogFont(lf);
    exit(1);
}

#else

static test_for_unicode(void)
{
    _ERR_PRINTF("%s: The support for Unicode is not enabled!\n",
        __FUNCTION__);
}

#endif /* _MGCHARSET_UNICODE */

#ifdef _MGCHARSET_ARABIC
static void test_for_arabic(void)
{
    PLOGFONT lf = NULL;

    lf = CreateLogFontEx("*", "helvetica", "ISO8859-6",
            FONT_WEIGHT_REGULAR,
            FONT_SLANT_ROMAN,
            FONT_FLIP_NONE,
            FONT_OTHER_NONE,
            FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
            10, 0);

    if (lf == NULL) {
        _ERR_PRINTF("%s: Failed to create logfont for Arabic (ISO8859-6)\n",
            __FUNCTION__);
        goto error;
    }

    if (test_from_file(lf, "res/arabic.txt"))
        goto error;

    DestroyLogFont(lf);
    return;

error:
    if (lf) DestroyLogFont(lf);
    exit(1);
}

#else

static test_for_arabic(void)
{
    _ERR_PRINTF("%s: The support for Unicode is not enabled!\n",
        __FUNCTION__);
}

#endif /* _MGCHARSET_ARABIC */

#ifdef _MGCHARSET_HEBREW
static void test_for_hebrew(void)
{
    PLOGFONT lf = NULL;

    lf = CreateLogFontEx("*", "helvetica", "ISO8859-8",
            FONT_WEIGHT_REGULAR,
            FONT_SLANT_ROMAN,
            FONT_FLIP_NONE,
            FONT_OTHER_NONE,
            FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
            10, 0);

    if (lf == NULL) {
        _ERR_PRINTF("%s: Failed to create logfont for Hebrew (ISO8859-8)\n",
            __FUNCTION__);
        goto error;
    }

    if (test_from_file(lf, "res/hebrew.txt"))
        goto error;

    DestroyLogFont(lf);
    return;

error:
    if (lf) DestroyLogFont(lf);
    exit(1);
}

#else

static test_for_hebrew(void)
{
    _ERR_PRINTF("%s: The support for Unicode is not enabled!\n",
        __FUNCTION__);
}

#endif /* _MGCHARSET_HEBREW */

int MiniGUIMain (int argc, const char* argv[])
{
    _MG_PRINTF ("========= START TO TEST UNICODE BIDI PROCESSING\n");
    test_for_unicode();
    _MG_PRINTF ("========= END OF TEST UNICODE BIDI PROCESSING\n");

    _MG_PRINTF ("========= START TO TEST HEBREW BIDI PROCESSING\n");
    test_for_hebrew();
    _MG_PRINTF ("========= END OF TEST HEBREW BIDI PROCESSING\n");

    _MG_PRINTF ("========= START TO TEST ARABIC BIDI PROCESSING\n");
    test_for_arabic();
    _MG_PRINTF ("========= END OF TEST ARABIC BIDI PROCESSING\n");

    exit(0);
    return 0;
}

#else
#error "To test BIDILogAChars2VisAChars, please use MiniGUI 3.4.0"
#endif /* checking version */

