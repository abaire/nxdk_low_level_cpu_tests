#ifndef XBOX
#error Must be built with nxdk
#endif

#include "test_runner.h"
#include "test_suite.h"
#include "testfloat_tests/test_testfloat_extf80.h"
#include "testfloat_tests/test_testfloat_f32.h"
#include "testfloat_tests/test_testfloat_f64.h"

static const char* kDefaultLogPath =
    R"(e:\devkit\nxdk_low_level_cpu_tests\log_testfloat.json)";

static constexpr TestCase kTests[] = {
    TestCase::From<TestFloatF32>(),
    TestCase::From<TestFloatF64>(),
    TestCase::From<TestFloatExtF80>(),
};

int main() {
  TestRunner runner("TestFloat", kDefaultLogPath, kTests, std::size(kTests));
  return runner.Run();
}
