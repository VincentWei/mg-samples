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
** $Id: Arabic.c  2008-02-19 MaLin $
**
** The demo of font.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
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

RECT arabic_rc = {14, 0, 320, 250};

static int reset_fp_pos (void)
{
    arabic_rc.left = 14;
    arabic_rc.top = 0;
    arabic_rc.right = 800 -14;
    arabic_rc.bottom = 480;

    if (!fp){
        fp = fopen("arabictext.txt", "rb");
        if (!fp) {
            fprintf (stderr, "cannot open %s \n", "arabictext.txt");
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
        if (c == 0xa || c == 0xd) {
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
    int line_height = GetFontHeight (hdc);
    BOOL read_over = FALSE;
    while(1){
        pline = read_one_line(&read_over);
        if (!pline) continue;

        if (mode == TRUE){ //TextOut
            if (GetTextAlign (hdc) == TA_RIGHT)
                TextOut (hdc, arabic_rc.right, 10+i*25, pline);
            else
                TextOut (hdc, arabic_rc.left, 10+i*line_height, pline);
        }
        else {
            if (direction == TRUE)
                linenum = DrawText (hdc, pline, -1, &arabic_rc, DT_RIGHT);
            else
                linenum = DrawText (hdc, pline, -1, &arabic_rc, DT_LEFT);
            arabic_rc.top += linenum;
        }
        if (read_over) break;
        i++;
    }
} 

void OnModeArabic (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT iso8859_6)
{
    SelectFont (hdc, iso8859_6);
    //SetBkColor (hdc, COLOR_green);
    reset_fp_pos();

    switch (textmodedire) {
        case 1:
            SetTextAlign (hdc, TA_RIGHT);
            OnModeTextOut (hdc, TRUE, 0);
            break;
        case 2:
            SetTextAlign (hdc, TA_LEFT);
            OnModeTextOut (hdc, TRUE, 0);
            break;
        case 3:
            OnModeTextOut (hdc, FALSE, TRUE); //right
            break;
        case 4:
            OnModeTextOut (hdc, FALSE, FALSE);//left
            break;
    }
}


