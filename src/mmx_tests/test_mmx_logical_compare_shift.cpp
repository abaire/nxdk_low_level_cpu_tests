#include "mmx_tests/test_mmx_logical_compare_shift.h"

#include <cstdint>

#include "mmx_tests/mmx_test_common.h"

static uint64_t RunPand(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pand mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPandn(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pandn mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPor(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "por mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPxor(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pxor mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPcmpeqb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pcmpeqb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPcmpeqw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pcmpeqw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPcmpeqd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pcmpeqd mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPcmpgtb(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pcmpgtb mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPcmpgtw(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pcmpgtw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

static uint64_t RunPcmpgtd(uint64_t a, uint64_t b) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pcmpgtd mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(a), "m"(b)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsllwImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psllw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPslldImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "pslld mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsllqImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psllq mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsrlwImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psrlw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsrldImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psrld mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsrlqImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psrlq mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsrawImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psraw mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

template <uint8_t Imm>
static uint64_t RunPsradImm(uint64_t val) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "psrad mm0, %2\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "i"(Imm)
      : "mm0");
  return out;
}

static uint64_t RunPsrawReg(uint64_t val, uint64_t count) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "movq mm1, %2\n\t"
      "psraw mm0, mm1\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "m"(count)
      : "mm0", "mm1");
  return out;
}

static uint64_t RunPsradReg(uint64_t val, uint64_t count) {
  uint64_t out;
  __asm__ __volatile__(
      "movq mm0, %1\n\t"
      "movq mm1, %2\n\t"
      "psrad mm0, mm1\n\t"
      "movq %0, mm0\n\t"
      "emms\n\t"
      : "=m"(out)
      : "m"(val), "m"(count)
      : "mm0", "mm1");
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

bool TestMmxLogicalCompareShift::Test() {
  bool pass = true;

  // 1. Bitwise logical operations (pand, pandn, por, pxor)
  const uint64_t a_log = 0xF0F000FF00AA55CCULL;
  const uint64_t b_log = 0x3333FFFF5555AAAAULL;
  pass &= CheckResult("pand", RunPand(a_log, b_log), 0x303000FF00000088ULL);
  pass &= CheckResult("pandn", RunPandn(a_log, b_log), 0x0303FF005555AA22ULL);
  pass &= CheckResult("por", RunPor(a_log, b_log), 0xF3F3FFFF55FFFFEEULL);
  pass &= CheckResult("pxor", RunPxor(a_log, b_log), 0xC3C3FF0055FFFF66ULL);

  // 2. Packed equality comparisons (pcmpeqb, pcmpeqw, pcmpeqd)
  const uint64_t a_cmp = 0x123456789ABCDEF0ULL;
  pass &= CheckResult("pcmpeqb", RunPcmpeqb(a_cmp, 0x120056009A00DE00ULL),
                      0xFF00FF00FF00FF00ULL);
  pass &= CheckResult("pcmpeqw", RunPcmpeqw(a_cmp, 0x123400009ABC0000ULL),
                      0xFFFF0000FFFF0000ULL);
  pass &= CheckResult("pcmpeqd", RunPcmpeqd(a_cmp, 0x1234567800000000ULL),
                      0xFFFFFFFF00000000ULL);

  // 3. Packed signed greater-than comparisons (pcmpgtb, pcmpgtw, pcmpgtd)
  const uint64_t a_gtb = 0xFDFE0605FF00807FULL;
  const uint64_t b_gtb = 0xFEFD050500FF7F80ULL;
  pass &=
      CheckResult("pcmpgtb", RunPcmpgtb(a_gtb, b_gtb), 0x00FFFF0000FF00FFULL);

  const uint64_t a_gtw = 0xFFFF000080007FFFULL;
  const uint64_t b_gtw = 0x0000FFFF7FFF8000ULL;
  pass &=
      CheckResult("pcmpgtw", RunPcmpgtw(a_gtw, b_gtw), 0x0000FFFF0000FFFFULL);

  const uint64_t a_gtd = 0xFFFFFFFF7FFFFFFFULL;
  const uint64_t b_gtd = 0x0000000080000000ULL;
  pass &=
      CheckResult("pcmpgtd", RunPcmpgtd(a_gtd, b_gtd), 0x00000000FFFFFFFFULL);

  // 4. In-range logical shifts
  const uint64_t s_in = 0x123456789ABCDEF0ULL;
  pass &= CheckResult("psllw (4)", RunPsllwImm<4>(s_in), 0x23406780ABC0EF00ULL);
  pass &= CheckResult("pslld (4)", RunPslldImm<4>(s_in), 0x23456780ABCDEF00ULL);
  pass &= CheckResult("psllq (4)", RunPsllqImm<4>(s_in), 0x23456789ABCDEF00ULL);

  pass &= CheckResult("psrlw (4)", RunPsrlwImm<4>(s_in), 0x0123056709AB0DEFULL);
  pass &= CheckResult("psrld (4)", RunPsrldImm<4>(s_in), 0x0123456709ABCDEFULL);
  pass &= CheckResult("psrlq (4)", RunPsrlqImm<4>(s_in), 0x0123456789ABCDEFULL);

  // 5. In-range arithmetic right shifts (sign extension)
  const uint64_t sa_in = 0xF23416788ABC1EF0ULL;
  pass &=
      CheckResult("psraw (4)", RunPsrawImm<4>(sa_in), 0xFF230167F8AB01EFULL);
  pass &=
      CheckResult("psrad (4)", RunPsradImm<4>(sa_in), 0xFF234167F8ABC1EFULL);

  // 6. Out-of-bounds shifts (count >= element width)
  pass &=
      CheckResult("psllw (16)", RunPsllwImm<16>(s_in), 0x0000000000000000ULL);
  pass &=
      CheckResult("psrlw (16)", RunPsrlwImm<16>(s_in), 0x0000000000000000ULL);
  pass &=
      CheckResult("pslld (32)", RunPslldImm<32>(s_in), 0x0000000000000000ULL);
  pass &=
      CheckResult("psrld (32)", RunPsrldImm<32>(s_in), 0x0000000000000000ULL);
  pass &=
      CheckResult("psllq (64)", RunPsllqImm<64>(s_in), 0x0000000000000000ULL);
  pass &=
      CheckResult("psrlq (64)", RunPsrlqImm<64>(s_in), 0x0000000000000000ULL);

  const uint64_t mixed_w = 0x80007FFF80007FFFULL;
  pass &= CheckResult("psraw (16)", RunPsrawImm<16>(mixed_w),
                      0xFFFF0000FFFF0000ULL);
  pass &= CheckResult("psraw (100)", RunPsrawReg(mixed_w, 100),
                      0xFFFF0000FFFF0000ULL);

  const uint64_t mixed_d = 0x800000007FFFFFFFULL;
  pass &= CheckResult("psrad (32)", RunPsradImm<32>(mixed_d),
                      0xFFFFFFFF00000000ULL);
  pass &= CheckResult("psrad (100)", RunPsradReg(mixed_d, 100),
                      0xFFFFFFFF00000000ULL);

  return pass;
}
