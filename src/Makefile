# fileno()
CFLAGS = -D_XOPEN_SOURCE
# vasprintf()
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Wall -Wextra -pedantic -std=c99 -g -Wshadow
CFLAGS += -Wno-unused-function
# gcc complains about integer overflow in numflow.h, but that doesn't happen.
CFLAGS += -Wno-overflow 
lib_dir = ../lib/
lib = libdiceexpr.so
lib_link = diceexpr

BISON_FLAGS = -d

test_dir = ../test/
test_sources = $(wildcard $(addprefix ${test_dir}, *.c))
test_objects = $(test_sources:.c=.o)
test_bin = $(addprefix ${test_dir}, test)


.PHONY: default all clean debug check clean_check

default: CFLAGS += -O2 -DNDEBUG
default: all

all: str.o de.tab.c lex.yy.c
	mkdir -p $(lib_dir)
	$(CC) $(CFLAGS) $^ -shared -o $(addprefix ${lib_dir}, ${lib})

debug: CFLAGS += -O0
debug: all

str.o: str.c str.h
	$(CC) $(CFLAGS) $< -c -o $@

de.tab.c: de.y str.o
	bison -d $<

lex.yy.c: de.l
	flex $<

check: CFLAGS = $(shell pkg-config --cflags check) -I. -L$(lib_dir) -O2 -g -Wall \
	-Wextra -pedantic -std=c99
check: LD_LIBS = $(shell pkg-config --libs check) -l$(basename ${lib_link})
check: $(test_objects)
	$(CC) $(CFLAGS) -o $(test_bin) $(test_objects) $(LD_LIBS)
	$(addprefix LD_LIBRARY_PATH=, ${lib_dir}) $(test_bin)

$(addprefix ${test_dir}, %.o): $(addprefix ${test_dir}, %.c)
	$(CC) $(CFLAGS) $< -c -o $@ $(LD_LIBS)

clean:
	-rm de.tab.* lex.yy.c *.o $(addprefix ${test_dir}, *.o test)

clean_check:
	-rm $(addprefix ${test_dir}, *.o test) 
