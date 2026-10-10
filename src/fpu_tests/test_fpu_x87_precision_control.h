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
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_FPU_X87_PRECISION_CONTROL_H
