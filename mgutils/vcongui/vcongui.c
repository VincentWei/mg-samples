/*
** $Id: vcongui.c 1 2008-01-30 07:41:08Z wangjian $
**
** Virtual Console on MiniGUI.
**
** Copyright (C) 1999 ~ 2002 Wei Yongming
** Copyright (C) 2003 ~ 2007 Feynman Software.
** 
** Create date: 1999.09.23
*/ 

/*
**  This source is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public
**  License as published by the Free Software Foundation; either
**  version 2 of the License, or (at your option) any later version.
**
**  This software is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public
**  License along with this library; if not, write to the Free
**  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
**  MA 02111-1307, USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef __MGUTILS_LIB__
#include "../../include/mgutils.h"
#else
#include <mgutils/mgutils.h>
#endif

#ifdef  _MGUTILS_VCONGUI

int MiniGUIMain (int args, const char* arg[])
{
    CHILDINFO ChildInfo = {1, NULL, NULL, NULL, NULL, 1, 0, 0, 0, 0};

#ifdef _MGRM_PROCESSES
    if (JoinLayer ("vcongui", "vcongui", 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif
   
    ChildInfo.rows = 25;
    ChildInfo.cols = 80;
    VCOnMiniGUI (&ChildInfo);
    return 0;
}

#else

int MiniGUIMain (int argc, const char* argv[])
{
    fprintf (stderr, "Please enable the support for VCOnGUI module in mGUtils.\n");
    return 0;
}

#endif /* _MGUTILS_VCONGUI */

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
