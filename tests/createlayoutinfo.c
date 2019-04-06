/*
** createtextrunsinfo.c
**
**  Test code for Layout of MiniGUI 3.4.0
**  The following APIs are covered:
**
**      CreateTextRuns
**      UBidiGetParagraphEmbeddingLevelsAlt
**      InitBasicShapingEngine
**      CreateLayout
**      LayoutNextLine
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
#include <minigui/control.h>

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0)) \
        && defined(_MGCHARSET_UNICODE) && defined(_MGDEVEL_MODE)

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

static inline void check_tc(const struct test_case* tc, const char* line)
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
        if (level != tc->rel[i]) {
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

static void check_reorder(const struct test_case* tc, const Uchar32* visual_ucs,
        int* indics, int nr_reordered)
{
    for (int i = 0; i < nr_reordered; i++) {
        if (tc->ucs[indics[i]] != visual_ucs[i]) {
            _ERR_PRINTF("%s failed:0x%06x vs 0x%06x at index %d\n",
                    __FUNCTION__, tc->ucs[indics[i]], visual_ucs[i], i);
            goto failed;
        }
    }

    return;

failed:
    for (int i = 0; i < tc->nr_ucs; i++) {
        _ERR_PRINTF("%04x ", visual_ucs[i]);
    }

    _ERR_PRINTF("\n");
    exit(1);
}

static BOOL print_glyph (GHANDLE ctxt, Glyph32 gv,
        const GLYPHPOS* pos, const RENDERDATA* data)
{
    printf("==== Glyph Info ====\n");
    printf("CHAR INDEX     : %d\n", data->uc_index);
    printf("UNICODE CODE   : 0x%08X\n", data->uc);
    printf("LOGFONT        : %p\n", data->logfont);
    printf("GLYPH VALUE    : 0x%08X\n", gv);
    printf("POSITION       : position: (%d, %d); offset: (%d, %d),\n",
            pos->x, pos->y, pos->x_off, pos->y_off);

    return TRUE;
}

static void do_test(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiLevel* check_levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    check_levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);

    if (levels == NULL || check_levels == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for embedding levels\n",
                __FUNCTION__);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    if (UBidiGetParagraphEmbeddingLevelsAlt(tc->ucs, tc->nr_ucs,
                &base_dir, levels) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiGetParagraphEmbeddingLevelsAlt\n",
                __FUNCTION__);
        exit(1);
    }

    int pel;
    switch (base_dir) {
    case BIDI_PGDIR_LTR:
        pel = 0;
        break;
    case BIDI_PGDIR_RTL:
        pel = 1;
        break;
    default:
        _ERR_PRINTF("%s: UBidiGetParagraphEmbeddingLevelsAlt returns a bad resolved paragraph direction. (%d vs 0x%04x)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
        break;
    }

    if (pel != tc->pel) {
        _ERR_PRINTF("%s: The resolved paragraph embedding level does not matched (%d vs %d)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    TEXTRUNS* runinfo;
    runinfo = CreateTextRuns(tc->ucs, tc->nr_ucs, LANGCODE_unknown, base_dir,
            "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-16-UTF-8",
            MakeRGB(0, 0, 0), 0, NULL);

    if (runinfo) {
        LAYOUT* layout;
        LAYOUTLINE* line = NULL;
        int bos_len;
        BreakOppo* bos = NULL;

        bos_len = UStrGetBreaks(SCRIPT_LATIN,
                CTR_CAPITALIZE, WBR_NORMAL, LBP_NORMAL,
                tc->ucs, tc->nr_ucs, &bos);
        if (bos_len <= 0) {
            _ERR_PRINTF("%s: UStrGetBreaks failed\n", __FUNCTION__);
            exit(1);
        }

        if (!InitBasicShapingEngine(runinfo)) {
            _ERR_PRINTF("%s: InitBasicShapingEngine returns FALSE\n",
                    __FUNCTION__);
            exit(1);
        }

        layout = CreateLayout(runinfo, GRF_LINE_EXTENT_VARIABLE, bos + 1, FALSE,
            0, 0, 0, 0, 10, NULL, 0);

        int max_extent = random() % 100;
        while ((line = LayoutNextLine(layout, line, max_extent, FALSE,
                print_glyph, NULL))) {
        }

        DestroyLayout(layout);
        free (bos);
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    DestroyTextRuns(runinfo);

    free(levels);
    free(check_levels);
}

static int _nr_glyphs;

static BOOL count_glyphs (GHANDLE ctxt, Glyph32 gv,
        const GLYPHPOS* pos, const RENDERDATA* data)
{
    _nr_glyphs++;
    return TRUE;
}

static void do_test_persist(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiLevel* check_levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    check_levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);

    if (levels == NULL || check_levels == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for embedding levels\n",
                __FUNCTION__);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    if (UBidiGetParagraphEmbeddingLevelsAlt(tc->ucs, tc->nr_ucs,
                &base_dir, levels) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiGetParagraphEmbeddingLevelsAlt\n",
                __FUNCTION__);
        exit(1);
    }

    int pel;
    switch (base_dir) {
    case BIDI_PGDIR_LTR:
        pel = 0;
        break;
    case BIDI_PGDIR_RTL:
        pel = 1;
        break;
    default:
        _ERR_PRINTF("%s: UBidiGetParagraphEmbeddingLevelsAlt returns a bad resolved paragraph direction. (%d vs 0x%04x)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
        break;
    }

    if (pel != tc->pel) {
        _ERR_PRINTF("%s: The resolved paragraph embedding level does not matched (%d vs %d)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    TEXTRUNS* runinfo;
    runinfo = CreateTextRuns(tc->ucs, tc->nr_ucs, LANGCODE_unknown, base_dir,
            "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-16-UTF-8",
            MakeRGB(0, 0, 0), 0, NULL);

    if (runinfo) {
        LAYOUT* layout;
        LAYOUTLINE* line = NULL;
        int bos_len;
        BreakOppo* bos = NULL;

        bos_len = UStrGetBreaks(SCRIPT_LATIN,
                CTR_CAPITALIZE, WBR_NORMAL, LBP_NORMAL,
                tc->ucs, tc->nr_ucs, &bos);
        if (bos_len <= 0) {
            _ERR_PRINTF("%s: UStrGetBreaks failed\n", __FUNCTION__);
            exit(1);
        }

        if (!InitBasicShapingEngine(runinfo)) {
            _ERR_PRINTF("%s: InitBasicShapingEngine returns FALSE\n",
                    __FUNCTION__);
            exit(1);
        }

        layout = CreateLayout(runinfo, GRF_LINE_EXTENT_VARIABLE, bos + 1, TRUE,
            0, 0, 0, 0, 10, NULL, 0);

        int i = 0;
        int max_extent = random() % 100;
        _nr_glyphs = 0;
        while ((line = LayoutNextLine(layout, line, max_extent, FALSE,
                count_glyphs, NULL))) {
            printf("==== Line Info for LayoutNextLine (%p) ====\n", line);
            printf("LINE NO.:           : %d\n", i);
            printf("MAX EXTENT     : %d\n", max_extent);
            printf("NR OF GLYPHS   : %d\n", _nr_glyphs);

            _nr_glyphs = 0;
            max_extent = random() % 100;
        }

        line = NULL;
        int line_no = 0;
        while ((line = LayoutNextLine(layout, line, max_extent, FALSE,
                print_glyph, NULL))) {

            int nr_chars, nr_glyphs;
            if (!GetLayoutLineInfo(line, &max_extent, &nr_chars, &nr_glyphs,
                NULL, NULL, NULL, NULL, NULL)) {
                _ERR_PRINTF("%s: GetLayoutLineInfo returns FALSE\n", __FUNCTION__);
                exit(1);
            }

            printf("==== Line Info from GetLayoutLineInfo (%p) ====\n", line);
            printf("LINE NO.:           : %d\n", line_no);
            printf("MAX EXTENT     : %d\n", max_extent);
            printf("NR OF GLYPHS   : %d\n", nr_glyphs);
            line_no++;
        }

        DestroyLayout(layout);
        free (bos);
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    DestroyTextRuns(runinfo);

    free(levels);
    free(check_levels);
}

#define MAX_CHARS   1024

static Uchar32 _reordered_uchars[MAX_CHARS];
static int _nr_reordered_uchars;

static BOOL collect_reordered_uchars(GHANDLE ctxt, Glyph32 gv,
        const GLYPHPOS* pos, const RENDERDATA* data)
{
    const struct test_case* tc = (const struct test_case*)ctxt;

    if (BIDI_IS_EXPLICIT_OR_BN (UCharGetBidiType(data->uc))) {
        // skip
    }
    else {
        _reordered_uchars[_nr_reordered_uchars] = data->uc;
        _nr_reordered_uchars++;
    }

    return TRUE;
}

static void check_reordered_uchars(const struct test_case* tc)
{
    int i;

    if (tc->len_indics != _nr_reordered_uchars) {
        _ERR_PRINTF("%s: numbers of valid characters not matched (%d vs %d)\n",
                __FUNCTION__, tc->len_indics, _nr_reordered_uchars);
        goto error;
    }

    for (i = 0; i < tc->len_indics; i++) {
        Uchar32 uc = tc->ucs[tc->ovi[i]];

        if (uc != _reordered_uchars[i]) {
            _ERR_PRINTF("%s: Failed when checking the reordered uchars (%04X vs %04X)\n",
                    __FUNCTION__, uc, _reordered_uchars[i]);
            goto error;
        }
    }

    return;

error:
    _MG_PRINTF("CORRECT REORDERED CHARS (%d):\n", tc->len_indics);
    for (i = 0; i < tc->len_indics; i++) {
        Uchar32 uc = tc->ucs[tc->ovi[i]];
        _MG_PRINTF("0x%06X ", uc);
    }
    _MG_PRINTF("\n");

    _ERR_PRINTF("REORDERED CHARS WE GOT (%d):\n", _nr_reordered_uchars);
    for (i = 0; i < _nr_reordered_uchars; i++) {
        Uchar32 uc = _reordered_uchars[i];
        _ERR_PRINTF("0x%06X ", uc);
    }
    _ERR_PRINTF("\n");

    exit(1);
}

static void check_text_runs(TEXTRUNS* runinfo, const struct test_case* tc,
    BidiLevel* levels)
{
    void* ctxt = NULL;
    const char* fontname;
    int start_index;
    int length;
    LanguageCode lang_code;
    ScriptType script;
    BidiLevel embedding_level;
    Uint8 flags;
    BidiLevel* check_levels;

    check_levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);

    int run = 0;
    int n = 0;
    while ((ctxt = GetNextTextRunInfo(runinfo, ctxt, &fontname, &start_index,
            &length, &lang_code, &script, &embedding_level, &flags))) {

        if (lang_code == LANGCODE_unknown) {
            _ERR_PRINTF("%s: Got a bad language code\n", __FUNCTION__);
            exit(1);
        }

        printf("==== Text Run %d ====\n", run);
        printf("FONTNAME        : %s\n", fontname?fontname:"DEFAULT");
        printf("START           : %d\n", start_index);
        printf("LENGTH          : %d\n", length);
        printf("LANGCODE        : %s\n", LanguageCodeToISO639s1(lang_code));

        char script_name[5] = {};
        Uint32 script_iso = ScriptTypeToISO15924(script);
        char* iso_name = (char*)&script_iso;
        script_name[0] = iso_name[3];
        script_name[1] = iso_name[2];
        script_name[2] = iso_name[1];
        script_name[3] = iso_name[0];

        printf("SCRIPT          : %s\n", script_name);
        printf("EMBEDDING LEVEL : %d\n", embedding_level);
        printf("FLAGS           : %02x\n", flags);

        //if (logfont == NULL) getchar();

        run++;

        for (int i = 0; i < length; i++) {
            check_levels[n + i] = embedding_level;
        }

        n += length;
    }

    if (memcmp (levels, check_levels, sizeof(BidiLevel) * tc->nr_ucs)) {
        _ERR_PRINTF("%s: embedding levels not matched\n",
                __FUNCTION__);

        for (int i = 0; i < tc->nr_ucs; i++) {
            _ERR_PRINTF("%d ", check_levels[i]);
        }

        _ERR_PRINTF("\n");
        exit(1);
    }

    free(check_levels);
}

static void do_test_reorder(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    if (levels == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for embedding levels\n",
                __FUNCTION__);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    if (UBidiGetParagraphEmbeddingLevelsAlt(tc->ucs, tc->nr_ucs,
                &base_dir, levels) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiGetParagraphEmbeddingLevelsAlt\n",
                __FUNCTION__);
        exit(1);
    }

    int pel;
    switch (base_dir) {
    case BIDI_PGDIR_LTR:
        pel = 0;
        break;
    case BIDI_PGDIR_RTL:
        pel = 1;
        break;
    default:
        _ERR_PRINTF("%s: UBidiGetParagraphEmbeddingLevelsAlt returns a bad resolved paragraph direction. (%d vs 0x%04x)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
        break;
    }

    if (pel != tc->pel) {
        _ERR_PRINTF("%s: The resolved paragraph embedding level does not matched (%d vs %d)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    TEXTRUNS* runinfo;
    runinfo = CreateTextRuns(tc->ucs, tc->nr_ucs, LANGCODE_unknown, base_dir,
            "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-16-UTF-8",
            MakeRGB(0, 0, 0), 0, NULL);

    if (runinfo) {

        check_text_runs(runinfo, tc, levels);

        LAYOUT* layout;
        LAYOUTLINE* line = NULL;
        int bos_len;
        BreakOppo* bos = NULL;

        bos_len = UStrGetBreaks(SCRIPT_LATIN,
                CTR_CAPITALIZE, WBR_NORMAL, LBP_NORMAL,
                tc->ucs, tc->nr_ucs, &bos);
        if (bos_len <= 0) {
            _ERR_PRINTF("%s: UStrGetBreaks failed\n", __FUNCTION__);
            exit(1);
        }

        if (!InitBasicShapingEngine(runinfo)) {
            _ERR_PRINTF("%s: InitBasicShapingEngine returns FALSE\n",
                    __FUNCTION__);
            exit(1);
        }

        layout = CreateLayout(runinfo, GRF_LINE_EXTENT_VARIABLE, bos + 1, FALSE,
            0, 0, 0, 0, 10, NULL, 0);

        int max_extent = random() % 100;

        _nr_reordered_uchars = 0;
        while ((line = LayoutNextLine(layout, line, -1, FALSE,
                collect_reordered_uchars, (GHANDLE)tc))) {

            check_reordered_uchars(tc);

            _nr_reordered_uchars = 0;
        }

        DestroyLayout(layout);
        free (bos);
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    DestroyTextRuns(runinfo);

    free(levels);
}

static int _index_ellipsis = -1;
static int _line_width = 0;
static BOOL find_ellipsis(GHANDLE ctxt, Glyph32 gv,
        const GLYPHPOS* pos, const RENDERDATA* data)
{
    if (pos->ellipsis) {
        _index_ellipsis = data->uc_index;
    }

    _line_width += pos->advance;
    return TRUE;
}

static void do_test_ellipsis(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    if (levels == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for embedding levels\n",
                __FUNCTION__);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    if (UBidiGetParagraphEmbeddingLevelsAlt(tc->ucs, tc->nr_ucs,
                &base_dir, levels) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiGetParagraphEmbeddingLevelsAlt\n",
                __FUNCTION__);
        exit(1);
    }

    int pel;
    switch (base_dir) {
    case BIDI_PGDIR_LTR:
        pel = 0;
        break;
    case BIDI_PGDIR_RTL:
        pel = 1;
        break;
    default:
        _ERR_PRINTF("%s: UBidiGetParagraphEmbeddingLevelsAlt returns a bad resolved paragraph direction. (%d vs 0x%04x)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
        break;
    }

    if (pel != tc->pel) {
        _ERR_PRINTF("%s: The resolved paragraph embedding level does not matched (%d vs %d)\n",
                __FUNCTION__, tc->pel, pel);
        exit(1);
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
        base_dir = BIDI_PGDIR_WLTR;
        break;
    }

    TEXTRUNS* runinfo;
    runinfo = CreateTextRuns(tc->ucs, tc->nr_ucs, LANGCODE_unknown, base_dir,
            "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-16-UTF-8",
            MakeRGB(0, 0, 0), 0, NULL);

    if (runinfo) {

        check_text_runs(runinfo, tc, levels);

        LAYOUT* layout;
        LAYOUTLINE* line = NULL;
        int bos_len;
        BreakOppo* bos = NULL;

        bos_len = UStrGetBreaks(SCRIPT_LATIN,
                CTR_CAPITALIZE, WBR_NORMAL, LBP_NORMAL,
                tc->ucs, tc->nr_ucs, &bos);
        if (bos_len <= 0) {
            _ERR_PRINTF("%s: UStrGetBreaks failed\n", __FUNCTION__);
            exit(1);
        }

        if (!InitBasicShapingEngine(runinfo)) {
            _ERR_PRINTF("%s: InitBasicShapingEngine returns FALSE\n",
                    __FUNCTION__);
            exit(1);
        }

        int nr_lines = 0;
        _index_ellipsis = -1;
        _line_width = 0;
        int max_extent = random() % 50;
        layout = CreateLayout(runinfo,
                GRF_LINE_EXTENT_VARIABLE | GRF_OVERFLOW_ELLIPSIZE_START, bos + 1, FALSE,
                0, 0, 0, 0, 10, NULL, 0);
        while ((line = LayoutNextLine(layout, line, max_extent, TRUE,
                find_ellipsis, (GHANDLE)tc))) {
            nr_lines++;
        }
        DestroyLayout(layout);

        if (nr_lines > 1) {
            _ERR_PRINTF("%s: not an ellipsized single line.\n", __FUNCTION__);
            exit(1);
        }

        if (_index_ellipsis < 0 && _line_width > max_extent) {
            _ERR_PRINTF("%s: did not find any ellipsis but line_width (%d) > max_extent (%d).\n",
                __FUNCTION__, _line_width, max_extent);
            exit(1);
        }

        nr_lines = 0;
        _index_ellipsis = -1;
        _line_width = 0;

        max_extent = random() % 50;
        layout = CreateLayout(runinfo,
                GRF_LINE_EXTENT_VARIABLE | GRF_OVERFLOW_ELLIPSIZE_MIDDLE, bos + 1, FALSE,
                0, 0, 0, 0, 10, NULL, 0);
        while ((line = LayoutNextLine(layout, line, max_extent, TRUE,
                find_ellipsis, (GHANDLE)tc))) {
        }
        DestroyLayout(layout);

        if (nr_lines > 1) {
            _ERR_PRINTF("%s: not an ellipsized single line.\n", __FUNCTION__);
            exit(1);
        }

        if (_index_ellipsis < 0 && _line_width > max_extent) {
            _ERR_PRINTF("%s: did not find any ellipsis but line_width (%d) > max_extent (%d).\n",
                __FUNCTION__, _line_width, max_extent);
            exit(1);
        }

        nr_lines = 0;
        _index_ellipsis = -1;
        _line_width = 0;

        max_extent = random() % 50;
        layout = CreateLayout(runinfo,
                GRF_LINE_EXTENT_VARIABLE | GRF_OVERFLOW_ELLIPSIZE_END, bos + 1, FALSE,
                0, 0, 0, 0, 10, NULL, 0);
        while ((line = LayoutNextLine(layout, line, max_extent, TRUE,
                find_ellipsis, (GHANDLE)tc))) {
        }
        DestroyLayout(layout);

        if (nr_lines > 1) {
            _ERR_PRINTF("%s: not an ellipsized single line.\n", __FUNCTION__);
            exit(1);
        }

        if (_index_ellipsis < 0 && _line_width > max_extent) {
            _ERR_PRINTF("%s: did not find any ellipsis but line_width (%d) > max_extent (%d).\n",
                __FUNCTION__, _line_width, max_extent);
            exit(1);
        }

        free (bos);
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    DestroyTextRuns(runinfo);

    free(levels);
}

#define TEST_MODE_PERSIST   1
#define TEST_MODE_REORDER   2
#define TEST_MODE_ELLIPSIS  3

static int bidi_character_test(const char* filename, int test_mode)
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

        // check_tc(&tc, buff);

        // true test here
        switch (test_mode) {
        case TEST_MODE_PERSIST:
            do_test_persist(&tc);
            break;
        case TEST_MODE_REORDER:
            do_test_reorder(&tc);
            break;
        case TEST_MODE_ELLIPSIS:
            do_test_ellipsis(&tc);
            break;
        default:
            do_test(&tc);
            break;
        }

        destroy_test_case(&tc);
    }

    fclose(fp);
    return 0;
}

int MiniGUIMain (int argc, const char* argv[])
{
    double start_time, end_time;
    int test_mode = 0;

    if (argc > 1)
        test_mode = atoi(argv[1]);

    srandom(time(NULL));

    _MG_PRINTF ("========= START TO TEST CreateLayout (BidiCharacterTest.txt)\n");
    start_time = get_curr_time();
    bidi_character_test("ucd/BidiCharacterTest.txt", test_mode);
    end_time = get_curr_time();
    _MG_PRINTF ("========= END OF TEST CreateLayout (BidiCharacterTest.txt)\n");

    _MG_PRINTF ("Totol elapsed time: %.2f\n", end_time - start_time);

    exit(0);
    return 0;
}

#else
#error "To test APIs related to text runs, please use MiniGUI 3.4.0, enable support for UNICODE, and enable developer mode."
#endif /* checking version */

