/*
** bidilogachars2visachars.c
**
**  Test code for BIDILogAChars2VisAChars of MiniGUI 3.4.0
**  The following APIs are covered:
**
**      BIDIGetTextLogicalAChars
**      GetACharBIDIType
**      BIDIGetTextVisualAChars
**      BIDILogAChars2VisACharsEx
**      BIDIGetLogicalEmbedLevelsEx
**      BIDIGetVisualEmbedLevelsEx
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
#define MAX_UCHARS          128

#define TOKEN_HAVE_NO_BREAK_OPPORTUNITY "×"
#define TOKEN_HAVE_BREAK_OPPORTUNITY    "÷"

#define BREAK_NOTALLOWED                0
#define BREAK_ALLOWED                   1

#if 0
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

        if (*current == '#') {
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
#endif

static int test_from_file(PLOGFONT lf, const char* filename)
{
    FILE* fp = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        _ERR_PRINTF("%s: Failed to open %s file\n",
                __FUNCTION__, filename);
        return 1;
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

