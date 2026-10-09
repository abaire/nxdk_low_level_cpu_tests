#include "test_fpu_basic_arithmetic.h"

#include "fpu_test_common.h"

bool TestFpuBasicArithmetic::Test() {
  LogMsg("\n============================================================\n");
  LogMsg("Test: FPU Basic Arithmetic (placeholder)\n");

  bool pass = true;

  double sum = 0.0;
  for (int i = 1; i <= 1000; ++i) {
    sum += static_cast<double>(i);
  }
  if (sum == 500500.0) {
    LogMsg("  [PASS] double sum(1..1000) == 500500.0 (got %f)\n", sum);
  } else {
    LogMsg("  [FAIL] double sum(1..1000) != 500500.0 (got %f)\n", sum);
    pass = false;
  }

  double factorial = 1.0;
  for (int i = 2; i <= 10; ++i) {
    factorial *= static_cast<double>(i);
  }
  if (factorial == 3628800.0) {
    LogMsg("  [PASS] double 10! == 3628800.0 (got %f)\n", factorial);
  } else {
    LogMsg("  [FAIL] double 10! != 3628800.0 (got %f)\n", factorial);
    pass = false;
  }

  float float_sum = 0.0f;
  for (int i = 1; i <= 10; ++i) {
    float_sum += static_cast<float>(i);
  }
  if (float_sum == 55.0f) {
    LogMsg("  [PASS] float sum(1..10) == 55.0 (got %f)\n",
           static_cast<double>(float_sum));
  } else {
    LogMsg("  [FAIL] float sum(1..10) != 55.0 (got %f)\n",
           static_cast<double>(float_sum));
    pass = false;
  }

  return pass;
}
