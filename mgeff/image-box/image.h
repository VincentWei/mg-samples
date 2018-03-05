/*!============================================================================
 * \file image.h 
 * \brief  
 * \author DongKai
 * \version 1.0
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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

