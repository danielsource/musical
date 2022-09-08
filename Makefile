CC      ?= cc
CFLAGS  ?= -std=c99 -g -Wall -Wextra -Wpedantic
LDFLAGS ?= -lraylib -lm

entry_point = src/musical.o
sources = $(wildcard src/*.c)
headers = $(wildcard src/*.h)
test_sources = $(wildcard test/*.c)
test_headers = $(wildcard test/*.h)
tests = $(test_sources:.c=)
objects = $(sources:.c=.o)

all: musical

test: $(tests)
	@$(foreach t,$(tests),./$(t))

clean:
	rm -f musical $(objects) $(tests) $(test_sources:.c=.o)

musical: $(objects)
	$(CC) -o $@ $(objects) $(LDFLAGS)

src/%.o: src/%.c $(headers)
	$(CC) -o $@ -c $(CFLAGS) $<

test/%: test/%.c $(test_headers) musical
	$(CC) -Isrc -o $@.o -c $(CFLAGS) $<
	$(CC) -o $@ $@.o $(filter-out $(entry_point),$(objects)) $(LDFLAGS)

.PHONY: all debug test clean
