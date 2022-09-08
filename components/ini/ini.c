/*
* 使用gcc -o ini ini.c -Wall -g -I./ 进行编译链接
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "ini.h"

#define INI_MAX_FILE_SIZE (1024 * 16)
#define INT_VALUE_LEN (32)

#define UNKNOW_POS   (-1) /* 初始化的位置信息，表明位置未知 */

#define LEFT_BRACE  '['
#define RIGHT_BRACE ']'
#define LINE_CHANGE '\n'  /* 0x0A */
#define LINE_END    '\r'  /* 0x0D */
#define STR_END     '\0'  /* 0x00 */

static int IsNewLine(char c)
{
	return ((LINE_CHANGE == c) ||  (LINE_END == c)) ? 1 : 0;
}

static int IsStrEnd(char c)
{
	return (STR_END == c) ? 1 : 0;
}

static int IsLeftBrace(char c)
{
	return (LEFT_BRACE == c) ? 1 : 0;
}

static int IsRightBrace(char c)
{
	return (RIGHT_BRACE == c) ? 1 : 0;
}

/*******************************************************************************
函数名称: LoadIniFile
功能说明: 将ini文件的内容载入内存
输入参数:
    fileName: 要装载的ini文件名
输出参数:
    buf      : 要装载到内存
    fileSize : 文件的大小
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
static int LoadIniFile(const char *fileName,
    char *buf, int *fileSize)
{
	FILE *f = NULL;
	int i = 0;

	assert(NULL != fileName);
	assert(NULL != buf);

	*fileSize = 0;

    /* 以只读方式打开文件 */
	f = fopen(fileName, "r");
	if (NULL == f)
    {
		return -INI_ERR_OPEN_FILE_FOR_READ;
	}

	/* 装载文件 */
	buf[i] = fgetc(f);
	while (buf[i] != (char)EOF)
    {
		i++;
		if (i >= INI_MAX_FILE_SIZE)
        { /* 文件不能超过指定大小 */
            return -INI_ERR_FILE_TOO_LARGE;
		}

		buf[i] = fgetc(f);
	}

	buf[i] = STR_END;
	*fileSize = i;

	fclose(f);

	return 0;
}

/*******************************************************************************
函数名称: SaveIniFile
功能说明: 将内存中的内容写入到ini文件中去
输入参数:
    fileName: 要装载的ini文件名
    buf     : 要装载到内存
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
static int SaveIniFile(const char *fileName, const char *buf)
{
	FILE *f = NULL;

	assert(NULL != fileName);
	assert(NULL != buf);

    /* 以只写方式打开文件 */
	f = fopen(fileName, "w");
	if (NULL == f)
    {
		return -INI_ERR_OPEN_FILE_FOR_WRITE;
	}

	/* 写入文件 */
	if (EOF == fputs(buf, f))
	{
		fclose(f);
		return -INI_ERR_WRITE_FILE;
	}

	fclose(f);

	return 0;
}

/*******************************************************************************
函数名称: ParseFile
功能说明: 无
输入参数:
    buf     : 装载有ini文件内容的内存
    section : 要查找的section的名字
    key     : 要查找的key的名字
输出参数:
    sectionBegin  : section的起始位置
    sectionEnd    : section的结束位置
    keyBegin  : key的起始位置
    keyEnd    : key的结束位置
    valueBegin: value的起始位置
    valueEnd  : value的结束位置
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
#if 0

static int ParseFile(
    const char *buf, const char *section, const char *key,
    int *sectionBegin, int *sectionEnd, int *keyBegin, int *keyEnd,
    int *valueBegin, int *valueEnd)
{
	const char *p = buf;
	int i = 0;
	int sectionLen;
	int keyLen;

    sectionLen = strlen(section);
    keyLen  = strlen(key);

	assert(NULL != buf);
	assert((NULL != section) && (0 != sectionLen));
	assert((NULL != key) && (0 != keyLen));

    *sectionBegin = UNKNOW_POS;
	*sectionEnd = UNKNOW_POS;
    *keyBegin = UNKNOW_POS;
    *keyEnd = UNKNOW_POS;
    *valueBegin = UNKNOW_POS;
    *valueEnd = UNKNOW_POS;

	while (!IsStrEnd(p[i]))
    {
		if (((0 == i) ||  IsNewLine(p[i-1])) && IsLeftBrace(p[i]))
		{ /* 找到了'[' */
			int secBegin = i + 1;

			/* 寻找']' */
			do
            {
				i++;
			} while (!IsRightBrace(p[i]) && !IsStrEnd(p[i]));

			if ((0 == strncmp(p + secBegin, section, i - secBegin))
                && (sectionLen == i - secBegin))
            {
				int newLineBegin = 0;

				i++;

				/* 跳过']'后的空白 */
				while (isspace(p[i]))
				{
					i++;
				}

				/* 找到了section */
				*sectionBegin = secBegin;
				*sectionEnd = i;

				while (!(IsNewLine(p[i-1]) && IsLeftBrace(p[i]))
				    && !IsStrEnd(p[i]))
			    {
					int j = 0;

					/* 到了一个新行 */
					newLineBegin = i;

					while (!IsNewLine(p[i]) && !IsStrEnd(p[i]))
                    {
						i++;
					}

					/* now i  is equal to end of the line */
					j = newLineBegin;

					if (';' != p[j]) /* skip over comment */
					{
						while ((j < i) && ('=' != p[j]))
                        {
							j++;
							if ('=' == p[j])
                            {
								if ((strncmp(key,p+newLineBegin,j-newLineBegin)==0)
                                    && (keyLen==j-newLineBegin))
								{
									/*find the key ok */
									*keyBegin = newLineBegin;
									*keyEnd = j-1;

									*valueBegin = j+1;
									*valueEnd = i;

									return 0;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}

	return -INI_ERR_PARSE_FILE;
}

#else

static int ParseFile(
    const char *buf, const char *section, const char *key,
    int *sectionBegin, int *sectionEnd, int *keyBegin, int *keyEnd,
    int *valueBegin, int *valueEnd)
{
	const char *p = buf;
	int i = 0;
	int sectionLen;
	int keyLen;

    sectionLen = strlen(section);
    keyLen  = strlen(key);

	assert(NULL != buf);
	assert((NULL != section) && (0 != sectionLen));
	assert((NULL != key) && (0 != keyLen));

    *sectionBegin = UNKNOW_POS;
	*sectionEnd = UNKNOW_POS;
    *keyBegin = UNKNOW_POS;
    *keyEnd = UNKNOW_POS;
    *valueBegin = UNKNOW_POS;
    *valueEnd = UNKNOW_POS;

	while (!IsStrEnd(p[i]))
    {
		if (((0 == i) ||  IsNewLine(p[i-1])) && IsLeftBrace(p[i]))
		{ /* 找到了'[' */
			int secBegin = i + 1;

			/* 寻找']' */
			do
            {
				i++;
			} while (!IsRightBrace(p[i]) && !IsStrEnd(p[i]));

			if ((0 == strncmp(p + secBegin, section, i - secBegin))
                && (sectionLen == i - secBegin))
            {
				int newLineBegin = 0;

				i++;

				/* 跳过']'后的空白 */
				while (isspace(p[i]))
				{
					i++;
				}

				/* 找到了section */
				*sectionBegin = secBegin;
				*sectionEnd = i;

				while (!(IsNewLine(p[i-1]) && IsLeftBrace(p[i]))
				    && !IsStrEnd(p[i]))
			    {
					int j = 0;

					/* 到了一个新行 */
					newLineBegin = i;

					while (!IsNewLine(p[i]) && !IsStrEnd(p[i]))
                    {
						i++;
					}

					/* now i  is equal to end of the line */
					j = newLineBegin;

					if (';' != p[j]) /* skip over comment */
					{
						while ((j < i) && ('=' != p[j]))
                        {
							j++;
							if ('=' == p[j])
                            {
								if ((strncmp(key,p+newLineBegin,j-newLineBegin)==0)
                                    && (keyLen==j-newLineBegin))
								{
									/*find the key ok */
									*keyBegin = newLineBegin;
									*keyEnd = j-1;

									*valueBegin = j+1;
									*valueEnd = i;

									return 0;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}

	return -INI_ERR_PARSE_FILE;
}

#endif

/*******************************************************************************
函数名称: IniReadString
功能说明: 从ini文件中读指定字段的值
输入参数:
    fileName    : 要读取的ini文件名
    section     : 要读取的section的名字
    key         : 要读取的key的名字
    valueSize       : 读取到的value的最大长度
    defaultValue: 如果找不到对应的字段，那么就将此默认值赋值给value
输出参数:
    value   : 读取到的value值
返 回 值:
    > 0: 成功代码
    ==0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
int IniReadString(
    const char *fileName, const char *section, const char *key,
    char *value, int valueSize, const char *defaultValue)
{
	char *buf = NULL;
	int fileSize = 0;
	int secBegin = 0;
    int secEnd = 0;
    int keyBegin= 0;
    int keyEnd = 0;
    int valueBegin = 0;
    int valueEnd = 0;
    int copyCnt = 0;

	/* 检查参数 */
    if ((NULL == fileName) || (0 == strlen(fileName)))
    {
        return -INI_ERR_NO_FILE_NAME;
    }

    if ((NULL == section) || (0 == strlen(section)))
    {
        return -INI_ERR_NO_SECTION;
    }

    if ((NULL == key) || (0 == strlen(key)))
    {
        return -INI_ERR_NO_KEY;
    }

    if ((NULL == value) || (0 >= valueSize))
    {
        return -INI_ERR_NO_VALUE;
    }

    /* 分配内存 */
    buf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == buf)
    {
        return -INI_ERR_MALLOC;
    }

    /* 载入文件 */
	if (0 != LoadIniFile(fileName, buf, &fileSize))
	{ /* 载入失败 */
		if (NULL != defaultValue)
		{
			strncpy(value, defaultValue, valueSize);
            free(buf);
            return INI_ERR_LOAD_FILE;
		}

        free(buf);
		return -INI_ERR_LOAD_FILE;
	}

	if (0 != ParseFile(buf, section, key, &secBegin, &secEnd,
        &keyBegin, &keyEnd, &valueBegin, &valueEnd))
	{ /* 指定字段未找到 */
		if (NULL != defaultValue)
		{
			strncpy(value, defaultValue, valueSize);
            free(buf);
            return INI_ERR_PARSE_FILE;
		}

        free(buf);
		return -INI_ERR_PARSE_FILE;
	}

	copyCnt = valueEnd - valueBegin;

    /* 扣除字符串结束符，所以要-1 */
	if ((valueSize - 1) < copyCnt)
	{
		copyCnt = (valueSize - 1);
	}

	memcpy(value, buf + valueBegin, copyCnt);
	value[copyCnt] = STR_END;

    free(buf);
	return 0;
}

/*******************************************************************************
函数名称: IniWriteString
功能说明: 将值写入ini文件的指定字段
输入参数:
    fileName    : 要写入的ini文件名
    section     : 要写入的section的名字
    key         : 要写入的key的名字
    value       : 要写入的value值
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
int IniWriteString(
    const char *fileName, const char *section, const char *key,
    const char *value)
{
	char *wrBuf = NULL;
	char *buf = NULL;
	int fileSize = 0;
	int ret = 0;
	int secBegin = UNKNOW_POS;
    int secEnd = 0;
    int keyBegin= 0;
    int keyEnd = 0;
    int valueBegin = 0;
    int valueEnd = 0;
    int valueSize = (int)strlen(value);

	/* 检查参数 */
    if ((NULL == fileName) || (0 == strlen(fileName)))
    {
        return -INI_ERR_NO_FILE_NAME;
    }

    if ((NULL == section) || (0 == strlen(section)))
    {
        return -INI_ERR_NO_SECTION;
    }

    if ((NULL == key) || (0 == strlen(key)))
    {
        return -INI_ERR_NO_KEY;
    }

    if (NULL == value)
    {
        return -INI_ERR_NO_VALUE;
    }

    /* 分配内存 */
    buf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == buf)
    {
        return -INI_ERR_MALLOC;
    }

    /* 分配内存 */
    wrBuf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == wrBuf)
    {
        free(buf);
        return -INI_ERR_MALLOC;
    }

    memset(wrBuf, 0, INI_MAX_FILE_SIZE);

    /* 载入文件 */
	if (0 == LoadIniFile(fileName, buf, &fileSize))
	{ /* 载入成功 */
		ParseFile(buf, section, key, &secBegin, &secEnd,
            &keyBegin, &keyEnd, &valueBegin, &valueEnd);
	}

	if (UNKNOW_POS == secBegin)
	{ /* section位置未初始化 */
		if (0 == fileSize)
		{ /* 无文件 */
			sprintf(wrBuf + fileSize, "[%s]\n%s=%s\n",
                section, key, value);
		}
		else
		{ /* 无指定的section */
			memcpy(wrBuf,buf,fileSize);
			sprintf(wrBuf + fileSize, "\n[%s]\n%s=%s\n",
                section, key, value);
		}
	}
	else if (UNKNOW_POS == keyBegin)
	{ /* 有指定的section，但是没有指定的key */
		memcpy(wrBuf, buf, secEnd);
		sprintf(wrBuf + secEnd, "%s=%s\n", key, value);
		memcpy(wrBuf + secEnd + strlen(key) + valueSize + 2,
            buf + secEnd, fileSize - secEnd);
	}
	else
	{ /* 有指定的section，也有指定的key */
		memcpy(wrBuf, buf, valueBegin);
		memcpy(wrBuf + valueBegin, value, valueSize);
		memcpy(wrBuf + valueBegin + valueSize, buf + valueEnd,
            fileSize - valueEnd);
	}

	ret = SaveIniFile(fileName, wrBuf);

    free(wrBuf);
    free(buf);

    return ret;
}

/*******************************************************************************
函数名称: IniReadInt
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
int IniReadInt(
    const char *fileName, const char *section, const char *key,
    int defaultValue)
{
	char value[INT_VALUE_LEN] = {0};

	if (0 != IniReadString(fileName, section, key,
        value, INT_VALUE_LEN, NULL))
	{
		return defaultValue;
	}

	return atoi(value);
}


