#include "test_fpu_basic_arithmetic.h"

#include "fpu_test_common.h"

bool TestFpuBasicArithmetic::Test() {
  bool pass = true;

  double sum = 0.0;
  for (int i = 1; i <= 1000; ++i) {
    sum += static_cast<double>(i);
  }
  if (sum == 500500.0) {
    TEST_PASS("double sum(1..1000) == 500500.0 (got %f)", sum);
  } else {
    TEST_FAIL_EX(TestFields("expected", 500500.0, "actual", sum),
                 "double sum(1..1000) != 500500.0 (got %f)", sum);
    pass = false;
  }

  double factorial = 1.0;
  for (int i = 2; i <= 10; ++i) {
    factorial *= static_cast<double>(i);
  }
  if (factorial == 3628800.0) {
    TEST_PASS("double 10! == 3628800.0 (got %f)", factorial);
  } else {
    TEST_FAIL_EX(TestFields("expected", 3628800.0, "actual", factorial),
                 "double 10! != 3628800.0 (got %f)", factorial);
    pass = false;
  }

  float float_sum = 0.0f;
  for (int i = 1; i <= 10; ++i) {
    float_sum += static_cast<float>(i);
  }
  if (float_sum == 55.0f) {
    TEST_PASS("float sum(1..10) == 55.0 (got %f)",
              static_cast<double>(float_sum));
  } else {
    TEST_FAIL_EX(
        TestFields("expected", 55.0, "actual", static_cast<double>(float_sum)),
        "float sum(1..10) != 55.0 (got %f)", static_cast<double>(float_sum));
    pass = false;
  }

  return pass;
}
