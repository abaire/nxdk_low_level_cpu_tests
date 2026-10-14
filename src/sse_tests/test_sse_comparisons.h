#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_COMPARISONS_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_COMPARISONS_H

//! Validates SSE comparison instructions and CPU status flags.
//!
//! Validates that:
//! 1. `cmpps` / `cmpss` predicate immediates 0 to 7 (EQ, LT, LE, UNORD, NEQ,
//!    NLT, NLE, ORD) yield 0xFFFFFFFF for true condition and 0x00000000 for
//!    false condition across equal, unequal, ordered, and NaN operands.
//! 2. `comiss` sets CPU EFLAGS (ZF, PF, CF) according to operand relations:
//!    - Unordered / NaN operands: ZF=1, PF=1, CF=1
//!    - Greater than: ZF=0, PF=0, CF=0
//!    - Less than: ZF=0, PF=0, CF=1
//!    - Equal: ZF=1, PF=0, CF=0
//! 3. `ucomiss` sets the identical CPU EFLAGS conditions for comparison
//! relations.
//! 4. `comiss` sets the Invalid Operation sticky flag in MXCSR on Quiet NaN
//!    operands, whereas `ucomiss` does not raise an Invalid Operation exception
//!    on Quiet NaNs.
struct TestSseComparisons {
  static constexpr const char* Name() { return "SSE Comparisons and Flags"; }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_SSE_COMPARISONS_H
