#ifndef XBOX
#error Must be built with nxdk
#endif

#include "sse_tests/sse_runner_case.h"
#include "sse_tests/sse_test_common.h"
#include "sse_tests/test_sse_approximations.h"
#include "sse_tests/test_sse_comparisons.h"
#include "sse_tests/test_sse_min_max.h"
#include "sse_tests/test_sse_mxcsr_ftz.h"
#include "test_runner.h"
#include "test_suite.h"

static const char* kDefaultLogPath =
    R"(e:\devkit\nxdk_low_level_cpu_tests\log_sse.json)";

static constexpr TestCase kTests[] = {
    TestCase::From<TestSseMinMax>(),
    TestCase::From<TestSseComparisons>(),
    TestCase::From<TestSseApproximations>(),
    TestCase::From<TestSseMxcsrFtz>(),
};

int main() {
  TestRunner runner("SSE", kDefaultLogPath, kTests, std::size(kTests));
  return runner.Run();
}
