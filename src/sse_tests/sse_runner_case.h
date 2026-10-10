#ifndef NXDK_LOW_LEVEL_CPU_TESTS_SSE_RUNNER_CASE_H
#define NXDK_LOW_LEVEL_CPU_TESTS_SSE_RUNNER_CASE_H

#include <cstdint>

struct SSERunnerCase {
  uint32_t a[4];
  uint32_t b[4];
  uint32_t mxcsr_in;
  uint32_t expected[4];
  uint32_t expected_mxcsr;
  uint32_t mxcsr_mask;
  uint32_t expected_eflags;
  uint32_t eflags_mask;
  const char* desc;
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_SSE_RUNNER_CASE_H
