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
** $Id$
**
** The demo of font.
**
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
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
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

char linebuf[4096];
static FILE *fp = NULL;
static RECT drawtext_rc = {10, 10, 700, 480};

static int reset_fp_pos (void)
{
    drawtext_rc.left = 10;
    drawtext_rc.top = 10;
    drawtext_rc.right = 800-10;
    drawtext_rc.bottom = 600;

    if (!fp){
        fp = fopen("hangultext.txt", "rb");
        if (!fp) {
            fprintf (stderr, "cannot open %s \n", "hangultext.txt");
        }
    }
    if (fp){
        fseek(fp, 0, SEEK_SET);
    }
    return 1;
}

static char* read_one_line(BOOL* read_over)
{
    int i = 0;
    char c = 0;
    if (fp == NULL){
        *read_over = TRUE;
        return NULL;
    }
    memset (linebuf, 0, 4096);
    while(!(*read_over = feof(fp))) {
        c = fgetc(fp);
        if (c == 0xa) {
            break;
        }
        linebuf[i] = c;
        i++;
    }
    if (i > 0)
        return linebuf;
    else
        return NULL;
}

static void OnModeTextOut (HDC hdc, BOOL mode, BOOL direction)
{
    char* pline = NULL;
    int i = 0, linenum = 0;
    BOOL read_over = FALSE;
    while(1){
        pline = read_one_line(&read_over);
        if (read_over) break;
        if (!pline) continue;

        if (mode == TRUE){
            if (GetTextAlign (hdc) == TA_RIGHT)
                TextOut (hdc, drawtext_rc.right, 10+i*25, pline);
            else
                TextOut (hdc, drawtext_rc.left, 10+i*25, pline);
        }
        else{
            if (direction == TRUE)
                linenum = DrawText (hdc, pline, -1, &drawtext_rc, DT_RIGHT);
            else
                linenum = DrawText (hdc, pline, -1, &drawtext_rc, DT_LEFT);
            drawtext_rc.top += linenum;
        }
        i++;
    }
}

void OnModeHangul (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT ksc5601_0)
{
    SelectFont (hdc, ksc5601_0);
    //SetBkColor (hdc, COLOR_yellow);
    reset_fp_pos();

    switch (textmodedire) {
        case 1:
            SetTextAlign (hdc, TA_RIGHT);
            OnModeTextOut (hdc, TRUE, 0);
            break;
        case 2:
            SetTextAlign (hdc, TA_LEFT);
            OnModeTextOut (hdc, TRUE, 0); //TextOut
            break;
        case 3:
            SetTextAlign (hdc, TA_RIGHT);
            OnModeTextOut (hdc, FALSE, TRUE);
            break;
        case 4:
            SetTextAlign (hdc, TA_LEFT);
            OnModeTextOut (hdc, FALSE, FALSE); //DrawText
            break;
    }
}


