#include "mmx_tests/test_mmx_extensions.h"

#include <cstdint>
#include <cstring>

#include "mmx_tests/mmx_test_common.h"

static uint64_t RunPavgb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pavgb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPavgw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pavgw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPminub(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pminub mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPmaxub(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmaxub mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPminsw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pminsw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPmaxsw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmaxsw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPmulhuw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmulhuw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint32_t RunPmovmskb(uint64_t a) {
  uint32_t mask = 0;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmovmskb %0, mm0\n\t"
      "emms\n\t"
      : "=r"(mask)
      : "m"(a)
      : "mm0");
  return mask;
}

static uint32_t RunPextrw0(uint64_t a) {
  uint32_t out = 0;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pextrw %0, mm0, 0\n\t"
      "emms\n\t"
      : "=r"(out)
      : "m"(a)
      : "mm0");
  return out;
}

static uint32_t RunPextrw2(uint64_t a) {
  uint32_t out = 0;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pextrw %0, mm0, 2\n\t"
      "emms\n\t"
      : "=r"(out)
      : "m"(a)
      : "mm0");
  return out;
}

static uint64_t RunPinsrw2(uint64_t a, uint16_t w) {
  uint64_t out;
  uint32_t w32 = w;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pinsrw mm0, %2, 2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "r"(w32)
      : "mm0");
  return out;
}

static uint64_t RunPsadbw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psadbw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPshufwReverse(uint64_t a) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pshufw mm0, mm0, 0x1B\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a)
      : "mm0");
  return out;
}

static void RunMaskmovq(uint8_t dest[8], uint64_t src, uint64_t mask) {
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "movq mm1, %2\n\t"
      "maskmovq mm0, mm1\n\t"
      "emms\n\t"
      :
      : "D"(dest), "m"(src), "m"(mask)
      : "mm0", "mm1", "memory");
}

static bool CheckResult(const char* op_name, uint64_t actual,
                        uint64_t expected) {
  if (actual == expected) {
    TEST_PASS("%-12s : 0x%016llX", op_name, actual);
    return true;
  }
  TEST_FAIL_EX(TestFields("op", op_name, "expected", Hex64(expected), "actual",
                          Hex64(actual)),
               "%-12s expected 0x%016llX got 0x%016llX", op_name, expected,
               actual);
  return false;
}

bool TestMmxExtensions::Test() {
  bool pass = true;

  // 1. Packed average with rounding up (pavgb, pavgw)
  const uint64_t a_avgb = 0x32C864FF00010000ULL;
  const uint64_t b_avgb = 0x3CC965FF00020100ULL;
  pass &= CheckResult("pavgb", RunPavgb(a_avgb, b_avgb), 0x37C965FF00020100ULL);

  const uint64_t a_avgw = 0xFFFF03E8FFFF0000ULL;
  const uint64_t b_avgw = 0xFFFF07D000000001ULL;
  pass &= CheckResult("pavgw", RunPavgw(a_avgw, b_avgw), 0xFFFF05DC80000001ULL);

  // 2. Unsigned byte min/max (pminub, pmaxub)
  const uint64_t a_ub = 0x10FF00FA80053070ULL;
  const uint64_t b_ub = 0x2000FF057F0A3070ULL;
  pass &= CheckResult("pminub", RunPminub(a_ub, b_ub), 0x100000057F053070ULL);
  pass &= CheckResult("pmaxub", RunPmaxub(a_ub, b_ub), 0x20FFFFFA800A3070ULL);

  // 3. Signed word min/max (pminsw, pmaxsw)
  const uint64_t a_sw = 0x7FFF8000FFF60014ULL;
  const uint64_t b_sw = 0x80007FFF0014FFF6ULL;
  pass &= CheckResult("pminsw", RunPminsw(a_sw, b_sw), 0x80008000FFF6FFF6ULL);
  pass &= CheckResult("pmaxsw", RunPmaxsw(a_sw, b_sw), 0x7FFF7FFF00140014ULL);

  // 4. Unsigned word multiply high (pmulhuw)
  const uint64_t a_mulhu = 0x000203E88000FFFFULL;
  const uint64_t b_mulhu = 0xFFFF00148000FFFFULL;
  pass &= CheckResult("pmulhuw", RunPmulhuw(a_mulhu, b_mulhu),
                      0x000100004000FFFEULL);

  // 5. Byte mask extraction (pmovmskb)
  const uint64_t a_mask = 0x8000800080800080ULL;
  uint32_t mask_res = RunPmovmskb(a_mask);
  if (mask_res == 0x000000ADu) {
    TEST_PASS("pmovmskb     : 0x%08X", mask_res);
  } else {
    TEST_FAIL_EX(TestFields("op", "pmovmskb", "expected", Hex32(0xADu),
                            "actual", Hex32(mask_res)),
                 "pmovmskb expected 0x000000AD got 0x%08X", mask_res);
    pass = false;
  }

  // 6. Word extraction (pextrw)
  const uint64_t a_ext = 0x123456789ABCDEF0ULL;
  uint32_t ext0 = RunPextrw0(a_ext);
  uint32_t ext2 = RunPextrw2(a_ext);
  if (ext0 == 0xDEF0u && ext2 == 0x5678u) {
    TEST_PASS("pextrw       : lane0=0x%04X lane2=0x%04X", ext0, ext2);
  } else {
    TEST_FAIL_EX(TestFields("op", "pextrw", "lane0_expected", Hex32(0xDEF0u),
                            "lane0_actual", Hex32(ext0), "lane2_expected",
                            Hex32(0x5678u), "lane2_actual", Hex32(ext2)),
                 "pextrw lane0 expected 0xDEF0 got 0x%04X, lane2 expected "
                 "0x5678 got 0x%04X",
                 ext0, ext2);
    pass = false;
  }

  // 7. Word insertion (pinsrw)
  pass &=
      CheckResult("pinsrw", RunPinsrw2(a_ext, 0xAAAA), 0x1234AAAA9ABCDEF0ULL);

  // 8. Sum of absolute byte differences (psadbw)
  const uint64_t a_sad = 0x0807060504030201ULL;
  const uint64_t b_sad = 0x0102030405060708ULL;
  pass &= CheckResult("psadbw", RunPsadbw(a_sad, b_sad), 0x0000000000000020ULL);

  // 9. Word shuffle (pshufw)
  const uint64_t a_shuf = 0x4444333322221111ULL;
  pass &=
      CheckResult("pshufw", RunPshufwReverse(a_shuf), 0x1111222233334444ULL);

  // 10. Masked memory store (maskmovq)
  uint8_t mem[8];
  std::memset(mem, 0xAA, sizeof(mem));
  const uint64_t src_mm = 0x1122334455667788ULL;
  const uint64_t msk_mm = 0x8000800080800080ULL;
  RunMaskmovq(mem, src_mm, msk_mm);
  uint64_t mem_val;
  std::memcpy(&mem_val, mem, sizeof(mem_val));
  pass &= CheckResult("maskmovq", mem_val, 0x11AA33AA5566AA88ULL);

  return pass;
}
