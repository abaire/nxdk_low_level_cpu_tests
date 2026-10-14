#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_ARITHMETIC_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_ARITHMETIC_H

//! Validates 64-bit MMX SIMD arithmetic instructions on the Xbox Celeron CPU.
//!
//! Validates that:
//! 1. Packed byte, word, and doubleword wraparound addition (`paddb`, `paddw`,
//!    `paddd`) perform two's complement modulo addition without setting flags.
//! 2. Packed byte, word, and doubleword wraparound subtraction (`psubb`,
//!    `psubw`, `psubd`) perform two's complement modulo subtraction without
//!    setting flags.
//! 3. Packed signed saturated byte addition and subtraction (`paddsb`,
//!    `psubsb`) clamp positive overflow to 0x7F (+127) and negative underflow
//!    to 0x80 (-128).
//! 4. Packed signed saturated word addition and subtraction (`paddsw`,
//!    `psubsw`) clamp positive overflow to 0x7FFF (+32767) and negative
//!    underflow to 0x8000 (-32768).
//! 5. Packed unsigned saturated byte addition and subtraction (`paddusb`,
//!    `psubusb`) clamp overflow to 0xFF (255) and underflow to 0x00 (0).
//! 6. Packed unsigned saturated word addition and subtraction (`paddusw`,
//!    `psubusw`) clamp overflow to 0xFFFF (65535) and underflow to 0x0000 (0).
//! 7. Packed word multiply low (`pmullw`) retains the low 16 bits of each
//!    16x16-bit product.
//! 8. Packed word multiply high signed (`pmulhw`) retains the high 16 bits of
//!    each signed 16x16-bit product.
//! 9. Packed multiply and add (`pmaddwd`) multiplies signed 16-bit words into
//!    intermediate 32-bit products and adds adjacent pairs, wrapping around to
//!    0x80000000 when all four inputs are -32768.
struct TestMmxArithmetic {
  static constexpr const char* Name() { return "MMX Packed Arithmetic"; }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_ARITHMETIC_H
