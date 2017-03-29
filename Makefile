CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99
# SHELL := /bin/bash
#ALL_TESTS := $(addsuffix .test, $(basename $(wildcard ./tests/*.txt)))

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

#test: $(ALL_TESTS)
test:
	./swap -f ./tests/testFirst1.txt -a first -m 1000 -q 7

%.test : %.txt %.out
	./swap -f $< -a first -m 1000 -q 7 | diff - $(word 2, $?)

all: test
	@echo "Success, all tests passed."

$(OBJ): $(HDR)
