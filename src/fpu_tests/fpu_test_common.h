#ifndef NXDK_LOW_LEVEL_CPU_TESTS_FPU_TEST_COMMON_H
#define NXDK_LOW_LEVEL_CPU_TESTS_FPU_TEST_COMMON_H

#include <hal/debug.h>
#include <pbkit/pbkit.h>
#include <windows.h>

#include <cstdint>
#include <deque>
#include <string>

void LogMsg(const char* fmt, ...);
const std::deque<std::string>& GetOnScreenLog();
void ClearOnScreenLog();

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_FPU_TEST_COMMON_H
