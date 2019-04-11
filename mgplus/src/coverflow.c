/*
 *   This file is part of mGPlus, a component for MiniGUI.
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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <stdint.h>
#include <sys/time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgplus/mgplus.h>

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480

#define DEFAULT_WIDTH        1280
#define DEFAULT_HEIGHT       720

static char* g_pBitmapName [] = {"./res/CoverFlow1.bmp",
                                 "./res/CoverFlow2.bmp",
                                 "./res/CoverFlow3.bmp",
                                 "./res/CoverFlow4.bmp",
                                 "./res/CoverFlow5.bmp",
                                 "./res/CoverFlow6.bmp",
                                 "./res/CoverFlow7.bmp"};
static int g_nTotalCount = sizeof (g_pBitmapName) / sizeof (g_pBitmapName [0]);
static BITMAP g_CoverBitmap [7];
static MPPOINT g_CoverUpPoint [7][4];
static int g_nFlameCount = 10;
static MPPOINT g_CoverDownPoint [7][4];
static MPPOINT g_OutCoverUpPoint [2][4];
static MPPOINT g_OutCoverDownPoint [2][4];

static HGRAPHICS g_hGraphics;
static HPATH g_hPath;
static HBRUSH g_hBrush;

typedef struct _ST_COVERFLOW_NODE
{
    MPPOINT mpUpPoint [4];
    MPPOINT mpDownPoint [4];
    struct _ST_COVERFLOW_NODE * pNextNode;
    struct _ST_COVERFLOW_NODE * pPreNode;
    int nImgIndex;
    int nCurPos;
}ST_COVERFLOW_NODE;

typedef ST_COVERFLOW_NODE* ST_COVERFLOW_NODE_PT;

static ST_COVERFLOW_NODE_PT g_pSTCoverFlowHead;
static ST_COVERFLOW_NODE_PT g_pSTCoverFlowCur;
static ST_COVERFLOW_NODE_PT g_pSTCoverFlowLeftPointer [4];
static ST_COVERFLOW_NODE_PT g_pSTCoverFlowRightPointer [4];
static int g_nLeftNumber;
static int g_nRightNumber;

static void ResetDownPoint (MPPOINT* pUpPoint, MPPOINT* pDownPoint)
{
    pDownPoint [0].x = pUpPoint [0].x;
    pDownPoint [0].y = pUpPoint [3].y + (pUpPoint [3].y - pUpPoint [0].y);

    pDownPoint [1].x = pUpPoint [1].x;
    pDownPoint [1].y = pUpPoint [2].y + (pUpPoint [2].y - pUpPoint [1].y);

    pDownPoint [2].x = pUpPoint [2].x;
    pDownPoint [2].y = pUpPoint [2].y;

    pDownPoint [3].x = pUpPoint [3].x;
    pDownPoint [3].y = pUpPoint [3].y;
}

static void InitCoverFlowPoints (int n_total_flame)
{
    MPPOINT* pCurUpPoint;
    MPPOINT* pCurDownPoint;

    g_nFlameCount = n_total_flame;
    //left first up cover
    pCurUpPoint = &g_CoverUpPoint [0][0];
    pCurDownPoint = &g_CoverDownPoint [0][0];

    pCurUpPoint [0].x = 0;
    pCurUpPoint [0].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 233 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //left first down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);


    //left second up cover
    pCurUpPoint = &g_CoverUpPoint [1][0];
    pCurDownPoint = &g_CoverDownPoint [1][0];

    pCurUpPoint [0].x = 80 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 313 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //left second down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);

    //left third up cover
    pCurUpPoint = &g_CoverUpPoint [2][0];
    pCurDownPoint = &g_CoverDownPoint [2][0];

    pCurUpPoint [0].x = 179 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 377 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //left third down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);


    //middle cover
    pCurUpPoint = &g_CoverUpPoint [6][0];
    pCurDownPoint = &g_CoverDownPoint [6][0];

    pCurUpPoint [0].x = 336 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 81 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 944 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 81 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 597 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 597 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //middle down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);


    //right first up cover
    pCurUpPoint = &g_CoverUpPoint [3][0];
    pCurDownPoint = &g_CoverDownPoint [3][0];

    pCurUpPoint [0].x = 1047 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 1280 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //right first down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);


    //right second up cover
    pCurUpPoint = &g_CoverUpPoint [4][0];
    pCurDownPoint = &g_CoverDownPoint [4][0];

    pCurUpPoint [0].x = 967 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 1200 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //right second down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);


    //right third up cover
    pCurUpPoint = &g_CoverUpPoint [5][0];
    pCurDownPoint = &g_CoverDownPoint [5][0];

    pCurUpPoint [0].x = 902 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 1099 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //right third down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);


    //right out up cover
    pCurUpPoint = &g_OutCoverUpPoint [1][0];
    pCurDownPoint = &g_OutCoverDownPoint [1][0];

    pCurUpPoint [0].x = 1280 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 1513 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [1].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //right out down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);

    //left out up cover
    pCurUpPoint = &g_OutCoverUpPoint [0][0];
    pCurDownPoint = &g_OutCoverDownPoint [0][0];

    pCurUpPoint [0].x = -233 * SCREEN_WIDTH / DEFAULT_WIDTH;
    pCurUpPoint [0].y = 186 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [1].x = 0;
    pCurUpPoint [1].y = 113 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [2].x = pCurUpPoint [1].x;
    pCurUpPoint [2].y = 562 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    pCurUpPoint [3].x = pCurUpPoint [0].x;
    pCurUpPoint [3].y = 489 * SCREEN_HEIGHT / DEFAULT_HEIGHT;

    //left out down cover
    ResetDownPoint (pCurUpPoint, pCurDownPoint);
}



static void RedrawCoverFlow (int nCount, BOOL bTurn)
{
    int i, nRightNode, nLeftNode;
    //unsigned int nFrontTickCount, nBackTickCount;

    nRightNode = 0;


    //nFrontTickCount = GetTickCount ();
    if (g_nRightNumber > 3)
        nRightNode = 4;
    else
        nRightNode = g_nRightNumber;

    if (g_nLeftNumber > 3)
        nLeftNode = 4;
    else
        nLeftNode = g_nLeftNumber;

    MGPlusGraphicClear (g_hGraphics, 0xFF000000);
    if (bTurn)
    {
        if (nCount > g_nFlameCount / 2)
        {
            //draw right
            for (i = nRightNode - 1; i >= 1 && (nRightNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }

            for (i = nLeftNode - 1; i >= 0 && (nLeftNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }

            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);

            MGPlusDrawImageWithPoints (g_hGraphics, \
                    g_pSTCoverFlowRightPointer [0]->nImgIndex,\
                    g_pSTCoverFlowRightPointer [0]->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, \
                    g_pSTCoverFlowRightPointer [0]->nImgIndex,\
                    g_pSTCoverFlowRightPointer [0]->mpDownPoint, 4);

            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [0]->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
        }
        else
        {
            for (i = nRightNode - 1; i >= 0 && (nRightNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }

            for (i = nLeftNode - 1; i >= 0 && (nLeftNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }

            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
        }
    }
    else
    {
        if (nCount > g_nFlameCount / 2)
        {
            for (i = nRightNode - 1; i >= 0 && (nRightNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }

#if 1
            for (i = nLeftNode - 1; i >= 1 && (nLeftNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }
#endif

            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);

            MGPlusDrawImageWithPoints (g_hGraphics, \
                    g_pSTCoverFlowLeftPointer [0]->nImgIndex,\
                    g_pSTCoverFlowLeftPointer [0]->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, \
                    g_pSTCoverFlowLeftPointer [0]->nImgIndex,\
                    g_pSTCoverFlowLeftPointer [0]->mpDownPoint, 4);

            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowLeftPointer [0]->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
        }
        else
        {
            for (i = nRightNode - 1; i >= 0 && (nRightNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }
            for (i = nLeftNode - 1; i >= 0 && (nLeftNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowLeftPointer [i]->nImgIndex,\
                        g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowLeftPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }
            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
        }

    }

    //nBackTickCount = GetTickCount ();
    //printf ("Draw tick count %d\n", nBackTickCount - nFrontTickCount);

    //nFrontTickCount = GetTickCount ();
    MGPlusGraphicSave (g_hGraphics, HDC_SCREEN, 0, 0, 0, 0, 0, 0);
    //nBackTickCount = GetTickCount ();
    //printf ("Save tick count %d\n", nBackTickCount - nFrontTickCount);
}


static void DrawCoverFlow (int nCount, BOOL bTurn)
{
    int i;
    MPPOINT* pSrcUpPoint = NULL;
    MPPOINT* pDstUpPoint = NULL;
    MPPOINT* pSrcDownPoint = NULL;
    MPPOINT* pDstDownPoint = NULL;
    MPPOINT* pCurUpPoint = NULL;
    MPPOINT* pCurDownPoint = NULL;

    /* prevent warning */
    i = (int)((intptr_t)pSrcDownPoint & (intptr_t)pDstDownPoint);

    if (bTurn)
    {
        for (i = 0; i < 4; i++)
        {
            if (!g_pSTCoverFlowRightPointer [i])
                break;
            else
            {
                if (i == 3)
                {
                    pSrcUpPoint = &g_OutCoverUpPoint [1][0];
                    pSrcDownPoint = &g_OutCoverDownPoint [1][0];

                    pDstUpPoint = &g_CoverUpPoint [3][0];
                    pDstDownPoint = &g_CoverDownPoint [3][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;

                }
                if (i == 2)
                {
                    pSrcUpPoint = &g_CoverUpPoint [3][0];
                    pSrcDownPoint = &g_CoverDownPoint [3][0];

                    pDstUpPoint = &g_CoverUpPoint [4][0];
                    pDstDownPoint = &g_CoverDownPoint [4][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;

                }
                if (i == 1)
                {
                    pSrcUpPoint = &g_CoverUpPoint [4][0];
                    pSrcDownPoint = &g_CoverDownPoint [4][0];

                    pDstUpPoint = &g_CoverUpPoint [5][0];
                    pDstDownPoint = &g_CoverDownPoint [5][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;

                }
                if (i == 0)
                {
                    pSrcUpPoint = &g_CoverUpPoint [5][0];
                    pSrcDownPoint = &g_CoverDownPoint [5][0];

                    pDstUpPoint = &g_CoverUpPoint [6][0];
                    pDstDownPoint = &g_CoverDownPoint [6][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;
                }

                pCurUpPoint [0].x = pSrcUpPoint [0].x - (pSrcUpPoint [0].x - pDstUpPoint [0].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [0].y = pSrcUpPoint [0].y - (pSrcUpPoint [0].y - pDstUpPoint [0].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [1].x = pSrcUpPoint [1].x - (pSrcUpPoint [1].x - pDstUpPoint [1].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [1].y = pSrcUpPoint [1].y - (pSrcUpPoint [1].y - pDstUpPoint [1].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [2].x = pSrcUpPoint [2].x - (pSrcUpPoint [2].x - pDstUpPoint [2].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [2].y = pSrcUpPoint [2].y - (pSrcUpPoint [2].y - pDstUpPoint [2].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [3].x = pSrcUpPoint [3].x - (pSrcUpPoint [3].x - pDstUpPoint [3].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [3].y = pSrcUpPoint [3].y - (pSrcUpPoint [3].y - pDstUpPoint [3].y)\
                                    * nCount / g_nFlameCount;

                ResetDownPoint (pCurUpPoint, pCurDownPoint);

            }
        }

        //do the middle
        {
            pSrcUpPoint = &g_CoverUpPoint [6][0];
            pSrcDownPoint = &g_CoverDownPoint [6][0];

            pDstUpPoint = &g_CoverUpPoint [2][0];
            pDstDownPoint = &g_CoverDownPoint [2][0];

            pCurUpPoint = g_pSTCoverFlowCur->mpUpPoint;
            pCurDownPoint = g_pSTCoverFlowCur->mpDownPoint;

            pCurUpPoint [0].x = pSrcUpPoint [0].x - (pSrcUpPoint [0].x - pDstUpPoint [0].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [0].y = pSrcUpPoint [0].y - (pSrcUpPoint [0].y - pDstUpPoint [0].y)\
                                * nCount / g_nFlameCount;

            pCurUpPoint [1].x = pSrcUpPoint [1].x - (pSrcUpPoint [1].x - pDstUpPoint [1].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [1].y = pSrcUpPoint [1].y - (pSrcUpPoint [1].y - pDstUpPoint [1].y)\
                                * nCount / g_nFlameCount;

            pCurUpPoint [2].x = pSrcUpPoint [2].x - (pSrcUpPoint [2].x - pDstUpPoint [2].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [2].y = pSrcUpPoint [2].y - (pSrcUpPoint [2].y - pDstUpPoint [2].y)\
                                * nCount / g_nFlameCount;

            pCurUpPoint [3].x = pSrcUpPoint [3].x - (pSrcUpPoint [3].x - pDstUpPoint [3].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [3].y = pSrcUpPoint [3].y - (pSrcUpPoint [3].y - pDstUpPoint [3].y)\
                                * nCount / g_nFlameCount;

            ResetDownPoint (pCurUpPoint, pCurDownPoint);
        }

        //do the left
        for (i = 0; i < 3; i++)
        {
            if (!g_pSTCoverFlowLeftPointer [i])
                break;
            else
            {
                if (i == 0)
                {
                    pSrcUpPoint = &g_CoverUpPoint [2][0];
                    pSrcDownPoint = &g_CoverDownPoint [2][0];

                    pDstUpPoint = &g_CoverUpPoint [1][0];
                    pDstDownPoint = &g_CoverDownPoint [1][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }

                if (i == 1)
                {
                    pSrcUpPoint = &g_CoverUpPoint [1][0];
                    pSrcDownPoint = &g_CoverDownPoint [1][0];

                    pDstUpPoint = &g_CoverUpPoint [0][0];
                    pDstDownPoint = &g_CoverDownPoint [0][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }
#if 0
                if (i == 2)
                {
                    pSrcUpPoint = &g_CoverUpPoint [0][0];
                    pSrcDownPoint = &g_CoverDownPoint [0][0];

                    pDstUpPoint = &g_OutCoverUpPoint [0][0];
                    pDstDownPoint = &g_OutCoverDownPoint [0][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;

                }

                if (i == 1)
                {
                    pSrcUpPoint = &g_CoverUpPoint [1][0];
                    pSrcDownPoint = &g_CoverDownPoint [1][0];

                    pDstUpPoint = &g_CoverUpPoint [0][0];
                    pDstDownPoint = &g_CoverDownPoint [0][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }

                if (i == 0)
                {
                    pSrcUpPoint = &g_CoverUpPoint [2][0];
                    pSrcDownPoint = &g_CoverDownPoint [2][0];

                    pDstUpPoint = &g_OutCoverUpPoint [1][0];
                    pDstDownPoint = &g_OutCoverDownPoint [1][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }
#endif

                pCurUpPoint [0].x = pSrcUpPoint [0].x - (pSrcUpPoint [0].x - pDstUpPoint [0].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [0].y = pSrcUpPoint [0].y - (pSrcUpPoint [0].y - pDstUpPoint [0].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [1].x = pSrcUpPoint [1].x - (pSrcUpPoint [1].x - pDstUpPoint [1].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [1].y = pSrcUpPoint [1].y - (pSrcUpPoint [1].y - pDstUpPoint [1].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [2].x = pSrcUpPoint [2].x - (pSrcUpPoint [2].x - pDstUpPoint [2].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [2].y = pSrcUpPoint [2].y - (pSrcUpPoint [2].y - pDstUpPoint [2].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [3].x = pSrcUpPoint [3].x - (pSrcUpPoint [3].x - pDstUpPoint [3].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [3].y = pSrcUpPoint [3].y - (pSrcUpPoint [3].y - pDstUpPoint [3].y)\
                                    * nCount / g_nFlameCount;

                ResetDownPoint (pCurUpPoint, pCurDownPoint);
            }
        }
    }
    else
    {
        //draw the right
        for (i = 0; i < 4; i++)
        {
            if (!g_pSTCoverFlowRightPointer [i])
                break;
            else
            {
                if (i == 2)
                {

                    pSrcUpPoint = &g_CoverUpPoint [3][0];
                    pSrcDownPoint = &g_CoverDownPoint [3][0];

                    pDstUpPoint = &g_OutCoverUpPoint [1][0];
                    pDstDownPoint = &g_OutCoverDownPoint [1][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;

                }
                if (i == 1)
                {

                    pSrcUpPoint = &g_CoverUpPoint [4][0];
                    pSrcDownPoint = &g_CoverDownPoint [4][0];

                    pDstUpPoint = &g_CoverUpPoint [3][0];
                    pDstDownPoint = &g_CoverDownPoint [3][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;

                }
                if (i == 0)
                {
                    pDstUpPoint = &g_CoverUpPoint [4][0];
                    pDstDownPoint = &g_CoverDownPoint [4][0];

                    pSrcUpPoint = &g_CoverUpPoint [5][0];
                    pSrcDownPoint = &g_CoverDownPoint [5][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;

                }
#if 0
                if (i == 0)
                {
                    pSrcUpPoint = &g_CoverUpPoint [5][0];
                    pSrcDownPoint = &g_CoverDownPoint [5][0];

                    pDstUpPoint = &g_CoverUpPoint [6][0];
                    pDstDownPoint = &g_CoverDownPoint [6][0];

                    pCurUpPoint = g_pSTCoverFlowRightPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowRightPointer [i]->mpDownPoint;
                }
#endif

                pCurUpPoint [0].x = pSrcUpPoint [0].x - (pSrcUpPoint [0].x - pDstUpPoint [0].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [0].y = pSrcUpPoint [0].y - (pSrcUpPoint [0].y - pDstUpPoint [0].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [1].x = pSrcUpPoint [1].x - (pSrcUpPoint [1].x - pDstUpPoint [1].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [1].y = pSrcUpPoint [1].y - (pSrcUpPoint [1].y - pDstUpPoint [1].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [2].x = pSrcUpPoint [2].x - (pSrcUpPoint [2].x - pDstUpPoint [2].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [2].y = pSrcUpPoint [2].y - (pSrcUpPoint [2].y - pDstUpPoint [2].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [3].x = pSrcUpPoint [3].x - (pSrcUpPoint [3].x - pDstUpPoint [3].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [3].y = pSrcUpPoint [3].y - (pSrcUpPoint [3].y - pDstUpPoint [3].y)\
                                    * nCount / g_nFlameCount;

                ResetDownPoint (pCurUpPoint, pCurDownPoint);
            }

        }

        //do the middle
        {
            pSrcUpPoint = &g_CoverUpPoint [6][0];
            pSrcDownPoint = &g_CoverDownPoint [6][0];

            pDstUpPoint = &g_CoverUpPoint [5][0];
            pDstDownPoint = &g_CoverDownPoint [5][0];

            pCurUpPoint = g_pSTCoverFlowCur->mpUpPoint;
            pCurDownPoint = g_pSTCoverFlowCur->mpDownPoint;

            pCurUpPoint [0].x = pSrcUpPoint [0].x - (pSrcUpPoint [0].x - pDstUpPoint [0].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [0].y = pSrcUpPoint [0].y - (pSrcUpPoint [0].y - pDstUpPoint [0].y)\
                                * nCount / g_nFlameCount;

            pCurUpPoint [1].x = pSrcUpPoint [1].x - (pSrcUpPoint [1].x - pDstUpPoint [1].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [1].y = pSrcUpPoint [1].y - (pSrcUpPoint [1].y - pDstUpPoint [1].y)\
                                * nCount / g_nFlameCount;

            pCurUpPoint [2].x = pSrcUpPoint [2].x - (pSrcUpPoint [2].x - pDstUpPoint [2].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [2].y = pSrcUpPoint [2].y - (pSrcUpPoint [2].y - pDstUpPoint [2].y)\
                                * nCount / g_nFlameCount;

            pCurUpPoint [3].x = pSrcUpPoint [3].x - (pSrcUpPoint [3].x - pDstUpPoint [3].x)\
                                * nCount / g_nFlameCount;
            pCurUpPoint [3].y = pSrcUpPoint [3].y - (pSrcUpPoint [3].y - pDstUpPoint [3].y)\
                                * nCount / g_nFlameCount;

            ResetDownPoint (pCurUpPoint, pCurDownPoint);
        }

        for (i = 0; i < 4; i++)
        {
            if (!g_pSTCoverFlowLeftPointer [i])
                break;
            else
            {
                if (i == 0)
                {
                    pSrcUpPoint = &g_CoverUpPoint [2][0];
                    pSrcDownPoint = &g_CoverDownPoint [2][0];

                    pDstUpPoint = &g_CoverUpPoint [6][0];
                    pDstDownPoint = &g_CoverDownPoint [6][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }
#if 1
                if (i == 1)
                {
                    pSrcUpPoint = &g_CoverUpPoint [1][0];
                    pSrcDownPoint = &g_CoverDownPoint [1][0];

                    pDstUpPoint = &g_CoverUpPoint [2][0];
                    pDstDownPoint = &g_CoverDownPoint [2][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }

                if (i == 2)
                {
                    pSrcUpPoint = &g_CoverUpPoint [0][0];
                    pSrcDownPoint = &g_CoverDownPoint [0][0];

                    pDstUpPoint = &g_CoverUpPoint [1][0];
                    pDstDownPoint = &g_CoverDownPoint [1][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }

                if (i == 3)
                {
                    pSrcUpPoint = &g_OutCoverUpPoint [0][0];
                    pSrcDownPoint = &g_OutCoverDownPoint [0][0];

                    pDstUpPoint = &g_CoverUpPoint [0][0];
                    pDstDownPoint = &g_CoverDownPoint [0][0];

                    pCurUpPoint = g_pSTCoverFlowLeftPointer [i]->mpUpPoint;
                    pCurDownPoint = g_pSTCoverFlowLeftPointer [i]->mpDownPoint;
                }
#endif
                pCurUpPoint [0].x = pSrcUpPoint [0].x - (pSrcUpPoint [0].x - pDstUpPoint [0].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [0].y = pSrcUpPoint [0].y - (pSrcUpPoint [0].y - pDstUpPoint [0].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [1].x = pSrcUpPoint [1].x - (pSrcUpPoint [1].x - pDstUpPoint [1].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [1].y = pSrcUpPoint [1].y - (pSrcUpPoint [1].y - pDstUpPoint [1].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [2].x = pSrcUpPoint [2].x - (pSrcUpPoint [2].x - pDstUpPoint [2].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [2].y = pSrcUpPoint [2].y - (pSrcUpPoint [2].y - pDstUpPoint [2].y)\
                                    * nCount / g_nFlameCount;

                pCurUpPoint [3].x = pSrcUpPoint [3].x - (pSrcUpPoint [3].x - pDstUpPoint [3].x)\
                                    * nCount / g_nFlameCount;
                pCurUpPoint [3].y = pSrcUpPoint [3].y - (pSrcUpPoint [3].y - pDstUpPoint [3].y)\
                                    * nCount / g_nFlameCount;

                ResetDownPoint (pCurUpPoint, pCurDownPoint);
            }
        }
    }

    RedrawCoverFlow (nCount, bTurn);

    if (bTurn)
    {
        if (nCount >= g_nFlameCount)
        {

            if (g_nLeftNumber == 2)
            {
                g_pSTCoverFlowLeftPointer [3] = NULL;
                g_pSTCoverFlowLeftPointer [2] = g_pSTCoverFlowLeftPointer [1];
                g_pSTCoverFlowLeftPointer [1] = g_pSTCoverFlowLeftPointer [0];
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowCur;
            }
            else if (g_nLeftNumber == 1)
            {
                g_pSTCoverFlowLeftPointer [3] = NULL;
                g_pSTCoverFlowLeftPointer [2] = NULL;
                g_pSTCoverFlowLeftPointer [1] = g_pSTCoverFlowLeftPointer [0];
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowCur;
            }
            else if (g_nLeftNumber == 0)
            {
                g_pSTCoverFlowLeftPointer [3] = NULL;
                g_pSTCoverFlowLeftPointer [2] = NULL;
                g_pSTCoverFlowLeftPointer [1] = NULL;
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowCur;
            }
            else
            {
                g_pSTCoverFlowLeftPointer [3] = g_pSTCoverFlowLeftPointer [2];
                g_pSTCoverFlowLeftPointer [2] = g_pSTCoverFlowLeftPointer [1];
                g_pSTCoverFlowLeftPointer [1] = g_pSTCoverFlowLeftPointer [0];
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowCur;

            }

            g_nLeftNumber ++;

            g_pSTCoverFlowCur = g_pSTCoverFlowRightPointer [0];

            if (g_nRightNumber > 0)
                g_nRightNumber -- ;

            if (g_nRightNumber > 3)
            {
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowRightPointer [1];
                g_pSTCoverFlowRightPointer [1] = g_pSTCoverFlowRightPointer [2];
                g_pSTCoverFlowRightPointer [2] = g_pSTCoverFlowRightPointer [3];
                g_pSTCoverFlowRightPointer [3] = g_pSTCoverFlowRightPointer [3]->pNextNode;
            }
            else if (g_nRightNumber == 3)
            {
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowRightPointer [1];
                g_pSTCoverFlowRightPointer [1] = g_pSTCoverFlowRightPointer [2];
                g_pSTCoverFlowRightPointer [2] = g_pSTCoverFlowRightPointer [2]->pNextNode;
                g_pSTCoverFlowRightPointer [3] = NULL;
            }
            else if (g_nRightNumber == 2)
            {
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowRightPointer [1];
                g_pSTCoverFlowRightPointer [1] = g_pSTCoverFlowRightPointer [2];
                g_pSTCoverFlowRightPointer [2] = NULL;
                g_pSTCoverFlowRightPointer [3] = NULL;
            }
            else if (g_nRightNumber == 1)
            {
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowRightPointer [1];
                g_pSTCoverFlowRightPointer [1] = NULL;
                g_pSTCoverFlowRightPointer [2] = NULL;
                g_pSTCoverFlowRightPointer [3] = NULL;
            }
            else
            {
                g_pSTCoverFlowRightPointer [0] = NULL;
                g_pSTCoverFlowRightPointer [1] = NULL;
                g_pSTCoverFlowRightPointer [2] = NULL;
                g_pSTCoverFlowRightPointer [3] = NULL;
            }
        }
    }
    else
    {
        if (nCount >= g_nFlameCount)
        {

            if (g_nRightNumber >= 2)
            {
                g_pSTCoverFlowRightPointer [3] = g_pSTCoverFlowRightPointer [2];
                g_pSTCoverFlowRightPointer [2] = g_pSTCoverFlowRightPointer [1];
                g_pSTCoverFlowRightPointer [1] = g_pSTCoverFlowRightPointer [0];
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowCur;
            }
            else if (g_nRightNumber == 1)
            {
                g_pSTCoverFlowRightPointer [3] = NULL;
                g_pSTCoverFlowRightPointer [2] = NULL;
                g_pSTCoverFlowRightPointer [1] = g_pSTCoverFlowRightPointer [0];
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowCur;
            }
            else if (g_nRightNumber == 0)
            {
                g_pSTCoverFlowRightPointer [3] = NULL;
                g_pSTCoverFlowRightPointer [2] = NULL;
                g_pSTCoverFlowRightPointer [1] = NULL;
                g_pSTCoverFlowRightPointer [0] = g_pSTCoverFlowCur;
            }
            g_nRightNumber ++;

            g_pSTCoverFlowCur = g_pSTCoverFlowLeftPointer [0];

            g_nLeftNumber -- ;
            if (g_nLeftNumber >= 3)
            {
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowLeftPointer [1];
                g_pSTCoverFlowLeftPointer [1] = g_pSTCoverFlowLeftPointer [2];
                g_pSTCoverFlowLeftPointer [2] = g_pSTCoverFlowLeftPointer [3];
                g_pSTCoverFlowLeftPointer [3] = g_pSTCoverFlowLeftPointer [3]->pPreNode;
            }
            else if (g_nLeftNumber == 2)
            {
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowLeftPointer [1];
                g_pSTCoverFlowLeftPointer [1] = g_pSTCoverFlowLeftPointer [2];
                g_pSTCoverFlowLeftPointer [2] = NULL;
                g_pSTCoverFlowLeftPointer [3] = NULL;
            }
            else if (g_nLeftNumber == 1)
            {
                g_pSTCoverFlowLeftPointer [0] = g_pSTCoverFlowLeftPointer [1];
                g_pSTCoverFlowLeftPointer [1] = NULL;
            }
            else
            {
                g_pSTCoverFlowLeftPointer [0] = NULL;
            }



        }
    }
}


#if 0
static void CoverFlowAnimate (int interval, BOOL bTurn)
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;

    int count;
    int interval_sec;
    int interval_nsec;
    int frame_num = 10;
    struct timeval tv;
    int start_time;

    gettimeofday(&tv,NULL);
    start_time =tv.tv_sec*1000 + (tv.tv_usec+999)/1000;

    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex, NULL);

    interval_sec = interval/1000;
    interval_nsec = (interval%1000)*1000000;

    //if (enter)
    //    count = 0;
    //else
    count = 0;//frame_num;

    while(1)
    {
        struct timeval tv;
        struct timespec timeout;

        gettimeofday(&tv, NULL);
        timeout.tv_sec = tv.tv_sec + interval_sec;
        timeout.tv_nsec = tv.tv_usec*1000 + interval_nsec;
        if(timeout.tv_nsec > 1000000000){
            timeout.tv_sec ++;
            timeout.tv_nsec -= 1000000000;
        }

        pthread_mutex_lock(&mutex);
        DrawCoverFlow (count, bTurn);

        if (1) {
            count++;
            if (count > frame_num) {
                pthread_mutex_unlock (&mutex);
                break;
            }
        }
        else {
            count--;
            if (count < 0) {
                pthread_mutex_unlock (&mutex);
                break;
            }
        }
        pthread_cond_timedwait(&cond, &mutex, &timeout);
        pthread_mutex_unlock(&mutex);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}
#endif

static void InitCoverFlowNodePos (int nImgNumber)
{
    ST_COVERFLOW_NODE_PT pCurNode;
    int i;
    MPPOINT* pCurUpPoint;
    MPPOINT* pCurDownPoint;

    pCurNode = g_pSTCoverFlowCur;

    for (i = 0; i < nImgNumber; i++)
    {
        if (i == 0)
        {
            pCurUpPoint = &g_CoverUpPoint [6][0];
            pCurDownPoint = &g_CoverDownPoint [6][0];

            pCurNode->mpUpPoint [0].x = pCurUpPoint [0].x;
            pCurNode->mpUpPoint [0].y = pCurUpPoint [0].y;

            pCurNode->mpUpPoint [1].x = pCurUpPoint [1].x;
            pCurNode->mpUpPoint [1].y = pCurUpPoint [1].y;

            pCurNode->mpUpPoint [2].x = pCurUpPoint [2].x;
            pCurNode->mpUpPoint [2].y = pCurUpPoint [2].y;

            pCurNode->mpUpPoint [3].x = pCurUpPoint [3].x;
            pCurNode->mpUpPoint [3].y = pCurUpPoint [3].y;

            pCurNode->mpDownPoint [0].x = pCurDownPoint [0].x;
            pCurNode->mpDownPoint [0].y = pCurDownPoint [0].y;

            pCurNode->mpDownPoint [1].x = pCurDownPoint [1].x;
            pCurNode->mpDownPoint [1].y = pCurDownPoint [1].y;

            pCurNode->mpDownPoint [2].x = pCurDownPoint [2].x;
            pCurNode->mpDownPoint [2].y = pCurDownPoint [2].y;

            pCurNode->mpDownPoint [3].x = pCurDownPoint [3].x;
            pCurNode->mpDownPoint [3].y = pCurDownPoint [3].y;
        }
        else if (i == 1)
        {
            pCurUpPoint = &g_CoverUpPoint [5][0];
            pCurDownPoint = &g_CoverDownPoint [5][0];

            pCurNode->mpUpPoint [0].x = pCurUpPoint [0].x;
            pCurNode->mpUpPoint [0].y = pCurUpPoint [0].y;

            pCurNode->mpUpPoint [1].x = pCurUpPoint [1].x;
            pCurNode->mpUpPoint [1].y = pCurUpPoint [1].y;

            pCurNode->mpUpPoint [2].x = pCurUpPoint [2].x;
            pCurNode->mpUpPoint [2].y = pCurUpPoint [2].y;

            pCurNode->mpUpPoint [3].x = pCurUpPoint [3].x;
            pCurNode->mpUpPoint [3].y = pCurUpPoint [3].y;

            pCurNode->mpDownPoint [0].x = pCurDownPoint [0].x;
            pCurNode->mpDownPoint [0].y = pCurDownPoint [0].y;

            pCurNode->mpDownPoint [1].x = pCurDownPoint [1].x;
            pCurNode->mpDownPoint [1].y = pCurDownPoint [1].y;

            pCurNode->mpDownPoint [2].x = pCurDownPoint [2].x;
            pCurNode->mpDownPoint [2].y = pCurDownPoint [2].y;

            pCurNode->mpDownPoint [3].x = pCurDownPoint [3].x;
            pCurNode->mpDownPoint [3].y = pCurDownPoint [3].y;
        }
        else if (i == 2)
        {
            pCurUpPoint = &g_CoverUpPoint [4][0];
            pCurDownPoint = &g_CoverDownPoint [4][0];

            pCurNode->mpUpPoint [0].x = pCurUpPoint [0].x;
            pCurNode->mpUpPoint [0].y = pCurUpPoint [0].y;

            pCurNode->mpUpPoint [1].x = pCurUpPoint [1].x;
            pCurNode->mpUpPoint [1].y = pCurUpPoint [1].y;

            pCurNode->mpUpPoint [2].x = pCurUpPoint [2].x;
            pCurNode->mpUpPoint [2].y = pCurUpPoint [2].y;

            pCurNode->mpUpPoint [3].x = pCurUpPoint [3].x;
            pCurNode->mpUpPoint [3].y = pCurUpPoint [3].y;

            pCurNode->mpDownPoint [0].x = pCurDownPoint [0].x;
            pCurNode->mpDownPoint [0].y = pCurDownPoint [0].y;

            pCurNode->mpDownPoint [1].x = pCurDownPoint [1].x;
            pCurNode->mpDownPoint [1].y = pCurDownPoint [1].y;

            pCurNode->mpDownPoint [2].x = pCurDownPoint [2].x;
            pCurNode->mpDownPoint [2].y = pCurDownPoint [2].y;

            pCurNode->mpDownPoint [3].x = pCurDownPoint [3].x;
            pCurNode->mpDownPoint [3].y = pCurDownPoint [3].y;
        }
        else if (i == 3)
        {
            pCurUpPoint = &g_CoverUpPoint [3][0];
            pCurDownPoint = &g_CoverDownPoint [3][0];

            pCurNode->mpUpPoint [0].x = pCurUpPoint [0].x;
            pCurNode->mpUpPoint [0].y = pCurUpPoint [0].y;

            pCurNode->mpUpPoint [1].x = pCurUpPoint [1].x;
            pCurNode->mpUpPoint [1].y = pCurUpPoint [1].y;

            pCurNode->mpUpPoint [2].x = pCurUpPoint [2].x;
            pCurNode->mpUpPoint [2].y = pCurUpPoint [2].y;

            pCurNode->mpUpPoint [3].x = pCurUpPoint [3].x;
            pCurNode->mpUpPoint [3].y = pCurUpPoint [3].y;

            pCurNode->mpDownPoint [0].x = pCurDownPoint [0].x;
            pCurNode->mpDownPoint [0].y = pCurDownPoint [0].y;

            pCurNode->mpDownPoint [1].x = pCurDownPoint [1].x;
            pCurNode->mpDownPoint [1].y = pCurDownPoint [1].y;

            pCurNode->mpDownPoint [2].x = pCurDownPoint [2].x;
            pCurNode->mpDownPoint [2].y = pCurDownPoint [2].y;

            pCurNode->mpDownPoint [3].x = pCurDownPoint [3].x;
            pCurNode->mpDownPoint [3].y = pCurDownPoint [3].y;
        }
        else
        {
            pCurUpPoint = &g_OutCoverUpPoint [1][0];
            pCurDownPoint = &g_OutCoverDownPoint [1][0];

            pCurNode->mpUpPoint [0].x = pCurUpPoint [0].x;
            pCurNode->mpUpPoint [0].y = pCurUpPoint [0].y;

            pCurNode->mpUpPoint [1].x = pCurUpPoint [1].x;
            pCurNode->mpUpPoint [1].y = pCurUpPoint [1].y;

            pCurNode->mpUpPoint [2].x = pCurUpPoint [2].x;
            pCurNode->mpUpPoint [2].y = pCurUpPoint [2].y;

            pCurNode->mpUpPoint [3].x = pCurUpPoint [3].x;
            pCurNode->mpUpPoint [3].y = pCurUpPoint [3].y;

            pCurNode->mpDownPoint [0].x = pCurDownPoint [0].x;
            pCurNode->mpDownPoint [0].y = pCurDownPoint [0].y;

            pCurNode->mpDownPoint [1].x = pCurDownPoint [1].x;
            pCurNode->mpDownPoint [1].y = pCurDownPoint [1].y;

            pCurNode->mpDownPoint [2].x = pCurDownPoint [2].x;
            pCurNode->mpDownPoint [2].y = pCurDownPoint [2].y;

            pCurNode->mpDownPoint [3].x = pCurDownPoint [3].x;
            pCurNode->mpDownPoint [3].y = pCurDownPoint [3].y;
        }

        pCurNode = pCurNode->pNextNode;
    }
}

static void InitCoverFlowNode (int nImgNumber)
{
    int i;

    if (nImgNumber >= 7)
    {
        ST_COVERFLOW_NODE_PT pCurNode;

        g_pSTCoverFlowHead = (ST_COVERFLOW_NODE_PT) malloc (sizeof (ST_COVERFLOW_NODE));

        pCurNode = g_pSTCoverFlowHead;
        pCurNode->nImgIndex = 0;
        pCurNode->pNextNode = NULL;
        pCurNode->pPreNode = NULL;

        for (i = 1; i < nImgNumber; i++)
        {
            pCurNode->pNextNode = (ST_COVERFLOW_NODE_PT) malloc (sizeof (ST_COVERFLOW_NODE));
            (pCurNode->pNextNode)->pPreNode = pCurNode;
            pCurNode = pCurNode->pNextNode;
            pCurNode->nImgIndex = i;
            pCurNode->pNextNode = NULL;
        }
        g_pSTCoverFlowCur = g_pSTCoverFlowHead;
        g_pSTCoverFlowLeftPointer [0] = NULL;
        g_pSTCoverFlowLeftPointer [1] = NULL;
        g_pSTCoverFlowLeftPointer [2] = NULL;
        g_pSTCoverFlowLeftPointer [3] = NULL;
        g_nLeftNumber = 0;

        g_nRightNumber = nImgNumber - 1;
        pCurNode = g_pSTCoverFlowHead->pNextNode;
        g_pSTCoverFlowRightPointer [0] = pCurNode;
        pCurNode = pCurNode->pNextNode;
        g_pSTCoverFlowRightPointer [1] = pCurNode;
        pCurNode = pCurNode->pNextNode;
        g_pSTCoverFlowRightPointer [2] = pCurNode;
        pCurNode = pCurNode->pNextNode;
        g_pSTCoverFlowRightPointer [3] = pCurNode;

    }
    else
    {
        g_pSTCoverFlowHead = (ST_COVERFLOW_NODE_PT) malloc (sizeof (ST_COVERFLOW_NODE));
        ST_COVERFLOW_NODE_PT pCurNode;

        pCurNode = g_pSTCoverFlowHead;
        pCurNode->nImgIndex = 0;
        pCurNode->pNextNode = NULL;
        pCurNode->pPreNode = NULL;

        g_pSTCoverFlowRightPointer [0] = NULL;
        g_pSTCoverFlowRightPointer [1] = NULL;
        g_pSTCoverFlowRightPointer [2] = NULL;
        g_pSTCoverFlowRightPointer [3] = NULL;

        for (i = 1; i < nImgNumber; i++)
        {
            pCurNode->pNextNode = (ST_COVERFLOW_NODE_PT) malloc (sizeof (ST_COVERFLOW_NODE));
            (pCurNode->pNextNode)->pPreNode = pCurNode;
            pCurNode = pCurNode->pNextNode;
            g_pSTCoverFlowRightPointer [i - 1] = pCurNode;
            pCurNode->nImgIndex = i;
            pCurNode->pNextNode = NULL;
        }

        g_pSTCoverFlowCur = g_pSTCoverFlowHead;
        g_pSTCoverFlowLeftPointer [0] = NULL;
        g_pSTCoverFlowLeftPointer [1] = NULL;
        g_pSTCoverFlowLeftPointer [2] = NULL;
        g_pSTCoverFlowLeftPointer [3] = NULL;
        g_nLeftNumber = 0;

        g_nRightNumber = nImgNumber - 1;
    }
    InitCoverFlowNodePos (nImgNumber);

}

BOOL g_bStartUp = TRUE;

static LRESULT CoverFlowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
            {
                int i;
                InitCoverFlowPoints (10);
                g_hPath = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);
                g_hBrush = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
                g_hGraphics = MGPlusGraphicCreateFromDC (HDC_SCREEN);
                for (i = 0; i < g_nTotalCount; i ++)
                {
                    MGPlusGraphicLoadBitmap (g_hGraphics, i, &g_CoverBitmap [i]);
                }
                MGPlusSetSolidBrushColor (g_hBrush, 0x7F000000);
                MGPlusSetCompositingMode (g_hGraphics, MP_COMP_OP_SRC_OVER);
                InitCoverFlowNode (7);
            };
        break;

        case MSG_PAINT:
        {
#if 0
            int i, nRightNode;

            nRightNode = 0;

            if (g_nRightNumber > 3)
                nRightNode = 4;
            else
                nRightNode = g_nRightNumber;

            MGPlusGraphicClear (g_hGraphics, 0xFF000000);
            for (i = nRightNode - 1; i >= 0 && (nRightNode != 0); i--)
            {
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpUpPoint, 4);
                MGPlusDrawImageWithPoints (g_hGraphics, \
                        g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                        g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);

                MGPlusPathReset (g_hPath);
                MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);
                MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
            }

            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpUpPoint, 4);
            MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                    g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusPathReset (g_hPath);
            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowCur->mpDownPoint, 4);
            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);

            MGPlusGraphicSave (g_hGraphics, HDC_SCREEN, 0, 0, 0, 0, 0, 0);
#endif
            if (g_bStartUp)
            {
                SendMessage (hWnd, MSG_KEYUP, SCANCODE_F2, 0);
                g_bStartUp = FALSE;
            }
        }
        return 0;

        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            switch(scancode)
            {
                case SCANCODE_F1:
                    {
                        HDC hdc = GetClientDC (hWnd);
                        unsigned int nBTickCount, nFTickCount;
                        int i;
                        if (g_hGraphics == MP_GENERIC_ERROR)
                        {
                            printf("create graphics failed\n");
                            return 0;
                        }

                        for (i = 0; i < 7; i++)
                        {
                            nFTickCount = GetTickCount ();
                            MGPlusDrawImageWithPoints (g_hGraphics, i, &g_CoverUpPoint [i][0], 4);
                            MGPlusDrawImageWithPoints (g_hGraphics, i, &g_CoverDownPoint [i][0], 4);
                            MGPlusPathReset (g_hPath);
                            MGPlusPathAddLines (g_hPath, &g_CoverDownPoint [i][0], 4);
                            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
                            nBTickCount = GetTickCount ();
                            printf ("%d: F1 Draw tick count %d\n", i, nBTickCount - nFTickCount);
                        }

                        if (MGPlusGraphicSave (g_hGraphics, hdc, 0, 0, 0, 0, 0, 0) == MP_OK)
                            printf ("save graphic ok!\n");

                        ReleaseDC (hdc);
                    }
                    break;
                case SCANCODE_F2:
                    {
                        int i, nRightNode;

                        nRightNode = 0;

                        if (g_nRightNumber > 3)
                            nRightNode = 4;
                        else
                            nRightNode = g_nRightNumber;

                        MGPlusGraphicClear (g_hGraphics, 0xFF000000);
                        for (i = nRightNode - 1; i >= 0 && (nRightNode != 0); i--)
                        {
                            MGPlusDrawImageWithPoints (g_hGraphics, \
                                    g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                                    g_pSTCoverFlowRightPointer [i]->mpUpPoint, 4);
                            MGPlusDrawImageWithPoints (g_hGraphics, \
                                    g_pSTCoverFlowRightPointer [i]->nImgIndex,\
                                    g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);

                            MGPlusPathReset (g_hPath);
                            MGPlusPathAddLines (g_hPath, g_pSTCoverFlowRightPointer [i]->mpDownPoint, 4);
                            MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);
                        }

                        MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                                g_pSTCoverFlowCur->mpUpPoint, 4);
                        MGPlusDrawImageWithPoints (g_hGraphics, g_pSTCoverFlowCur->nImgIndex,
                                g_pSTCoverFlowCur->mpDownPoint, 4);
                        MGPlusPathReset (g_hPath);
                        MGPlusPathAddLines (g_hPath, g_pSTCoverFlowCur->mpDownPoint, 4);
                        MGPlusFillPath (g_hGraphics, g_hBrush, g_hPath);

                        MGPlusGraphicSave (g_hGraphics, HDC_SCREEN, 0, 0, 0, 0, 0, 0);
                    }
                    break;

                case SCANCODE_CURSORBLOCKLEFT:
                    {
                        //CoverFlowAnimate (1, TRUE);

                        int count;
                        count = 0;

                        if (g_nRightNumber > 0)
                        {
                            while (count <= g_nFlameCount)
                            {
                                DrawCoverFlow (count, TRUE);
                                if (count == 8)
                                    count = g_nFlameCount;
                                else
                                    count = count + 4;
                            }
                        }

                    }
                    break;
                case SCANCODE_CURSORBLOCKRIGHT:
                    {
                        int count;
                        count = 0;

                        if (g_nLeftNumber > 0)
                        {
                            while (count <= g_nFlameCount)
                            {
                                DrawCoverFlow (count, FALSE);
                                if (count == 8)
                                    count = g_nFlameCount;
                                else
                                    count = count + 4;
                            }
                        }
                    }
                    break;
            }
        }
        break;


        case MSG_RBUTTONDOWN:
        {

            MGPlusGraphicClear (g_hGraphics, 0xFF000000);
            MGPlusGraphicSave (g_hGraphics, HDC_SCREEN, 0, 0, 0, 0, 0, 0);
        }
        break;

        case MSG_CLOSE:
        MGPlusGraphicDelete (g_hGraphics);
        MGPlusBrushDelete (g_hBrush);
        MGPlusPathDelete (g_hPath);
        DestroyMainWindow(hWnd);
        PostQuitMessage(hWnd);
        return 0;
        default:
        break;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;
    int i;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "coverflow", 0, 0);
#endif

#ifdef _MISC_MOUSECALIBRATE
    mouse_calibrate();
#endif

    for (i = 0; i < g_nTotalCount; i ++)
    {
        LoadBitmap (HDC_SCREEN, &g_CoverBitmap [i], g_pBitmapName [i]);
    }

    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_AUTOSECONDARYDC;
    CreateInfo.spCaption = "";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = CoverFlowProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = SCREEN_WIDTH;
    CreateInfo.by = SCREEN_HEIGHT;
    CreateInfo.iBkColor = COLOR_black;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

#if 0
    hdc = GetSecondaryDC (hMainWnd);
    SetSecondaryDC (hMainWnd, hdc, ON_UPDSECDC_DONOTHING);
    ReleaseSecondaryDC (hMainWnd, hdc);

    ShowWindow(hMainWnd, SW_SHOWNORMAL);
    UpdateAll (hMainWnd, TRUE);
    AlphaAnimate(hMainWnd, 30, TRUE);
#endif

    ShowWindow(hMainWnd, SW_SHOWNORMAL);
    while (GetMessage(&Msg, hMainWnd))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    for (i = 0; i < g_nTotalCount; i ++)
    {
        UnloadBitmap (&g_CoverBitmap [i]);
    }
    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif
