CC ?= gcc
CFLAGS ?= -O2 -g -pedantic -Wall -Wextra -march=native
#CFLAGS ?= -O2 -pedantic -Wall

SRC := $(shell echo src/*.c)
OBJ := $(SRC:.c=.o)


default: all

all: OsuSimular

clean:
	rm -f CC ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

OsuSimular: main.c ${OBJ}
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
