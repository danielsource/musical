#include "chord_finder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

const Chord chords[] = {{"3-3", "Diminished Triad"},
                        {"3-6", "Diminished Triad (First Inversion)"},
                        {"6-3", "Diminished Triad (Second Inversion)"},
                        {"4-4", "Augmented Triad"},
                        {"3-4", "Minor"},
                        {"4-5", "Minor (First Inversion)"},
                        {"5-3", "Minor (Second Inversion)"},
                        {"4-3", "Major"},
                        {"3-5", "Major (First Inversion)"},
                        {"5-4", "Major (Second Inversion)"},
                        {"4-3-4", "Major 7"},
                        {"3-4-1", "Major 7 (First Inversion)"},
                        {"4-1-4", "Major 7 (Second Inversion)"},
                        {"1-4-3", "Major 7 (Third Inversion)"},
                        {"4-3-3", "Dominant 7"},
                        {"3-3-2", "Dominant 7 (First Inversion)"},
                        {"3-2-4", "Dominant 7 (Second Inversion)"},
                        {"2-4-3", "Dominant 7 (Third Inversion)"},
                        {"3-4-3", "Minor 7"},
                        {"4-3-2", "Minor 7 (First Inversion)"},
                        {"3-2-3", "Minor 7 (Second Inversion)"},
                        {"2-3-4", "Minor 7 (Third Inversion)"},
                        {"3-3-4", "Half Diminished 7"},
                        {"3-4-2", "Half-Diminished 7 (First Inversion)"},
                        {"4-2-3", "Half-Diminished 7 (Second Inversion)"},
                        {"2-3-3", "Half-Diminished 7 (Third Inversion)"},
                        {"3-3-3", "Diminished 7"},
                        {"3-4-4", "Minor-Major 7"},
                        {"4-4-1", "Minor-Major 7 (First Inversion)"},
                        {"4-1-3", "Minor-Major 7 (Second Inversion)"},
                        {"1-3-4", "Minor-Major 7 (Third Inversion)"},
                        {"4-4-3", "Augmented Major 7"},
                        {"4-3-1", "Augmented Major 7 (First Inversion)"},
                        {"3-1-4", "Augmented Major 7 (Second Inversion)"},
                        {"1-4-4", "Augmented Major 7 (Third Inversion)"},
                        {"4-4-2", "Augmented 7"},
                        {"4-2-2", "Augmented 7 (First Inversion)"},
                        {"2-2-4", "Augmented 7 (Second Inversion)"},
                        {"2-4-4", "Augmented 7 (Third Inversion)"},
                        {"2-5", "sus2"},
                        {"7-7-2", "sus2"},
                        {"7-7-1", "Minor sus2"},
                        {"5-2", "sus4"},
                        {"5-2-5", "sus4"},
                        {"2-2-3", "add2"},
                        {"2-3-5", "add2 (Ninth as Root)"},
                        {"3-5-2", "add2 (First Inversion)"},
                        {"5-2-2", "add2 (Second Inversion)"},
                        {"4-3-4-3", "Major 9th"},
                        {"7-4-3", "Major 9th (Missing 3rd)"},
                        {"4-7-3", "Major 9th (Missing 5th)"},
                        {"4-3-3-4", "Dominant 9th"},
                        {"7-3-4", "Dominant 9th (Missing 3rd)"},
                        {"4-6-4", "Dominant 9th (Missing 5th)"},
                        {"4-3-3-3", "Dominant Minor 9th"},
                        {"7-3-3", "Dominant Minor 9th (Missing 3rd)"},
                        {"4-6-3", "Dominant Minor 9th (Missing 5th)"},
                        {"3-4-3-3", "Minor 9th"},
                        {"3-7-3", "Minor 9th (Missing 5th)"},
                        {"4-3-3-5", "Dominant 7 Sharp 9th"}};

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
