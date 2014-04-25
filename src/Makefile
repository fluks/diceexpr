# fileno()
CFLAGS = -D_XOPEN_SOURCE
CFLAGS += -Wall -Wextra -pedantic -std=c11 -O2 -g
CFLAGS += -Wno-unused-function
bin_dir = ../bin
bin = de

.PHONY: all clean

all: y.tab.c lex.yy.c
	mkdir -p $(bin_dir)
	$(CC) $(CFLAGS) $^ -o $(bin_dir)/$(bin)

y.tab.c: de.y
	yacc -d $<

lex.yy.c: de.l
	lex $<

clean:
	-rm y.tab.* lex.yy.c
