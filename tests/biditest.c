/*
** biditest.c
**
**  Test code for UBA implementation of MiniGUI 3.4.0
**  The following APIs are covered:
**
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

#if (_MINIGUI_VERSION_CODE >= _VERSION_CODE(3,4,0)) \
        && defined(_MGCHARSET_UNICODE)

#include "helpers.h"

#define MAX_LINE_LEN        4096
#define CHAR_REMOVED        -1

struct test_case {
    // number of levels
    int         nr_levels;

    // numuber of reordered indics
    int         nr_reorder_indics;

    // number of bidi types
    int         nr_bidi_types;

    // bitset for paragraph levels
    Uint32      bitset;

    // levels
    BidiLevel*  levels;

    // reordered indics
    int*        reorder_indics;

    // bidi types
    BidiType*   bidi_types;
};

static void init_test_case(struct test_case* tc)
{
    memset (tc, 0, sizeof (struct test_case));
}

static void destroy_test_case(struct test_case* tc)
{
    if (tc->levels) {
        free(tc->levels);
    }

    if (tc->reorder_indics) {
        free(tc->reorder_indics);
    }

    if (tc->bidi_types) {
        free(tc->bidi_types);
    }

    init_test_case(tc);
}

static void clear_for_new_test_case(struct test_case* tc)
{
    destroy_test_case(tc);
}

static void clear_for_new_sub_test_case(struct test_case* tc)
{
    if (tc->bidi_types) {
        free(tc->bidi_types);
        tc->bidi_types = NULL;
        tc->nr_bidi_types = 0;
    }

    tc->bitset = 0;
}

static void test_case_push_back_level(struct test_case* tc, BidiLevel level)
{
    tc->nr_levels++;

    if (tc->levels == NULL) {
        tc->levels = (BidiLevel*)malloc(sizeof(BidiLevel));
    }
    else {
        tc->levels = (BidiLevel*)realloc(tc->levels,
            sizeof(BidiLevel) * tc->nr_levels);
    }

    if (tc->levels == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for new level!\n",
            __FUNCTION__);
        exit(1);
    }

    tc->levels[tc->nr_levels - 1] = level;
}

static void test_case_push_back_reorder_index(struct test_case* tc, int index)
{
    tc->nr_reorder_indics++;

    if (tc->reorder_indics == NULL) {
        tc->reorder_indics = (int*)malloc(sizeof(int));
    }
    else {
        tc->reorder_indics = (int*)realloc(tc->reorder_indics,
                sizeof(int) * tc->nr_reorder_indics);
    }

    if (tc->reorder_indics == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for new level!\n",
            __FUNCTION__);
        exit(1);
    }

    tc->reorder_indics[tc->nr_reorder_indics - 1] = index;
}

static void test_case_push_back_bidi_type(struct test_case* tc,
        BidiType bidi_type)
{
    tc->nr_bidi_types++;

    if (tc->bidi_types == NULL) {
        tc->bidi_types = (BidiType*)malloc(sizeof(BidiType));
    }
    else {
        tc->bidi_types = (BidiType*)realloc(tc->bidi_types,
                sizeof(BidiType) * tc->nr_bidi_types);
    }

    if (tc->bidi_types == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for new level!\n",
            __FUNCTION__);
        exit(1);
    }

    tc->bidi_types[tc->nr_bidi_types - 1] = bidi_type;
}

static void test_case_set_bitset(struct test_case* tc, Uint32 bitset)
{
    tc->bitset = bitset;
}

static int parse_levels(struct test_case* tc, const char* line)
{
    int nr_read = 0;
    const char* current;

    current = line;
    while (*current) {

        while (*current == ' ' ||  *current == '\t')
            current++;

        if (*current == '#' || *current == '\n' || *current == '\r') {
            return nr_read;
        }
        else if (*current == 'x') {
            test_case_push_back_level(tc, (BidiLevel)-1);
            nr_read++;
            current++;
        }
        else if (isdigit(*current)) {
            int level;
            int n = sscanf(current, "%d", &level);
            if (n == 1) {
                test_case_push_back_level(tc, (BidiLevel)level);
                nr_read++;
            }
            else {
                _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                return nr_read;
            }

            while (isdigit(*current))
                current++;
        }
    }

    return nr_read;
}

static int parse_reorder(struct test_case* tc, const char* line)
{
    int nr_read = 0;
    const char* current;

    current = line;
    while (*current) {

        while (*current == ' ' ||  *current == '\t')
            current++;

        if (*current == '#' || *current == '\n' || *current == '\r') {
            return nr_read;
        }
        else if (isdigit(*current)) {
            int index;
            int n = sscanf(current, "%d", &index);
            if (n == 1) {
                test_case_push_back_reorder_index(tc, index);
                nr_read++;
            }
            else {
                _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                return nr_read;
            }

            while (isdigit(*current))
                current++;
        }
    }

    return nr_read;
}

#define ADD_BIDI_MAP_ENTRY(type, c) \
    { #type, BIDI_TYPE_##type },

#define ADD_BIDI_MAP_ENTRY_ALIAS(alias, type) \
    { #alias, BIDI_TYPE_##type },

static const struct bidi_class_name_to_id {
    const char* bidi_name;
    BidiType    bidi_type;
} bidi_name_to_id[] =
{
    ADD_BIDI_MAP_ENTRY (LRE, '+')    /* Left-to-Right Embedding */
    ADD_BIDI_MAP_ENTRY (RLE, '+')    /* Right-to-Left Embedding */
    ADD_BIDI_MAP_ENTRY (LRO, '+')    /* Left-to-Right Override */
    ADD_BIDI_MAP_ENTRY (RLO, '+')    /* Right-to-Left Override */
    ADD_BIDI_MAP_ENTRY (PDF, '-')    /* Pop Directional Flag */
    ADD_BIDI_MAP_ENTRY (LRI, '+')    /* Left-to-Right Isolate */
    ADD_BIDI_MAP_ENTRY (RLI, '+')    /* Right-to-Left Isolate */
    ADD_BIDI_MAP_ENTRY (FSI, '+')    /* First-Strong Isolate */
    ADD_BIDI_MAP_ENTRY (PDI, '-')    /* Pop Directional Isolate */
    ADD_BIDI_MAP_ENTRY (LTR, 'L')    /* Left-To-Right letter */
    ADD_BIDI_MAP_ENTRY (RTL, 'R')    /* Right-To-Left letter */
    ADD_BIDI_MAP_ENTRY (NSM, '`')    /* Non Spacing Mark */
    ADD_BIDI_MAP_ENTRY (AL,  'A')    /* Arabic Letter */
    ADD_BIDI_MAP_ENTRY (EN,  '1')    /* European Numeral */
    ADD_BIDI_MAP_ENTRY (AN,  '9')    /* Arabic Numeral */
    ADD_BIDI_MAP_ENTRY (ES,  'w')    /* European number Separator */
    ADD_BIDI_MAP_ENTRY (ET,  'w')    /* European number Terminator */
    ADD_BIDI_MAP_ENTRY (CS,  'w')    /* Common Separator */
    ADD_BIDI_MAP_ENTRY (BN,  'b')    /* Boundary Neutral */
    ADD_BIDI_MAP_ENTRY (BS,  'B')    /* Block Separator */
    ADD_BIDI_MAP_ENTRY (SS,  'S')    /* Segment Separator */
    ADD_BIDI_MAP_ENTRY (WS,  '_')    /* WhiteSpace */
    ADD_BIDI_MAP_ENTRY (ON,  'n')    /* Other Neutral */

    ADD_BIDI_MAP_ENTRY_ALIAS (L, LTR)
    ADD_BIDI_MAP_ENTRY_ALIAS (R, RTL)
    ADD_BIDI_MAP_ENTRY_ALIAS (B, BS)
    ADD_BIDI_MAP_ENTRY_ALIAS (S, SS)
};

static int parse_subcase(struct test_case* tc, const char* line)
{
    int field_idx = 0;
    int nr_read = 0;
    const char* current;

    current = line;
    while (*current) {

        while (*current == ' ' ||  *current == '\t')
            current++;

        if (*current == '#' || *current == '\n' || *current == '\r') {
            return nr_read;
        }
        else if (*current == ';') {
            field_idx++;
            current++;
        }
        else if (field_idx == 0 && isupper(*current)) {
            for (int i = 0; i < TABLESIZE(bidi_name_to_id); i++) {
                BidiType bidi_type;
                if (strncmp(current, bidi_name_to_id[i].bidi_name,
                        strlen(bidi_name_to_id[i].bidi_name)) == 0) {
                    bidi_type = bidi_name_to_id[i].bidi_type;

                    test_case_push_back_bidi_type(tc, bidi_type);
                    nr_read++;
                }
            }

            while (isupper(*current))
                current++;
        }
        else if (field_idx > 0 && isxdigit(*current)) {
            Uint32 bitset;
            int n = sscanf(current, "%X", &bitset);
            if (n == 1) {
                test_case_set_bitset(tc, bitset);
                nr_read++;
            }
            else {
                _ERR_PRINTF("%s: bad format: %s\n", __FUNCTION__, line);
                return 0;
            }

            while (isxdigit(*current))
                current++;
        }
    }

    return nr_read;
}

#define LINE_TYPE_IGNORED   0
#define LINE_TYPE_LEVELS    1
#define LINE_TYPE_REORDER   2
#define LINE_TYPE_SUBCASE   3

#define PREFIX_LEVELS       "@Levels:"
#define PREFIX_REORDER      "@Reorder:"

static int parse_one_line(const char* line, struct test_case* tc)
{
    int line_type = LINE_TYPE_IGNORED;

    if (!line)
        return 0;

    if (*line == '@') {
        if (!strncasecmp(line, PREFIX_LEVELS, strlen(PREFIX_LEVELS))) {
            line_type = LINE_TYPE_LEVELS;
        }
        else if (!strncasecmp(line, PREFIX_REORDER, strlen(PREFIX_REORDER))) {
            line_type = LINE_TYPE_REORDER;
        }
    }
    else if (*line == '#') {
        line_type = LINE_TYPE_IGNORED;
    }
    else if (isupper (*line)) {
        line_type = LINE_TYPE_SUBCASE;
    }
    else {
        line_type = LINE_TYPE_IGNORED;
    }

    switch (line_type) {
    case LINE_TYPE_IGNORED:
        return 0;

    case LINE_TYPE_LEVELS:
        clear_for_new_test_case(tc);
        parse_levels(tc, line + strlen(PREFIX_LEVELS));
        break;

    case LINE_TYPE_REORDER:
        parse_reorder(tc, line + strlen(PREFIX_REORDER));
        break;

    case LINE_TYPE_SUBCASE:
        clear_for_new_sub_test_case(tc);
        parse_subcase(tc, line);
        break;
    }

    return 1;
}

static void check_levels(const struct test_case* tc, const BidiLevel* levels)
{
    for (int i = 0; i < tc->nr_levels; i++) {
        int level = (int)levels[i];
        if (tc->levels[i] != -1 && level != tc->levels[i]) {
            _ERR_PRINTF("%s failed: %d vs %d at index %d\n",
                    __FUNCTION__, tc->levels[i], level, i);
            goto failed;
        }
    }

    return;

failed:
    for (int i = 0; i < tc->nr_levels; i++) {
        int level = (int)levels[i];
        _ERR_PRINTF("%d ", level);
    }

    _ERR_PRINTF("\n");
    exit(1);
}

static void check_indics(const struct test_case* tc, int* indics, int check_len)
{
    for (int i = 0; i < check_len; i++) {
        if (tc->reorder_indics[i] != indics[i]) {
            _ERR_PRINTF("%s failed: %d vs %d at index %d\n",
                    __FUNCTION__, tc->reorder_indics[i], indics[i], i);
            exit(1);
        }
    }
}

static void do_test(const struct test_case* tc)
{
    BidiType base_dir;
    BidiLevel* levels;
    int* indics;
    int i;

    levels = (BidiLevel*)malloc(sizeof(BidiLevel) * tc->nr_levels);
    indics = (int*)malloc(sizeof(int) * tc->nr_levels);

    if (levels == NULL || indics == NULL) {
        _ERR_PRINTF("%s: Failed to allocate memory for levels and indics\n",
                __FUNCTION__);
        exit(1);
    }

    if (tc->bitset & 0x01) {
        base_dir = BIDI_PGDIR_ON;
    }
    else if (tc->bitset & 0x02) {
        base_dir = BIDI_PGDIR_LTR;
    }
    else if (tc->bitset & 0x04) {
        base_dir = BIDI_PGDIR_RTL;
    }
    else {
        _ERR_PRINTF("%s: bad bitset value: 0x%02x\n",
                __FUNCTION__, tc->bitset);
        exit(1);
    }

    if (UBidiGetParagraphEmbeddingLevels(tc->bidi_types,
                NULL, tc->nr_bidi_types,
                &base_dir, levels) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiGetParagraphEmbeddingLevels\n",
                __FUNCTION__);
        exit(1);
    }

    for (i = 0; i < tc->nr_bidi_types; i++) {
        indics[i] = i;
    }

    if (UBidiReorderLine(0, tc->bidi_types, tc->nr_bidi_types,
                0, base_dir, levels, NULL, indics, NULL, NULL) == 0) {
        _ERR_PRINTF("%s: Failed to call UBidiReorderLine\n",
                __FUNCTION__);
        exit(1);
    }

    check_levels(tc, levels);

    int j = 0;
    int nr_reordered;
    for (i = 0; i < tc->nr_bidi_types; i++) {
        if (!BIDI_IS_EXPLICIT_OR_BN (tc->bidi_types[indics[i]]))
            indics[j++] = indics[i];
    }
    nr_reordered = j;

    if (nr_reordered != tc->nr_reorder_indics) {
        _ERR_PRINTF("%s: the reorder indics length does not matched: %d | %d\n",
                __FUNCTION__, tc->nr_reorder_indics, nr_reordered);
        exit(1);
    }

    check_indics(tc, indics, nr_reordered);

    free(indics);
    free(levels);
}

static int bidi_test(const char* filename)
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

    init_test_case(&tc);
    while (TRUE) {

        if (!fgets(buff, MAX_LINE_LEN, fp)) {
            return 0;
        }

        line++;
        printf("==== LINE %d ====\n", line);
        printf("\t%s\n", buff);

        if (parse_one_line(buff, &tc) && tc.nr_bidi_types > 0) {
            printf("CASE: \n%s", buff);
            // true test here
            do_test(&tc);
        }
    }

    destroy_test_case(&tc);
    fclose(fp);
    return 0;
}

int MiniGUIMain (int argc, const char* argv[])
{
    _MG_PRINTF ("========= START TO TEST UBA (BidiTest.txt)\n");
    bidi_test("ucd/BidiTest.txt");
    _MG_PRINTF ("========= END OF TEST UBA (BidiTest.txt)\n");

    exit(0);
    return 0;
}

#else
#error "To test UBA implementation, please use MiniGUI 3.4.0 and enable support for UNICODE"
#endif /* checking version */

