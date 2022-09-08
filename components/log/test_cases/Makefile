CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

TEST_MAIN = test_log

LOG_SRC = test_log.c ../log.c

TARGET_ALL = $(TEST_MAIN)

all: $(TARGET_ALL)

$(TEST_MAIN):
	gcc -o $@ $(LOG_SRC) $(CFLAGS)
	
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
