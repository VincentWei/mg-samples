#define CAPTION     "mywindow ���f�ܨ�"
#define EXAMPLE1    "MyWins �ܨ�"
#define EXAMPLE2    "myMessageBox �ܨ�"
#define EXAMPLE3    "myWinMessage �ܨ�"
#define EXAMPLE4    "myWinTernary �ܨ�"
#define EXAMPLE5    "errorWindow �ܨ�"
#define EXAMPLE6    "StatusWin �ܨ�"
#define EXAMPLE7    "���"
#define EXAMPLE8    "ProcessWin �ܨҡI"
#define EXAMPLE9    "���U"
#define EXAMPLE10   "���"
#define EXAMPLE11   "�J�f"
#define EXAMPLE12   "�h�X"

char * Flavor [] = {
        "����",
        "�Ŧ�",
        "��ܧA���w���C��I",
        NULL
    };

char *Ternary [] = {
        "�ܳ��w",
        "�����w",
        "�D�`���w",
        NULL
};

char* prompts [] = {
    "myMessageBox �O�@��²�檺������ , �i�H�ϥ�printf���檺�榡�C",
    "myWinMessage �O�@�Ӵ��ܵ��f�A�]�t�@�ӫ��s�C",
    "�A���w�o�Ӵ����ܡH",
    "�o�O�ӫܦ��Ϊ����ܵ��f�I",
    "�t�Υ��b��l�ơA�еy��...",
    0
};

char * Pmenu [] = {
        "���� 1",
        "���� 2",
        "���� 3",
        "���� 4",
        NULL
};

myWINBUTTON MenuItems[] = {
    {
        "�r�Ŧ�1",
        IDC_WB1,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "�r�Ŧ�2",
        IDC_WB2,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "�r�Ŧ�3",
        IDC_WB3,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
    NULL
    }
};

myWINENTRY EntryItems [] = {
    {
        "�s���1",
        &Pmenu[0] ,
        80,
        ES_LEFT ,
    },
    {
        "�s���2",
        &Pmenu[1] ,
        80,
        ES_LEFT ,
    },
    {
        "�s���3",
        &Pmenu[2] ,
        80,
        ES_LEFT ,
    },
    {
    NULL
    }
};

#define HELP_TITLE     "���U�H��"
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

