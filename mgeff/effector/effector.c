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
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <stdio.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include    <mgeff/mgeff.h>

#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__)

static BOOL load_res (HDC hdc);
static HDC createDCByPicture(HDC hdc, const char* path, int w, int h);
static void get_pic_idx (int* cur_pic, int* next_pic);
static int get_duration (void);
static int get_stay_time (void);
static int mGEffFillAnimation(HWND hWnd,
                              HDC src1_dc, HDC src2_dc, HDC dst_dc,
                              const char* effector_name, int duration);
static void delete_cur_anim (void);

#define PICTURE_NUM		2

/* status */
#define STA_STAY  0
#define STA_PLAY  1

/* size */
#define WINDOW_W	380
#define WINDOW_H	380
#define CANVAS_W	320
#define CANVAS_H	240

/* TIMER */
#define STAY_TIMER_ID	1

/* ID of controls */
#define IDC_DURATION   100
#define IDC_STAYTIME 110
#define IDL_EFFECTOR  200
#define IDC_CANVAS  300


static int g_status = STA_STAY;
static HDC g_hdc_pic[2];
static HDC g_canvas_hdc = HDC_INVALID;
static HWND g_main_hwnd = HWND_INVALID;
static MGEFF_ANIMATION g_anim_handle = NULL;


static CTRLDATA s_ctrl_data[] =
{ 
    {
        "static",
        WS_CHILD | SS_LEFT | WS_VISIBLE,
        10, 20, 40, 20,
        IDC_STATIC,
        "Effector: ",
        0
    },
    {
        CTRL_COMBOBOX,
        WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOTIFY,
        55, 15, 120, 25,
        IDL_EFFECTOR,
        "",
        120
    },

    {
        "static",
        WS_CHILD | SS_RIGHT | WS_VISIBLE,
        190, 20, 110, 20,
        IDC_STATIC,
        "Effector Duration (ms): ",
        0
    },
    {
        CTRL_COMBOBOX,
        WS_CHILD | WS_VISIBLE | 
        CBS_READONLY | CBS_AUTOSPIN | CBS_AUTOLOOP,
        300, 18, 50, 20,
        IDC_DURATION, 
        "",
        0
    },
    
    {
        "static",
        WS_CHILD | SS_LEFT | WS_VISIBLE,
        10, 60, 80, 20,
        IDC_STATIC,
        "Stay Time (ms): ",
        0
    },
    {
        CTRL_COMBOBOX,
        WS_CHILD | WS_VISIBLE | 
        CBS_READONLY | CBS_AUTOSPIN | CBS_AUTOLOOP,
        85, 57, 50, 20,
        IDC_STAYTIME,
        "",
        0
    },

    {
        "static",
        WS_CHILD | SS_LEFT | WS_VISIBLE,
        10, 90, CANVAS_W, CANVAS_H,
        IDC_CANVAS,
        "",
        0
    },

};

static DLGTEMPLATE s_dlg_data =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, WINDOW_W, WINDOW_H,
    "MGEff Demo (effector)",
    0, 0,
    TABLESIZE(s_ctrl_data),
	NULL,
    0
};


static const char* g_effector_names[] =
{
    MGEFF_MINOR_leafwindow         ,
    MGEFF_MINOR_zip                ,
    MGEFF_MINOR_flip               ,
    MGEFF_MINOR_cubicrotate        ,
    MGEFF_MINOR_alpha              ,
    MGEFF_MINOR_scroll             ,
    MGEFF_MINOR_zoom               ,
    MGEFF_MINOR_push               ,
    MGEFF_MINOR_clear              ,
    MGEFF_MINOR_centersplit        ,
    MGEFF_MINOR_radarscan          ,
    MGEFF_MINOR_fivepointedstar    ,
    /* MGEFF_MINOR_roller             , */
    /* MGEFF_MINOR_blockflip          , */
    /* MGEFF_MINOR_es_cubicrotate     , */
    /* MGEFF_MINOR_es_rectrotate      , */
    /* MGEFF_MINOR_es_coverflow       , */
    /* MGEFF_MINOR_mgplus_rotate      , */
    MGEFF_MINOR_mgplus_cubicrotate ,
    MGEFF_MINOR_mgplus_flip        ,

    NULL,
};

static const char* s_pic_paths[] =
{
    "res/1.png",
    "res/2.png",
};


static void do_selected_effector (void)
{
    int index = SendMessage (GetDlgItem (g_main_hwnd, IDL_EFFECTOR), CB_GETCURSEL, 0, 0);
    int duration = get_duration ();
    int cur_pic_idx, next_pic_idx;

    delete_cur_anim ();
    
    g_canvas_hdc = GetClientDC (GetDlgItem (g_main_hwnd, IDC_CANVAS));

    /* printf ("play effector: %s %x\n", g_effector_names[index], g_canvas_hdc); */

    get_pic_idx (&cur_pic_idx, &next_pic_idx);
    mGEffFillAnimation (GetDlgItem (g_main_hwnd, IDC_CANVAS),
                        g_hdc_pic[cur_pic_idx], g_hdc_pic[next_pic_idx], g_canvas_hdc,
                        g_effector_names[index], duration);

    g_status = STA_PLAY;
}

static BOOL stay_timer_cb (HWND hwnd, LINT id, DWORD count)
{
    do_selected_effector ();
    return FALSE;
}


static void anim_finished_cb(MGEFF_ANIMATION handle)
{
    /* do_selected_effector (); */
    
    g_status = STA_STAY;
    SetTimerEx (GetDlgItem (g_main_hwnd, IDC_CANVAS), STAY_TIMER_ID, get_stay_time (), stay_timer_cb);
}


static LRESULT canvas_win_proc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_PAINT:
        if (g_status == STA_STAY) {
            HDC hdc = BeginPaint(hWnd);
            int cur_pic;

            get_pic_idx (&cur_pic, NULL);
            BitBlt (g_hdc_pic[cur_pic], 0, 0, 0, 0, hdc, 0, 0, 0);
            EndPaint(hWnd, hdc);
            return 0;
        }
        break;
    default:
        break;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}



static LRESULT WindProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i;
    switch (message) {
    case MSG_INITDIALOG:

        g_main_hwnd = hDlg;
        
        SendDlgItemMessage(hDlg, IDC_DURATION, CB_SETSPINFORMAT, 0, (LPARAM)"%4d");
        SendDlgItemMessage(hDlg, IDC_DURATION, CB_SETSPINRANGE, 1000, 9500);
        SendDlgItemMessage(hDlg, IDC_DURATION, CB_SETSPINVALUE, 5000, 0);
        SendDlgItemMessage(hDlg, IDC_DURATION, CB_SETSPINPACE, 500, 1000);

        SendDlgItemMessage(hDlg, IDC_STAYTIME, CB_SETSPINFORMAT, 0, (LPARAM)"%4d");
        SendDlgItemMessage(hDlg, IDC_STAYTIME, CB_SETSPINRANGE, 0, 6000);
        SendDlgItemMessage(hDlg, IDC_STAYTIME, CB_SETSPINVALUE, 1000, 0);
        SendDlgItemMessage(hDlg, IDC_STAYTIME, CB_SETSPINPACE, 500, 1000);

        i = 0;
        while (g_effector_names[i] != NULL) {
            SendDlgItemMessage(hDlg, IDL_EFFECTOR, CB_ADDSTRING, 0, (LPARAM)g_effector_names [i++]);
        }

        {
            HDC hdc;
            hdc = GetDC (hDlg);
            if (! load_res (hdc)) {
                ReleaseDC (hdc);
                return -1;
            }
            ReleaseDC (hdc);
            mGEffInit ();
        }

        SendDlgItemMessage(hDlg, IDL_EFFECTOR, CB_SETCURSEL, 0, 0);

        SetWindowCallbackProc (GetDlgItem (hDlg, IDC_CANVAS), canvas_win_proc);

        SetTimerEx (GetDlgItem (g_main_hwnd, IDC_CANVAS), STAY_TIMER_ID, get_stay_time (), stay_timer_cb);

        return TRUE;
        
    case MSG_COMMAND:
        break;
    case MSG_CLOSE:
        EndDialog (hDlg, IDCANCEL);
        break;
        
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "MGEffect Demo" , 0 , 0);
#endif
    
    s_dlg_data.controls = s_ctrl_data;
    
    DialogBoxIndirectParam (&s_dlg_data, HWND_DESKTOP, WindProc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

static BOOL load_res (HDC hdc)
{
    int i;
    for (i = 0; i < PICTURE_NUM; ++i) {
        g_hdc_pic[i] = createDCByPicture (hdc, s_pic_paths[i], 0, 0);
        if (g_hdc_pic[i] == HDC_INVALID) {
            return FALSE;
        }
    }
    return TRUE;
}

static HDC createDCByPicture(HDC hdc, const char* path, int w, int h)
{
    BITMAP bmp;
    HDC dc;

    if (LoadBitmap(hdc, &bmp, path)) {
        return HDC_INVALID;
    }
    
    if (w == 0)
        w = bmp.bmWidth;

    if (h == 0)
        h = bmp.bmHeight;

    /* printf ("width: %d height: %d\n", w, h); */
    
    dc = CreateCompatibleDCEx(hdc, w, h);
    FillBoxWithBitmap(dc, 0, 0, w, h, &bmp);
    UnloadBitmap(&bmp);
    
    return dc;
}


static void get_pic_idx (int* cur_pic, int* next_pic)
{
    static int cur_pic_idx = 0;

    if (cur_pic) {
        *cur_pic = cur_pic_idx % PICTURE_NUM;
    }

    if (next_pic) {
        *next_pic = (cur_pic_idx + 1) % PICTURE_NUM;
        ++ cur_pic_idx;
    }
}

static void delete_cur_anim (void)
{
    if (g_anim_handle) {
        /* mGEffAnimationStop (g_anim_handle); */
        mGEffAnimationDelete (g_anim_handle);
        g_anim_handle = NULL;
        ReleaseDC (g_canvas_hdc);
    }
}

static int get_stay_time (void)
{
    /* printf ("stay time: %d\n", SendMessage (GetDlgItem (g_main_hwnd, IDC_STAYTIME), CB_GETSPINVALUE, 0, 0)); */
    return SendMessage (GetDlgItem (g_main_hwnd, IDC_STAYTIME), CB_GETSPINVALUE, 0, 0) / 10;
}

static int get_duration (void)
{
    return SendMessage (GetDlgItem (g_main_hwnd, IDC_DURATION), CB_GETSPINVALUE, 0, 0) / 10;
}


static int mGEffFillAnimation(HWND hWnd,
                              HDC src1_dc, HDC src2_dc, HDC dst_dc,
                              const char* effector_name, int duration)
{
    unsigned long key = mGEffStr2Key(effector_name);
    MGEFF_EFFECTOR effector = mGEffEffectorCreate (key);
    MGEFF_SOURCE source1 = mGEffCreateSource (src1_dc);
    MGEFF_SOURCE source2 = mGEffCreateSource (src2_dc);
    MGEFF_SINK sink = mGEffCreateHDCSink (dst_dc);
    MGEFF_ANIMATION handle;

    mGEffEffectorAppendSource (effector, source1);
    mGEffEffectorAppendSource (effector, source2);
    mGEffSetBufferSink(sink, src1_dc);
    mGEffEffectorSetSink (effector, sink);

    if (key == MGEFF_EFFECTOR_PUSH) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_BOTTOM2TOP);
    }
    else if (key == MGEFF_EFFECTOR_ZOOM) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LDOWN2RUP);
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
    }
    else if (key == MGEFF_EFFECTOR_ZIP) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
    }
    else if(key == MGEFF_EFFECTOR_CENTERSPLIT) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_CENTER2OUTSIDE_4);
    }
    else if (key == MGEFF_EFFECTOR_FIVEPOINTEDSTAR) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
    }
    else if (key == MGEFF_EFFECTOR_RADARSCAN) {
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_PIECES, 100);
        mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_STARTANGLE, 30);
    }

    handle = mGEffAnimationCreateWithEffector (effector);
    g_anim_handle = handle;

    mGEffAnimationSetDuration (handle, duration);
    mGEffAnimationSetFinishedCb (handle, anim_finished_cb);
    /* mGEffAnimationSetProperty (handle, MGEFF_PROP_KEEPALIVE, 0); */
    
#if 1
    mGEffAnimationAsyncRun (handle);
    /* mGEffAnimationWait(&hWnd, handle); */
#else
    mGEffAnimationSyncRun (handle);
#endif

    /* printf ("After wait\n"); */

    /* mGEffAnimationDelete (handle); */
    /* g_anim_handle = NULL; */

    /* mGEffEffectorDelete(effector); */

    return 0;
}
