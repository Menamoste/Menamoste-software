# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =
LDLIBS = 

SRC = main.c
OBJ = ${SRC:.c=.o}

all: main

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} main     # remove main program

# END