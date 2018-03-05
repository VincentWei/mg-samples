/*! ============================================================================
 * \file image.c 
 * \brief  
 * \author DongKai
 * \version 1.0
 *
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "image.h"

const char *PICTURE_PATH[IMAGE_TOTAL] = {
    "./res/im1.jpg",
    "./res/im2.jpg",
    "./res/im3.jpg",
    "./res/im4.jpg",
    "./res/im5.jpg",
    "./res/im6.jpg",
    "./res/im7.jpg",
    "./res/im8.jpg",
    "./res/im9.jpg",
    "./res/im10.jpg",
    "./res/im11.jpg",
    "./res/im12.jpg",
    "./res/im13.jpg",
    "./res/im14.jpg",
    "./res/im15.jpg",
    "./res/im16.jpg"
};

Image *g_image[IMAGE_TOTAL];

static int g_id = 0;
static int is_expand = 0;

HDC createDCByPicture (HDC ref_dc, const char *path)
{
    BITMAP bmp;
    HDC dc;

    LoadBitmap (ref_dc, &bmp, path);

    dc = CreateCompatibleDCEx (ref_dc, bmp.bmWidth, bmp.bmHeight);

    FillBoxWithBitmap (dc, 0, 0, bmp.bmWidth, bmp.bmHeight, &bmp);

    UnloadBitmap (&bmp);

    return dc;
}

void do_expand (MGEFF_ANIMATION handle, HWND hwnd, int id, RECT * rc)
{
    if (id >= 0 && id < IMAGE_TOTAL) {
        g_id = id;
        is_expand = 1;

        g_image[id]->orig_rc = *rc;

        UpdateWindow (hwnd, FALSE);
    }
}

void do_shrink (MGEFF_ANIMATION handle, HWND hwnd, int id, RECT * rc)
{
    if (id >= 0 && id < IMAGE_TOTAL) {
        g_id = id;
        is_expand = 1;

        g_image[id]->orig_rc = *rc;

        UpdateWindow (hwnd, FALSE);
    }
}

int ImagesInitialize (void)
{
    int i, j;

    for (i = 0; i < IMG_ROW_COUNT; ++i) {
        for (j = 0; j < IMG_LINE_COUNT; ++j) {
            int n = i * IMG_LINE_COUNT + j;

            if (NULL != g_image[n]) {
                fprintf (stderr, "Please don't multi init images.\n");
                return -1;
            }

            g_image[n] = (Image *) calloc (1, sizeof (Image));

            if (NULL == g_image[n]) {
                fprintf (stderr, "Not enough memory!\n");
                exit (-1);
            }

            g_image[n]->hdc =
                createDCByPicture (HDC_SCREEN, PICTURE_PATH[n]);

            if (HDC_INVALID == g_image[n]->hdc) {
                fprintf (stderr, "Create DC by picture failed.\n");
                exit (-1);
            }

            g_image[n]->rc.left =
                HEADER_WIDTH + j * REVIEW_WIDTH + j * IMAGE_GAP;

            g_image[n]->rc.top =
                HEADER_HEIGHT + i * REVIEW_HEIGHT + i * IMAGE_GAP;

            g_image[n]->rc.right = g_image[n]->rc.left + REVIEW_WIDTH;

            g_image[n]->rc.bottom = g_image[n]->rc.top + REVIEW_HEIGHT;
        } /* end for */
    } /* end for */

    return 0;
}

void ImagesFinalize (void)
{
}

void ImagePaint (HDC hdc)
{
    int i, j;

    SetPenColor (hdc, COLOR_lightwhite);

    for (i = 0; i < IMG_ROW_COUNT; ++i) {
        for (j = 0; j < IMG_LINE_COUNT; ++j) {
            int n = i * IMG_LINE_COUNT + j;

            if (n == g_id) {
                BitBlt (g_image[n]->hdc, 0, 0, REVIEW_WIDTH, REVIEW_HEIGHT,
                        hdc, g_image[n]->rc.left, g_image[n]->rc.top, -1);

                BitBlt (g_image[g_id]->hdc,
                        0, 0,
                        RECTW (g_image[g_id]->orig_rc),
                        RECTH (g_image[g_id]->orig_rc),
                        g_image[g_id]->hdc,
                        g_image[g_id]->orig_rc.left,
                        g_image[g_id]->orig_rc.top,
                        -1);
            }
            else {
                BitBlt (g_image[n]->hdc, 0, 0, REVIEW_WIDTH, REVIEW_HEIGHT,
                        hdc, g_image[n]->rc.left, g_image[n]->rc.top, -1);
            }

            Rectangle (hdc,
                       g_image[n]->rc.left,
                       g_image[n]->rc.top,
                       g_image[n]->rc.right,
                       g_image[n]->rc.bottom);
        }
    }
}

BOOL CursorInImage (int x, int y, int *index)
{
    int i, j;

    for (i = 0; i < IMG_ROW_COUNT; ++i) {
        for (j = 0; j < IMG_LINE_COUNT; ++j) {
            int n = i * IMG_LINE_COUNT + j;

            if (PtInRect (&g_image[n]->rc, x, y)) {
                if (NULL != index)
                    *index = n;

                return TRUE;
            }
        }
    }
    return FALSE;
}

int ExpandImageWithAnimation (HWND hwnd, int index)
{
    MGEFF_ANIMATION animation;
    RECT dst_rc = { 0, 0, 0, 0 };
    BOOL is_ready = TRUE;

    if ((index >= 0 && index < 2) || (index >= 4 && index < 6)) {
        dst_rc.left = g_image[0]->rc.left;
        dst_rc.top = g_image[0]->rc.top;
        dst_rc.right = g_image[10]->rc.right;
        dst_rc.bottom = g_image[10]->rc.bottom;

        puts ("A");
    }
    else if ((index >= 8 && index < 10) || (index >= 12 && index < 14)) {
        dst_rc.left = g_image[0]->rc.left;
        dst_rc.top = g_image[4]->rc.top;
        dst_rc.right = g_image[6]->rc.right;
        dst_rc.bottom = g_image[12]->rc.bottom;

        puts ("B");
    }
    else if ((index >= 2 && index < 4) || (index >= 6 && index < 8)) {
        dst_rc.left = g_image[1]->rc.left;
        dst_rc.top = g_image[1]->rc.top;
        dst_rc.right = g_image[3]->rc.right;
        dst_rc.bottom = g_image[9]->rc.bottom;

        puts ("C");
    }
    else if ((index >= 10 && index < 12) || (index >= 14 && index < 16)) {
        dst_rc.left = g_image[5]->rc.left;
        dst_rc.top = g_image[5]->rc.top;
        dst_rc.right = g_image[15]->rc.right;
        dst_rc.bottom = g_image[15]->rc.bottom;

        puts ("D");
    }
    else {
        is_ready = FALSE;
    }

    if (is_ready) {
        animation =
            mGEffAnimationCreate ((void *) hwnd, (void *) do_expand,
                                  index, MGEFF_RECT);

        mGEffAnimationSetProperty (animation, MGEFF_PROP_KEEPALIVE, 0);

        mGEffAnimationSetStartValue (animation, &g_image[index]->rc);
        mGEffAnimationSetEndValue (animation, &dst_rc);

        mGEffAnimationSetDuration (animation, 1000);

        mGEffAnimationSetCurve (animation, Linear);

        mGEffAnimationAsyncRun (animation);

        return 0;
    }

    return -1;
}

int ShrinkImageWithAnimation (HWND hwnd, int index)
{
    MGEFF_ANIMATION animation;

    if (index >= 0 && index < IMAGE_TOTAL
        && g_image[index]->is_expanded) {
        animation =
            mGEffAnimationCreate ((void *) hwnd, (void *) do_shrink,
                                  index, MGEFF_RECT);

        mGEffAnimationSetProperty (animation, MGEFF_PROP_KEEPALIVE, 0);

        mGEffAnimationSetStartValue (animation,
                                     &g_image[index]->orig_rc);
        mGEffAnimationSetEndValue (animation, &g_image[index]->rc);

        mGEffAnimationSetDuration (animation, 1000);

        mGEffAnimationSetCurve (animation, Linear);

        mGEffAnimationAsyncRun (animation);

        return 0;
    }

    return -1;
}

