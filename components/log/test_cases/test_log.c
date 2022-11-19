#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "../log.h"

int main(int argc, char *argv[])
{
    int i = 0;

    void *log = log_create("test_log");
    assert(log);
    
    LOG_TRACE(log, "Test %d\n", i++);
    LOG_TRACE(log, "Test %d\n", i++);
    usleep(1000*1000);
    LOG_TRACE(log, "Test %d\n", i++);
    LOG_TRACE(log, "Test %d\n", i++);

    log_close(log);
    
    printf("test log finished\n");

    return 0;
}


