#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "chord_finder.h"

const Chord chords[] = {{"0", "Perfect First", 0},
                        {"1", "Minor Second", 0},
                        {"2", "Major Second", 0},
                        {"3", "Minor Third", 0},
                        {"4", "Major Third", 0},
                        {"5", "Perfect Fourth", 0},
                        {"6", "Diminished Sixth\n(Tritone)", 0},
                        {"7", "Perfect Fifth", 0},
                        {"8", "Minor Sixth", 0},
                        {"9", "Major Sixth", 0},
                        {"10", "Minor Seventh", 0},
                        {"11", "Major Seventh", 0},
                        {"12", "Perfect Eigth\n(Octave)", 0},
                        {"13", "Augmented Eight", 0},
                        {"3-3", "Diminished Triad", 0},
                        {"3-6", "Diminished Triad\n(First Inversion)", 2},
                        {"6-3", "Diminished Triad\n(Second Inversion)", 1},
                        {"4-4", "Augmented Triad", 0},
                        {"3-4", "Minor", 0},
                        {"4-5", "Minor\n(First Inversion)", 2},
                        {"5-3", "Minor\n(Second Inversion)", 1},
                        {"4-3", "Major", 0},
                        {"3-5", "Major\n(First Inversion)", 2},
                        {"5-4", "Major\n(Second Inversion)", 1},
                        {"4-3-4", "Major 7", 0},
                        {"3-4-1", "Major 7\n(First Inversion)", 3},
                        {"4-1-4", "Major 7\n(Second Inversion)", 2},
                        {"1-4-3", "Major 7\n(Third Inversion)", 1},
                        {"4-3-3", "Dominant 7", 0},
                        {"3-3-2", "Dominant 7\n(First Inversion)", 3},
                        {"3-2-4", "Dominant 7\n(Second Inversion)", 2},
                        {"2-4-3", "Dominant 7\n(Third Inversion)", 1},
                        {"3-4-3", "Minor 7", 0},
                        {"4-3-2", "Minor 7\n(First Inversion)", 3},
                        {"3-2-3", "Minor 7\n(Second Inversion)", 2},
                        {"2-3-4", "Minor 7\n(Third Inversion)", 1},
                        {"3-3-4", "Half Diminished 7", 0},
                        {"3-4-2", "Half-Diminished 7\n(First Inversion)", 3},
                        {"4-2-3", "Half-Diminished 7\n(Second Inversion)", 2},
                        {"2-3-3", "Half-Diminished 7\n(Third Inversion)", 1},
                        {"3-3-3", "Diminished 7", 0},
                        {"3-4-4", "Minor-Major 7", 0},
                        {"4-4-1", "Minor-Major 7\n(First Inversion)", 3},
                        {"4-1-3", "Minor-Major 7\n(Second Inversion)", 2},
                        {"1-3-4", "Minor-Major 7\n(Third Inversion)", 1},
                        {"4-4-3", "Augmented Major 7", 0},
                        {"4-3-1", "Augmented Major 7\n(First Inversion)", 3},
                        {"3-1-4", "Augmented Major 7\n(Second Inversion)", 2},
                        {"1-4-4", "Augmented Major 7\n(Third Inversion)", 1},
                        {"4-4-2", "Augmented 7", 0},
                        {"4-2-2", "Augmented 7\n(First Inversion)", 3},
                        {"2-2-4", "Augmented 7\n(Second Inversion)", 2},
                        {"2-4-4", "Augmented 7\n(Third Inversion)", 1},
                        {"2-5", "sus2", 0},
                        {"7-7-2", "sus2", 0},
                        {"7-7-1", "Minor sus2", 0},
                        {"5-2", "sus4", 0},
                        {"5-2-5", "sus4", 0},
                        {"2-2-3", "add2", 0},
                        {"2-3-5", "add2\n(Ninth as Root)", 3},
                        {"3-5-2", "add2\n(First Inversion)", 2},
                        {"5-2-2", "add2\n(Second Inversion)", 1},
                        {"4-3-4-3", "Major 9th", 0},
                        {"7-4-3", "Major 9th\n(Missing 3rd)", 0},
                        {"4-7-3", "Major 9th\n(Missing 5th)", 0},
                        {"4-3-3-4", "Dominant 9th", 0},
                        {"7-3-4", "Dominant 9th\n(Missing 3rd)", 0},
                        {"4-6-4", "Dominant 9th\n(Missing 5th)", 0},
                        {"4-3-3-3", "Dominant Minor 9th", 0},
                        {"7-3-3", "Dominant Minor 9th\n(Missing 3rd)", 0},
                        {"4-6-3", "Dominant Minor 9th\n(Missing 5th)", 0},
                        {"3-4-3-3", "Minor 9th", 0},
                        {"3-7-3", "Minor 9th\n(Missing 5th)", 0},
                        {"4-3-3-5", "Dominant 7 Sharp 9th", 0}};

const char *const notes[] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};

void get_chord_name(char *dest, int dest_len, int *intervals, int len) {
  if (len <= 0) {
    strncpy(dest, "(null)", dest_len);
    return;
  } else if (len == 1) {
    strncpy(dest, notes[intervals[0] % 12], dest_len);
    return;
  }
  char *chord_signature = calloc(1, sizeof(char));

  for (int i = 1; i < len; i++) {
    int size = strlen(chord_signature) + 1,
        note = abs(intervals[i] - intervals[i - 1]);
    char *s = malloc(3 * sizeof(char));
    snprintf(s, 3, "%d", note);
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
      snprintf(dest, dest_len, "%s %s", notes[intervals[chords[i].root] % 12], chords[i].name);
      break;
    }
  }
  if (!is_chord_found) {
    if (len > 2)
      strncpy(dest, "Unrecognized chord", dest_len);
    else
      strncpy(dest, "Unrecognized interval", dest_len);
  }

  free(chord_signature);
}
