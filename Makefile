CC      = cc
CFLAGS  = -std=c99 -g
LDFLAGS = -lraylib -lm

sources = $(wildcard src/*.c)
headers = $(wildcard src/*.h)
objects = $(sources:.c=.o)

all: musical

debug: CFLAGS += -O0 -Wall -Wextra -Wpedantic
debug: musical

clean:
	rm -f musical $(objects)

musical: $(objects)
	$(CC) -o $@ $(objects) $(LDFLAGS)

src/%.o: src/%.c $(headers)
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY: all debug run clean
