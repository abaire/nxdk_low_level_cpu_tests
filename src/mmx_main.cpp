#ifndef XBOX
#error Must be built with nxdk
#endif

#include "mmx_tests/mmx_test_common.h"
#include "mmx_tests/test_mmx_arithmetic.h"
#include "mmx_tests/test_mmx_extensions.h"
#include "mmx_tests/test_mmx_logical_compare_shift.h"
#include "mmx_tests/test_mmx_pack_unpack.h"
#include "mmx_tests/test_mmx_x87_aliasing.h"
#include "test_runner.h"
#include "test_suite.h"

static const char* kDefaultLogPath =
    R"(e:\devkit\nxdk_low_level_cpu_tests\log_mmx.json)";

static constexpr TestCase kTests[] = {
    TestCase::From<TestMmxArithmetic>(),
    TestCase::From<TestMmxLogicalCompareShift>(),
    TestCase::From<TestMmxPackUnpack>(),
    TestCase::From<TestMmxExtensions>(),
    TestCase::From<TestMmxX87Aliasing>(),
};

int main() {
  TestRunner runner("MMX", kDefaultLogPath, kTests, std::size(kTests));
  return runner.Run();
}
