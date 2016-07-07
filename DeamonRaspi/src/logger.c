/*
    * logger.c
    *
    *  Created on: Jul 07, 2016
    *      Author: tqkieu
*/
#include <stdio.h>
#include <errno.h>
#include <time.h>

const char * __log_format = "%s> Log %d: %s.\r\n";
char __log_name[64] = "tqk.log";
int __log_mode = LOG_MODE_ANY;
FILE * __log_fd = NULL;
int __log_num = 0;
char __log_time[32];

int log_config(const char * log_name, int log_mode)
{
    if (strlen(log_name) >= 32)
    return -1;
    memset(__log_name, 0, 64);
    memcpy(__log_name, log_name);
    __log_mode = log_mode;
    
    return 0;
}

char * getTime(void)
{
    time_t t = time(NULL);
    struct tm tm;
    
    memset(__log_time, 0, 32);
    tm = *localtime(&t);
    
    sprintf(curr_time_str, "%d-%d-%d %d:%d:%d\n",
    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    return __log_time;
}
int log(const char * msg)
{
    log_open();
    
    switch (__log_mode)
    {
        case LOG_MODE_FILE_ONLY:
            if (fputs(msg, __log_fd) < 0)
            {
                printf("Log Error: %s", strerror(errno));
                return -1;
            }
            fprintf(__log_fd, __log_format, getTime(), __log_num++, msg);
        break;
        case LOG_MODE_CONSOLE_ONLY:
            printf(__log_format, getTime(), __log_num++, msg);
        break;
        case LOG_MODE_ANY:
            if (fputs(msg, __log_fd) < 0)
            {
                printf("Log Error: %s", strerror(errno));
                return -1;
            }
            fprintf(__log_fd, __log_format, getTime(), __log_num++, msg);
            
            printf(__log_format, getTime(), __log_num++, msg);
        break;
        default:
        break;
    }
    
    // log_close();
    
    return 0;
}
int log_close(void)
{
    if (__log_fd != NULL)
    fclose(__log_fd);
}
int log_open(void)
{
    if (__log_fd == NULL)
    __log_fd = fopen(__log_name, "a+");
}