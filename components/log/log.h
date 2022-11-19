

#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TO_NULL        0x00
#define LOG_TO_FILE        0x01
#define LOG_TO_SCREEN      0x02
#define LOG_TO_SCNFILE     (LOG_TO_FILE | LOG_TO_SCREEN)


extern void *log_create(const char *log_name);
extern void  log_trace(void *log, const char *format, ...);
extern void  log_add_one_trace(void *log, void *buf);
extern void  log_close(void *log);


#define LOG_TRACE(log_hnd, fmt, ...)  log_trace(log_hnd, "[%s:%s:%d]: "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
       
#ifdef __cplusplus
}
#endif

#endif

