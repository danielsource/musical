
#include "chord_finder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

const Chord chords[] = {
    {"4-3", "Major"},
    {"3-4", "Minor"},
    {"4-3-3-5", "Dominant 7 Sharp 9th"},
};

void get_chord_name(char *dest, int dest_size, char **intervals, int len) {
  char *chord_signature = calloc(1, sizeof(char));

  for (int i = 1; i < len; i++) {
    int size = strlen(chord_signature) + 1,
        note = abs(atoi(intervals[i]) - atoi(intervals[i - 1]));
    char *s = malloc((strlen(intervals[i]) + 1) * sizeof(char));
    sprintf(s, "%d", note);
    size += strlen(s);
    if (i != 1) size += strlen("-");
    chord_signature = realloc(chord_signature, size * sizeof(char));
    if (i != 1) strcat(chord_signature, "-");
    strcat(chord_signature, s);
    free(s);
  }

  bool is_chord_found = false;
  for (int i = 0; i < LENGTH(chords); i++) {
    if (strcmp(chord_signature, chords[i].signature) == 0) {
      is_chord_found = true;
      strncpy(dest, chords[i].name, dest_size);
      break;
    }
  }
  if (!is_chord_found) strncpy(dest, "Unknown chord", dest_size);

  puts(chord_signature);
  free(chord_signature);
}
