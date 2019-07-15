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
** $Id$
**
** The demo of font.
**
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>


#define MIN_FONT_SIZE 0
#define MAX_FONT_SIZE 72


#define ID_FAMILY   100
#define ID_CHARSET  101
#define ID_WEIGHT   102
#define ID_SLANT    103
#define ID_FLIP     104
#define ID_SIZE     105
#define ID_DISPLAY  106

#define ID_SHOW_FAMILY   107
#define ID_SHOW_CHARSET  108
#define ID_SHOW_WEIGHT   109
#define ID_SHOW_SLANT    110
#define ID_SHOW_FLIP     111
#define ID_SHOW_SIZE     112
#define ID_SHOW_DISPLAY  113

#define ID_Type     200
#define ID_Other    201
#define ID_UnderLine 202
#define ID_StruckOut 203

#define ID_SHOW_TYPE  205
#define ID_SHOW_OTHER 206
#define ID_SHOW_UNDERLINE 207
#define ID_SHOW_STRUCKOUT 208

typedef struct _Font
{
    char type[16];
    char family[16];
    char charset[16];
    int size;
    struct _Font *next;
}Font;

static Font gFont_info =
{
    "",
    "",
    "",
    0,  
    NULL
};

typedef struct _DevFontInfo
{
    char rf[8][16]; //rbf_family
    char vf[8][16];
    char uf[8][16];
    char qf[8][16];
    char tf[8][16];
    char type[5][8];
    char charset[16][16];
    int fnum[7];//rf_num, vf_num, uf_num; qf_num, tf_num, type_num, charset_num
}DevFontInfo;

static int nr_font_stack = 0;
static DevFontInfo* devfont_info;
static int sel_type = 0;


enum type
{
    rbf,vbf, upf, qpf, ttf
};

char *types[] =
{
    "rbf", "vbf", "upf", "qpf", "ttf"
};

//unicode -> iso8859-1
static UChar32 src1[] = {
    0x004D, 0x0069, 0x006E, 0x0069, 0x0047, 0x0055, 0x0049, 0x0020, 0x0044, 0x0065, 0x006D, 0x006F
};
//unicode -> GB2312
static UChar32 src2[] = {
    0x6B22, 0x8FCE, 0x4F7F, 0x7528, 0x004D, 0x0069, 0x006E, 0x0069, 0x0047, 0x0055, 0x0049
};
//unicode -> BIG5
static UChar32 src3[] = {
    0x6B61, 0x8FCE, 0x4F7F, 0x7528, 0x004D, 0x0069, 0x006E, 0x0069, 0x0047, 0x0055, 0x0049
};
//unicode -> UTF-8
static UChar32 src4[] = {
    0x6B22, 0x8FCE, 0x6B61, 0x8FCE, 0x4F7F, 0x7528, 0x004D, 0x0069, 0x006E, 0x0069, 0x0047, 0x0055, 0x0049
};


static CTRLDATA ctrls [] =
{

    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        10, 10, 80, 10,
        ID_SHOW_TYPE,
        "Type", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        10, 30, 50, 80,
        ID_Type,
        "", 0
    },
    {    CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        80, 10, 80, 10,
        ID_SHOW_FAMILY,
        "family", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        80, 30, 50, 80,
        ID_FAMILY,
        "{", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        150, 10, 80, 10,
        ID_SHOW_CHARSET,
        "charset", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        150, 30, 50, 80,
        ID_CHARSET,
        "", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        220, 10, 80, 10,
        ID_SHOW_WEIGHT,
        "weight", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        220, 30, 50, 80,
        ID_WEIGHT,
        "", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        10, 120, 50, 10,
        ID_SHOW_SLANT,
        "slant", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        10, 140, 40, 50,
        ID_SLANT,
        "", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        70, 120, 50, 10,
        ID_SHOW_FLIP,
        "flip", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        70, 140, 40, 50,
        ID_FLIP,
        "", 0
    },

    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        130, 120, 50, 10,
        ID_SHOW_OTHER,
        "Other", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        130, 140, 75, 50,
        ID_Other,
        "", 0
    },

    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        220, 120, 50, 10,
        ID_SHOW_UNDERLINE,
        "UnderLine", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        220, 140, 80, 50,
        ID_UnderLine,
        "", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        220, 200, 50, 10,
        ID_SHOW_STRUCKOUT,
        "StruckOut", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        220, 220, 80, 50,
        ID_StruckOut,
        "", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        350, 10, 20, 10,
        ID_SHOW_SIZE,
        "size", 0
    },
    {
        CTRL_LISTBOX,
        WS_VISIBLE|WS_CHILD|LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
        350, 30, 20, 220,
        ID_SIZE,
        "", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD,
        10, 200, 80, 10,
        ID_SHOW_DISPLAY,
        "preview", 0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE|WS_CHILD|WS_BORDER,
        10, 220, 200, 50,
        ID_DISPLAY,
        "MiniGUI Demo", 0

    },
    {
        CTRL_BUTTON,
        WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON|BS_CENTER|BS_VCENTER,
        60, 300, 50, 15,
        IDCANCEL,
        "CANCEL", 0
    },
    {
        CTRL_BUTTON,
        WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON|BS_CENTER|BS_VCENTER,
        230, 300, 50, 15,
        IDOK,
        "OK", 0
    },
};

static DLGTEMPLATE Dlg = 
{
    WS_BORDER|WS_CAPTION|WS_VISIBLE,
    WS_EX_NONE,
    20, 20, 400, 350,
    "font selecting dialog",
    0, 0,
    TABLESIZE(ctrls), ctrls,
    0,
};




static char* other_str [] =
{
    "AUTOSCALE",       
    "TTFNOCACHE",     
    "TTFKERN",         
    "TTFNOCACHEKERN" , 
    "LCDPORTRAIT",     
    "LCDPORTRAITKERN",
};

static char other_arg [] =
{
    FONT_OTHER_AUTOSCALE ,       
    FONT_OTHER_TTFNOCACHE ,      
    FONT_OTHER_TTFKERN ,        
    FONT_OTHER_TTFNOCACHEKERN , 
    FONT_OTHER_LCDPORTRAIT  ,   
    FONT_OTHER_LCDPORTRAITKERN , 
};

static char* underline_str [] =
{
    "FONT_UNDERLINE_NONE",       
    "FONT_UNDERLINE_LINE",      

};
static char  underline_arg [] =
{
    FONT_UNDERLINE_NONE,        
    FONT_UNDERLINE_LINE,     

};

static char* struckout_str [] = 
{
    "FONT_STRUCKOUT_NONE",         
    "FONT_STRUCKOUT_LINE",        

};

static char struckout_arg [] =
{
    FONT_STRUCKOUT_NONE,         
    FONT_STRUCKOUT_LINE,        

};

static char* weight_str [] =
{
    "regular",
    "bold",
    "book",
    "demibold",
    "light",
    "subpixel",
};
static char weight_arg [] = 
{
    FONT_WEIGHT_REGULAR,
    FONT_WEIGHT_BOLD,
    FONT_WEIGHT_BOOK,
    FONT_WEIGHT_DEMIBOLD,
    FONT_WEIGHT_LIGHT,
    FONT_WEIGHT_SUBPIXEL,
};

static char* slant_str [] = 
{
    "italic",
    "roman",
};
static char slant_arg [] =
{
    FONT_SLANT_ITALIC,
    FONT_SLANT_ROMAN,
};

static char* flip_str [] =
{
    "NIL",
    "horz",
    "vert",
    "horzvert",
};

static char flip_arg [] = 
{
    FONT_FLIP_NIL,
    FONT_FLIP_HORZ,
    FONT_FLIP_VERT,
    FONT_FLIP_HORZVERT,
};

static void InsertString (HWND hWnd, char** strs, int str_num)
{
    int i;
    for (i=0; i<str_num; i++)
    {
        SendMessage (hWnd, LB_INSERTSTRING, i, (LPARAM)strs[i]);
    }
    SendMessage (hWnd, LB_SETCURSEL, 0, 0);
}

static void ChangeListBox (HWND hWnd)
{
    int i = 0;
    HWND hctrl;
    sel_type = SendMessage (GetDlgItem (hWnd, ID_Type), LB_GETCURSEL, 0, 0);

    hctrl = GetDlgItem (hWnd, ID_FAMILY);

    SendMessage (hctrl, LB_RESETCONTENT, 0, 0);
    if (strcmp (devfont_info->type[sel_type], "rbf") == 0)
    {
        for (i=0; i<devfont_info->fnum[0]; i++)
        {
            SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->rf[i]);
        }
    }
    else if (strcmp (devfont_info->type[sel_type], "vbf") == 0)
    {
        for (i=0; i<devfont_info->fnum[1]; i++)
        {
            SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->vf[i]);
        }
    }
    else if (strcmp (devfont_info->type[sel_type], "upf") == 0)
    {
        for (i=0; i<devfont_info->fnum[2]; i++)
        {
            SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->uf[i]);
        }
    }
    else if (strcmp (devfont_info->type[sel_type], "qpf") == 0)
    {
        for (i=0; i<devfont_info->fnum[3]; i++)
        {
            SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->qf[i]);
        }
    }
    else if (strcmp (devfont_info->type[sel_type], "ttf") == 0)
    {
        for (i=0; i<devfont_info->fnum[4]; i++)
        {
            SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->tf[i]);
        }
    }
    SendMessage (hctrl, LB_SETCURSEL, 0, 0);


}

static int InitListBox (HWND hWnd)
{
    char str_buffer [LEN_FONT_NAME + 1];
    int i;
    HWND hctrl;

    hctrl = GetDlgItem (hWnd, ID_Type);
    for (i=0; i<devfont_info->fnum[5]; i++)
    {
        SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->type[i]);
    }
    SendMessage (hctrl, LB_SETCURSEL, 0, 0);

    hctrl = GetDlgItem (hWnd, ID_FAMILY);
    for (i=0; i<devfont_info->fnum[0]; i++)
    {
        SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->rf[i]);
    }
    SendMessage (hctrl, LB_SETCURSEL, 0, 0);

    hctrl = GetDlgItem (hWnd, ID_CHARSET);
    for (i=0; i<devfont_info->fnum[6]; i++)
    {
        SendMessage (hctrl, LB_INSERTSTRING, i, (LPARAM)devfont_info->charset[i]);
    }
    SendMessage (hctrl, LB_SETCURSEL, 0, 0);

    hctrl = GetDlgItem (hWnd, ID_WEIGHT);
    InsertString (hctrl, weight_str, TABLESIZE(weight_str));

    hctrl = GetDlgItem (hWnd, ID_SLANT);
    InsertString (hctrl, slant_str, TABLESIZE(slant_str));

    hctrl = GetDlgItem (hWnd, ID_FLIP);
    InsertString (hctrl, flip_str, TABLESIZE(flip_str));

    hctrl = GetDlgItem (hWnd, ID_Other);
    InsertString (hctrl, other_str, TABLESIZE(other_str));

    hctrl = GetDlgItem (hWnd, ID_UnderLine);
    InsertString (hctrl, underline_str, TABLESIZE(underline_str));

    hctrl = GetDlgItem (hWnd, ID_StruckOut);
    InsertString (hctrl, struckout_str, TABLESIZE(struckout_str));

    hctrl = GetDlgItem (hWnd, ID_SIZE);
    UpdateWindow(hctrl,TRUE);

    for (i=MIN_FONT_SIZE; i<=MAX_FONT_SIZE; i++)
    {
        snprintf(str_buffer, 4, "%d", i);
        SendMessage (hctrl, LB_ADDSTRING, 0, (LPARAM)str_buffer);
    }
    return 0;
}

static LOGFONT* CreateSelFont(HWND hWnd)
{
    int sel = 0;
    HWND hctrl;
    char* family = NULL;
    char* charset = NULL;
    char* type = NULL;
    char weight, slant, flip, other, underline, struckout;

    sel = SendMessage (GetDlgItem (hWnd, ID_Type), LB_GETCURSEL, 0, 0);
    type = devfont_info->type [sel];
    sel_type = sel;

    hctrl = GetDlgItem (hWnd, ID_FAMILY);
    sel = SendMessage (hctrl, LB_GETCURSEL, 0, 0);

    if (strcmp (devfont_info->type[sel_type], "rbf") == 0)
        family = devfont_info->rf [sel];
    else if (strcmp (devfont_info->type[sel_type], "vbf") == 0)
        family = devfont_info->vf [sel];
    else if (strcmp (devfont_info->type[sel_type], "upf") == 0)
        family = devfont_info->uf [sel];
    else if (strcmp (devfont_info->type[sel_type], "qpf") == 0)
        family = devfont_info->qf [sel];
    else if (strcmp (devfont_info->type[sel_type], "ttf") == 0)
        family = devfont_info->tf [sel];


    hctrl = GetDlgItem (hWnd, ID_CHARSET);
    sel = SendMessage (hctrl, LB_GETCURSEL, 0, 0);
    charset = devfont_info->charset[sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_WEIGHT), LB_GETCURSEL, 0, 0);
    weight = weight_arg [sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_SLANT), LB_GETCURSEL, 0, 0);
    slant = slant_arg [sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_FLIP), LB_GETCURSEL, 0, 0);
    flip = flip_arg [sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_Other), LB_GETCURSEL, 0, 0);
    other = other_arg [sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_UnderLine), LB_GETCURSEL, 0, 0);
    underline = underline_arg [sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_StruckOut), LB_GETCURSEL, 0, 0);
    struckout = struckout_arg [sel];

    sel = SendMessage (GetDlgItem (hWnd, ID_SIZE), LB_GETCURSEL, 0, 0);

    return CreateLogFont (type, family, 
            charset, weight, slant, flip, 
            other, underline, struckout , sel+MIN_FONT_SIZE, 0);
}



static void FontGetDetailedInfo (const char *name)
{
    int i = 0, j = 0, charset_nr;
    Font* font_info = &gFont_info;
    Font* new_font_info;
    char* font[10];
    char* charset[10];
    char pszCharSet[10];
    char* buff = (char*) malloc((strlen(name)+1) * sizeof(char));
    memset (font, 0, 10);
    memset (charset, 0, 10);
    memset(buff, 0, (strlen(name)+1));
    strcpy(buff, name);

    charset[0] = strtok (buff, ",");
    while ( charset[i] != NULL )
    {
        i++;
        charset[i] = strtok (NULL, ",");
    }
    charset_nr = i;

    i = 0;
    font[0] = strtok ((char *)(*charset), "-");
    while ( font[i] != NULL )
    {
        i++;
        font[i] = strtok (NULL, "-");
    }

    if ( strcmp(font[5], "ISO8859") == 0)
    {
        sprintf(pszCharSet,"%s-%s",font[5], font[6]);
    }
    else 
    {
        sprintf(pszCharSet, "%s",font[5]);
    }

    while (font_info -> next != NULL)
        font_info = font_info -> next;

    for (j = 0; j < charset_nr; j++ )
    {
        new_font_info = (Font*)malloc (sizeof(Font));

        if (j == 0)
            strcpy (new_font_info -> charset, pszCharSet);
        else 
            strcpy (new_font_info->charset, charset[j]);


#ifndef _MGCHARSET_GB
        if ( !strncasecmp (new_font_info->charset, 
                    "GB2312", sizeof("GB2312")-1))
        {
            printf ("MiniGUI.cfg have %s, but not configure to support\n", 
                    new_font_info->charset);
            free (new_font_info);
            continue;
        }
        else
#endif
#ifndef _MGCHARSET_GBK
            if ( !strncasecmp (new_font_info->charset, "GBK", sizeof("GBK")-1))
            {
                printf ("MiniGUI.cfg have %s, but not configure to support\n", 
                        new_font_info->charset);
                free (new_font_info);
                continue;
            }
            else
#endif
#ifndef _MGCHARSET_BIG5
                if ( !strncasecmp (new_font_info->charset, "BIG5", sizeof("BIG5")-1))
                {
                    printf ("MiniGUI.cfg have %s, but not configure to support\n", 
                            new_font_info->charset);
                    free (new_font_info);
                    continue;
                }
                else
#endif
#ifndef _MGCHARSET_UNICODE
                    if ( !strncasecmp (new_font_info->charset, "UTF", sizeof("UTF")-1) )
                    {
                        printf ("MiniGUI.cfg have %s, but not configure to support\n", 
                                new_font_info->charset);
                        free (new_font_info);
                        continue;
                    }
#endif

        strcpy (new_font_info->type, font[0]);
        strcpy (new_font_info->family, font[1]);

        new_font_info->size = atoi(font[4]);
        new_font_info->next = NULL;
        font_info -> next = new_font_info;
        font_info = font_info -> next;
        nr_font_stack++;
    }
}

static void GetDevFontInfo (DevFontInfo* devfont_info, Font* font)
{
    int i = 0;
    char type = 0;
    enum type j;

    DevFontInfo* devinfo = devfont_info;
    Font* fontinfo = font;
    while (fontinfo != NULL)
    {
        for (i = 0; i < devinfo->fnum[6]; i++)
        {
            if ( strcmp (devinfo->charset[i], fontinfo->charset) == 0)
                break;
        }
        if (i == devinfo->fnum[6])
        {
            strcpy (devinfo->charset[devinfo->fnum[6]], fontinfo->charset);
            devinfo->fnum[6] ++;
        }


        for (j = rbf; j < ttf; j++)
        {
            if (strcmp(fontinfo->type, types[j]) == 0)
                break;
        }

        switch (j)
        {
            case rbf:
                for (i = 0; i < devinfo->fnum[0]; i++)
                {
                    if ( strcmp (devinfo->rf[i], fontinfo->family) == 0)
                        break;
                }
                if (i == devinfo->fnum[0])
                {
                    strcpy (devinfo->rf[devinfo->fnum[0]], fontinfo->family);
                    devinfo->fnum[0] ++;
                    type |= 0x01;
                }
                break;

            case vbf:
                for (i = 0; i < devinfo->fnum[1]; i++)
                {
                    if ( strcmp (devinfo->vf[i], fontinfo->family) == 0)
                        break;
                }
                if (i == devinfo->fnum[1])
                {
                    strcpy (devinfo->vf[devinfo->fnum[1]], fontinfo->family);
                    devinfo->fnum[1] ++;
                    type |= 0x02;
                }
                break;

            case upf:
                for (i = 0; i < devinfo->fnum[2]; i++)
                {
                    if ( strcmp (devinfo->uf[i], fontinfo->family) == 0)
                        break;
                }
                if (i == devinfo->fnum[2])
                {
                    strcpy (devinfo->uf[devinfo->fnum[2]], fontinfo->family);
                    devinfo->fnum[2] ++;
                    type |= 0x04;
                }
                break;

            case qpf:
                for (i = 0; i < devinfo->fnum[3]; i++)
                {
                    if ( strcmp (devinfo->qf[i], fontinfo->family) == 0)
                        break;
                }
                if (i == devinfo->fnum[3])
                {
                    strcpy (devinfo->qf[devinfo->fnum[3]], fontinfo->family);
                    devinfo->fnum[3] ++;
                    type |= 0x08;
                }
                break;

            case ttf:
                for (i = 0; i < devinfo->fnum[4]; i++)
                {
                    if ( strcmp (devinfo->tf[i], fontinfo->family) == 0)
                        break;
                }
                if (i == devinfo->fnum[4])
                {
                    strcpy (devinfo->tf[devinfo->fnum[4]], fontinfo->family);
                    devinfo->fnum[4] ++;
                    type |= 0x10;
                }
                break;

            default:
                break;
        }

        fontinfo = fontinfo->next;    
    }
    i = 0;
    if (type & 0x01)
    {
        strcpy (devinfo->type[i], "rbf");
        i++;
    }
    if (type & 0x02)
    {
        strcpy (devinfo->type[i], "vbf");
        i++;
    }
    if (type & 0x04)
    {
        strcpy (devinfo->type[i], "upf");
        i++;
    }
    if (type & 0x08)
    {
        strcpy (devinfo->type[i], "qpf");
        i++;
    }
    if (type & 0x10)
    {
        strcpy (devinfo->type[i], "ttf");
        i++;
    }
    devinfo->fnum[5] = i;

}

static void DynamicLoadDevFont (void)
{
    char* devfont_name1 = "rbf-fixed-rrncnn-6-12-ISO8859-1";
    char* filename1 = "font/6x12-iso8859-1.bin";
    LoadDevFontFromFile (devfont_name1, filename1);

    char* devfont_name2 = "rbf-fixed-rrncnn-12-12-GB2312-0";
    char* filename2 = "font/song-12-gb2312.bin";
    LoadDevFontFromFile (devfont_name2, filename2);

    char* devfont_name3 = "rbf-ming-rrncnn-12-12-BIG5";
    char* filename3 = "font/ming-12-big5.bin";
    LoadDevFontFromFile (devfont_name3, filename3);

    char* devfont_name4 = "upf-fmsong-rrncnn-12-12-ISO8859-1,ISO8859-15,GB2312,BIG5,GBK,UTF-8";
    char* filename4 = "font/fmsong-latin-12.upf";
    LoadDevFontFromFile (devfont_name4, filename4);

}

static PLOGFONT DisplayText (HWND hWnd, PLOGFONT font)
{
    unsigned char dest[128];
    HWND hwnd_display;
    PLOGFONT old_font;

    memset (dest, 0, sizeof(dest));
    hwnd_display = GetDlgItem(hWnd, ID_DISPLAY);

    old_font = SetWindowFont(hwnd_display, font);

    if (strcmp(font->charset, "GB2312-0") == 0 || strcmp (font->charset, "GBK") == 0)
    {
        WCS2MBS (font, dest, src2, TABLESIZE(src2), TABLESIZE(dest));
    }
    else if (strcmp(font->charset, "BIG5") == 0)
    {
        WCS2MBS (font, dest, src3, TABLESIZE(src3), TABLESIZE(dest));
    }
    else if (strcmp(font->charset, "UTF-8") == 0)
    {
        WCS2MBS (font, dest, src4, TABLESIZE(src4), TABLESIZE(dest));
    }
    else
    {
        WCS2MBS (font, dest, src1, TABLESIZE(src1), TABLESIZE(dest));
    }
    SetWindowText (hwnd_display, (const char*)dest);
    UpdateWindow(hwnd_display, TRUE);
    return old_font;
}

static int dialog_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    LOGFONT* font;
    LOGFONT* old_font;
    HWND hwnd_display;
    Font* font_info;
    DEVFONT* devfont;
    char  devfont_name[128] ={0};

    switch (message)
    {
        case MSG_INITDIALOG:
            {

                DynamicLoadDevFont ();

                devfont = (DEVFONT *)GetNextDevFont (NULL);
                while (devfont != NULL)
                {
                    strncpy(devfont_name,devfont->name,127);
                    FontGetDetailedInfo (devfont->name);
                    devfont = (DEVFONT*)GetNextDevFont (devfont);
                    memset(devfont_name,0,128);
                }

                font_info = gFont_info.next;
                devfont_info = calloc (1, sizeof(DevFontInfo));
                GetDevFontInfo (devfont_info, font_info);


                InitListBox (hWnd);

                hwnd_display = GetDlgItem(hWnd, ID_DISPLAY);
                old_font = GetWindowFont (hwnd_display);
                SetWindowAdditionalData (hwnd_display, (DWORD)old_font);

                font = CreateSelFont(hWnd);
                if (font)
                {
                    SetWindowFont(hwnd_display, font);
                    UpdateWindow(hwnd_display, TRUE);
                }

            }
            break;


        case MSG_COMMAND:
            hwnd_display = GetDlgItem(hWnd, ID_DISPLAY);
            switch (LOWORD (wParam))
            {
                case ID_Type:
                case ID_FAMILY:
                case ID_CHARSET:
                case ID_WEIGHT:
                case ID_SLANT:
                case ID_FLIP:
                case ID_Other:
                case ID_UnderLine:
                case ID_StruckOut:
                case ID_SIZE:
                    if (HIWORD (wParam) == LBN_SELCHANGE)
                    {
                        if (LOWORD(wParam) == ID_Type)
                        {
                            ChangeListBox (hWnd);
                        }

                        font = CreateSelFont(hWnd);
                        if (font)
                        {
                            old_font = DisplayText (hWnd, font);
                            DestroyLogFont(old_font);
                        }
                    }

                    break;

                case IDOK:
                    EndDialog (hWnd, 0);
                    break;

                case IDCANCEL:
                    font = GetWindowFont (hwnd_display);
                    SetWindowFont (hwnd_display,
                            (PLOGFONT)GetWindowAdditionalData(hwnd_display));
                    DestroyLogFont (font);
                    EndDialog (hWnd, 0);
                    break;
            }
            break;

        case MSG_CLOSE:
            EndDialog (hWnd, 0);

    }
    return DefaultDialogProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])

{

    int i = 0;

    Dlg.w <<= 1;
    Dlg.h <<= 1;

    for (i=0; i<TABLESIZE(ctrls); i++)
    {
        ctrls[i].x <<= 1;
        ctrls[i].y <<= 1;
        ctrls[i].w <<= 1;
        ctrls[i].h <<= 1;
    }

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0); 
#endif


    DialogBoxIndirectParam(&Dlg, HWND_DESKTOP, dialog_proc, 0);

    Dlg.w >>= 1;
    Dlg.h >>= 1;

    for (i=0; i<TABLESIZE(ctrls); i++)
    {
        ctrls[i].x >>= 1;
        ctrls[i].y >>= 1;
        ctrls[i].w >>= 1;
        ctrls[i].h >>= 1;
    }

    return 1;

}
