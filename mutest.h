#ifndef MUTEST_H
#define MUTEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MUT_NOCOLOR
#define RED(s) s
#define GREEN(s) s
#define YELLOW(s) s
#define CYAN(s) s
#define MUT_CHECK_MARK "[PASS]"
#define MUT_CROSS_MARK "[FAIL]"
#define MUT_ARROW_RIGHT "->"
#else
#define RED(s) "\x1b[31m" s "\x1b[0m"
#define GREEN(s) "\x1b[32m" s "\x1b[0m"
#define YELLOW(s) "\x1b[33m" s "\x1b[0m"
#define CYAN(s) "\x1b[36m" s "\x1b[0m"
#define MUT_CHECK_MARK "\xE2\x9C\x94"  // ✓
#define MUT_CROSS_MARK "\xE2\x9C\x98"  // ✘
#define MUT_ARROW_RIGHT "\xE2\x9E\x9C" // ➜
#endif

#define MUT_PASS() return 1

static int mut_tests_run = 0;
static int mut_tests_failed = 0;
struct test_def {
  const char *name;
  int passed;
  char *msg;
};
static struct {
  struct test_def *tests;

  int size;
  int capacity;
} mut_test_env = {0};

static void mut_add_test(const char *name) {
  if (mut_test_env.capacity == 0)
    mut_test_env.capacity = 256;
  else
    mut_test_env.capacity *= 2;
  mut_test_env.tests = (struct test_def*)realloc(mut_test_env.tests, mut_test_env.capacity * sizeof(struct test_def));
  mut_test_env.tests[mut_test_env.size].name = name;
  mut_test_env.tests[mut_test_env.size].passed = 0;
  mut_test_env.tests[mut_test_env.size].msg = NULL;
  mut_test_env.size++;
}

#define _MUT_ASSERT_HELPER(_msg, test, ...)                                    \
  do {                                                                         \
    if (!(test)) {                                                             \
      ssize_t sz =                                                             \
          snprintf(NULL, 0, "%s:%d: " _msg, __FILE__, __LINE__, __VA_ARGS__);  \
      char *s = malloc(sz + 1);                                                \
      snprintf(s, sz + 1, "%s:%d: " _msg, __FILE__, __LINE__, __VA_ARGS__);    \
      printf("%s:%d: " RED(MUT_CROSS_MARK) " " _msg "\n", __FILE__, __LINE__,  \
             __VA_ARGS__);                                                     \
      mut_test_env.tests[mut_test_env.size - 1].msg = s;                       \
      return 0;                                                                \
    }                                                                          \
  } while (0)

#define MUT_ASSERT(msg, test)                                      \
  _MUT_ASSERT_HELPER(msg "%s", test, "")

#define MUT_FAIL(msg) MUT_ASSERT(msg, 0)

// Check if condition is true
#define MUT_ASSERT_TRUE(expr) MUT_ASSERT("Expected true: " #expr, (expr))

// Check if condition is false
#define MUT_ASSERT_FALSE(expr) MUT_ASSERT("Expected false: " #expr, !(expr))

// Check equality of integers
#define MUT_ASSERT_EQ(actual, expect) _MUT_ASSERT_HELPER("Expected " #actual " == " #expect " but " #actual " = %d", (actual) == (expect), actual)

// Check inequality of integers
#define MUT_ASSERT_NEQ(actual, expect) MUT_ASSERT("Expected " #actual " != " #expect, (actual) != (expect))

// Compare C strings
#define MUT_ASSERT_STR_EQ(actual, expect)                               \
    MUT_ASSERT("Expected string equality: " #actual " == " #expect, strcmp((actual), (expect)) == 0);

// Null checks
#define MUT_ASSERT_NULL(ptr) MUT_ASSERT("Expected NULL: " #ptr, (ptr) == NULL)
#define MUT_ASSERT_NOT_NULL(ptr)                        \
  MUT_ASSERT("Expected non-NULL: " #ptr, (ptr) != NULL)

// Floating point equality with tolerance
#define MUT_ASSERT_FLOAT_EQ(actual, expect, epsilon)                    \
  _MUT_ASSERT_HELPER("Expected " #actual " ≈ " #expect                  \
                     " ± " #epsilon " but " #actual " = %f",            \
                     fabs((actual) - (expect)) < (epsilon), actual)

#define MUT_TEST(name) int name()

#define MUT_RUN_TEST(test)                                                     \
  do {                                                                         \
    printf("Running " CYAN("%s") "...\n", #test);                              \
    mut_add_test(#test);                                                       \
    mut_tests_run++;                                                           \
    if (test() == 1) {                                                         \
      printf(GREEN(MUT_CHECK_MARK) " %s\n", #test);                            \
      mut_test_env.tests[mut_test_env.size - 1].passed = 1;                    \
    } else {                                                                   \
      mut_tests_failed++;                                                      \
      mut_test_env.tests[mut_test_env.size - 1].passed = 0;                    \
    }                                                                          \
  } while (0)

#define MUT_SHIFT(argc, argv) (argc--,*argv++)

#define MUT_REPORT(argc, argv)                                                 \
  do {                                                                         \
    char *mut_junit_output = NULL;                                             \
    const char *prog = MUT_SHIFT(argc, argv);                                  \
    while (argc) {                                                             \
      char *s = MUT_SHIFT(argc, argv);                                         \
      if (!strcmp(s, "--junit")) {                                             \
        mut_junit_output = MUT_SHIFT(argc, argv);                              \
      }                                                                        \
    }                                                                          \
    printf("\n===== Test Report =====\n");                                     \
    if (mut_tests_failed) {                                                    \
      printf("Tests run: " CYAN("%d") "\n", mut_tests_run);                    \
      printf("Tests failed: " RED("%d") "\n", mut_tests_failed);               \
    } else {                                                                   \
      printf(GREEN("All %d tests passed\n"), mut_tests_run);                   \
    }                                                                          \
    printf("=======================\n");                                       \
    if (mut_junit_output) {                                                    \
      FILE *f = fopen(mut_junit_output, "w");                                  \
      if (f) {                                                                 \
        fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");            \
        fprintf(f, "<testsuite tests=\"%d\" failures=\"%d\">\n",               \
                mut_tests_run, mut_tests_failed);                              \
        for (int i = 0; i < mut_tests_run; ++i) {                              \
          fprintf(f, "  <testcase name=\"%s\">\n",                             \
                  mut_test_env.tests[i].name);                                 \
          if (!mut_test_env.tests[i].passed)                                   \
            fprintf(f, "    <failure>%s</failure>\n", mut_test_env.tests[i].msg); \
          fprintf(f, "  </testcase>\n");                                       \
        }                                                                      \
        fprintf(f, "</testsuite>\n");                                          \
        fclose(f);                                                             \
        printf("JUnit XML report written to %s\n", mut_junit_output);          \
      }                                                                        \
    }                                                                          \
    return mut_tests_failed;                                                   \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif // MUTEST_H
