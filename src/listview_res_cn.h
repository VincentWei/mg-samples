#define everage_score                          "ƽ����" 
#define getting_the_average_score              "����ƽ����"

typedef struct _SCORE
{
    char *name;
    int scr[SUB_NUM];
} SCORE;


static char * caption [] =
{
    "����", "����", "��ѧ", "Ӣ��"
};

static char *classes [] =
{
    "1��", "2��", "3��"
};
static SCORE scores[] =
{
    {"С��", {81, 96, 75}},
    {"Сǿ", {98, 62, 84}},
    {"С��", {79, 88, 89}},
    {"С��", {79, 88, 89}},
};
