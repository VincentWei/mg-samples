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
 *   This file is part of mGEff, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */

#ifndef  IMAGE_INC
#define  IMAGE_INC

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgeff/mgeff.h>

#define ORIGIN_WIDTH  320
#define ORIGIN_HEIGHT 240
#define REVIEW_WIDTH  95
#define REVIEW_HEIGHT 70
#define IMAGE_GAP 8
#define HEADER_WIDTH  20
#define HEADER_HEIGHT 20
#define FOOTER_WIDTH  20
#define FOOTER_HEIGHT 20
#define IMG_LINE_COUNT 4
#define IMG_ROW_COUNT 4
#define IMAGE_TOTAL (IMG_LINE_COUNT * IMG_ROW_COUNT)

typedef struct {
    RECT rc;
    RECT orig_rc;
    HDC  hdc;
    BOOL is_expanded;
} Image;

int  ImagesInitialize(void);
void ImagePaint(HDC hdc);
BOOL CursorInImage(int x, int y, int* index);
int ExpandImageWithAnimation(HWND hwnd, int index);
int ShrinkImageWithAnimation(HWND hwnd, int index);
void ImagesFinalize(void);
#endif   /* ----- #ifndef IMAGE_INC  ----- */

