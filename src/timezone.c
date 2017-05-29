/*
** $Id: timezone.c 770 2009-12-24 03:04:19Z wanzheng $
**
** timezone.c: worldtime timezone for FHAS.
**
** Copyright (C) 2002 ~ 2006 Feynman Software, all rights reserved.
**
** Use of this source package is subject to specific license terms
** from Beijing Feynman Software Technology Co., Ltd.
**
** URL: http://www.minigui.com
**
** Current maintainer: Wei Yongming (ymwei@minigui.org).
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
  
static char* sys_tz = "TZ=GMT-8";

static char buffer[50];

static struct tm my_tm;
static char ctime_buff [50];

static const char *Week_C[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 
static const char *Month_C[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

const char* gettime (char *timezone)
{
    time_t cur_time;

    if ( (cur_time = time(NULL)) != -1) {
        putenv (timezone); tzset();
        localtime_r (&cur_time, &my_tm);
        putenv (sys_tz); tzset();
        sprintf (buffer, "%02d:%02d", my_tm.tm_hour, my_tm.tm_min);
    }                
    return (buffer);
}

const char* getday(char *timezone)
{
    time_t cur_time;
    char *p;

    if ( (cur_time = time(NULL)) != -1) {
        putenv (timezone); tzset();
        ctime_r (&cur_time, ctime_buff);
        putenv (sys_tz); tzset();
        p = ctime_buff;
        p += 4;
        if( *p == ' ' )
            p++;
        strncpy(buffer,p,6);
        buffer[6] = '\0';
    }                
    return (buffer);
}

const char* getdate(char *timezone)
{
    time_t cur_time;

    if ( (cur_time = time(NULL)) != -1) {
        putenv (timezone); tzset();
        localtime_r (&cur_time, &my_tm);
        putenv (sys_tz); tzset();
        sprintf (buffer, "%s %s %2d %02d:%02d", 
                Week_C[my_tm.tm_wday], Month_C[my_tm.tm_mon], my_tm.tm_mday,
                my_tm.tm_hour, my_tm.tm_min);
    }                
    return (buffer);
}

const char* getdatetime (char *timezone)
{
    time_t cur_time;

    if ( (cur_time = time(NULL)) != -1) {
        putenv (timezone); tzset();
        localtime_r (&cur_time, &my_tm);
        putenv (sys_tz); tzset();
        sprintf (buffer, "%4d-%02d-%02d %s %02d:%02d:%02d", 
                my_tm.tm_year + 1900, my_tm.tm_mon+1, my_tm.tm_mday, 
                Week_C[my_tm.tm_wday], 
                my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec);
    }                
    return (buffer);
}

const char* getdatefull(char *timezone)
{
    time_t cur_time;
    char *p;
    char *space = " ";

    if ( (cur_time = time(NULL)) != -1) {
        putenv (timezone); tzset();
        ctime_r (&cur_time, ctime_buff);
        putenv (sys_tz); tzset();
        p = ctime_buff;
        p += 0;
        if( *p == ' ' )
            p++;
        strcpy(buffer, space);
        strcat(buffer, p);
    }                
    return (buffer);
}

