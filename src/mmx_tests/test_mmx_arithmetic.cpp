#include "mmx_tests/test_mmx_arithmetic.h"

#include <cstdint>

#include "mmx_tests/mmx_test_common.h"

static uint64_t RunPaddb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPaddw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPaddd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddd mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubd mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPaddsb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddsb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubsb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubsb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPaddsw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddsw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubsw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubsw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPaddusb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddusb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubusb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubusb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPaddusw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "paddusw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPsubusw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psubusw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPmullw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmullw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPmulhw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmulhw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPmaddwd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pmaddwd mm0, %2\n\t"
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
    TEST_PASS("%-10s : 0x%016llX", op_name, actual);
    return true;
  }
  TEST_FAIL_EX(TestFields("op", op_name, "expected", Hex64(expected), "actual",
                          Hex64(actual)),
               "%-10s expected 0x%016llX got 0x%016llX", op_name, expected,
               actual);
  return false;
}

bool TestMmxArithmetic::Test() {
  bool pass = true;

  // 1. Modulo wraparound byte arithmetic (paddb, psubb)
  const uint64_t a_b = 0x55AA00FE01FF807FULL;
  const uint64_t b_b = 0xAA550003FF010101ULL;
  pass &= CheckResult("paddb", RunPaddb(a_b, b_b), 0xFFFF000100008180ULL);
  pass &= CheckResult("psubb", RunPsubb(a_b, b_b), 0xAB5500FB02FE7F7EULL);

  // 2. Modulo wraparound word arithmetic (paddw, psubw)
  const uint64_t a_w = 0x1234FFFF80007FFFULL;
  const uint64_t b_w = 0xEDCC000100010001ULL;
  pass &= CheckResult("paddw", RunPaddw(a_w, b_w), 0x0000000080018000ULL);
  pass &= CheckResult("psubw", RunPsubw(a_w, b_w), 0x2468FFFE7FFF7FFEULL);

  // 3. Modulo wraparound doubleword arithmetic (paddd, psubd)
  const uint64_t a_d = 0xFFFFFFFF7FFFFFFFULL;
  const uint64_t b_d = 0x0000000100000001ULL;
  pass &= CheckResult("paddd", RunPaddd(a_d, b_d), 0x0000000080000000ULL);
  pass &= CheckResult("psubd", RunPsubd(a_d, b_d), 0xFFFFFFFE7FFFFFFEULL);

  // 4. Signed saturation byte arithmetic (paddsb, psubsb)
  const uint64_t a_sb = 0x7F80050090807F70ULL;
  const uint64_t b_sb = 0x80FFFB0080010120ULL;
  pass &= CheckResult("paddsb", RunPaddsb(a_sb, b_sb), 0xFF80000080817F7FULL);

  const uint64_t a_sub_sb = 0x807F807F00057080ULL;
  const uint64_t b_sub_sb = 0x01817F80000AF07FULL;
  pass &= CheckResult("psubsb", RunPsubsb(a_sub_sb, b_sub_sb),
                      0x807F807F00FB7F80ULL);

  // 5. Signed saturation word arithmetic (paddsw, psubsw)
  const uint64_t a_sw = 0x7FFF900080007000ULL;
  const uint64_t b_sw = 0x0001800000012000ULL;
  pass &= CheckResult("paddsw", RunPaddsw(a_sw, b_sw), 0x7FFF800080017FFFULL);

  const uint64_t a_sub_sw = 0x80007FFF00058000ULL;
  const uint64_t b_sub_sw = 0x00018001000A7FFFULL;
  pass &= CheckResult("psubsw", RunPsubsw(a_sub_sw, b_sub_sw),
                      0x80007FFFFFFB8000ULL);

  // 6. Unsigned saturation byte arithmetic (paddusb, psubusb)
  const uint64_t a_ub = 0x10F005AA00FF8070ULL;
  const uint64_t b_ub = 0x20200A55000180A0ULL;
  pass &= CheckResult("paddusb", RunPaddusb(a_ub, b_ub), 0x30FF0FFF00FFFFFFULL);
  pass &= CheckResult("psubusb", RunPsubusb(a_ub, b_ub), 0x00D0005500FE0000ULL);

  // 7. Unsigned saturation word arithmetic (paddusw, psubusw)
  const uint64_t a_uw = 0x0005F000FFFF1234ULL;
  const uint64_t b_uw = 0x000A200000011000ULL;
  pass &= CheckResult("paddusw", RunPaddusw(a_uw, b_uw), 0x000FFFFFFFFF2234ULL);
  pass &= CheckResult("psubusw", RunPsubusw(a_uw, b_uw), 0x0000D000FFFE0234ULL);

  // 8. Word multiplication (pmullw, pmulhw)
  const uint64_t a_mul = 0x7FFF8000FFF603E8ULL;
  const uint64_t b_mul = 0x7FFF800000140014ULL;
  pass &= CheckResult("pmullw", RunPmullw(a_mul, b_mul), 0x00010000FF384E20ULL);
  pass &= CheckResult("pmulhw", RunPmulhw(a_mul, b_mul), 0x3FFF4000FFFF0000ULL);

  // 9. Multiply and add (pmaddwd)
  const uint64_t a_madd = 0x8000800007D003E8ULL;
  const uint64_t b_madd = 0x80008000FFFC0003ULL;
  pass &=
      CheckResult("pmaddwd", RunPmaddwd(a_madd, b_madd), 0x80000000FFFFEC78ULL);

  return pass;
}
