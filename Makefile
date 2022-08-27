all: musical

clean:
	rm -f musical

musical: musical.c
	cc -o $@ $< -std=c99 -L. -lraylib -lm -ggdb -O0

.PHONY: all clean
