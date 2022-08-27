#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "chord_finder.h"
#include "config.h"
#include "util.h"

#define NOTE_NULL INT_MAX

int main(int argc, char *argv[]) {
  if (!program_name) program_name = argv[0];
  if (argc < 2) {
    fprintf(stderr,
            "Usage: %s <musical note indices sorted in ascending order...>\n",
            program_name);
    exit(EXIT_FAILURE);
  }
  char name[32] = {0};
  get_chord_name(name, LENGTH(name), &argv[1], argc - 1);
  printf("%s\n", name);
  return EXIT_SUCCESS;
}
