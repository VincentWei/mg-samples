#define CAPTION     "mywindow �ӿ�ʾ��"
#define EXAMPLE1    "MyWins ʾ��"
#define EXAMPLE2    "myMessageBox ʾ��"
#define EXAMPLE3    "myWinMessage ʾ��"
#define EXAMPLE4    "myWinTernary ʾ��"
#define EXAMPLE5    "errorWindow ʾ��"
#define EXAMPLE6    "StatusWin ʾ��"
#define EXAMPLE7    "ѡ��"
#define EXAMPLE8    "ProcessWin ʾ����"
#define EXAMPLE9    "����"
#define EXAMPLE10   "�˵�"
#define EXAMPLE11   "���"
#define EXAMPLE12   "�˳�"

char * Flavor [] = {
        "��ɫ",
        "��ɫ",
        "ѡ����ϲ������ɫ��",
        NULL
    };

char *Ternary [] = {
        "��ϲ��",
        "��ϲ��",
        "�ǳ�ϲ��",
        NULL
};

char* prompts [] = {
    "myMessageBox ��һ���򵥵���Ϣ�� , ����ʹ��printf���ĸ�ʽ��",
    "myWinMessage ��һ����ʾ����,����һ����ť��",
    "��ϲ�������Ϸ��?",
    "���Ǹ������õ���ʾ���ڣ�",
    "ϵͳ���ڳ�ʼ��,���Ե�...",
    0
};

char * Pmenu [] = {
        "��Ŀ1",
        "��Ŀ2",
        "��Ŀ 3",
        "��Ŀ 4",
        NULL
};

myWINBUTTON MenuItems[] = {
    {
        "�ַ���1",
        IDC_WB1,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "�ַ���2",
        IDC_WB2,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "�ַ���3",
        IDC_WB3,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
    NULL
    }
};

myWINENTRY EntryItems [] = {
    {
        "�༭��1",
        &Pmenu[0] ,
        80,
        ES_LEFT ,
    },
    {
        "�༭��2",
        &Pmenu[1] ,
        80,
        ES_LEFT ,
    },
    {
        "�༭��3",
        &Pmenu[2] ,
        80,
        ES_LEFT ,
    },
    {
    NULL
    }
};

#define HELP_TITLE     "������Ϣ"
#define HELP_MSG     \
        "We use the SpinBox control in this Help Message Box.\n\n" \
        "You can click the up arrow of the control to scroll up " \
        "the message, and click the down arrow of the control to scroll down. " \
        "You can also scroll the message by typing ArrowDown and ArrowUp keys.\n\n" \
        "In your application, you can call 'myWinHelpMessage' function " \
        "to build a Help Message box like this.\n\n" \
        "The Help Message Box is useful for some PDA-like applications.\n\n" \
        "The SpinBox control allways have the fixed width and height. " \
        "You can read the source of 'ext/control/spinbox.c' to know how to " \
        "build such a control.\n\n" \
        "If you want to know how to use this control, please read the " \
        "source of 'ext/mywins/helpwin.c' in the MiniGUI source tree."

