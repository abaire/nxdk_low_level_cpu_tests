#ifndef NXDK_LOW_LEVEL_CPU_TESTS_SSE_TEST_COMMON_H
#define NXDK_LOW_LEVEL_CPU_TESTS_SSE_TEST_COMMON_H

#include <cstdint>
#include <deque>
#include <string>

void LogMsg(const char* fmt, ...);
const std::deque<std::string>& GetOnScreenLog();
void ClearOnScreenLog();

inline uint32_t ReadMxcsr() {
  uint32_t mxcsr = 0;
  __asm__ __volatile__("stmxcsr %0" : "=m"(mxcsr));
  return mxcsr;
}

inline void WriteMxcsr(uint32_t mxcsr) {
  __asm__ __volatile__("ldmxcsr %0" : : "m"(mxcsr));
}

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_SSE_TEST_COMMON_H
