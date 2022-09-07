#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

#include "../src/language.h"
#include "../src/util.h"
#include "../src/chord_finder.h"

int tests_run = 0;

static bool y_or_n(const char *prompt, bool y_default) {
  size_t len = strlen(prompt) + strlen(" [Y/n]: ") + 1;
  char s[len];
  snprintf(s, len, "%s [%s]: ", prompt, y_default ? "Y/n" : "y/N");
  int c = 0;
  while (c != EOF) {
    printf(s);
    c = getchar();
    if (c == 'y' || c == 'Y' || (c == '\n' && y_default))
      return true;
    else if (c == 'n' || c == 'N' || (c == '\n' && !y_default))
      return false;
    while (getchar() != '\n')
      ;  // clear buffer
  }
  return false;
}

static char *test_signatures_and_roots() {
  bool same = true;
  for (int i = 0; i < LENGTH(*chords); i++) {
    for (int j = 1; j < LANGUAGE_LAST; j++) {
      const Chord *prev_chord = &chords[j - 1][i];
      const Chord *chord = &chords[j][i];
      if (strcmp(prev_chord->signature, chord->signature) != 0 ||
          prev_chord->root != chord->root) {
        same = false;
        printf("%s|%s|%d != %s|%s|%d\n", chord->name, chord->signature,
               chord->root, prev_chord->name, prev_chord->signature,
               prev_chord->root);
      }
    }
  }
  ASSERT("These chords don't have the same signatures", same);
  return 0;
}

static char *test_chord_names() {
  for (int i = 0; i < LENGTH(*chords); i++) {
    for (int j = 0; j < LANGUAGE_LAST; j++) {
      const Chord *chord = &chords[j][i];
      const char *name = chord->name;
      if (!*name)
        name = "EMPTY";
      if (j != LANGUAGE_LAST - 1)
        printf("%s, ", name);
      else
        printf("%s", name);
    }
    putchar('\n');
  }
  ASSERT("These chords don't look right.",
         y_or_n("Do these chords look right?", true));
  return 0;
}

static char *all_tests() {
  RUN_TEST(test_signatures_and_roots);
  RUN_TEST(test_chord_names);
  return 0;
}

int main(void) {
  RUN_TEST_SUITE(all_tests);
  return 0;
}
