#include "fpu_tests/test_fpu_x87_precision_control.h"

#include <cstdint>

#include "fpu_test_common.h"

static uint16_t ReadFpuControlWord() {
  uint16_t cw = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(cw));
  return cw;
}

static void WriteFpuControlWord(uint16_t cw) {
  __asm__ __volatile__("fldcw word ptr %0" : : "m"(cw));
}

static bool ReadFpuStackEmpty() {
  uint16_t sw = 0;
  __asm__ __volatile__("fnstsw word ptr %0" : "=m"(sw));
  // C1 (bit 10), C0 (bit 11), and the 2-bit top-of-stack pointer (bits
  // 12-13) are all zero when the FPU register stack is empty.
  return ((sw >> 10) & 0xF) == 0;
}

// Copies a 64-bit value with x87 `fld qword` / `fstp qword` inline assembly
// so the compiler cannot replace the copy with a register move, memcpy, or
// SSE instruction.
static void CopyQwordViaX87(uint64_t& dst, const uint64_t& src) {
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(dst)
      : "m"(src)
      : "memory");
}

bool TestFpuX87PrecisionControl::Test() {
  // 64-bit patterns whose low 29 bits are non-zero.
  static constexpr uint64_t kPatterns[] = {
      0xc80008f8c0023c2fULL,  // Actual Mega Man 2 packed sprite entry.
      0x0123456789abcdefULL,
      0x3ff0000000000001ULL,  // Double with mantissa LSB set.
  };

  static constexpr struct {
    const char* name;
    uint16_t pc_bits;
  } kModes[] = {
      {"Single precision (PC = 00b)", 0x0000},
      {"Double precision (PC = 10b)", 0x0200},
      {"Extended precision (PC = 11b)", 0x0300},
  };

  const uint16_t original_cw = ReadFpuControlWord();
  bool pass = true;

  for (const auto& mode : kModes) {
    const uint16_t cw =
        static_cast<uint16_t>((original_cw & ~0x0300) | mode.pc_bits);
    WriteFpuControlWord(cw);
    TEST_SCOPE(mode.name);

    for (const auto& pattern : kPatterns) {
      uint64_t dst = 0;
      const uint64_t src = pattern;
      CopyQwordViaX87(dst, src);

      if (dst == src) {
        TEST_PASS("qword copy preserved: 0x%08x%08x",
                  static_cast<unsigned>(dst >> 32), static_cast<unsigned>(dst));
      } else {
        TEST_FAIL_EX(
            TestFields("precision_control", mode.name, "expected", Hex64(src),
                       "actual", Hex64(dst)),
            "qword copy 0x%08x%08x != 0x%08x%08x",
            static_cast<unsigned>(dst >> 32), static_cast<unsigned>(dst),
            static_cast<unsigned>(src >> 32), static_cast<unsigned>(src));
        pass = false;
      }
    }
  }

  WriteFpuControlWord(original_cw);
  if (ReadFpuControlWord() == original_cw && ReadFpuStackEmpty()) {
    TEST_PASS("FPU control word restored; register stack empty");
  } else {
    TEST_FAIL("FPU control word not restored or register stack not empty");
    pass = false;
  }

  return pass;
}
