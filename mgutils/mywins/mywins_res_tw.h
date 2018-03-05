#define CAPTION     "mywindow 接口示例"
#define EXAMPLE1    "MyWins 示例"
#define EXAMPLE2    "myMessageBox 示例"
#define EXAMPLE3    "myWinMessage 示例"
#define EXAMPLE4    "myWinTernary 示例"
#define EXAMPLE5    "errorWindow 示例"
#define EXAMPLE6    "StatusWin 示例"
#define EXAMPLE7    "選擇"
#define EXAMPLE8    "ProcessWin 示例！"
#define EXAMPLE9    "幫助"
#define EXAMPLE10   "菜單"
#define EXAMPLE11   "入口"
#define EXAMPLE12   "退出"

char * Flavor [] = {
        "紅色",
        "藍色",
        "選擇你喜歡的顏色！",
        NULL
    };

char *Ternary [] = {
        "很喜歡",
        "不喜歡",
        "非常喜歡",
        NULL
};

char* prompts [] = {
    "myMessageBox 是一個簡單的消息框 , 可以使用printf風格的格式。",
    "myWinMessage 是一個提示窗口，包含一個按鈕。",
    "你喜歡這個游戲嗎？",
    "這是個很有用的提示窗口！",
    "系統正在初始化，請稍等...",
    0
};

char * Pmenu [] = {
        "項目 1",
        "項目 2",
        "項目 3",
        "項目 4",
        NULL
};

myWINBUTTON MenuItems[] = {
    {
        "字符串1",
        IDC_WB1,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "字符串2",
        IDC_WB2,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
        "字符串3",
        IDC_WB3,
        WS_VISIBLE | BS_PUSHBUTTON  
    },
    {
    NULL
    }
};

myWINENTRY EntryItems [] = {
    {
        "編輯框1",
        &Pmenu[0] ,
        80,
        ES_LEFT ,
    },
    {
        "編輯框2",
        &Pmenu[1] ,
        80,
        ES_LEFT ,
    },
    {
        "編輯框3",
        &Pmenu[2] ,
        80,
        ES_LEFT ,
    },
    {
    NULL
    }
};

#define HELP_TITLE     "幫助信息"
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

