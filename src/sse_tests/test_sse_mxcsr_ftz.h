#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_MXCSR_FTZ_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_MXCSR_FTZ_H

//! Validates SSE MXCSR control register, Flush-to-Zero (FTZ), and
//! Denormals-Are-Zeros (DAZ).
//!
//! Validates that:
//! 1. The default MXCSR register value masks all exceptions (bits 7-12 set,
//!    0x1F80 mask pattern).
//! 2. With FTZ enabled (bit 15 = 1) and underflow exceptions masked, an
//! underflowing
//!    scalar or vector floating-point operation flushes the result to zero and
//!    sets both Underflow (UE, bit 4) and Inexact (PE, bit 5) sticky flags.
//! 3. With FTZ disabled (bit 15 = 0), the same underflowing operation computes
//! a
//!    denormalized non-zero float.
//! 4. `fxsave` reports `MXCSR_MASK` with bit 6 (DAZ) cleared (0), confirming
//!    that DAZ is a reserved bit on Coppermine silicon and would trigger a
//!    General Protection fault (#GP) if written via `ldmxcsr`.
//! 5. The original MXCSR register contents are restored upon test completion.
struct TestSseMxcsrFtz {
  static constexpr const char* Name() { return "SSE MXCSR and Flush-to-Zero"; }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_MXCSR_FTZ_H
