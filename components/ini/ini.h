#ifndef __INI_H__
#define __INI_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tagINI_ERROR_CODE_E
{
    INI_ERR_START = 0x4000,
    INI_ERR_OPEN_FILE_FOR_READ,
    INI_ERR_OPEN_FILE_FOR_WRITE,
    INI_ERR_WRITE_FILE,
    INI_ERR_LOAD_FILE,
    INI_ERR_PARSE_FILE,
    INI_ERR_MALLOC,
    INI_ERR_FILE_TOO_LARGE,
    INI_ERR_NO_FILE_NAME,
    INI_ERR_NO_SECTION,
    INI_ERR_NO_KEY,
    INI_ERR_NO_VALUE,
} INI_ERROR_CODE_E; /* End of INI_ERROR_CODE_E */


extern int IniReadString(const char *fileName, const char *section, const char *key,
    char *value, int valueSize, const char *defaultValue);
extern int IniReadInt(const char *fileName, const char *section, const char *key, int defaultValue);
extern int IniWriteString(const char *fileName, const char *section, const char *key, const char *value);

#ifdef __cplusplus
};
#endif

#endif //end of __INI_H__

