#include <pthread.h>
#include <sys/time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgplus/mgplus.h>

#define C2_KEY_LEFT     SCANCODE_CURSORBLOCKLEFT     /* 方向左键 */
#define C2_KEY_RIGHT    SCANCODE_CURSORBLOCKRIGHT    /* 方向右键 */
#define C2_KEY_UP       SCANCODE_CURSORBLOCKUP       /* 方向上键 */
#define C2_KEY_DOWN     SCANCODE_CURSORBLOCKDOWN     /* 方向下键 */

#define C2_KEY_OK       SCANCODE_ENTER          /* 确定键 */
#define C2_KEY_EXIT     SCANCODE_ESCAPE         /* 退出键 */
#define C2_KEY_SOURCE   SCANCODE_F1             /* 信源键 */
#define C2_KEY_USB      SCANCODE_F2             /* USB键 */

#define C2_KEY_PLAY     SCANCODE_P              /* 播放键 */
#define C2_KEY_PAUSE    SCANCODE_L              /* 暂停键 */
#define C2_KEY_STOP     SCANCODE_S              /* 停止键 */
#define C2_KEY_FORWORD  SCANCODE_F              /* 快进键 */
#define C2_KEY_REWIND   SCANCODE_R              /* 快退键 */
#define C2_KEY_NEXT     SCANCODE_N              /* 下一个键 */
#define C2_KEY_PREV     SCANCODE_R              /* 上一个键 */

#define C2_KEY_1        SCANCODE_1              /* 数字键 */
#define C2_KEY_2        SCANCODE_2
#define C2_KEY_3        SCANCODE_3
#define C2_KEY_4        SCANCODE_4
#define C2_KEY_5        SCANCODE_5
#define C2_KEY_6        SCANCODE_6
#define C2_KEY_7        SCANCODE_7
#define C2_KEY_8        SCANCODE_8
#define C2_KEY_9        SCANCODE_9
#define C2_KEY_0        SCANCODE_0

#define MAIN_WND_WIDTH  592
#define MAIN_WND_HEIGHT 523

#define MENU_START_X    25
#define MENU_START_Y    116
#define MENU_WIDTH      270 
#define MENU_HEIGHT     50 

#define DISK_START_X    297
#define DISK_START_Y    116
#define DISK_WIDTH      270 
#define DISK_HEIGHT     50 

#define PI 3.14159265358979323846

typedef struct _3_d_coordinate {
    int x;
    int y;
    int z;
}THREE_D_COORDINATE;

static char *res_file[] =
{
    "res/pic1.jpg",
    //"res/gundam.bmp"
    "res/pic2.jpg"
};
static BITMAP bmp_res[2];

//static int g_frame_num=5;
//static HDC memdc;

void GUIAPI UpdateAll (HWND hWnd, BOOL fErase)
{
    MSG Msg;
    UpdateWindow(hWnd, fErase);
    while (PeekMessageEx (&Msg, hWnd, MSG_PAINT, MSG_PAINT+1,
                            FALSE, PM_REMOVE))
    {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }
}

typedef void (* ANIMATE_DRAW)(int index, void *context);

typedef struct _rotate_info
{
    int     frame_num;  /* */
    int     w;          /* w of hdc */
    int     h;          /* h of hdc */
    int     inc_x;      /* increment of y */
    int     inc_y;      /* increment of y */
    float   inc_a;      /* increment of rotate angle*/
    HDC     hdc;        /* the destination DC */
    HGRAPHICS hgp;  /*  */
    PBITMAP bmp_bk; /* the picture of background */
    PBITMAP bmp_fg; /* the picture of foreground */
}ROTATEINFO;

void InitRotateInfo(HWND hwnd, ROTATEINFO* info, int frame_num, 
        PBITMAP bmp_bk, PBITMAP bmp_fg)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    info->frame_num = frame_num;
    info->w = RECTW(rc);
    info->h = RECTH(rc);
    info->inc_x = (RECTW(rc)+frame_num)/(2*frame_num);
    info->inc_y = (RECTH(rc)+frame_num)/(2*frame_num);
    info->inc_a = (2*360*PI)/(180.0*frame_num);

    info->bmp_bk = bmp_bk;
    info->bmp_fg = bmp_fg;

    info->hdc = GetClientDC(hwnd);
    info->hgp = MGPlusGraphicCreateFromDC(info->hdc);
    MGPlusSetInterpolationMode(info->hgp, MP_INTERPOLATION_MODE_BILINEAR);

    MGPlusGraphicLoadBitmap(info->hgp, 0, info->bmp_bk);
    MGPlusGraphicLoadBitmap(info->hgp, 1, info->bmp_fg);
}

void DestroyRotateInfo(ROTATEINFO* info)
{
    MGPlusGraphicDelete(info->hgp);
    ReleaseDC(info->hdc);
}

#define Z_PRP 

void draw_rotatepic(int index, void *context)
{
    ROTATEINFO *info = (ROTATEINFO *)context;
    MPPOINT pt[4];
    THREE_D_COORDINATE three_d;
    //float z_angle = PI/4;

    /* Z轴上的消失点 */
    int zprp = info->w*2;
#if 1
    int r = (info->w - 2*index*info->inc_x)/2;
    int x0 = index*info->inc_x + r;
    int y0 = index*info->inc_y;
#else
    int r = info->w/2;
    int x0 = r;
    int y0 = 0;
#endif
    /*draw background picture*/
    pt[0].x = pt[0].y = 0;
    pt[1].x = info->w;
    pt[1].y = 0;
    pt[2].x = info->w;
    pt[2].y = info->h;
    pt[3].x = 0;
    pt[3].y = info->h;

    MGPlusDrawImageWithPoints(info->hgp, 0, pt, 4);

    if (index < info->frame_num-1)
    {
        /*the right-bottom Vertex*/
        three_d.x = x0 + cos(-info->inc_a*index)*r;
        three_d.y = info->h - y0;
        three_d.z = sin(-info->inc_a*index)*r;

        pt[2].x = three_d.x*zprp/(zprp - three_d.z);
        pt[2].y = three_d.y*zprp/(zprp - three_d.z);

        /*the left-bottom Vertex*/
        three_d.x = x0 + cos(PI-info->inc_a*index)*r;
        three_d.y = info->h - y0;
        three_d.z = sin(PI-info->inc_a*index)*r;

        pt[3].x = three_d.x*zprp/(zprp - three_d.z);
        pt[3].y = three_d.y*zprp/(zprp - three_d.z);

        /*draw foreground picture*/
        /*the left-top Vertex*/
        pt[0].x = pt[3].x;
        pt[0].y = info->h - pt[3].y;
        if (pt[0].y == pt[3].y) {
            /*when index==28*/
            pt[0].y -= 1;
            pt[3].y += 1;
        }

        /*the right-top Vertex*/
        pt[1].x = pt[2].x;
        pt[1].y = info->h - pt[2].y;
        if (pt[1].y == pt[2].y) {
            pt[1].y -= 1;
            pt[2].y += 1;
        }
#if 0
        /*the right-bottom Vertex*/
        pt[2].x = pt[1].x;
        pt[2].y = pt[1].y + (info->h - 2*index*info->inc_y);

        /*the left-bottom Vertex*/
        pt[3].x = pt[0].x;
        pt[3].y = pt[0].y + (info->h - 2*index*info->inc_y);
#endif
        MGPlusDrawImageWithPoints(info->hgp, 1, pt, 4);
    }

    MGPlusGraphicSave(info->hgp, info->hdc, 0, 0, 0, 0, 0, 0);
}

void RunAnimate(int interval, int frame_num, ANIMATE_DRAW cbdraw, void *context)
{
	pthread_cond_t cond;
	pthread_mutex_t mutex;

    int index=0;
	int interval_sec;
	int interval_nsec;
	struct timeval tv;
    //int start_time;

    if (!interval || !frame_num || !cbdraw || !context)
        return;

	gettimeofday(&tv,NULL);
	//start_time =tv.tv_sec*1000 + (tv.tv_usec+999)/1000;

	pthread_cond_init(&cond,NULL);
	pthread_mutex_init(&mutex, NULL);

	interval_sec = interval/1000;
	interval_nsec = (interval%1000)*1000000;

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
        cbdraw(index, context);
        index++;
        if (index > frame_num-1)
            break;

		pthread_cond_timedwait(&cond, &mutex, &timeout);
		pthread_mutex_unlock(&mutex);
    }

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

typedef struct _blinds_info
{
    HDC hdc;
    int frame_num;
    int part_num;
    int part_w;
    int part_h;
    div_t frame_h;
    div_t final;
    PBITMAP bmp_bk;
    PBITMAP bmp_fg;
}BLINDSINFO;

void InitBlindsInfo(HWND hwnd, BLINDSINFO *info, int frame_num,
        int part_num, PBITMAP bmp_bk, PBITMAP bmp_fg)
{
    RECT rc;
    div_t part;

    info->hdc = GetClientDC(hwnd);
    GetClientRect(hwnd, &rc);

    info->frame_num = frame_num;
    info->part_num = part_num;

    /*count the max height of blind*/
    part = div(RECTH(rc)+part_num, part_num);
    info->part_h = part.quot;
    info->part_w = RECTW(rc);

    /*count the height of blind which 1~part_num-1*/
    info->frame_h = div(info->part_h, frame_num);

    /*count the last height of blind*/
    info->final = div(info->part_h - part_num + part.rem, frame_num);
    info->bmp_bk = bmp_bk;
    info->bmp_fg = bmp_fg;
}

void DestroyBlindsInfo(BLINDSINFO *info)
{
    ReleaseDC(info->hdc);
}

void draw_blinds(int index, void * context)
{
    int i, h;
    BLINDSINFO *info = (BLINDSINFO *)context;

    for(i=0;i<info->part_num-1;i++)
    {
        if (index < info->frame_h.rem)
            h = i*info->part_h+index*(info->frame_h.quot+1);
        else
            h = i*info->part_h+index*(info->frame_h.quot)+info->frame_h.rem;

        FillBoxWithBitmapPart(info->hdc,
            0, h,
            info->part_w, 
            (index < info->frame_h.rem)?info->frame_h.quot+1:info->frame_h.quot,
            0, 0,
            info->bmp_fg, 0, h);
    }

    /*draw the final part*/
    if (index < info->final.rem)
        h = i*info->part_h+index*(info->final.quot+1);
    else
        h = i*info->part_h+index*(info->final.quot)+info->final.rem;

    FillBoxWithBitmapPart(info->hdc,
            0, h,
            info->part_w, 
            (index < info->final.rem)?info->final.quot+1:info->final.quot,
            //part_w, h,
            0, 0,
            info->bmp_fg, 0, h);

    //BitBlt(info->hdc, 0, 0, 0, 0, HDC_SCREEN, 0, 0, 0);
}


/*
 * param part_num:  the number of blind
 * param frame_num: the number of frame
 * */
#if 0
void BlindsAnimate(HWND hwnd, int interval, int part_num, int frame_num, PBITMAP bmp)
{
	pthread_cond_t cond;
	pthread_mutex_t mutex;

    int index=0;
	int interval_sec;
	int interval_nsec;
	struct timeval tv;
    int start_time;

    div_t part, final;
    div_t frame_h;
    int part_h;

    RECT rc;
    HDC hdc;

	gettimeofday(&tv, NULL);
	start_time =tv.tv_sec*1000 + (tv.tv_usec+999)/1000;

	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);

	interval_sec = interval/1000;
	interval_nsec = (interval%1000)*1000000;
    
    hdc = GetSecondaryDC(hwnd);
    GetClientRect(hwnd, &rc);
    /*count the max height of blind*/
    part = div(RECTH(rc)+part_num, part_num);
    part_h = part.quot;

    /*count the height of blind which 1~part_num-1*/
    frame_h = div(part_h, frame_num);
    /*count the last height of blind*/
    final = div(part_h - part_num + part.rem, frame_num);

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
        draw_blinds(index, part_num, RECTW(rc), part_h, 
                frame_h, final, bmp); 

        index++;
        if (index > frame_num-1)
            break;

		pthread_cond_timedwait(&cond, &mutex, &timeout);
		pthread_mutex_unlock(&mutex);
    }
    //ReleaseDC(hdc);

    //hdc = GetSecondaryDC(hwnd);
    SetSecondaryDC(hwnd, hdc, ON_UPDSECDC_DEFAULT);
    ReleaseSecondaryDC(hwnd, hdc);

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}
#endif

static LRESULT BlindDemoWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
        {
            int i;
            for(i=0;i<sizeof(res_file)/sizeof(char *);i++)
                LoadBitmap (HDC_SCREEN, &bmp_res[i], res_file[i]);

            break;
        }
        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            switch(scancode)
            {
                case SCANCODE_F1:
                {
                    BLINDSINFO info ={0};

                    InitBlindsInfo(hWnd, &info, 30, 8, &bmp_res[0], &bmp_res[1]);
                    //BlindsAnimate(hWnd, 50, 4, 30, &bmp_res[0]);
                    //BlindsAnimate(hWnd, 50, 8, 30, &bmp_res[0]);
                    RunAnimate(50, 30, draw_blinds, (void *)&info);
                    DestroyBlindsInfo(&info);
                    break;
                }
                case SCANCODE_F2:
                {
                    ROTATEINFO info={0};
                    InitRotateInfo(hWnd, &info, 30, &bmp_res[0], &bmp_res[1]); 
                    RunAnimate(50, 30, draw_rotatepic, (void *)&info);
                    DestroyRotateInfo(&info);
                    break;
                }
                default:
                    break;
            }
            break;
        }
#if 1
        case MSG_PAINT:
        {
            HDC hdc = BeginPaint(hWnd);
            FillBoxWithBitmap(hdc, g_rcScr.left,g_rcScr.top, 
                    RECTW(g_rcScr), RECTH(g_rcScr), &bmp_res[0]);
            EndPaint(hWnd, hdc);
            break;
        }
#endif
        case MSG_CLOSE:
        {
            int i;
            for(i=0;i<sizeof(res_file)/sizeof(char *);i++)
                UnloadBitmap (&bmp_res[i]);
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            return 0;
        }
        default:
            break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    //HDC     hdc;
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "blind", 0, 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE|WS_BORDER|WS_CAPTION;
    CreateInfo.dwExStyle = WS_NONE;//WS_EX_AUTOPRIVATEDC;
    CreateInfo.spCaption = "Please push F1,F2";
    //CreateInfo.hMenu = hMenu;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = BlindDemoWinProc;
    CreateInfo.lx = g_rcScr.left;
    CreateInfo.ty = g_rcScr.top;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom ;
    CreateInfo.iBkColor = COLOR_black;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;
#if 0
    hdc = GetSecondaryDC(hMainWnd);
    SetSecondaryDC(hMainWnd, hdc, ON_UPDSECDC_DONOTHING);
    ReleaseSecondaryDC(hMainWnd, hdc);

    ShowWindow(hMainWnd, SW_SHOWNORMAL);
    UpdateAll (hMainWnd, TRUE);
    AlphaAnimate(hMainWnd, 30, TRUE);
#endif
    while (GetMessage(&Msg, hMainWnd))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif
