#ifndef NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_PACK_UNPACK_H
#define NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_PACK_UNPACK_H

//! Validates MMX packing with saturation and interleaving unpacking on the Xbox
//! Celeron CPU.
//!
//! Validates that:
//! 1. `packsswb` packs 16-bit signed words into 8-bit signed bytes with signed
//!    saturation ([-128, 127]), placing destination words in the lower 4 bytes
//!    and source words in the upper 4 bytes.
//! 2. `packssdw` packs 32-bit signed doublewords into 16-bit signed words with
//!    signed saturation ([-32768, 32767]), placing destination doublewords in
//!    the lower 2 words and source doublewords in the upper 2 words.
//! 3. `packuswb` packs 16-bit signed words into 8-bit unsigned bytes with
//!    unsigned saturation ([0, 255]), clamping negative input words to 0x00 and
//!    positive words > 255 to 0xFF.
//! 4. `punpcklbw` interleaves low-order bytes from destination and source.
//! 5. `punpcklwd` interleaves low-order words from destination and source.
//! 6. `punpckldq` interleaves low-order doublewords from destination and
//! source.
//! 7. `punpckhbw` interleaves high-order bytes from destination and source.
//! 8. `punpckhwd` interleaves high-order words from destination and source.
//! 9. `punpckhdq` interleaves high-order doublewords from destination and
//!    source.
struct TestMmxPackUnpack {
  static constexpr const char* Name() { return "MMX Pack & Unpack"; }
  static constexpr const char* Source() { return __FILE__; }
  static bool Test();
};

#endif  // NXDK_LOW_LEVEL_CPU_TESTS_TEST_MMX_PACK_UNPACK_H
