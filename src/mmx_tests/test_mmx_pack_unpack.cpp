#include "mmx_tests/test_mmx_pack_unpack.h"

#include <cstdint>

#include "mmx_tests/mmx_test_common.h"

static uint64_t RunPacksswb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "packsswb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPackssdw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "packssdw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPackuswb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "packuswb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPunpcklbw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "punpcklbw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPunpcklwd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "punpcklwd mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPunpckldq(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "punpckldq mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPunpckhbw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "punpckhbw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPunpckhwd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "punpckhwd mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPunpckhdq(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "punpckhdq mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
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

bool TestMmxPackUnpack::Test() {
  bool pass = true;

  // 1. packsswb: 16-bit signed to 8-bit signed saturation
  const uint64_t a_sswb = 0xFE00FF8000FF0050ULL;
  const uint64_t b_sswb = 0xFFFF80007FFF0000ULL;
  pass &= CheckResult("packsswb", RunPacksswb(a_sswb, b_sswb),
                      0xFF807F0080807F50ULL);

  // 2. packssdw: 32-bit signed to 16-bit signed saturation
  const uint64_t a_ssdw = 0x0001000000001234ULL;
  const uint64_t b_ssdw = 0xFFFF0000FFFF8000ULL;
  pass &= CheckResult("packssdw", RunPackssdw(a_ssdw, b_ssdw),
                      0x800080007FFF1234ULL);

  // 3. packuswb: 16-bit signed to 8-bit unsigned saturation (negatives to 0)
  const uint64_t a_uswb = 0x00FF0064FFFFFE0CULL;
  const uint64_t b_uswb = 0x0000800013880100ULL;
  pass &= CheckResult("packuswb", RunPackuswb(a_uswb, b_uswb),
                      0x0000FFFFFF640000ULL);

  // 4. Low-order unpacking
  const uint64_t a_unpck = 0x0706050403020100ULL;
  const uint64_t b_unpck = 0x7060504030201000ULL;
  pass &= CheckResult("punpcklbw", RunPunpcklbw(a_unpck, b_unpck),
                      0x3003200210010000ULL);
  pass &= CheckResult("punpcklwd", RunPunpcklwd(a_unpck, b_unpck),
                      0x3020030210000100ULL);
  pass &= CheckResult("punpckldq", RunPunpckldq(a_unpck, b_unpck),
                      0x3020100003020100ULL);

  // 5. High-order unpacking
  pass &= CheckResult("punpckhbw", RunPunpckhbw(a_unpck, b_unpck),
                      0x7007600650054004ULL);
  pass &= CheckResult("punpckhwd", RunPunpckhwd(a_unpck, b_unpck),
                      0x7060070650400504ULL);
  pass &= CheckResult("punpckhdq", RunPunpckhdq(a_unpck, b_unpck),
                      0x7060504007060504ULL);

  return pass;
}
