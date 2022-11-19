

#ifndef _LOG_MGR_H_
#define _LOG_MGR_H_

#ifdef __cplusplus
extern "C" {
#endif

int log_mgr_init(const char *log_name);
void log_mgr_destroy(void);
int log_mgr_get_level(unsigned int pid);
void log_mgr_set_level(unsigned int pid, int level);
void log_mgr_trace(unsigned int pid, unsigned char level, const char *fmt, ...);

#define SET_MODULE_ID(pid)  static unsigned int g_pid = pid;

#define SET_LOG_LEVEL(level) log_mgr_set_level(g_pid, level)

#if 1

#define LOG_DEBUG(fmt, ...)    \
    log_mgr_trace(g_pid, 4, "[DEBUG][%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)       \
    log_mgr_trace(g_pid, 3, "[INFO ][%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
   
#define LOG_WARN(fmt, ...)         \
    log_mgr_trace(g_pid, 2, "[WARN ][%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
   
#define LOG_ERROR(fmt, ...)        \
    log_mgr_trace(g_pid, 1, "[ERROR][%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
   
#define LOG_EMERG(fmt, ...)        \
    log_mgr_trace(g_pid, 0, "[EMERG][%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_EVENT(fmt, ...)        \
    log_mgr_trace(g_pid, 0, "[EVENT][%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else

#define LOG_DEBUG(fmt, ...)    \
    log_mgr_trace(g_pid, 4, "[DEBUG][%lld][%s:%s:%d]: "fmt, \
        (unsigned long long)pthread_self(),  __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)       \
    log_mgr_trace(g_pid, 3, "[INFO ][%lld][%s:%s:%d]: "fmt, \
        (unsigned long long)pthread_self(),  __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
   
#define LOG_WARN(fmt, ...)         \
    log_mgr_trace(g_pid, 2, "[WARN ][%lld][%s:%s:%d]: "fmt, \
        (unsigned long long)pthread_self(),  __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
   
#define LOG_ERROR(fmt, ...)        \
    log_mgr_trace(g_pid, 1, "[ERROR][%lld][%s:%s:%d]: "fmt, \
        (unsigned long long)pthread_self(),  __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
   
#define LOG_EMERG(fmt, ...)        \
    log_mgr_trace(g_pid, 0, "[EMERG][%lld][%s:%s:%d]: "fmt, \
        (unsigned long long)pthread_self(),  __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_EVENT(fmt, ...)        \
    log_mgr_trace(g_pid, 0, "[EVENT][%lld][%s:%s:%d]: "fmt, \
        (unsigned long long)pthread_self(),  __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)


#endif
       
#ifdef __cplusplus
}
#endif

#endif

