CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../

POBJS = 

TEST_MAIN = test

SRC = test_queue.c ../queue.c

TARGET_ALL = $(TEST_MAIN)

all: $(TARGET_ALL)

$(TEST_MAIN):
	gcc -o $@ $(SRC) $(CFLAGS)
	
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
