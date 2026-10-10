#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_APPROXIMATIONS_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_APPROXIMATIONS_H

//! Validates SSE reciprocal and reciprocal square root approximation
//! instructions
//! (`rcpss`, `rcpps`, `rsqrtss`, `rsqrtps`).
//!
//! On the Intel Coppermine Celeron, reciprocal and reciprocal square root use
//! an internal hardware lookup table with 12-bit mantissa interpolation.
//!
//! Validates that:
//! 1. `rcpss` on test values (including powers of 2 and non-powers of 2)
//!    approximates 1/x within the relative error bound of 1.5 * 2^-12
//!    (~0.0366%).
//! 2. `rcpss` on exact hardware baseline values:
//!    - 1.0f -> 0x3F7FF000 (~0.999756f)
//!    - 2.0f -> 0x3EFFF000 (~0.499878f)
//!    - 0.5f -> 0x3FFFF000 (~1.999512f)
//! 3. `rcpss` on boundary conditions:
//!    - +0.0f yields +Inf (0x7F800000).
//!    - -0.0f yields -Inf (0xFF800000).
//!    - No numeric exceptions (ZE, etc.) are raised in MXCSR.
//! 4. `rsqrtss` on test values approximates 1/sqrt(x) within the relative
//!    error bound of 1.5 * 2^-12 (~0.0366%).
//! 5. `rsqrtss` on exact hardware baseline values:
//!    - 1.0f -> 0x3F7FF000 (~0.999756f)
//!    - 4.0f -> 0x3EFFF000 (~0.499878f)
//!    - 0.25f -> 0x3FFFF000 (~1.999512f)
//! 6. `rsqrtss` on negative numbers produces Quiet NaN (0xFFC00000) without
//!    raising an Invalid Operation exception (IE) in MXCSR.
//! 7. Vector versions (`rcpps`, `rsqrtps`) produce lane results matching scalar
//!    versions across all four vector lanes.
struct TestSseApproximations {
  static constexpr const char* Name() {
    return "SSE Approximations (rcp/rsqrt)";
  }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_APPROXIMATIONS_H
