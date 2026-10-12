#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_LOGICAL_COMPARE_SHIFT_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_LOGICAL_COMPARE_SHIFT_H

//! Validates MMX bitwise logical, packed comparison, and packed shift
//! instructions on the Xbox Celeron CPU.
//!
//! Validates that:
//! 1. Bitwise logical `pand`, `pandn`, `por`, and `pxor` perform 64-bit boolean
//!    operations, specifically `pandn` computing `(~DEST) & SRC`.
//! 2. Packed byte, word, and doubleword equality comparisons (`pcmpeqb`,
//!    `pcmpeqw`, `pcmpeqd`) set matching lanes to all 1s and non-matching lanes
//!    to all 0s.
//! 3. Packed byte, word, and doubleword signed greater-than comparisons
//!    (`pcmpgtb`, `pcmpgtw`, `pcmpgtd`) perform two's complement comparisons
//!    setting lanes where DEST > SRC to all 1s.
//! 4. Packed word, doubleword, and quadword logical left shifts (`psllw`,
//!    `pslld`, `psllq`) shift bits left by immediate count and register count,
//!    filling vacated bits with 0.
//! 5. Packed word, doubleword, and quadword logical right shifts (`psrlw`,
//!    `psrld`, `psrlq`) shift bits right by immediate count and register count,
//!    filling vacated bits with 0.
//! 6. Packed word and doubleword arithmetic right shifts (`psraw`, `psrad`)
//!    shift bits right preserving and replicating the sign bit.
//! 7. Out-of-bounds shift counts (>= 16 for word, >= 32 for doubleword, >= 64
//!    for quadword) clear destination lanes to 0 for logical shifts, and
//!    replicate sign bits for arithmetic right shifts.
struct TestMmxLogicalCompareShift {
  static constexpr const char* Name() { return "MMX Logical, Compare & Shift"; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_LOGICAL_COMPARE_SHIFT_H
