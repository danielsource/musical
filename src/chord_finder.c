#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"
#include "chord_finder.h"
#include "util.h"

const Chord chords[LANGUAGE_LAST][LENGTH(*chords)] = {
  [PORTUGUESE] = {
    {"0",       "Primeira justa", 0},
    {"1",       "Segunda menor (semitom)", 0},
    {"2",       "Segunda maior (tom)", 0},
    {"3",       "Terça menor", 0},
    {"4",       "Terça maior", 0},
    {"5",       "Quarta justa", 0},
    {"6",       "Quinta diminuta (trítono)", 0},
    {"7",       "Quinta justa", 0},
    {"8",       "Sexta menor", 0},
    {"9",       "Sexta maior", 0},
    {"10",      "Sétima menor", 0},
    {"11",      "Sétima maior", 0},
    {"12",      "Oitava justa", 0},
    {"13",      "Nona menor", 0},
    {"14",      "Nona maior", 0},
    {"15",      "Décima menor", 0},
    {"16",      "Décima maior", 0},
    {"17",      "Décima primeira justa", 0},
    {"18",      "Décima primeira aumentada", 0},
    {"19",      "Décima segunda justa (tritave)", 0},
    {"20",      "Décima terceira menor", 0},
    {"21",      "Décima terceira maior", 0},
    {"22",      "Décima quarta menor", 0},
    {"23",      "Décima quarta maior", 0},
    {"24",      "Décima quinta justa (oitava dupla)", 0},
    {"25",      "Décima quinta aumentada", 0},
    {"3-3",     "diminuto", 0},
    {"3-6",     "diminuto (primeira inversão)", 2},
    {"6-3",     "diminuto (segunda inversão)", 1},
    {"3-4",     "menor", 0},
    {"4-5",     "menor (primeira inversão)", 2},
    {"5-3",     "menor (segunda inversão)", 1},
    {"4-3",     "maior", 0},
    {"3-5",     "maior (primeira inversão)", 2},
    {"5-4",     "maior (segunda inversão)", 1},
    {"2-5",     "segunda suspensa", 0},
    {"5-5",     "segunda suspensa (segunda como tônica)", 2},
    {"5-2",     "quarta suspensa", 0},
    {"2-2-3",   "com segunda adicionada", 0},
    {"2-3-5",   "com segunda adicionada (segunda como tônica)", 3},
    {"3-5-2",   "com segunda adicionada (primeira inversão)", 2},
    {"5-2-2",   "com segunda adicionada (segunda inversão)", 1},
    {"4-3-7",   "com nona adicionada", 0},
    {"4-4",     "aumentado", 0},
    {"3-3-3",   "diminuto com sétima", 0},
    {"3-3-4",   "meio-diminuto", 0},
    {"3-4-2",   "meio-diminuto (primeira inversão)", 3},
    {"4-2-3",   "meio-diminuto (segunda inversão)", 2},
    {"2-3-3",   "meio-diminuto (terceira inversão)", 1},
    {"3-4-3",   "menor com sétima", 0},
    {"4-3-2",   "menor com sétima (primeira inversão)", 3},
    {"3-2-3",   "menor com sétima (segunda inversão)", 2},
    {"2-3-4",   "menor com sétima (terceira inversão)", 1},
    {"4-2-4",   "dominante com quinta bemol", 0},
    {"2-4-2",   "dominante com quinta bemol (terceira como tônica)", 3},
    {"4-3-3",   "com sétima dominante", 0},
    {"3-3-2",   "com sétima dominante (primeira inversão)", 3},
    {"3-2-4",   "com sétima dominante (segunda inversão)", 2},
    {"2-4-3",   "com sétima dominante (terceira inversão)", 1},
    {"3-4-4",   "menor com sétima maior", 0},
    {"4-4-1",   "menor com sétima maior (primeira inversão)", 3},
    {"4-1-3",   "menor com sétima maior (segunda inversão)", 2},
    {"1-3-4",   "menor com sétima maior (terceira inversão)", 1},
    {"4-3-4",   "com sétima maior", 0},
    {"3-4-1",   "com sétima maior (primeira inversão)", 3},
    {"4-1-4",   "com sétima maior (segunda inversão)", 2},
    {"1-4-3",   "com sétima maior (terceira inversão)", 1},
    {"4-4-3",   "aumentado com sétima maior", 0},
    {"4-3-1",   "aumentado com sétima maior (primeira inversão)", 3},
    {"3-1-4",   "aumentado com sétima maior (segunda inversão)", 2},
    {"1-4-4",   "aumentado com sétima maior (terceira inversão)", 1},
    {"4-4-2",   "aumentado com sétima menor", 0},
    {"4-2-2",   "aumentado com sétima menor (primeira inversão)", 3},
    {"2-2-4",   "aumentado com sétima menor (segunda inversão)", 2},
    {"2-4-4",   "aumentado com sétima menor (terceira inversão)", 1},
    {"3-4-3-4", "menor com nona", 0},
    {"3-7-4",   "menor com nona (faltando quinta)", 0},
    {"4-3-3-3", "dominante com nona menor", 0},
    {"7-3-3",   "dominante com nona menor (faltando terça)", 0},
    {"4-6-3",   "dominante com nona menor (faltando quinta)", 0},
    {"4-3-3-4", "dominante com nona maior", 0},
    {"7-3-4",   "dominante com nona maior (faltando terça)", 0},
    {"4-6-4",   "dominante com nona maior (faltando quinta)", 0},
    {"4-3-4-3", "com nona maior", 0},
    {"7-4-3",   "com nona maior (faltando terça)", 0},
    {"4-7-3",   "com nona maior (faltando quinta)", 0},
    {"4-3-3-5", "dominante com nona sustenido", 0},
    {"3-4-3-4-3", "menor com décima primeira", 0},
    {"4-3-3-4-3", "com décima primeira dominante", 0}
  },
  [ENGLISH] = {
    {"0",       "Perfect unison", 0},
    {"1",       "Minor second (semitone)", 0},
    {"2",       "Major second (tone)", 0},
    {"3",       "Minor third", 0},
    {"4",       "Major third", 0},
    {"5",       "Perfect fourth", 0},
    {"6",       "Diminished fifth (tritone)", 0},
    {"7",       "Perfect fifth", 0},
    {"8",       "Minor sixth", 0},
    {"9",       "Major sixth", 0},
    {"10",      "Minor seventh", 0},
    {"11",      "Major seventh", 0},
    {"12",      "Perfect octave", 0},
    {"13",      "Minor ninth", 0},
    {"14",      "Major ninth", 0},
    {"15",      "Minor tenth", 0},
    {"16",      "Major tenth", 0},
    {"17",      "Perfect eleventh", 0},
    {"18",      "Augmented eleventh", 0},
    {"19",      "Perfect twelfth (tritave)", 0},
    {"20",      "Minor thirteenth", 0},
    {"21",      "Major thirteenth", 0},
    {"22",      "Minor fourteenth", 0},
    {"23",      "Major fourteenth", 0},
    {"24",      "Perfect fifteenth (double octave)", 0},
    {"25",      "Augmented fifteenth", 0},
    {"3-3",     "diminished", 0},
    {"3-6",     "diminished (first inversion)", 2},
    {"6-3",     "diminished (second inversion)", 1},
    {"3-4",     "minor", 0},
    {"4-5",     "minor (first inversion)", 2},
    {"5-3",     "minor (second inversion)", 1},
    {"4-3",     "major", 0},
    {"3-5",     "major (first inversion)", 2},
    {"5-4",     "major (second inversion)", 1},
    {"2-5",     "suspended second", 0},
    {"5-5",     "suspended second (first inversion)", 2},
    {"5-2",     "suspended fourth", 0},
    {"2-2-3",   "added-second", 0},
    {"2-3-5",   "added-second (second as root)", 3},
    {"3-5-2",   "added-second (first inversion)", 2},
    {"5-2-2",   "added-second (second inversion)", 1},
    {"4-3-7",   "added-ninth", 0},
    {"4-4",     "augmented", 0},
    {"3-3-3",   "diminished seventh", 0},
    {"3-3-4",   "half-diminished", 0},
    {"3-4-2",   "half-diminished (first inversion)", 3},
    {"4-2-3",   "half-diminished (second inversion)", 2},
    {"2-3-3",   "half-diminished (third inversion)", 1},
    {"3-4-3",   "minor seventh", 0},
    {"4-3-2",   "minor seventh (first inversion)", 3},
    {"3-2-3",   "minor seventh (second inversion)", 2},
    {"2-3-4",   "minor seventh (third inversion)", 1},
    {"4-2-4",   "dominant seventh flat five", 0},
    {"2-4-2",   "dominant seventh flat five", 3},
    {"4-3-3",   "dominant seventh", 0},
    {"3-3-2",   "dominant seventh (first inversion)", 3},
    {"3-2-4",   "dominant seventh (second inversion)", 2},
    {"2-4-3",   "dominant seventh (third inversion)", 1},
    {"3-4-4",   "minor-major seventh", 0},
    {"4-4-1",   "minor-major seventh (first inversion)", 3},
    {"4-1-3",   "minor-major seventh (second inversion)", 2},
    {"1-3-4",   "minor-major seventh (third inversion)", 1},
    {"4-3-4",   "major seventh", 0},
    {"3-4-1",   "major seventh (first inversion)", 3},
    {"4-1-4",   "major seventh (second inversion)", 2},
    {"1-4-3",   "major seventh (third inversion)", 1},
    {"4-4-3",   "augmented major seventh", 0},
    {"4-3-1",   "augmented major seventh (first inversion)", 3},
    {"3-1-4",   "augmented major seventh (second inversion)", 2},
    {"1-4-4",   "augmented major seventh (third inversion)", 1},
    {"4-4-2",   "augmented seventh", 0},
    {"4-2-2",   "augmented seventh (first inversion)", 3},
    {"2-2-4",   "augmented seventh (second inversion)", 2},
    {"2-4-4",   "augmented seventh (third inversion)", 1},
    {"3-4-3-4", "minor ninth", 0},
    {"3-7-4",   "minor ninth (missing fifth)", 0},
    {"4-3-3-3", "dominant minor ninth", 0},
    {"7-3-3",   "dominant minor ninth (missing third)", 0},
    {"4-6-3",   "dominant minor ninth (missing fifth)", 0},
    {"4-3-3-4", "dominant ninth", 0},
    {"7-3-4",   "dominant ninth (missing third)", 0},
    {"4-6-4",   "dominant ninth (missing fifth)", 0},
    {"4-3-4-3", "major ninth", 0},
    {"7-4-3",   "major ninth (missing third)", 0},
    {"4-7-3",   "major ninth (missing fifth)", 0},
    {"4-3-3-5", "dominant seventh sharp ninth", 0},
    {"3-4-3-4-3", "minor eleventh", 0},
    {"4-3-3-4-3", "dominant eleventh", 0}
  },
  [ABBREVIATED_CHORDS] = {
    {"0",       "P1", 0},
    {"1",       "m2", 0},
    {"2",       "M2", 0},
    {"3",       "m3", 0},
    {"4",       "M3", 0},
    {"5",       "P4", 0},
    {"6",       "d5", 0},
    {"7",       "P5", 0},
    {"8",       "m6", 0},
    {"9",       "M6", 0},
    {"10",      "m7", 0},
    {"11",      "M7", 0},
    {"12",      "P8", 0},
    {"13",      "m9", 0},
    {"14",      "M9", 0},
    {"15",      "m10", 0},
    {"16",      "M10", 0},
    {"17",      "P11", 0},
    {"18",      "A11", 0},
    {"19",      "P12", 0},
    {"20",      "m13", 0},
    {"21",      "M13", 0},
    {"22",      "m14", 0},
    {"23",      "M14", 0},
    {"24",      "P15", 0},
    {"25",      "A15", 0},
    {"3-3",     "dim", 0},
    {"3-6",     "dim", 2},
    {"6-3",     "dim", 1},
    {"3-4",     "m", 0},
    {"4-5",     "m", 2},
    {"5-3",     "m", 1},
    {"4-3",     "", 0},
    {"3-5",     "", 2},
    {"5-4",     "", 1},
    {"2-5",     "sus2", 0},
    {"5-5",     "sus2", 2},
    {"5-2",     "sus4", 0},
    {"2-2-3",   "add2", 0},
    {"2-3-5",   "add2", 3},
    {"3-5-2",   "add2", 2},
    {"5-2-2",   "add2", 1},
    {"4-3-7",   "add9", 0},
    {"4-4",     "aug", 0},
    {"3-3-3",   "dim7", 0},
    {"3-3-4",   "m7(b5)", 0},
    {"3-4-2",   "m7(b5)", 3},
    {"4-2-3",   "m7(b5)", 2},
    {"2-3-3",   "m7(b5)", 1},
    {"3-4-3",   "m7", 0},
    {"4-3-2",   "m7", 3},
    {"3-2-3",   "m7", 2},
    {"2-3-4",   "m7", 1},
    {"4-2-4",   "7(b5)", 0},
    {"2-4-2",   "7(b5)", 3},
    {"4-3-3",   "7", 0},
    {"3-3-2",   "7", 3},
    {"3-2-4",   "7", 2},
    {"2-4-3",   "7", 1},
    {"3-4-4",   "mM7", 0},
    {"4-4-1",   "mM7", 3},
    {"4-1-3",   "mM7", 2},
    {"1-3-4",   "mM7", 1},
    {"4-3-4",   "maj7", 0},
    {"3-4-1",   "maj7", 3},
    {"4-1-4",   "maj7", 2},
    {"1-4-3",   "maj7", 1},
    {"4-4-3",   "maj7(#5)", 0},
    {"4-3-1",   "maj7(#5)", 3},
    {"3-1-4",   "maj7(#5)", 2},
    {"1-4-4",   "maj7(#5)", 1},
    {"4-4-2",   "aug7", 0},
    {"4-2-2",   "aug7", 3},
    {"2-2-4",   "aug7", 2},
    {"2-4-4",   "aug7", 1},
    {"3-4-3-4", "m9", 0},
    {"3-7-4",   "m9(no5)", 0},
    {"4-3-3-3", "7(b9)", 0},
    {"7-3-3",   "7(b9)(no3)", 0},
    {"4-6-3",   "7(b9)(no5)", 0},
    {"4-3-3-4", "9", 0},
    {"7-3-4",   "9(no3)", 0},
    {"4-6-4",   "9(no5)", 0},
    {"4-3-4-3", "maj9", 0},
    {"7-4-3",   "maj9(no3)", 0},
    {"4-7-3",   "maj9(no5)", 0},
    {"4-3-3-5", "7(#9)", 0},
    {"3-4-3-4-3", "m11", 0},
    {"4-3-3-4-3", "11", 0}
  }
};

const char *const notes[LANGUAGE_LAST][NOTE_REPRESENTATION_LAST][12] = {
  [PORTUGUESE] = {
    {"Dó", "Dó#", "Ré", "Ré#", "Mi", "Fá", "Fá#", "Sol", "Sol#", "Lá", "Lá#", "Si"},
    {"Dó", "Réb", "Ré", "Mib", "Mi", "Fá", "Solb", "Sol", "Láb", "Lá", "Sib", "Si"}
  },
  [ENGLISH] = {
    {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"},
    {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"}
  },
  [ABBREVIATED_CHORDS] = {
    {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"},
    {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"}
  }
};

bool get_chord_name(Chord *dest, int *intervals, int len, Language lang,
                    NoteRepresentation r) {
  if (len < 2)
    return false;

  char chord_signature[CHORD_SIGNATURE_LENGTH] = {0};
  for (int i = 1; i < len; i++) {
    unsigned char note = abs(intervals[i] - intervals[i - 1]);
    char s[4];
    snprintf(s, 4, "%d", note);
    if (i != 1)
      strncat(chord_signature, "-",
              CHORD_SIGNATURE_LENGTH - strlen(chord_signature) - 1);
    strncat(chord_signature, s,
            CHORD_SIGNATURE_LENGTH - strlen(chord_signature) - 1);
  }

  bool chord_found = false;
  for (int i = 0; i < LENGTH(chords[lang]); i++) {
    const Chord *c = &chords[lang][i];
    const char *root = notes[lang][r][intervals[c->root] % 12];
    const char *bass = notes[lang][r][intervals[0] % 12];
    char name[strlen(c->name) + strlen(root) + strlen(bass)];
    strcpy(name, c->name);
    if (strncmp(chord_signature, c->signature, CHORD_SIGNATURE_LENGTH - 1) ==
        0) {
      chord_found = true;
      if (len == 2)
        strncpy(dest->name, name, CHORD_NAME_LENGTH);
      else if (lang == ABBREVIATED_CHORDS)
        if (!c->root)
          snprintf(dest->name, CHORD_NAME_LENGTH, "%s%s", root, name);
        else
          snprintf(dest->name, CHORD_NAME_LENGTH, "%s%s/%s", root, name, bass);
      else
        snprintf(dest->name, CHORD_NAME_LENGTH, "%s %s", root, name);
      strncpy(dest->signature, c->signature, CHORD_SIGNATURE_LENGTH);
      dest->root = c->root;
      break;
    }
  }
  return chord_found;
}
