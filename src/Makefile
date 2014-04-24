CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -g
CFLAGS += -Wno-unused-function
# fileno()
CFLAGS += -D_XOPEN_SOURCE
build_dir = ../build
bin_dir = ../bin
bin = de

.PHONY: all

all: $(shell mkdir -p $(build_dir))
all: $(build_dir)/y.tab.c $(build_dir)/lex.yy.c
	mkdir -p $(bin_dir)
	$(CC) $(CFLAGS) -o $(bin_dir)/$(bin) $<

$(build_dir)/y.tab.c: de.y
	yacc -d $< -o $@

$(build_dir)/lex.yy.c: de.l
	lex $< -o $@
