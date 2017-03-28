# 
# Sample Makefile for project 1 
#
# The program used for illustration purposes is
# a simple program using prime numbers
#

## CC  = Compiler.
## CFLAGS = Compiler flags.
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -g
SHELL := /bin/bash
ALL_TESTS := $(addsuffix .test, $(basename $(wildcard ./tests/*.txt)))

## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.
SRC = driver.c process.c list.c memory.c
OBJ = driver.o process.o list.o memory.o
EXE = swap

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm

## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.
clobber: clean
		/bin/rm $(EXE) 

#test: $(ALL_TESTS)
test:
	./swap -f ./tests/testFirst1.txt -a first -m 1000 -q 7

%.test : %.txt %.out
	./swap -f $< -a first -m 1000 -q 7 | diff - $(word 2, $?)

all: test
	@echo "Success, all tests passed."

## Dependencies
driver.o:	process.h memory.h list.h
process.o:	process.h list.h
memory.o: memory.h process.h list.h
list.o: list.h
