#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_MIN_MAX_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_MIN_MAX_H

//! Validates Intel SSE Non-IEEE Min/Max operand selection rules.
//!
//! Per the Intel architecture manual, `minss`, `minps`, `maxss`, and `maxps`
//! return the second operand (SRC2) whenever either operand is a NaN, or when
//! comparing +0.0 and -0.0.
//!
//! Validates that:
//! 1. `minss(NaN, 1.0f)` returns 1.0f (SRC2 returned when SRC1 is NaN).
//! 2. `minss(1.0f, NaN)` returns NaN (SRC2 returned when SRC2 is NaN).
//! 3. `minss(+0.0f, -0.0f)` returns -0.0f (SRC2 returned on equal magnitude).
//! 4. `minss(-0.0f, +0.0f)` returns +0.0f (SRC2 returned on equal magnitude).
//! 5. `maxss(NaN, 1.0f)` returns 1.0f (SRC2 returned when SRC1 is NaN).
//! 6. `maxss(1.0f, NaN)` returns NaN (SRC2 returned when SRC2 is NaN).
//! 7. `maxss(+0.0f, -0.0f)` returns -0.0f (SRC2 returned on equal magnitude).
//! 8. `maxss(-0.0f, +0.0f)` returns +0.0f (SRC2 returned on equal magnitude).
//! 9. Vector versions (`minps`, `maxps`) preserve these non-IEEE rules across
//!    all four lanes simultaneously.
struct TestSseMinMax {
  static constexpr const char* Name() { return "SSE Min/Max Non-IEEE Rules"; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_MIN_MAX_H
