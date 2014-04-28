# fileno()
CFLAGS = -D_XOPEN_SOURCE
# vasprintf()
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Wall -Wextra -pedantic -std=c11 -g
CFLAGS += -Wno-unused-function
lib_dir = ../lib/
lib = libdiceexpr.so

.PHONY: default all clean debug

default: CFLAGS += -O2 -DNDEBUG
default: all

all: str.o y.tab.c lex.yy.c
	mkdir -p $(lib_dir)
	$(CC) $(CFLAGS) $^ -shared -o $(addprefix ${lib_dir}, ${lib})

debug: CFLAGS += -O0
debug: all

str.o: str.c str.h
	$(CC) $(CFLAGS) $< -c -o $@

y.tab.c: de.y str.o
	yacc -d $<

lex.yy.c: de.l
	lex $<

clean:
	-rm y.tab.* lex.yy.c *.o
