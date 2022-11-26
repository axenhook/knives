#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"
#include "log_mgr.h"


log_mgr_t g_log_mgr;

void log_mgr_set_level(unsigned int mid, int level)
{
    if (mid >= MIDS_NUM)
    {
        return;
    }
    
    g_log_mgr.level[mid] = level;

    return;
}

void log_mgr_set_name(unsigned int mid, const char *name)
{
    if (mid >= MIDS_NUM)
    {
        return;
    }
    
    strncpy(g_log_mgr.name[mid], name, MNAME_SIZE);
    g_log_mgr.name[mid][MNAME_SIZE - 1] = '\0';

    return;
}

int log_mgr_init(const char *log_name)
{
    memset(&g_log_mgr, 0, sizeof(log_mgr_t));
    g_log_mgr.log_hnd = log_create(log_name);
    if (g_log_mgr.log_hnd)
    {
        return 0;  // success
    }
    
    return -1; // failed
}

void log_mgr_destroy(void)
{
    if (g_log_mgr.log_hnd)
    {
        log_close(g_log_mgr.log_hnd);
        g_log_mgr.log_hnd = NULL;
    }
}

void log_mgr_trace(unsigned int mid, unsigned char level, const char *fmt, ...)
{
    if (level > g_log_mgr.level[mid])
    {
        return;
    }
    
    #define BUF_LEN           1024
    char buf[BUF_LEN];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUF_LEN, fmt, ap);
    va_end(ap);

    log_add_one_trace(g_log_mgr.log_hnd, buf);
}

