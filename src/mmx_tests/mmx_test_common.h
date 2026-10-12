#ifndef NXDK_LOW_LEVEL_CPU_TESTS_MMX_TEST_COMMON_H
#define NXDK_LOW_LEVEL_CPU_TESTS_MMX_TEST_COMMON_H

#include <hal/debug.h>
#include <pbkit/pbkit.h>
#include <windows.h>

#include <cstdint>
#include <deque>
#include <string>

void LogMsg(const char* fmt, ...);
const std::deque<std::string>& GetOnScreenLog();
void ClearOnScreenLog();

inline void Emms() { __asm__ __volatile__("emms" ::: "memory"); }

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_MMX_TEST_COMMON_H
