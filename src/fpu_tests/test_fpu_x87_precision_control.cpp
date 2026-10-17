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

namespace {

enum class X87Op { kAdd, kSub, kMul, kDiv };

struct RoundingCase {
  const char* name;
  X87Op op;
  uint64_t a;
  uint64_t b;
  uint64_t expected;
};

}  // namespace

// Computes `a <op> b` with x87 `qword` memory operands and stores the
// rounded result back as a qword.
static uint64_t ComputeQwordViaX87(X87Op op, uint64_t a, uint64_t b) {
  uint64_t result = 0;
  switch (op) {
    case X87Op::kAdd:
      __asm__ __volatile__(
          "fld qword ptr %1\n\t"
          "fadd qword ptr %2\n\t"
          "fstp qword ptr %0\n\t"
          : "=m"(result)
          : "m"(a), "m"(b)
          : "memory");
      break;
    case X87Op::kSub:
      __asm__ __volatile__(
          "fld qword ptr %1\n\t"
          "fsub qword ptr %2\n\t"
          "fstp qword ptr %0\n\t"
          : "=m"(result)
          : "m"(a), "m"(b)
          : "memory");
      break;
    case X87Op::kMul:
      __asm__ __volatile__(
          "fld qword ptr %1\n\t"
          "fmul qword ptr %2\n\t"
          "fstp qword ptr %0\n\t"
          : "=m"(result)
          : "m"(a), "m"(b)
          : "memory");
      break;
    case X87Op::kDiv:
      __asm__ __volatile__(
          "fld qword ptr %1\n\t"
          "fdiv qword ptr %2\n\t"
          "fstp qword ptr %0\n\t"
          : "=m"(result)
          : "m"(a), "m"(b)
          : "memory");
      break;
  }
  return result;
}

bool TestFpuX87PrecisionControlArithmetic::Test() {
  // Every expected result is 0x1.000002p0 (0x3FF0000020000000); the
  // doubly rounded result is noted per case.
  static constexpr RoundingCase kCases[] = {
      // 0x1.0000010000001p0 - 0x1p-53 = 0x1.000001p0 + 2^-53. Double
      // rounding: 0x1.000001p0 (tie) -> 1.0.
      {"fsub just above halfway", X87Op::kSub, 0x3FF0000010000001ULL,
       0x3CA0000000000000ULL, 0x3FF0000020000000ULL},
      // 0x1.000001p0 + 0x1p-80. Double rounding: 0x1.000001p0 (tie) -> 1.0.
      {"fadd just above halfway", X87Op::kAdd, 0x3FF0000010000000ULL,
       0x3AF0000000000000ULL, 0x3FF0000020000000ULL},
      // 0x1.000003p0 - 0x1p-80. Double rounding: 0x1.000003p0 (tie) ->
      // 0x1.000004p0.
      {"fsub just below halfway", X87Op::kSub, 0x3FF0000030000000ULL,
       0x3AF0000000000000ULL, 0x3FF0000020000000ULL},
      // 0x1.000000f8p0 * 0x1.00000008p0 = 0x1.000001p0 + 2^-53 - 2^-58.
      // Double rounding: 0x1.000001p0 (tie) -> 1.0.
      {"fmul just above halfway", X87Op::kMul, 0x3FF000000F800000ULL,
       0x3FF0000000800000ULL, 0x3FF0000020000000ULL},
      // 0x1.0000040000040p0 * 0x1.fffffep-1 is just below 0x1.000003p0.
      // Double rounding: 0x1.000003p0 (tie) -> 0x1.000004p0.
      {"fmul just below halfway", X87Op::kMul, 0x3FF0000040000040ULL,
       0x3FEFFFFFE0000000ULL, 0x3FF0000020000000ULL},
      // 0x1.0000010800001p0 / 0x1.00000008p0 is just above 0x1.000001p0.
      // Double rounding: 0x1.000001p0 (tie) -> 1.0.
      {"fdiv just above halfway", X87Op::kDiv, 0x3FF0000010800001ULL,
       0x3FF0000000800000ULL, 0x3FF0000020000000ULL},
      // 0x1.00000302p0 / 0x1.00000002p0 is just below 0x1.000003p0.
      // Double rounding: 0x1.000003p0 (tie) -> 0x1.000004p0.
      {"fdiv just below halfway", X87Op::kDiv, 0x3FF0000030200000ULL,
       0x3FF0000000200000ULL, 0x3FF0000020000000ULL},
  };

  const uint16_t original_cw = ReadFpuControlWord();
  // Mask all exceptions (bits 0-5), PC = 00b (bits 8-9), RC = 00b (bits
  // 10-11).
  const uint16_t cw = static_cast<uint16_t>((original_cw & ~0x0F00) | 0x003F);
  bool pass = true;

  for (const auto& test_case : kCases) {
    WriteFpuControlWord(cw);
    const uint64_t actual =
        ComputeQwordViaX87(test_case.op, test_case.a, test_case.b);
    WriteFpuControlWord(original_cw);

    if (actual == test_case.expected) {
      TEST_PASS("%s: 0x%08x%08x", test_case.name,
                static_cast<unsigned>(actual >> 32),
                static_cast<unsigned>(actual));
    } else {
      TEST_FAIL_EX(
          TestFields("case", test_case.name, "a", Hex64(test_case.a), "b",
                     Hex64(test_case.b), "expected", Hex64(test_case.expected),
                     "actual", Hex64(actual)),
          "%s: 0x%08x%08x != 0x%08x%08x", test_case.name,
          static_cast<unsigned>(actual >> 32), static_cast<unsigned>(actual),
          static_cast<unsigned>(test_case.expected >> 32),
          static_cast<unsigned>(test_case.expected));
      pass = false;
    }
  }

  __asm__ __volatile__("fnclex");
  if (ReadFpuControlWord() == original_cw && ReadFpuStackEmpty()) {
    TEST_PASS("FPU control word restored; register stack empty");
  } else {
    TEST_FAIL("FPU control word not restored or register stack not empty");
    pass = false;
  }

  return pass;
}
