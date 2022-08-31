CC      = cc
CFLAGS  = -std=c99 -g
LDFLAGS = -lraylib -lm

sources = $(wildcard src/*.c)
headers = $(wildcard src/*.h)
test_sources = $(wildcard test/*.c)
test_headers = $(wildcard test/*.h)
tests = $(test_sources:.c=)
objects = $(sources:.c=.o)

all: musical

debug: CFLAGS += -O0 -Wall -Wextra -Wpedantic
debug: musical

test: $(tests)
	@$(foreach t,$(tests),./$(t))

clean:
	rm -f musical $(objects)

musical: $(objects)
	$(CC) -o $@ $(objects) $(LDFLAGS)

src/%.o: src/%.c $(headers)
	$(CC) -o $@ -c $(CFLAGS) $<

test/%: test/%.c $(test_headers)
	$(CC) -o $@ $(CFLAGS) $<

.PHONY: all debug test clean
