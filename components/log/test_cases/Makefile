CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../

POBJS = 

TEST_LOG = test_log
TEST_LOG_MGR = test_log_mgr

TEST_LOG_SRC = test_log.c ../log.c
TEST_LOG_MGR_SRC = test_log_mgr.c ../log.c ../log_mgr.c

TARGET_ALL = $(TEST_LOG) $(TEST_LOG_MGR)

all: $(TARGET_ALL)

$(TEST_LOG):
	gcc -o $@ $(TEST_LOG_SRC) $(CFLAGS)
	
$(TEST_LOG_MGR):
	gcc -o $@ $(TEST_LOG_MGR_SRC) $(CFLAGS)
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
