/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*******************************************************************************

            Copyright(C), 2016~2019, axen.hook@foxmail.com
********************************************************************************
File Name: OS_LOG.C
Author   : axen.hook
Version  : 1.00
Date     : 02/Mar/2016
Description: 
Function List: 
    1. ...: 
History: 
    Version: 1.00  Author: axen.hook  Date: 02/Mar/2016
--------------------------------------------------------------------------------
    1. Primary version
*******************************************************************************/

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <assert.h>
#include <time.h>

#include "log.h"

#define DATA_TIME_STR_LEN 40

typedef struct log
{
    int   log_level;
    FILE *file_hnd;  
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

    sprintf(str, "%04d%02d%02d%02d%02d%02d", 
        pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday, 
        pt->tm_hour, pt->tm_min, pt->tm_sec);
    
    return 0;
}

void *log_create(const char *log_name)
{
	#define LOG_NAME_LEN     256
	
    log_t *log = (log_t *)malloc(sizeof(log_t));
    if (!log)
    {
        return NULL;
    }
    
    log->log_level = 2;
    
	char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN);
    
    char name[LOG_NAME_LEN];
    sprintf(name, "%s-%s.log", log_name, date_time);

    log->file_hnd = fopen(name, "wb+");
    if (!log->file_hnd)
    {
		free(log);
        return NULL;
    }

    fprintf(log->file_hnd, "%s\n", name);
    
    return log;
}

void log_close(void *log)
{
    log_t *tmp_log = (log_t *)log;
    if (!tmp_log)
    {
        return;
    }

    if (tmp_log->file_hnd)
    {
        fclose(tmp_log->file_hnd);
        tmp_log->file_hnd = NULL;
    }

    free(tmp_log);
}

void log_trace(void *log, int log_level, const char *format, ...)
{
	#define BUF_LEN           1024
	
    log_t *tmp_log = (log_t *)log;
    
    if (log_level > tmp_log->log_level)
    {
    	return;
    }

	char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN);

	char buf[BUF_LEN];
    va_list ap;
    va_start(ap, format);
    _vsnprintf(buf, BUF_LEN, format, ap);
    va_end(ap);

    //printf("%s %s", date_time, buf); // print to screen
    fprintf(tmp_log->file_hnd, "%s %s", date_time, buf);
}


