#include "mutest.h"

MUT_TEST(test_addition) {
  MUT_ASSERT("1 + 1 should equal 2", 1 + 1 == 2);
  MUT_PASS();
}

MUT_TEST(test_subtraction) {
  MUT_ASSERT("1 - 1 should equal 0", 1 - 1 == 0);
  MUT_PASS();
}

MUT_TEST(test_float_eq) {
  MUT_ASSERT_FLOAT_EQ(1.0005f, 1.0f, 0.01);
  MUT_PASS();
}

MUT_TEST(test_failure) {
  MUT_FAIL("this will fail");
  MUT_PASS();
}

MUT_TEST(test_eq_failure) {
  MUT_ASSERT_EQ(1 + 1, 3);
  MUT_PASS();
}

MUT_TEST(test_neq_failure) {
  MUT_ASSERT_NEQ(1 + 1, 2);
  MUT_PASS();
}

MUT_TEST(test_str_eq_failure) {
  const char *s = "some other string";
  MUT_ASSERT_STR_EQ(s, "some string");
  MUT_PASS();
}

MUT_TEST(test_float_eq_failure) {
  float f = 1.01f;
  MUT_ASSERT_FLOAT_EQ(f, 1.0f, 0.0001);
  MUT_PASS();
}

MUT_TEST(test_long_computation) {
  double d = 0.0;
  for (size_t i = 0; i < 1000000000ul; ++i) {
    d += (double)i;
  }
  MUT_ASSERT_FLOAT_EQ(d, 499999999067108992.0, 0.01);
  MUT_PASS();
}

int main(int argc, char **argv) {
  MUT_RUN_TEST(test_addition);
  MUT_RUN_TEST(test_subtraction);
  MUT_RUN_TEST(test_float_eq);
  MUT_RUN_TEST(test_failure);
  MUT_RUN_TEST(test_eq_failure);
  MUT_RUN_TEST(test_neq_failure);
  MUT_RUN_TEST(test_str_eq_failure);
  MUT_RUN_TEST(test_float_eq_failure);
  MUT_RUN_TEST(test_long_computation);

  MUT_REPORT(argc, argv);
}
