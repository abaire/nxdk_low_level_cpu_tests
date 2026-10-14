#include "sse_tests/test_sse_mxcsr_ftz.h"

#include <cstdint>

#include "sse_tests/sse_runner_case.h"
#include "sse_tests/sse_test_common.h"

static void RunDivss(uint32_t out[4], const uint32_t a[4],
                     const uint32_t b[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "divss xmm0, xmm1\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
}

bool TestSseMxcsrFtz::Test() {
  const uint32_t orig_mxcsr = ReadMxcsr();
  bool pass = true;

  // 1. Check default exception mask state (bits 7-12 must be set: 0x1F80)
  TEST_INFO_EX(TestFields("mxcsr", Hex32(orig_mxcsr)), "Current MXCSR: 0x%08X",
               orig_mxcsr);
  if ((orig_mxcsr & 0x1F80u) == 0x1F80u) {
    TEST_PASS("All SIMD exceptions masked in MXCSR (0x1F80)");
  } else {
    TEST_FAIL_EX(
        TestFields("expected_mask", Hex32(0x1F80u), "mxcsr", Hex32(orig_mxcsr)),
        "Expected exception masks (0x1F80) not set: 0x%08X", orig_mxcsr);
    pass = false;
  }

  // Smallest positive normal float: 0x00800000 (2^-126)
  static constexpr uint32_t kSmallestNormal = 0x00800000;
  static constexpr uint32_t kTwo = 0x40000000;  // 2.0f

  alignas(16) const uint32_t num[4] = {kSmallestNormal, 0, 0, 0};
  alignas(16) const uint32_t den[4] = {kTwo, 0, 0, 0};
  alignas(16) uint32_t out[4] = {0};

  // 2. Test with FTZ disabled (bit 15 = 0)
  // Clear sticky flags (0-5) and FTZ bit (15), preserve masks (7-12)
  uint32_t mxcsr_no_ftz = (orig_mxcsr & ~0x803Fu) | 0x1F80u;
  WriteMxcsr(mxcsr_no_ftz);
  RunDivss(out, num, den);
  uint32_t mxcsr_res1 = ReadMxcsr();

  // Denormalized result expected: 0x00400000 (2^-127)
  if (out[0] == 0x00400000) {
    TEST_PASS("FTZ=0 produced denormal 0x%08X", out[0]);
  } else {
    TEST_FAIL_EX(
        TestFields("expected", Hex32(0x00400000u), "actual", Hex32(out[0])),
        "FTZ=0 expected denormal 0x00400000 got 0x%08X", out[0]);
    pass = false;
  }

  // Underflow (bit 4, 0x10) sticky flag may be set depending on processor
  TEST_INFO_EX(TestFields("mxcsr_flags", Hex32(mxcsr_res1 & 0x3Fu)),
               "FTZ=0 sticky flags: 0x%02X", mxcsr_res1 & 0x3Fu);

  // 3. Test with FTZ enabled (bit 15 = 1)
  uint32_t mxcsr_with_ftz = (orig_mxcsr & ~0x003Fu) | 0x8000u | 0x1F80u;
  WriteMxcsr(mxcsr_with_ftz);
  RunDivss(out, num, den);
  uint32_t mxcsr_res2 = ReadMxcsr();

  // Result should flush to 0 (+0.0f = 0x00000000)
  if (out[0] == 0x00000000) {
    TEST_PASS("FTZ=1 flushed to zero (0x%08X)", out[0]);
  } else {
    TEST_FAIL_EX(TestFields("expected", Hex32(0u), "actual", Hex32(out[0])),
                 "FTZ=1 expected 0x00000000 got 0x%08X", out[0]);
    pass = false;
  }

  // Underflow (bit 4 = 0x10) and Inexact / Precision (bit 5 = 0x20) should be
  // set
  uint32_t ftz_sticky = mxcsr_res2 & 0x30u;
  if (ftz_sticky == 0x30u) {
    TEST_PASS("FTZ=1 set UE (bit 4) and PE (bit 5) in MXCSR (0x%02X)",
              ftz_sticky);
  } else {
    TEST_FAIL_EX(TestFields("expected_flags", Hex32(0x30u), "actual_flags",
                            Hex32(ftz_sticky)),
                 "FTZ=1 expected UE and PE (0x30) got 0x%02X", ftz_sticky);
    pass = false;
  }

  // 4. Test DAZ (bit 6) support via FXSAVE
  // Per Intel Architecture Software Developer's Manual (Vol 1, Section 11.6.3):
  // Writing a 1 to any reserved bit in MXCSR (including DAZ on processors where
  // it is not supported) causes a General Protection fault (#GP).
  // Software determines processor support for DAZ by inspecting the MXCSR_MASK
  // field (bytes 28-31) saved by the FXSAVE instruction. If bit 6 of MXCSR_MASK
  // is 0, DAZ is unsupported and treated as reserved.
  alignas(16) uint8_t fxsave_region[512] = {0};
  __asm__ __volatile__("fxsave [%0]\n\t" : : "r"(fxsave_region) : "memory");

  uint32_t mxcsr_mask = *reinterpret_cast<uint32_t*>(&fxsave_region[28]);
  TEST_INFO_EX(TestFields("mxcsr_mask", Hex32(mxcsr_mask)),
               "FXSAVE MXCSR_MASK: 0x%08X", mxcsr_mask);

  if ((mxcsr_mask & 0x0040u) == 0) {
    TEST_PASS(
        "DAZ (bit 6) in MXCSR_MASK is 0 (unsupported on Coppermine; reserved)");
  } else {
    TEST_INFO(
        "[NOTE] DAZ (bit 6) in MXCSR_MASK is 1 (supported on non-Coppermine or "
        "emulator)");
  }

  // 5. Restore original MXCSR
  WriteMxcsr(orig_mxcsr);
  if (ReadMxcsr() == orig_mxcsr) {
    TEST_PASS("Restored original MXCSR");
  } else {
    TEST_FAIL("Failed to restore original MXCSR");
    pass = false;
  }

  return pass;
}
