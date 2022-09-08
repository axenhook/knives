#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../ini.h"

int main(int argc, char *argv[])
{
#define TEST_BUF_SIZE 256
#define TEST_INI_FILE "test_ini.ini"
    
    char value[TEST_BUF_SIZE]={0};

    // section student
    assert(IniWriteString(TEST_INI_FILE, "student", "name", "Tony") == 0);
    assert(IniWriteString(TEST_INI_FILE, "student", "age", "20") == 0);

    assert(IniReadString(TEST_INI_FILE, "student", "name", value, TEST_BUF_SIZE, NULL) == 0);
    assert(strcmp(value, "Tony") == 0);

    assert(IniReadInt(TEST_INI_FILE, "student", "age", 0) == 20);

    // section sucess inc
    assert(IniWriteString(TEST_INI_FILE, "success inc", "test name", "what's your name") == 0);
    assert(IniWriteString(TEST_INI_FILE, "success inc", "test num", "123456") == 0);

    assert(IniReadString(TEST_INI_FILE, "success inc", "test name", value, TEST_BUF_SIZE, NULL) == 0);
    assert(strcmp(value, "what's your name") == 0);

    assert(IniReadInt(TEST_INI_FILE, "success inc", "test num", 0) == 123456);

	return 0;
}

