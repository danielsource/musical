#define ASSERT(message, test)             \
  do {                                    \
    if (!(test)) return "FAIL: " message; \
  } while (0)

#define RUN_TEST(test)           \
  do {                           \
    char *message = test();      \
    tests_run++;                 \
    if (message) return message; \
  } while (0)

#define RUN_TEST_SUITE(run_all_tests_func)  \
  do {                                      \
    puts("TEST SUITE: " __FILE__);          \
    char *result = run_all_tests_func();    \
    if (result)                             \
      puts(result);                         \
    else                                    \
      puts("[ALL TESTS PASSED]");           \
    printf("TESTS RUN: %d\n\n", tests_run); \
    return result != 0;                     \
  } while (0)

extern int tests_run;
