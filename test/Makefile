BIN = test
CFLAGS += $(shell pkg-config --cflags check) -I..
LDLIBS = $(shell pkg-config --libs check)
test_sources = $(wildcard *.c)
test_objects = $(test_sources:.c=.o)
objects := $(addprefix ../, $(objects))

.PHONY: check clean

check: $(test_objects)
	$(CC) $(CFLAGS) $(objects) $(test_objects) -o $(BIN) $(LDLIBS)
	./$(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(objects) -c $< -o $@ $(LDLIBS)

clean:
	-rm *.o $(BIN)
