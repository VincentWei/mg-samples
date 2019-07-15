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
** createtextrunsinfo.c
**
**  Test code for Text Runs of MiniGUI 3.4.0
**  The following APIs are covered:
**
**      CreateTextRuns
**      UBidiGetParagraphEmbeddingLevelsAlt
**      SetFontNameInTextRuns
**      SetTextColorInTextRuns
**      SetBackgroundColorInTextRuns
**      GetFontNameInTextRuns
**      GetTextColorInTextRuns
**      GetBackgroundColorInTextRuns
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
        if (tc->rel[i] > 0 && level != tc->rel[i]) {
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

static void do_test(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiLevel* got_levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    got_levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);

    if (levels == NULL || got_levels == NULL) {
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
        void* ctxt = NULL;
        const char* fontname;
        int start_index;
        int length;
        LanguageCode lang_code;
        ScriptType script;
        BidiLevel embedding_level;
        Uint8 flags;

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
                got_levels[n + i] = embedding_level;
            }

            n += length;
        }
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    check_levels(tc, got_levels);

#if 0
    if (memcmp (levels, got_levels, sizeof(BidiLevel) * tc->nr_ucs)) {
        _ERR_PRINTF("%s: embedding levels not matched\n",
                __FUNCTION__);

        for (int i = 0; i < tc->nr_ucs; i++) {
            _ERR_PRINTF("%d ", got_levels[i]);
        }

        _ERR_PRINTF("\n");
        exit(1);
    }
#endif

    DestroyTextRuns(runinfo);

    free(levels);
    free(got_levels);
}

static const char* fonts[] = {
    "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-12-UTF-8",
    "ttf-monospace-rrncns-U-20-UTF-8",
    "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-13-UTF-8",
    "ttf-monospace-rrncns-U-16-UTF-8",
    "ttf-Courier,宋体,Naskh,SansSerif-rrncns-U-16-UTF-8",
};

static BOOL check_change_font(TEXTRUNS* runinfo, const struct test_case* tc)
{
    int nr_loops = random() % tc->nr_ucs;
    int start_index;
    int len;
    const char* fontname;

    for (int i = 0; i < nr_loops; i++) {
        start_index = random() % tc->nr_ucs;
        len = random() % tc->nr_ucs;
        fontname = fonts[i % 5];

        if (SetFontNameInTextRuns(runinfo, start_index, len, fontname)) {
            _MG_PRINTF("%s, set fontname %d +%d: %s\n",
                __FUNCTION__, start_index, len, fontname);

            const char* my_name = GetFontNameInTextRuns(runinfo, start_index);
            if (strcmp(my_name, fontname)) {
                _ERR_PRINTF("%s, fontname not matched: %s vs %s; index: %d\n",
                    __FUNCTION__, fontname, my_name, start_index);
                return FALSE;
            }
        }
    }

    return TRUE;
}

static void do_test_change_font(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiLevel* got_levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    got_levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);

    if (levels == NULL || got_levels == NULL) {
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

        BOOL result = check_change_font(runinfo, tc);

        void* ctxt = NULL;
        const char* fontname;
        int start_index;
        int length;
        LanguageCode lang_code;
        ScriptType script;
        BidiLevel embedding_level;
        Uint8 flags;

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
                got_levels[n + i] = embedding_level;
            }

            n += length;
        }

        if (!result) {
            exit(1);
        }
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    check_levels(tc, got_levels);

#if 0
    if (memcmp (levels, got_levels, sizeof(BidiLevel) * tc->nr_ucs)) {
        _ERR_PRINTF("%s: embedding levels not matched\n",
                __FUNCTION__);

        for (int i = 0; i < tc->nr_ucs; i++) {
            _ERR_PRINTF("%d ", got_levels[i]);
        }

        _ERR_PRINTF("\n");
        exit(1);
    }
#endif

    DestroyTextRuns(runinfo);

    free(levels);
    free(got_levels);
}

static BOOL check_change_color(TEXTRUNS* runinfo, const struct test_case* tc)
{
    int nr_loops = random() % tc->nr_ucs;
    int start_index;
    int len;
    RGBCOLOR fg_color, bg_color, my_color;

    for (int i = 0; i < nr_loops; i++) {
        start_index = random() % tc->nr_ucs;
        len = random() % tc->nr_ucs;
        fg_color = MakeRGB(random() % 256, random() % 256, random() % 256);
        bg_color = MakeRGB(random() % 256, random() % 256, random() % 256);

        if (SetTextColorInTextRuns(runinfo, start_index, len, fg_color)) {
            my_color = GetTextColorInTextRuns(runinfo, start_index);
            if (my_color != fg_color) {
                _ERR_PRINTF("%s, foreground color not matched: 0x%08x vs 0x%08x\n",
                    __FUNCTION__, fg_color, my_color);
                return FALSE;
            }
        }

        if (SetBackgroundColorInTextRuns(runinfo, start_index, len, bg_color)) {
            my_color = GetBackgroundColorInTextRuns(runinfo, start_index);
            if (my_color != bg_color) {
                _ERR_PRINTF("%s, foreground color not matched: 0x%08x vs 0x%08x\n",
                    __FUNCTION__, bg_color, my_color);
                return FALSE;
            }
        }
    }

    return TRUE;
}

static void do_test_change_color(const struct test_case* tc)
{
    BidiLevel* levels;
    BidiLevel* got_levels;
    BidiType base_dir;
    int i;

    levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);
    got_levels = (BidiLevel*)malloc (sizeof(BidiLevel) * tc->nr_ucs);

    if (levels == NULL || got_levels == NULL) {
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

        BOOL result = check_change_color(runinfo, tc);

        void* ctxt = NULL;
        const char* fontname;
        int start_index;
        int length;
        LanguageCode lang_code;
        ScriptType script;
        BidiLevel embedding_level;
        Uint8 flags;

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
                got_levels[n + i] = embedding_level;
            }

            n += length;
        }

        if (!result) {
            exit(1);
        }
    }
    else {
        _ERR_PRINTF("%s: CreateTextRuns returns NULL\n", __FUNCTION__);
        exit(1);
    }

    check_levels(tc, got_levels);

#if 0
    if (memcmp (levels, got_levels, sizeof(BidiLevel) * tc->nr_ucs)) {
        _ERR_PRINTF("%s: embedding levels not matched\n",
                __FUNCTION__);

        for (int i = 0; i < tc->nr_ucs; i++) {
            _ERR_PRINTF("%d ", got_levels[i]);
        }

        _ERR_PRINTF("\n");
        exit(1);
    }
#endif

    DestroyTextRuns(runinfo);

    free(levels);
    free(got_levels);
}

#define TEST_MODE_DEFAULT       0
#define TEST_MODE_CHANGE_FONT   1
#define TEST_MODE_CHANGE_COLOR  2

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

        check_tc(&tc, buff);

        switch (test_mode) {
        case TEST_MODE_CHANGE_FONT:
            do_test_change_font(&tc);
            break;

        case TEST_MODE_CHANGE_COLOR:
            do_test_change_color(&tc);
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

    _MG_PRINTF ("========= START TO TEST CreateTextRuns (BidiCharacterTest.txt)\n");

    start_time = get_curr_time();
    bidi_character_test("ucd/BidiCharacterTest.txt", test_mode);
    end_time = get_curr_time();

    _MG_PRINTF ("========= END OF TEST CreateTextRuns (BidiCharacterTest.txt)\n");
    _MG_PRINTF ("Totol elapsed time: %.2f\n", end_time - start_time);

    exit(0);
    return 0;
}

#else
#error "To test APIs related to text runs, please use MiniGUI 3.4.0, enable support for UNICODE, and enable developer mode."
#endif /* checking version */

