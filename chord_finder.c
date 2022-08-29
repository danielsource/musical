#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"
#include "util.h"
#include "chord_finder.h"

const Chord chords[LANGUAGE_LAST][74] =
    {[PORTUGUESE] =
         {{"0", "Primeira Justa", {0}, 0},
          {"1", "Segunda Menor", "(Semitom)", 0},
          {"2", "Segunda Maior", "(Tom)", 0},
          {"3", "Terceira Menor", {0}, 0},
          {"4", "Terceira Maior", {0}, 0},
          {"5", "Quarta Justa", {0}, 0},
          {"6", "Quinta Diminuta", "(Trítono)", 0},
          {"7", "Quinta Justa", {0}, 0},
          {"8", "Sexta Menor", {0}, 0},
          {"9", "Sexta Maior", {0}, 0},
          {"10", "Sétima Menor", {0}, 0},
          {"11", "Sétima Maior", {0}, 0},
          {"12", "Oitava Justa", {0}, 0},
          {"13", "Oitava Aumentada", {0}, 0},
          {"3-3", "Diminuta", {0}, 0},
          {"3-6", "Diminuta", "(Primeira Inversão)", 2},
          {"6-3", "Diminuta", "(Segunda Inversão)", 1},
          {"4-4", "Aumentada", {0}, 0},
          {"3-4", "Menor", {0}, 0},
          {"4-5", "Menor", "(Primeira Inversão)", 2},
          {"5-3", "Menor", "(Segunda Inversão)", 1},
          {"4-3", "Maior", {0}, 0},
          {"3-5", "Maior", "(Primeira Inversão)", 2},
          {"5-4", "Maior", "(Segunda Inversão)", 1},
          {"4-3-4", "Maior com Sétima", {0}, 0},
          {"3-4-1", "Maior com Sétima", "(Primeira Inversão)", 3},
          {"4-1-4", "Maior com Sétima", "(Segunda Inversão)", 2},
          {"1-4-3", "Maior com Sétima", "(Terceira Inversão)", 1},
          {"4-3-3", "Maior com Sétima Dominante", {0}, 0},
          {"3-3-2", "Maior com Sétima Dominante", "(Primeira Inversão)", 3},
          {"3-2-4", "Maior com Sétima Dominante", "(Segunda Inversão)", 2},
          {"2-4-3", "Maior com Sétima Dominante", "(Terceira Inversão)", 1},
          {"3-4-3", "Menor com Sétima", {0}, 0},
          {"4-3-2", "Menor com Sétima", "(Primeira Inversão)", 3},
          {"3-2-3", "Menor com Sétima", "(Segunda Inversão)", 2},
          {"2-3-4", "Menor com Sétima", "(Terceira Inversão)", 1},
          {"3-3-4", "Meio-Diminuto com Sétima", {0}, 0},
          {"3-4-2", "Meio-Diminuto com Sétima", "(Primeira Inversão)", 3},
          {"4-2-3", "Meio-Diminuto com Sétima", "(Segunda Inversão)", 2},
          {"2-3-3", "Meio-Diminuto com Sétima", "(Terceira Inversão)", 1},
          {"3-3-3", "Diminuto com Sétima", {0}, 0},
          {"3-4-4", "Menor-Maior com Sétima", {0}, 0},
          {"4-4-1", "Menor-Maior com Sétima", "(Primeira Inversão)", 3},
          {"4-1-3", "Menor-Maior com Sétima", "(Segunda Inversão)", 2},
          {"1-3-4", "Menor-Maior com Sétima", "(Terceira Inversão)", 1},
          {"4-4-3", "Aumentado com Sétima Maior", {0}, 0},
          {"4-3-1", "Aumentado com Sétima Maior", "(Primeira Inversão)", 3},
          {"3-1-4", "Aumentado com Sétima Maior", "(Segunda Inversão)", 2},
          {"1-4-4", "Aumentado com Sétima Maior", "(Terceira Inversão)", 1},
          {"4-4-2", "Aumentado", {0}, 0},
          {"4-2-2", "Aumentado", "(Primeira Inversão)", 3},
          {"2-2-4", "Aumentado", "(Segunda Inversão)", 2},
          {"2-4-4", "Aumentado", "(Terceira Inversão)", 1},
          {"2-5", "sus2", {0}, 0},
          {"7-7-2", "sus2", {0}, 0},
          {"7-7-1", "sus2 Menor", {0}, 0},
          {"5-2", "sus4", {0}, 0},
          {"5-2-5", "sus4", {0}, 0},
          {"2-2-3", "add2", {0}, 0},
          {"2-3-5", "add2", "(Nona como Tônica)", 3},
          {"3-5-2", "add2", "(Primeira Inversão)", 2},
          {"5-2-2", "add2", "(Segunda Inversão)", 1},
          {"4-3-4-3", "Maior com Nona", {0}, 0},
          {"7-4-3", "Maior com Nona", "(Faltando Terça)", 0},
          {"4-7-3", "Maior com Nona", "(Faltando Quinta)", 0},
          {"4-3-3-4", "Maior com Nona Dominante", {0}, 0},
          {"7-3-4", "Maior com Nona Dominante", "(Faltando Terça)", 0},
          {"4-6-4", "Maior com Nona Dominante", "(Faltando Quinta)", 0},
          {"4-3-3-3", "Menor com Nona Dominante", {0}, 0},
          {"7-3-3", "Menor com Nona Dominante", "(Faltando Terça)", 0},
          {"4-6-3", "Menor com Nona Dominante", "(Faltando Quinta)", 0},
          {"3-4-3-3", "Menor com Nona", {0}, 0},
          {"3-7-3", "Menor com Nona", "(Faltando Quinta)", 0},
          {"4-3-3-5", "Maior com Sétima Dominante e Nona Sustenido", 0}},
     [ENGLISH] = {{"0", "Perfect Unison", {0}, 0},
                  {"1", "Minor Second", "(Semitone)", 0},
                  {"2", "Major Second", "(Tone)", 0},
                  {"3", "Minor Third", {0}, 0},
                  {"4", "Major Third", {0}, 0},
                  {"5", "Perfect Fourth", {0}, 0},
                  {"6", "Diminished Fifth", "(Tritone)", 0},
                  {"7", "Perfect Fifth", {0}, 0},
                  {"8", "Minor Sixth", {0}, 0},
                  {"9", "Major Sixth", {0}, 0},
                  {"10", "Minor Seventh", {0}, 0},
                  {"11", "Major Seventh", {0}, 0},
                  {"12", "Perfect Octave", {0}, 0},
                  {"13", "Augmented Octave", {0}, 0},
                  {"3-3", "Diminished", {0}, 0},
                  {"3-6", "Diminished", "(First Inversion)", 2},
                  {"6-3", "Diminished", "(Second Inversion)", 1},
                  {"4-4", "Augmented", {0}, 0},
                  {"3-4", "Minor", {0}, 0},
                  {"4-5", "Minor", "(First Inversion)", 2},
                  {"5-3", "Minor", "(Second Inversion)", 1},
                  {"4-3", "Major", {0}, 0},
                  {"3-5", "Major", "(First Inversion)", 2},
                  {"5-4", "Major", "(Second Inversion)", 1},
                  {"4-3-4", "Major 7", {0}, 0},
                  {"3-4-1", "Major 7", "(First Inversion)", 3},
                  {"4-1-4", "Major 7", "(Second Inversion)", 2},
                  {"1-4-3", "Major 7", "(Third Inversion)", 1},
                  {"4-3-3", "Dominant 7", {0}, 0},
                  {"3-3-2", "Dominant 7", "(First Inversion)", 3},
                  {"3-2-4", "Dominant 7", "(Second Inversion)", 2},
                  {"2-4-3", "Dominant 7", "(Third Inversion)", 1},
                  {"3-4-3", "Minor 7", {0}, 0},
                  {"4-3-2", "Minor 7", "(First Inversion)", 3},
                  {"3-2-3", "Minor 7", "(Second Inversion)", 2},
                  {"2-3-4", "Minor 7", "(Third Inversion)", 1},
                  {"3-3-4", "Half-Diminished 7", {0}, 0},
                  {"3-4-2", "Half-Diminished 7", "(First Inversion)", 3},
                  {"4-2-3", "Half-Diminished 7", "(Second Inversion)", 2},
                  {"2-3-3", "Half-Diminished 7", "(Third Inversion)", 1},
                  {"3-3-3", "Diminished 7", {0}, 0},
                  {"3-4-4", "Minor-Major 7", {0}, 0},
                  {"4-4-1", "Minor-Major 7", "(First Inversion)", 3},
                  {"4-1-3", "Minor-Major 7", "(Second Inversion)", 2},
                  {"1-3-4", "Minor-Major 7", "(Third Inversion)", 1},
                  {"4-4-3", "Augmented Major 7", {0}, 0},
                  {"4-3-1", "Augmented Major 7", "(First Inversion)", 3},
                  {"3-1-4", "Augmented Major 7", "(Second Inversion)", 2},
                  {"1-4-4", "Augmented Major 7", "(Third Inversion)", 1},
                  {"4-4-2", "Augmented 7", {0}, 0},
                  {"4-2-2", "Augmented 7", "(First Inversion)", 3},
                  {"2-2-4", "Augmented 7", "(Second Inversion)", 2},
                  {"2-4-4", "Augmented 7", "(Third Inversion)", 1},
                  {"2-5", "sus2", {0}, 0},
                  {"7-7-2", "sus2", {0}, 0},
                  {"7-7-1", "Minor sus2", {0}, 0},
                  {"5-2", "sus4", {0}, 0},
                  {"5-2-5", "sus4", {0}, 0},
                  {"2-2-3", "add2", {0}, 0},
                  {"2-3-5", "add2", "(Ninth as Root)", 3},
                  {"3-5-2", "add2", "(First Inversion)", 2},
                  {"5-2-2", "add2", "(Second Inversion)", 1},
                  {"4-3-4-3", "Major 9th", {0}, 0},
                  {"7-4-3", "Major 9th", "(Missing 3rd)", 0},
                  {"4-7-3", "Major 9th", "(Missing 5th)", 0},
                  {"4-3-3-4", "Dominant 9th", {0}, 0},
                  {"7-3-4", "Dominant 9th", "(Missing 3rd)", 0},
                  {"4-6-4", "Dominant 9th", "(Missing 5th)", 0},
                  {"4-3-3-3", "Dominant Minor 9th", {0}, 0},
                  {"7-3-3", "Dominant Minor 9th", "(Missing 3rd)", 0},
                  {"4-6-3", "Dominant Minor 9th", "(Missing 5th)", 0},
                  {"3-4-3-3", "Minor 9th", {0}, 0},
                  {"3-7-3", "Minor 9th", "(Missing 5th)", 0},
                  {"4-3-3-5", "Dominant 7 Sharp 9th", {0}, 0}}};

const char *const notes[LANGUAGE_LAST][12] = {
    [PORTUGUESE] = {"Dó", "Dó#", "Ré", "Ré#", "Mi", "Fá", "Fá#", "Só", "Só#",
                    "Lá", "Lá#", "Sí"},
    [ENGLISH] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#",
                 "B"}};

bool get_chord_name(Chord *dest, int *intervals, size_t len, Language lang) {
  if (len <= 1) return false;

  char chord_signature[CHORD_SIGNATURE_LENGTH] = {0};
  for (int i = 1; i < len; i++) {
    int note = abs(intervals[i] - intervals[i - 1]);
    char s[3];
    snprintf(s, 3, "%d", note);
    if (i != 1) strncat(chord_signature, "-", CHORD_SIGNATURE_LENGTH - 1);
    strncat(chord_signature, s, CHORD_SIGNATURE_LENGTH - 1);
  }

  bool chord_found = false;
  for (int i = 0; i < LENGTH(chords[lang]); i++) {
    if (strncmp(chord_signature, chords[lang][i].signature,
                CHORD_SIGNATURE_LENGTH - 1) == 0) {
      chord_found = true;
      snprintf(dest->name, CHORD_NAME_LENGTH, "%s %s", notes[lang][intervals[chords[lang][i].root] % 12],
               chords[lang][i].name);
      strncat(dest->parenthesis, chords[lang][i].parenthesis,
              CHORD_PARENTHESIS_LENGTH - 1);
      strncat(dest->signature, chords[lang][i].signature,
              CHORD_SIGNATURE_LENGTH - 1);
      dest->root = chords[lang][i].root;
      break;
    }
  }
  return chord_found;
}
