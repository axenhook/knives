

#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void *log_create(const char *log_name);
extern void  log_trace(void *log, const char *format, ...);
extern void  log_close(void *log);

#define LOG_TRACE(log_hnd, fmt, ...)  log_trace(log_hnd, "[%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
       
#ifdef __cplusplus
}
#endif

#endif

