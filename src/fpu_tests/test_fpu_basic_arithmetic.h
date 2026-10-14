#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_BASIC_ARITHMETIC_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_BASIC_ARITHMETIC_H

//! Placeholder test validating that the x86 floating point unit (FPU) is
//! present and performs deterministic single- and double-precision
//! arithmetic.
//!
//! Validates that:
//! 1. A double-precision sum of the integers 1 through 1000, computed
//!    entirely on the FPU, equals the closed-form value 500500.0.
//! 2. The double-precision product 10! computed on the FPU equals the
//!    closed-form value 3628800.0.
//! 3. A single-precision (float32) sum of the integers 1 through 10
//!    computed on the FPU equals the closed-form value 55.0f.
struct TestFpuBasicArithmetic {
  static constexpr const char* Name() { return "FPU Basic Arithmetic"; }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_BASIC_ARITHMETIC_H
