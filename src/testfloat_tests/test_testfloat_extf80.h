#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_EXTF80_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_EXTF80_H

//! Validates x87 80-bit double-extended-precision (extFloat80) arithmetic,
//! comparisons, and precision control using Berkeley TestFloat test vectors.
//!
//! Validates that:
//! 1. `faddp` extended-precision addition matches IEEE 754 reference results
//!    across all precision-control modes (single, double, extended) and
//!    rounding modes.
//! 2. `fsubp` extended-precision subtraction matches IEEE 754 reference results
//!    across all precision-control modes and rounding modes.
//! 3. `fmulp` extended-precision multiplication matches IEEE 754 reference
//! results
//!    across all precision-control modes and rounding modes.
//! 4. `fdivp` extended-precision division matches IEEE 754 reference results
//!    across all precision-control modes and rounding modes.
//! 5. `fsqrt` extended-precision square root matches IEEE 754 reference results
//!    across all precision-control modes and rounding modes.
//! 6. Extended-precision quiet equality (`fucompp`) and ordered comparisons
//!    (`fcompp`) match IEEE 754 ordering rules.
struct TestFloatExtF80 {
  static constexpr const char* Name() { return "TestFloat ExtF80"; }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_TESTFLOAT_EXTF80_H
