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
#define everage_score "������"
#define getting_the_average_score "�p�⥭����"
typedef struct _SCORE
{
    char *name;
    int scr[SUB_NUM];
} SCORE;


static char * caption [] =
{
     "�m�W", "�y��", "�ƾ�", "�^�y"
};

static char *classes [] =
{
     "1�Z", "2�Z", "3�Z"
};
static SCORE scores[] =
{
     {"�p��", {81, 96, 75}},
     {"�p�j", {98, 62, 84}},
     {"�p�G", {79, 88, 89}},
     {"�p�O", {79, 88, 89}},
};
	
		
