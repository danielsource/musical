#include <stdio.h>
#include <stdbool.h>

#include "test.h"

int tests_run = 0;

static bool y_or_n(const char *prompt, bool y_default) {
  int c = 0;
  while (c != EOF) {
    printf("%s [%s]: ", prompt, y_default ? "Y/n" : "y/N");
    c = getchar();
    if (c == 'y' || c == 'Y' || (c == '\n' && y_default))
      return true;
    else if (c == 'n' || c == 'N' || (c == '\n' && !y_default))
      return false;
    while (getchar() != '\n'); // clear buffer
  }
}

static char *test_chords() {
  ASSERT("You pressed [n]", y_or_n("????", true));
  return 0;
}

static char *all_tests() {
  RUN_TEST(test_chords);
  return 0;
}

int main(void) {
  RUN_TEST_SUITE(all_tests);
  return 0;
}
