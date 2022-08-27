CC      = cc
CFLAGS  = -std=c99 -ggdb -O0
LDFLAGS = -L. -lraylib -lm

sources = musical.c chord_finder.c timer.c
headers = config.h chord_finder.h timer.h util.h mouse_click.h
objects = $(sources:.c=.o)

all: musical

clean:
	rm -f musical $(objects)

musical: $(objects)
	$(CC) -o $@ $(objects) $(LDFLAGS)

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $<

.PHONY: all clean
