/*
* ʹ��gcc -o ini ini.c -Wall -g -I./ ���б�������
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "ini.h"

#define INI_MAX_FILE_SIZE (1024 * 16)
#define INT_VALUE_LEN (32)

#define UNKNOW_POS   (-1) /* ��ʼ����λ����Ϣ������λ��δ֪ */

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
��������: LoadIniFile
����˵��: ��ini�ļ������������ڴ�
�������:
    fileName: Ҫװ�ص�ini�ļ���
�������:
    buf      : Ҫװ�ص��ڴ�
    fileSize : �ļ��Ĵ�С
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static int LoadIniFile(const char *fileName,
    char *buf, int *fileSize)
{
	FILE *f = NULL;
	int i = 0;

	assert(NULL != fileName);
	assert(NULL != buf);

	*fileSize = 0;

    /* ��ֻ����ʽ���ļ� */
	f = fopen(fileName, "r");
	if (NULL == f)
    {
		return -INI_ERR_OPEN_FILE_FOR_READ;
	}

	/* װ���ļ� */
	buf[i] = fgetc(f);
	while (buf[i] != (char)EOF)
    {
		i++;
		if (i >= INI_MAX_FILE_SIZE)
        { /* �ļ����ܳ���ָ����С */
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
��������: SaveIniFile
����˵��: ���ڴ��е�����д�뵽ini�ļ���ȥ
�������:
    fileName: Ҫװ�ص�ini�ļ���
    buf     : Ҫװ�ص��ڴ�
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static int SaveIniFile(const char *fileName, const char *buf)
{
	FILE *f = NULL;

	assert(NULL != fileName);
	assert(NULL != buf);

    /* ��ֻд��ʽ���ļ� */
	f = fopen(fileName, "w");
	if (NULL == f)
    {
		return -INI_ERR_OPEN_FILE_FOR_WRITE;
	}

	/* д���ļ� */
	if (EOF == fputs(buf, f))
	{
		fclose(f);
		return -INI_ERR_WRITE_FILE;
	}

	fclose(f);

	return 0;
}

/*******************************************************************************
��������: ParseFile
����˵��: ��
�������:
    buf     : װ����ini�ļ����ݵ��ڴ�
    section : Ҫ���ҵ�section������
    key     : Ҫ���ҵ�key������
�������:
    sectionBegin  : section����ʼλ��
    sectionEnd    : section�Ľ���λ��
    keyBegin  : key����ʼλ��
    keyEnd    : key�Ľ���λ��
    valueBegin: value����ʼλ��
    valueEnd  : value�Ľ���λ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
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
		{ /* �ҵ���'[' */
			int secBegin = i + 1;

			/* Ѱ��']' */
			do
            {
				i++;
			} while (!IsRightBrace(p[i]) && !IsStrEnd(p[i]));

			if ((0 == strncmp(p + secBegin, section, i - secBegin))
                && (sectionLen == i - secBegin))
            {
				int newLineBegin = 0;

				i++;

				/* ����']'��Ŀհ� */
				while (isspace(p[i]))
				{
					i++;
				}

				/* �ҵ���section */
				*sectionBegin = secBegin;
				*sectionEnd = i;

				while (!(IsNewLine(p[i-1]) && IsLeftBrace(p[i]))
				    && !IsStrEnd(p[i]))
			    {
					int j = 0;

					/* ����һ������ */
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
		{ /* �ҵ���'[' */
			int secBegin = i + 1;

			/* Ѱ��']' */
			do
            {
				i++;
			} while (!IsRightBrace(p[i]) && !IsStrEnd(p[i]));

			if ((0 == strncmp(p + secBegin, section, i - secBegin))
                && (sectionLen == i - secBegin))
            {
				int newLineBegin = 0;

				i++;

				/* ����']'��Ŀհ� */
				while (isspace(p[i]))
				{
					i++;
				}

				/* �ҵ���section */
				*sectionBegin = secBegin;
				*sectionEnd = i;

				while (!(IsNewLine(p[i-1]) && IsLeftBrace(p[i]))
				    && !IsStrEnd(p[i]))
			    {
					int j = 0;

					/* ����һ������ */
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
��������: IniReadString
����˵��: ��ini�ļ��ж�ָ���ֶε�ֵ
�������:
    fileName    : Ҫ��ȡ��ini�ļ���
    section     : Ҫ��ȡ��section������
    key         : Ҫ��ȡ��key������
    valueSize       : ��ȡ����value����󳤶�
    defaultValue: ����Ҳ�����Ӧ���ֶΣ���ô�ͽ���Ĭ��ֵ��ֵ��value
�������:
    value   : ��ȡ����valueֵ
�� �� ֵ:
    > 0: �ɹ�����
    ==0: �ɹ�
    < 0: �������
˵    ��: ��
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

	/* ������ */
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

    /* �����ڴ� */
    buf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == buf)
    {
        return -INI_ERR_MALLOC;
    }

    /* �����ļ� */
	if (0 != LoadIniFile(fileName, buf, &fileSize))
	{ /* ����ʧ�� */
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
	{ /* ָ���ֶ�δ�ҵ� */
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

    /* �۳��ַ���������������Ҫ-1 */
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
��������: IniWriteString
����˵��: ��ֵд��ini�ļ���ָ���ֶ�
�������:
    fileName    : Ҫд���ini�ļ���
    section     : Ҫд���section������
    key         : Ҫд���key������
    value       : Ҫд���valueֵ
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
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

	/* ������ */
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

    /* �����ڴ� */
    buf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == buf)
    {
        return -INI_ERR_MALLOC;
    }

    /* �����ڴ� */
    wrBuf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == wrBuf)
    {
        free(buf);
        return -INI_ERR_MALLOC;
    }

    memset(wrBuf, 0, INI_MAX_FILE_SIZE);

    /* �����ļ� */
	if (0 == LoadIniFile(fileName, buf, &fileSize))
	{ /* ����ɹ� */
		ParseFile(buf, section, key, &secBegin, &secEnd,
            &keyBegin, &keyEnd, &valueBegin, &valueEnd);
	}

	if (UNKNOW_POS == secBegin)
	{ /* sectionλ��δ��ʼ�� */
		if (0 == fileSize)
		{ /* ���ļ� */
			sprintf(wrBuf + fileSize, "[%s]\n%s=%s\n",
                section, key, value);
		}
		else
		{ /* ��ָ����section */
			memcpy(wrBuf,buf,fileSize);
			sprintf(wrBuf + fileSize, "\n[%s]\n%s=%s\n",
                section, key, value);
		}
	}
	else if (UNKNOW_POS == keyBegin)
	{ /* ��ָ����section������û��ָ����key */
		memcpy(wrBuf, buf, secEnd);
		sprintf(wrBuf + secEnd, "%s=%s\n", key, value);
		memcpy(wrBuf + secEnd + strlen(key) + valueSize + 2,
            buf + secEnd, fileSize - secEnd);
	}
	else
	{ /* ��ָ����section��Ҳ��ָ����key */
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
��������: IniReadInt
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
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


