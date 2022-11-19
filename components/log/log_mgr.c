#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"

#define PIDS_NUM   256

typedef struct log_mgr
{
    int   level[PIDS_NUM];
    void *log_hnd;
} log_mgr_t; 

log_mgr_t g_log_mgr;

void log_mgr_set_level(unsigned int pid, int level)
{
    if (pid >= PIDS_NUM)
    {
        return;
    }
    
    g_log_mgr.level[pid] = level;

    return;
}

int log_mgr_get_level(unsigned int pid)
{
    if (pid >= PIDS_NUM)
    {
        return -1;
    }
    
    return g_log_mgr.level[pid];
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

void log_mgr_trace(unsigned int pid, unsigned char level, const char *fmt, ...)
{
    if (level > g_log_mgr.level[pid])
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

