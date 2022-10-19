
#ifndef __OS_UTILS_H__
#define __OS_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

extern u64_t os_get_cycle_count(void);
extern u64_t os_get_ms_count(void);
extern u64_t os_get_us_count(void);
extern u64_t os_get_second_count(void);
extern int32_t os_str_to_u64(const char *str, u64_t *value, uint32_t base);
extern char os_char_to_hex(char c);
extern int32_t os_str_to_hex(char *str, uint8_t *hex, uint32_t hex_len);
extern u64_t os_convert_u64(const u64_t src);
extern int32_t os_get_date_time_string(char *str, int32_t str_size);

#ifdef __cplusplus
}
#endif

#endif
