# fileno()
CFLAGS = -D_XOPEN_SOURCE
# vasprintf()
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Wall -Wextra -pedantic -std=c11 -O2 -g
CFLAGS += -Wno-unused-function
bin_dir = ../bin
bin = de

.PHONY: all clean

all: str.o y.tab.c lex.yy.c
	mkdir -p $(bin_dir)
	$(CC) $(CFLAGS) $^ -o $(bin_dir)/$(bin)

str.o: str.c
	$(CC) $(CFLAGS) $< -c -o $@

y.tab.c: de.y str.o
	yacc -d $<

lex.yy.c: de.l
	lex $<

clean:
	-rm y.tab.* lex.yy.c
