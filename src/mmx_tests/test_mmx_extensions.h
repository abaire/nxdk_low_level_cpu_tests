#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_EXTENSIONS_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_EXTENSIONS_H

//! Validates Pentium III / SSE integer MMX instructions on 64-bit MMX registers
//! supported by the Xbox Celeron processor.
//!
//! Validates that:
//! 1. `pavgb` and `pavgw` compute packed unsigned averages with rounding up
//!    (`(A + B + 1) >> 1`) for bytes and words.
//! 2. `pminub` and `pmaxub` compute packed unsigned byte minimum and maximum.
//! 3. `pminsw` and `pmaxsw` compute packed signed word minimum and maximum.
//! 4. `pmulhuw` computes packed unsigned 16x16-bit word multiplication,
//!    returning the high 16 bits of the product.
//! 5. `pmovmskb` extracts an 8-bit mask of sign bits from 8 bytes in an MMX
//!    register to a 32-bit general-purpose register, zero-extending bits 8-31.
//! 6. `pextrw` extracts an arbitrary word from an MMX register to the low 16
//!    bits of a general-purpose register, clearing bits 16-31.
//! 7. `pinsrw` inserts a 16-bit word from a general-purpose register into an
//!    arbitrary word lane of an MMX register.
//! 8. `psadbw` computes the sum of absolute byte differences across 8 lanes and
//!    stores the 16-bit sum in the lowest word lane, clearing words 1-3 to
//!    zero.
//! 9. `pshufw` shuffles 4 words within an MMX register based on an immediate
//!    byte selector.
//! 10. `maskmovq` conditionally writes bytes from an MMX register to memory at
//!     `[EDI]` based on the sign bit of each corresponding mask byte in a
//!     second MMX register.
struct TestMmxExtensions {
  static constexpr const char* Name() { return "SSE Integer MMX Extensions"; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_EXTENSIONS_H
