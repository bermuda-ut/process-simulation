#=============================================================================
#     FileName: Makefile
#         Desc: custom makefile for the project
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: mallocsizeof.me
#      Version: 0.0.1
#   LastChange: 2017-03-31 22:13:09
#=============================================================================
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99
# SHELL := /bin/bash

LIB = -lm
HDR = strategy.h process.h list.h memory.h storage.h advlist.h
SRC = strategy.c process.c list.c memory.c storage.c advlist.c driver.c
OBJ = $(SRC:.c=.o)

EXE = swap

## Top level target is executable.
$(EXE):	$(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIB)

clean:
	rm $(OBJ) 

clobber: clean
	rm $(EXE) 

test:
	./$(EXE) -f ./tests/input.txt -a first -m 1000 -q 7 2> /dev/null | diff - ./tests/output.txt
	./$(EXE) -f ./tests/testFirst1 -a first -m 1000 -q 7 2> /dev/null | diff - ./tests/testFirst1.out
	./$(EXE) -f ./tests/testWorst1 -a worst -m 1000 -q 7 2> /dev/null | diff - ./tests/testWorst1.out
	./$(EXE) -f ./tests/testBest1 -a best -m 1000 -q 7 2> /dev/null | diff - ./tests/testBest1.out
	@echo "Success, all tests passed"

$(OBJ): $(HDR)
