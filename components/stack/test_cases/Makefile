CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

TEST_MAIN = test

SRC = test_stack.c ../stack.c

TARGET_ALL = $(TEST_MAIN)

all: $(TARGET_ALL)

$(TEST_MAIN):
	gcc -o $@ $(SRC) $(CFLAGS)
	
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
