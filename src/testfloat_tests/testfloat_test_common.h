#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TESTFLOAT_TEST_COMMON_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TESTFLOAT_TEST_COMMON_H

#include <hal/debug.h>
#include <pbkit/pbkit.h>
#include <windows.h>

#include <cstdint>
#include <deque>
#include <string>

extern "C" {
#include "softfloat.h"
}

void LogMsg(const char* fmt, ...);
const std::deque<std::string>& GetOnScreenLog();
void ClearOnScreenLog();

void InitializeTestFloatEnvironment();

bool RunTestFloatBinaryF32(const char* name,
                           float32_t (*true_fn)(float32_t, float32_t),
                           float32_t (*subj_fn)(float32_t, float32_t));

bool RunTestFloatUnaryF32(const char* name, float32_t (*true_fn)(float32_t),
                          float32_t (*subj_fn)(float32_t));

bool RunTestFloatCompareF32(const char* name,
                            bool (*true_fn)(float32_t, float32_t),
                            bool (*subj_fn)(float32_t, float32_t));

bool RunTestFloatBinaryF64(const char* name,
                           float64_t (*true_fn)(float64_t, float64_t),
                           float64_t (*subj_fn)(float64_t, float64_t));

bool RunTestFloatUnaryF64(const char* name, float64_t (*true_fn)(float64_t),
                          float64_t (*subj_fn)(float64_t));

bool RunTestFloatCompareF64(const char* name,
                            bool (*true_fn)(float64_t, float64_t),
                            bool (*subj_fn)(float64_t, float64_t));

bool RunTestFloatBinaryExtF80(
    const char* name,
    void (*true_fn)(const extFloat80_t*, const extFloat80_t*, extFloat80_t*),
    void (*subj_fn)(const extFloat80_t*, const extFloat80_t*, extFloat80_t*));

bool RunTestFloatUnaryExtF80(
    const char* name, void (*true_fn)(const extFloat80_t*, extFloat80_t*),
    void (*subj_fn)(const extFloat80_t*, extFloat80_t*));

bool RunTestFloatCompareExtF80(
    const char* name, bool (*true_fn)(const extFloat80_t*, const extFloat80_t*),
    bool (*subj_fn)(const extFloat80_t*, const extFloat80_t*));

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TESTFLOAT_TEST_COMMON_H
