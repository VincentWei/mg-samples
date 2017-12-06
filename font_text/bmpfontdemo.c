/*
** $Id$
**
** bmpf-test.c: Bitmap Font unit test.
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

void OnModeBmpfont(HWND hWnd, HDC hdc, PLOGFONT bmplogfont)
{
    LOGFONT *old;
    TextOut (hdc, 0, 0, "The result of bmp-led-rrncnn-10-15-ISO8859-1:");
    old = SelectFont (hdc, bmplogfont);
    SetBkColor (hdc, PIXEL_blue);
    TextOut (hdc, 10, 20, "013456");
    TextOut (hdc, 10, 40, "ABCD");
    TextOut (hdc, 10, 60, "AC");
    TextOut (hdc, 10, 80, "3.5+A:B-");            

    SelectFont (hdc, old);

}

