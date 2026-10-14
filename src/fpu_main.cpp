#ifndef XBOX
#error Must be built with nxdk
#endif

#include "fpu_tests/fpu_test_common.h"
#include "fpu_tests/test_fpu_basic_arithmetic.h"
#include "fpu_tests/test_fpu_x87_precision_control.h"
#include "test_runner.h"
#include "test_suite.h"

static const char* kDefaultLogPath =
    R"(e:\devkit\nxdk_low_level_cpu_tests\log_fpu.json)";

static constexpr TestCase kTests[] = {
    TestCase::From<TestFpuBasicArithmetic>(),
    TestCase::From<TestFpuX87PrecisionControl>(),
};

int main() {
  TestRunner runner("FPU", kDefaultLogPath, kTests, std::size(kTests));
  return runner.Run();
}
