#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_X87_ALIASING_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_X87_ALIASING_H

//! Validates physical register aliasing between MMX registers and the x87 FPU
//! stack, tag word transitions, and EMMS instruction semantics.
//!
//! Validates that:
//! 1. Direct physical register aliasing: loading a 64-bit value into `MM0`
//!    directly modifies the low 64 bits (significand/mantissa) of physical
//!    `FP0` observable via `FXSAVE`, and sets the exponent bits [79:64] to
//!    0xFFFF.
//! 2. Loading a 64-bit value into `MM7` directly modifies physical `FP7` and
//!    sets exponent bits [79:64] to 0xFFFF.
//! 3. Pushing an x87 floating-point value onto the stack via `fld` decrements
//!    TOP and writes physical register `FP7` (`MM7`), directly readable via
//! 4. Executing an MMX instruction marks all 8 tags in the x87 FPU Tag Word
//!    (FTW) as non-empty (no tag equals 11b in `FNSTENV`, 0xFF in `FXSAVE`
//!    abridged tag byte) without changing the TOP of stack pointer in FSW.
//! 5. Executing `EMMS` sets all 8 tags in the x87 FPU Tag Word to empty (11b /
//!    0xFFFF in `FNSTENV`, 0x00 in `FXSAVE` abridged tag byte) without changing
//!    TOP and without clearing register values in `MM0`..`MM7`.
//! 6. Executing an x87 stack push (`fld1`) after an MMX instruction without
//!    `EMMS` triggers an x87 Stack Overflow Fault (setting SF=1, IE=1, and C1=1
//!    in the Status Word).
//! 7. Executing an x87 stack push (`fld1`) after `EMMS` succeeds cleanly
//! without
//!    setting any exception flags in the Status Word.
struct TestMmxX87Aliasing {
  static constexpr const char* Name() { return "MMX/x87 Aliasing & EMMS"; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_X87_ALIASING_H
