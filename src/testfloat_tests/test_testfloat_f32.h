#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_F32_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_F32_H

//! Validates x87 single-precision (float32) arithmetic, comparisons,
//! and conversions using Berkeley TestFloat test vectors.
//!
//! Validates that:
//! 1. `fadd` single-precision addition matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 2. `fsub` single-precision subtraction matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 3. `fmul` single-precision multiplication matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 4. `fdiv` single-precision division matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 5. `fsqrt` single-precision square root matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 6. Single-precision quiet equality (`fucompp`) and ordered comparisons
//!    (`fcompp`) match IEEE 754 ordering rules.
struct TestFloatF32 {
  static constexpr const char* Name() { return "TestFloat F32"; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_F32_H
