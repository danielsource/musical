#include <stdio.h>

#include "test.h"

int tests_run = 0;

static char *test_foobar() {
  ASSERT("0 is not equal to 1", 0 == 1);
  return 0;
}

static char *all_tests() {
  RUN_TEST(test_foobar);
  RUN_TEST(test_foobar);
  RUN_TEST(test_foobar);
  RUN_TEST(test_foobar);
  return 0;
}

int main(void) {
  RUN_TEST_SUITE(all_tests);
  return 0;
}
