#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_X87_PRECISION_CONTROL_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_X87_PRECISION_CONTROL_H

//! Regression test for x87 `fld`/`fstp` behavior across FPU control-word
//! precision-control (PC) states.
//!
//! Per the Intel x86 Architecture Manual (Volume 1, section 8.1.5.2) the PC
//! field only affects arithmetic instructions (`FADD`, `FSUB`, `FMUL`, `FDIV`,
//! `FSQRT`) and has no effect on data-transfer instructions (`FLD`, `FST`,
//! `FSTP`). Buggy emulators (e.g. unpatched xemu/QEMU) erroneously round a
//! 64-bit `fld qword` load under single-precision mode, zeroing the low 29
//! bits. Titles such as Mega Man 2 use `fld qword` / `fstp qword` as an
//! optimized 64-bit memory copy for non-floating-point data (packed sprite
//! structures), so this corruption destroys game data.
//!
//! Validates that:
//! 1. A 64-bit value with non-zero low 29 bits copied via `fld qword` /
//!    `fstp qword` is bit-exact when PC is single precision (24-bit, PC =
//!    00b), i.e. data transfer is unaffected by the PC field.
//! 2. The same holds when PC is double precision (PC = 10b).
//! 3. The same holds when PC is extended precision (PC = 11b).
//! 4. After the test, the original FPU control word is restored and the FPU
//!    register stack is empty.
struct TestFpuX87PrecisionControl {
  static constexpr const char* Name() {
    return "X87 fld/fstp Precision Control";
  }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

//! Regression test for x87 arithmetic rounding under single-precision
//! precision control (PC = 00b) with round-to-nearest-even and 64-bit
//! (`qword`) memory operands.
//!
//! Hardware computes the exact result of `FADD`/`FSUB`/`FMUL`/`FDIV` and
//! rounds it once to a 24-bit significand. An emulator that computes the
//! result in binary64 and then narrows it to 24 bits rounds twice, which
//! gives the wrong answer when the exact result lies just off a 24-bit
//! halfway point by less than half a binary64 ulp: the first rounding lands
//! exactly on the halfway point and ties-to-even then picks the wrong
//! neighbor.
//!
//! Validates that, with PC = 00b and RC = 00b, `fld qword` / `<op> qword` /
//! `fstp qword` produces the singly rounded 24-bit result for:
//! 1. `fsub` and `fadd` whose exact result is just above a 24-bit halfway
//!    point (double rounding would round down to even).
//! 2. `fsub` whose exact result is just below a 24-bit halfway point (double
//!    rounding would round up to even).
//! 3. `fmul` whose exact product is just above and just below a 24-bit
//!    halfway point.
//! 4. `fdiv` whose exact (non-terminating) quotient is just above and just
//!    below a 24-bit halfway point.
//! 5. After the test, the original FPU control word is restored and the FPU
//!    register stack is empty.
struct TestFpuX87PrecisionControlArithmetic {
  static constexpr const char* Name() {
    return "X87 Single Precision Control Arithmetic Rounding";
  }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_X87_PRECISION_CONTROL_H
