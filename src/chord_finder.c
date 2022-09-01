#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"
#include "util.h"
#include "chord_finder.h"

const Chord chords[LANGUAGE_LAST][72] = {
    [PORTUGUESE] = {
        {"0",       "primeira justa", {0}, 0},
        {"1",       "segunda menor", "(semitom)", 0},
        {"2",       "segunda maior", "(tom)", 0},
        {"3",       "terça menor", {0}, 0},
        {"4",       "terça maior", {0}, 0},
        {"5",       "quarta justa", {0}, 0},
        {"6",       "quinta diminuta", "(trítono)", 0},
        {"7",       "quinta justa", {0}, 0},
        {"8",       "sexta menor", {0}, 0},
        {"9",       "sexta maior", {0}, 0},
        {"10",      "sétima menor", {0}, 0},
        {"11",      "sétima maior", {0}, 0},
        {"12",      "oitava justa", {0}, 0},
        {"13",      "oitava aumentada", {0}, 0},
        {"3-3",     "diminuto", {0}, 0},
        {"3-6",     "diminuto", "(primeira inversão)", 2},
        {"6-3",     "diminuto", "(segunda inversão)", 1},
        {"4-4",     "aumentado", {0}, 0},
        {"3-4",     "menor", {0}, 0},
        {"4-5",     "menor", "(primeira inversão)", 2},
        {"5-3",     "menor", "(segunda inversão)", 1},
        {"4-3",     "maior", {0}, 0},
        {"3-5",     "maior", "(primeira inversão)", 2},
        {"5-4",     "maior", "(segunda inversão)", 1},
        {"4-3-4",   "maior com sétima maior", {0}, 0},
        {"3-4-1",   "maior com sétima maior", "(primeira inversão)", 3},
        {"4-1-4",   "maior com sétima maior", "(segunda inversão)", 2},
        {"1-4-3",   "maior com sétima maior", "(terceira inversão)", 1},
        {"4-3-3",   "maior com sétima dominante", {0}, 0},
        {"3-3-2",   "maior com sétima dominante", "(primeira inversão)", 3},
        {"3-2-4",   "maior com sétima dominante", "(segunda inversão)", 2},
        {"2-4-3",   "maior com sétima dominante", "(terceira inversão)", 1},
        {"3-4-3",   "menor com sétima", {0}, 0},
        {"4-3-2",   "menor com sétima", "(primeira inversão)", 3},
        {"3-2-3",   "menor com sétima", "(segunda inversão)", 2},
        {"2-3-4",   "menor com sétima", "(terceira inversão)", 1},
        {"3-3-4",   "meio-diminuto", {0}, 0},
        {"3-4-2",   "meio-diminuto", "(primeira inversão)", 3},
        {"4-2-3",   "meio-diminuto", "(segunda inversão)", 2},
        {"2-3-3",   "meio-diminuto", "(terceira inversão)", 1},
        {"3-3-3",   "diminuto com sétima", {0}, 0},
        {"3-4-4",   "menor-maior com sétima", {0}, 0},
        {"4-4-1",   "menor-maior com sétima", "(primeira inversão)", 3},
        {"4-1-3",   "menor-maior com sétima", "(segunda inversão)", 2},
        {"1-3-4",   "menor-maior com sétima", "(terceira inversão)", 1},
        {"4-4-3",   "aumentado com sétima maior", {0}, 0},
        {"4-3-1",   "aumentado com sétima maior", "(primeira inversão)", 3},
        {"3-1-4",   "aumentado com sétima maior", "(segunda inversão)", 2},
        {"1-4-4",   "aumentado com sétima maior", "(terceira inversão)", 1},
        {"4-4-2",   "aumentado com sétima menor", {0}, 0},
        {"4-2-2",   "aumentado com sétima menor", "(primeira inversão)", 3},
        {"2-2-4",   "aumentado com sétima menor", "(segunda inversão)", 2},
        {"2-4-4",   "aumentado com sétima menor", "(terceira inversão)", 1},
        {"2-5",     "segunda suspensa", {0}, 0},
        {"5-5",     "segunda suspensa", "(primeira inversão)", 2},
        {"5-2",     "quarta suspensa", {0}, 0},
        {"2-2-3",   "maior com nona adicionada", {0}, 0},
        {"2-3-5",   "maior com nona adicionada", "(nona como tônica)", 3},
        {"3-5-2",   "maior com nona adicionada", "(primeira inversão)", 2},
        {"5-2-2",   "maior com nona adicionada", "(segunda inversão)", 1},
        {"4-3-4-3", "maior com nona dominante", {0}, 0},
        {"7-4-3",   "maior com nona dominante", "(faltando terça)", 0},
        {"4-7-3",   "maior com nona dominante", "(faltando quinta)", 0},
        {"4-3-3-4", "dominante com nona maior", {0}, 0},
        {"7-3-4",   "dominante com nona maior", "(faltando terça)", 0},
        {"4-6-4",   "dominante com nona maior", "(faltando quinta)", 0},
        {"4-3-3-3", "dominante com nona menor", {0}, 0},
        {"7-3-3",   "dominante com nona menor", "(faltando terça)", 0},
        {"4-6-3",   "dominante com nona menor", "(faltando quinta)", 0},
        {"3-4-3-3", "menor com nona", {0}, 0},
        {"3-7-3",   "menor com nona", "(faltando quinta)", 0},
        {"4-3-3-5", "dominante com nona sustenido", {0}, 0}},
    [ENGLISH] = {
        {"0",       "perfect unison", {0}, 0},
        {"1",       "minor second", "(semitone)", 0},
        {"2",       "major second", "(tone)", 0},
        {"3",       "minor third", {0}, 0},
        {"4",       "major third", {0}, 0},
        {"5",       "perfect fourth", {0}, 0},
        {"6",       "diminished fifth", "(tritone)", 0},
        {"7",       "perfect fifth", {0}, 0},
        {"8",       "minor sixth", {0}, 0},
        {"9",       "major sixth", {0}, 0},
        {"10",      "minor seventh", {0}, 0},
        {"11",      "major seventh", {0}, 0},
        {"12",      "perfect octave", {0}, 0},
        {"13",      "augmented octave", {0}, 0},
        {"3-3",     "diminished", {0}, 0},
        {"3-6",     "diminished", "(first inversion)", 2},
        {"6-3",     "diminished", "(second inversion)", 1},
        {"4-4",     "augmented", {0}, 0},
        {"3-4",     "minor", {0}, 0},
        {"4-5",     "minor", "(first inversion)", 2},
        {"5-3",     "minor", "(second inversion)", 1},
        {"4-3",     "major", {0}, 0},
        {"3-5",     "major", "(first inversion)", 2},
        {"5-4",     "major", "(second inversion)", 1},
        {"4-3-4",   "major seventh", {0}, 0},
        {"3-4-1",   "major seventh", "(first inversion)", 3},
        {"4-1-4",   "major seventh", "(second inversion)", 2},
        {"1-4-3",   "major seventh", "(third inversion)", 1},
        {"4-3-3",   "dominant seventh", {0}, 0},
        {"3-3-2",   "dominant seventh", "(first inversion)", 3},
        {"3-2-4",   "dominant seventh", "(second inversion)", 2},
        {"2-4-3",   "dominant seventh", "(third inversion)", 1},
        {"3-4-3",   "minor seventh", {0}, 0},
        {"4-3-2",   "minor seventh", "(first inversion)", 3},
        {"3-2-3",   "minor seventh", "(second inversion)", 2},
        {"2-3-4",   "minor seventh", "(third inversion)", 1},
        {"3-3-4",   "half-diminished", {0}, 0},
        {"3-4-2",   "half-diminished", "(first inversion)", 3},
        {"4-2-3",   "half-diminished", "(second inversion)", 2},
        {"2-3-3",   "half-diminished", "(third inversion)", 1},
        {"3-3-3",   "diminished seventh", {0}, 0},
        {"3-4-4",   "minor-major seventh", {0}, 0},
        {"4-4-1",   "minor-major seventh", "(first inversion)", 3},
        {"4-1-3",   "minor-major seventh", "(second inversion)", 2},
        {"1-3-4",   "minor-major seventh", "(third inversion)", 1},
        {"4-4-3",   "augmented major seventh", {0}, 0},
        {"4-3-1",   "augmented major seventh", "(first inversion)", 3},
        {"3-1-4",   "augmented major seventh", "(second inversion)", 2},
        {"1-4-4",   "augmented major seventh", "(third inversion)", 1},
        {"4-4-2",   "augmented seventh", {0}, 0},
        {"4-2-2",   "augmented seventh", "(first inversion)", 3},
        {"2-2-4",   "augmented seventh", "(second inversion)", 2},
        {"2-4-4",   "augmented seventh", "(third inversion)", 1},
        {"2-5",     "suspended second", {0}, 0},
        {"5-5",     "suspended second", "(first inversion)", 2},
        {"5-2",     "suspended fourth", {0}, 0},
        {"2-2-3",   "major added-ninth", {0}, 0},
        {"2-3-5",   "major added-ninth", "(ninth as root)", 3},
        {"3-5-2",   "major added-ninth", "(first inversion)", 2},
        {"5-2-2",   "major added-ninth", "(second inversion)", 1},
        {"4-3-4-3", "major ninth", {0}, 0},
        {"7-4-3",   "major ninth", "(missing third)", 0},
        {"4-7-3",   "major ninth", "(missing fifth)", 0},
        {"4-3-3-4", "dominant ninth", {0}, 0},
        {"7-3-4",   "dominant ninth", "(missing third)", 0},
        {"4-6-4",   "dominant ninth", "(missing fifth)", 0},
        {"4-3-3-3", "dominant minor ninth", {0}, 0},
        {"7-3-3",   "dominant minor ninth", "(missing third)", 0},
        {"4-6-3",   "dominant minor ninth", "(missing fifth)", 0},
        {"3-4-3-3", "minor ninth", {0}, 0},
        {"3-7-3",   "minor ninth", "(missing fifth)", 0},
        {"4-3-3-5", "dominant seventh sharp ninth", {0}, 0}},
    [ABBREVIATED_CHORDS] = {
        {"0",       "P1", {0}, 0},
        {"1",       "m2", {0}, 0},
        {"2",       "M2", {0}, 0},
        {"3",       "m3", {0}, 0},
        {"4",       "M3", {0}, 0},
        {"5",       "P4", {0}, 0},
        {"6",       "d5", {0}, 0},
        {"7",       "P5", {0}, 0},
        {"8",       "m6", {0}, 0},
        {"9",       "M6", {0}, 0},
        {"10",      "m7", {0}, 0},
        {"11",      "M7", {0}, 0},
        {"12",      "P8", {0}, 0},
        {"13",      "A8", {0}, 0},
        {"3-3",     "dim", {0}, 0},
        {"3-6",     "dim", {0}, 2},
        {"6-3",     "dim", {0}, 1},
        {"4-4",     "aug", {0}, 0},
        {"3-4",     "m", {0}, 0},
        {"4-5",     "m", {0}, 2},
        {"5-3",     "m", {0}, 1},
        {"4-3",     "", {0}, 0},
        {"3-5",     "", {0}, 2},
        {"5-4",     "", {0}, 1},
        {"4-3-4",   "M7", {0}, 0},
        {"3-4-1",   "M7", {0}, 3},
        {"4-1-4",   "M7", {0}, 2},
        {"1-4-3",   "M7", {0}, 1},
        {"4-3-3",   "7", {0}, 0},
        {"3-3-2",   "7", {0}, 3},
        {"3-2-4",   "7", {0}, 2},
        {"2-4-3",   "7", {0}, 1},
        {"3-4-3",   "m7", {0}, 0},
        {"4-3-2",   "m7", {0}, 3},
        {"3-2-3",   "m7", {0}, 2},
        {"2-3-4",   "m7", {0}, 1},
        {"3-3-4",   "m7(b5)", {0}, 0},
        {"3-4-2",   "m7(b5)", {0}, 3},
        {"4-2-3",   "m7(b5)", {0}, 2},
        {"2-3-3",   "m7(b5)", {0}, 1},
        {"3-3-3",   "dim7", {0}, 0},
        {"3-4-4",   "mM7", {0}, 0},
        {"4-4-1",   "mM7", {0}, 3},
        {"4-1-3",   "mM7", {0}, 2},
        {"1-3-4",   "mM7", {0}, 1},
        {"4-4-3",   "augM7", {0}, 0},
        {"4-3-1",   "augM7", {0}, 3},
        {"3-1-4",   "augM7", {0}, 2},
        {"1-4-4",   "augM7", {0}, 1},
        {"4-4-2",   "aug7", {0}, 0},
        {"4-2-2",   "aug7", {0}, 3},
        {"2-2-4",   "aug7", {0}, 2},
        {"2-4-4",   "aug7", {0}, 1},
        {"2-5",     "sus2", {0}, 0},
        {"5-5",     "sus2", {0}, 2},
        {"5-2",     "sus4", {0}, 0},
        {"2-2-3",   "add9", {0}, 0},
        {"2-3-5",   "add9", {0}, 3},
        {"3-5-2",   "add9", {0}, 2},
        {"5-2-2",   "add9", {0}, 1},
        {"4-3-4-3", "M9", {0}, 0},
        {"7-4-3",   "M9", {0}, 0},
        {"4-7-3",   "M9", {0}, 0},
        {"4-3-3-4", "9", {0}, 0},
        {"7-3-4",   "9", {0}, 0},
        {"4-6-4",   "9", {0}, 0},
        {"4-3-3-3", "7(b9)", {0}, 0},
        {"7-3-3",   "7(b9)", {0}, 0},
        {"4-6-3",   "7(b9)", {0}, 0},
        {"3-4-3-3", "m9", {0}, 0},
        {"3-7-3",   "m9", {0}, 0},
        {"4-3-3-5", "7(#9)", {0}, 0}}};

const char *const notes[LANGUAGE_LAST][NOTE_REPRESENTATION_LAST][12] = {
    [PORTUGUESE] = {
        {"Dó", "Dó#", "Ré", "Ré#", "Mi", "Fá", "Fá#", "Sol", "Sol#", "Lá", "Lá#", "Si"},
        {"Dó", "Réb", "Ré", "Mib", "Mi", "Fá", "Solb", "Sol", "Láb", "Lá", "Sib", "Si"}},
    [ENGLISH] = {
        {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"},
        {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"}},
    [ABBREVIATED_CHORDS] = {
        {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"},
        {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"}}};

bool get_chord_name(Chord *dest, int *intervals, int len, Language lang,
                    NoteRepresentation r) {
  if (len <= 1) return false;

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
    if (strncmp(chord_signature, c->signature, CHORD_SIGNATURE_LENGTH - 1) ==
        0) {
      chord_found = true;
      if (len == 2)
        strncpy(dest->name, c->name, CHORD_NAME_LENGTH);
      else if (lang == ABBREVIATED_CHORDS)
        if (!c->root)
          snprintf(dest->name, CHORD_NAME_LENGTH, "%s%s",
                   notes[lang][r][intervals[c->root] % 12], c->name);
        else
          snprintf(dest->name, CHORD_NAME_LENGTH, "%s%s/%s",
                   notes[lang][r][intervals[c->root] % 12], c->name,
                   notes[lang][r][intervals[0] % 12]);
      else
        snprintf(dest->name, CHORD_NAME_LENGTH, "%s %s",
                 notes[lang][r][intervals[c->root] % 12], c->name);
      strncpy(dest->parenthesis, c->parenthesis, CHORD_PARENTHESIS_LENGTH);
      strncpy(dest->signature, c->signature, CHORD_SIGNATURE_LENGTH);
      dest->root = c->root;
      break;
    }
  }
  return chord_found;
}
