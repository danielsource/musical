#include "util.h"

#define ASSERT(message, test)    \
  do {                           \
    if (!(test)) return message; \
  } while (0)

#define RUN_TEST(test)                                                         \
  do {                                                                         \
    puts(ANSI_COLOR_WHITE ANSI_COLOR_BOLD "RUNNING" ANSI_COLOR_RESET           \
                                          ": " #test);                         \
    char *message = test();                                                    \
    tests_run++;                                                               \
    if (message) {                                                             \
      printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "FAIL" ANSI_COLOR_RESET ": " #test \
                                            ": ");                             \
      return message;                                                          \
    } else                                                                     \
      puts(ANSI_COLOR_GREEN ANSI_COLOR_BOLD "OK: " ANSI_COLOR_RESET #test);    \
  } while (0)

#define RUN_TEST_SUITE(run_all_tests_func)                               \
  do {                                                                   \
    puts(ANSI_COLOR_WHITE ANSI_COLOR_BOLD "TEST SUITE" ANSI_COLOR_RESET  \
                                          ": " __FILE__);                \
    char *result = run_all_tests_func();                                 \
    if (result)                                                          \
      puts(result);                                                      \
    else                                                                 \
      puts(ANSI_COLOR_GREEN ANSI_COLOR_BOLD                              \
           "[ALL TESTS PASSED]" ANSI_COLOR_RESET);                       \
    printf(ANSI_COLOR_WHITE ANSI_COLOR_BOLD "TESTS RUN" ANSI_COLOR_RESET \
                                            ": %d\n\n",                  \
           tests_run);                                                   \
    return result != 0;                                                  \
  } while (0)

extern int tests_run;
