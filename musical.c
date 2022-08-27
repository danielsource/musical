#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH(X) (sizeof(X) / sizeof(X)[0])
#define NOTE_NULL INT_MAX

typedef struct {
  char *signature;
  char *name;
} Chord;

const Chord chords[] = {
    {"4-3", "Major"},
    {"3-4", "Minor"},
    {"4-3-3-5", "Dominant 7 Sharp 9th"},
};

char *program_name;

int main(int argc, char *argv[]) {
  if (!program_name) program_name = argv[0];
  if (argc < 2) {
    fprintf(stderr,
            "Usage: %s <musical note indices sorted in ascending order...>\n",
            program_name);
    exit(EXIT_FAILURE);
  }

  int root = atoi(argv[1]);
  char *intervals[argc - 1];
  memcpy(intervals, &argv[1], (argc - 1) * sizeof(char *));
  char *chord_signature = calloc(1, sizeof(char));

  for (int i = 1; i < LENGTH(intervals); i++) {
    int size, note;
    char *s;
    note = abs(atoi(intervals[i]) - atoi(intervals[i - 1]));
    s = malloc((strlen(intervals[i]) + 1) * sizeof(char));
    sprintf(s, "%d", note);
    size = strlen(chord_signature) + 1;
    size += strlen(s);
    if (i != 2) size += strlen("-");
    chord_signature = realloc(chord_signature, size * sizeof(char));
    if (i != 2) strcat(chord_signature, "-");
    strcat(chord_signature, s);
    free(s);
  }

  bool is_chord_found = false;

  for (int i = 0; i < LENGTH(chords); i++) {
    if (strcmp(chord_signature, chords[i].signature) == 0) {
      is_chord_found = true;
      puts(chords[i].name);
    }
  }
  if (!is_chord_found)
    puts("Unknown chord");

  free(chord_signature);
  return EXIT_SUCCESS;
}
