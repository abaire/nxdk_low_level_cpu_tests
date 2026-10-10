#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_F64_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_F64_H

//! Validates x87 double-precision (float64) arithmetic, comparisons,
//! and conversions using Berkeley TestFloat test vectors.
//!
//! Note on x87 double-precision architecture:
//! The x87 FPU performs calculations in 80-bit extended-precision registers
//! with an extended exponent range [-16382, 16383]. Even with precision control
//! set to Double (53-bit significand), `fmul` and `fdiv` compute results using
//! this extended exponent range without IEEE 754 float64 subnormal bounds.
//! When `fstp qword` writes the register to 64-bit memory, the value undergoes
//! denormalization and a second rounding stage (double-rounding) with
//! x87-specific masked underflow flag behavior. The test harness models this
//! two-stage x87 hardware pipeline as the reference behavior for `f64_mul` and
//! `f64_div`.
//!
//! Validates that:
//! 1. `fadd` double-precision addition matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 2. `fsub` double-precision subtraction matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 3. `fmul` double-precision multiplication matches two-stage x87 hardware
//!    reference results and sets correct status word exception flags across all
//!    rounding modes.
//! 4. `fdiv` double-precision division matches two-stage x87 hardware
//!    reference results and sets correct status word exception flags across all
//!    rounding modes.
//! 5. `fsqrt` double-precision square root matches IEEE 754 reference results
//!    and sets correct status word exception flags across all rounding modes.
//! 6. Double-precision quiet equality (`fucompp`) and ordered comparisons
//!    (`fcompp`) match IEEE 754 ordering rules.
struct TestFloatF64 {
  static constexpr const char* Name() { return "TestFloat F64"; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_F64_H
