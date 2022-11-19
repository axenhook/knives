
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "log.h"

#define DATA_TIME_STR_LEN 40
#define MAX_LOG_LINES     10000
#define LOG_NAME_LEN      256

typedef struct log
{
    FILE *hnd;  
    char  name[LOG_NAME_LEN];
    unsigned char mode;
    int   lines;
} log_t; 

int get_date_time_string(char *str, int str_size)
{
    time_t curr_time = 0;
    struct tm *pt = NULL;

    if (!str)
    {
        return -1;
    }
    
    str[0] = 0;
    
    curr_time = time(NULL);
    pt = localtime(&curr_time);
    if (!pt)
    {
        return -2;
    }

    snprintf(str, str_size, "%04d%02d%02d%02d%02d%02d", 
        pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday, 
        pt->tm_hour, pt->tm_min, pt->tm_sec);
    
    return 0;
}

void add_one_trace(FILE *hnd, const char *trace)
{
	char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN);
    
    //printf("%s %s", date_time, trace); // print to screen
    fprintf(hnd, "%s %s", date_time, trace);
}

FILE *create_log_file(const char *log_name)
{
	char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN);
    
    char name[LOG_NAME_LEN];
    sprintf(name, "%s-%s.log", log_name, date_time);
    FILE *hnd = fopen(name, "wb+");
    if (!hnd)
    {
        return NULL;
    }

    add_one_trace(hnd, "log file created!!!\n");
    return hnd;
}

void *log_create(const char *log_name)
{
    log_t *log = (log_t *)malloc(sizeof(log_t));
    if (!log)
    {
        return NULL;
    }

    log->hnd = create_log_file(log_name);
    if (!log->hnd)
    {
        free(log);
        return NULL;
    }

    strncpy(log->name, log_name, sizeof(log->name));
    log->lines = 0;
    return log;
}

void log_close(void *log)
{
    log_t *tmp_log = (log_t *)log;
    if (!tmp_log)
    {
        return;
    }

    if (tmp_log->hnd)
    {
        add_one_trace(tmp_log->hnd, "log file closed!!!\n");
        fclose(tmp_log->hnd);
        tmp_log->hnd = NULL;
    }

    free(tmp_log);
}

void log_add_one_trace(void *log, void *buf)
{
    log_t *tmp_log = (log_t *)log;
    add_one_trace(tmp_log->hnd, buf);
    tmp_log->lines++;
    if (tmp_log->lines >= MAX_LOG_LINES)
    {
        FILE *hnd = create_log_file(tmp_log->name);
        if (hnd)
        {
            add_one_trace(tmp_log->hnd, "log file closed!!!\n");
            fclose(tmp_log->hnd);
            tmp_log->hnd = hnd;
            tmp_log->lines = 0;
        }        
    }
}

void log_trace(void *log, const char *fmt, ...)
{
    #define BUF_LEN           1024
    char buf[BUF_LEN];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUF_LEN, fmt, ap);
    va_end(ap);

    log_add_one_trace(log, buf);
}


