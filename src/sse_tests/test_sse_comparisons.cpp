#include "sse_tests/test_sse_comparisons.h"

#include <cstdint>

#include "sse_tests/sse_runner_case.h"
#include "sse_tests/sse_test_common.h"

// Macro to generate a wrapper executing cmpps with a literal immediate
#define DEFINE_CMPPS_FN(NAME, IMM)                       \
  static void NAME(uint32_t out[4], const uint32_t a[4], \
                   const uint32_t b[4]) {                \
    __asm__ __volatile__(                                \
        "movaps xmm0, xmmword ptr [%1]\n\t"              \
        "movaps xmm1, xmmword ptr [%2]\n\t"              \
        "cmpps xmm0, xmm1, " #IMM                        \
        "\n\t"                                           \
        "movaps xmmword ptr [%0], xmm0\n\t"              \
        :                                                \
        : "r"(out), "r"(a), "r"(b)                       \
        : "xmm0", "xmm1", "memory");                     \
  }

DEFINE_CMPPS_FN(RunCmppsEq, 0)
DEFINE_CMPPS_FN(RunCmppsLt, 1)
DEFINE_CMPPS_FN(RunCmppsLe, 2)
DEFINE_CMPPS_FN(RunCmppsUnord, 3)
DEFINE_CMPPS_FN(RunCmppsNeq, 4)
DEFINE_CMPPS_FN(RunCmppsNlt, 5)
DEFINE_CMPPS_FN(RunCmppsNle, 6)
DEFINE_CMPPS_FN(RunCmppsOrd, 7)

#undef DEFINE_CMPPS_FN

static uint32_t RunComiss(const uint32_t a[4], const uint32_t b[4]) {
  uint32_t flags = 0;
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "comiss xmm0, xmm1\n\t"
      "pushfd\n\t"
      "pop %0\n\t"
      : "=r"(flags)
      : "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
  return flags;
}

static uint32_t RunUcomiss(const uint32_t a[4], const uint32_t b[4]) {
  uint32_t flags = 0;
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "ucomiss xmm0, xmm1\n\t"
      "pushfd\n\t"
      "pop %0\n\t"
      : "=r"(flags)
      : "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
  return flags;
}

bool TestSseComparisons::Test() {
  static constexpr uint32_t kTrue = 0xFFFFFFFF;
  static constexpr uint32_t kFalse = 0x00000000;

  static constexpr uint32_t kZero = 0x00000000;  // 0.0f
  static constexpr uint32_t kOne = 0x3F800000;   // 1.0f
  static constexpr uint32_t kTwo = 0x40000000;   // 2.0f
  static constexpr uint32_t kQNaN = 0x7FC00000;  // Quiet NaN

  // EFLAGS bitmasks: CF is bit 0, PF is bit 2, ZF is bit 6
  static constexpr uint32_t kCfMask = 1u << 0;
  static constexpr uint32_t kPfMask = 1u << 2;
  static constexpr uint32_t kZfMask = 1u << 6;
  static constexpr uint32_t kEflagsCompareMask = kCfMask | kPfMask | kZfMask;

  bool pass = true;

  // 1. Verify cmpps predicates 0 to 7 across 4 vector lanes
  // Lanes:
  // Lane 0: 1.0f vs 1.0f (Equal)
  // Lane 1: 1.0f vs 2.0f (Less than)
  // Lane 2: 2.0f vs 1.0f (Greater than)
  // Lane 3: 1.0f vs QNaN (Unordered)
  alignas(16) const uint32_t a[4] = {kOne, kOne, kTwo, kOne};
  alignas(16) const uint32_t b[4] = {kOne, kTwo, kOne, kQNaN};
  alignas(16) uint32_t out[4] = {0};

  struct CmppsTest {
    const char* name;
    void (*fn)(uint32_t[4], const uint32_t[4], const uint32_t[4]);
    uint32_t expected[4];
  } kCmppsTests[] = {
      {"EQ (0)", RunCmppsEq, {kTrue, kFalse, kFalse, kFalse}},
      {"LT (1)", RunCmppsLt, {kFalse, kTrue, kFalse, kFalse}},
      {"LE (2)", RunCmppsLe, {kTrue, kTrue, kFalse, kFalse}},
      {"UNORD (3)", RunCmppsUnord, {kFalse, kFalse, kFalse, kTrue}},
      {"NEQ (4)", RunCmppsNeq, {kFalse, kTrue, kTrue, kTrue}},
      {"NLT (5)", RunCmppsNlt, {kTrue, kFalse, kTrue, kTrue}},
      {"NLE (6)", RunCmppsNle, {kFalse, kFalse, kTrue, kTrue}},
      {"ORD (7)", RunCmppsOrd, {kTrue, kTrue, kTrue, kFalse}},
  };

  for (const auto& t : kCmppsTests) {
    t.fn(out, a, b);
    if (out[0] == t.expected[0] && out[1] == t.expected[1] &&
        out[2] == t.expected[2] && out[3] == t.expected[3]) {
      TEST_PASS("cmpps %-10s", t.name);
    } else {
      TEST_FAIL_EX(
          TestFields("predicate", t.name, "expected0", Hex32(t.expected[0]),
                     "expected1", Hex32(t.expected[1]), "expected2",
                     Hex32(t.expected[2]), "expected3", Hex32(t.expected[3]),
                     "actual0", Hex32(out[0]), "actual1", Hex32(out[1]),
                     "actual2", Hex32(out[2]), "actual3", Hex32(out[3])),
          "cmpps %-10s exp (0x%08X,0x%08X,0x%08X,0x%08X) got "
          "(0x%08X,0x%08X,0x%08X,0x%08X)",
          t.name, t.expected[0], t.expected[1], t.expected[2], t.expected[3],
          out[0], out[1], out[2], out[3]);
      pass = false;
    }
  }

  // 2. Verify comiss and ucomiss EFLAGS settings
  struct FlagsTest {
    const char* name;
    uint32_t a_val;
    uint32_t b_val;
    uint32_t exp_flags;
  } kFlagsTests[] = {
      {"Equal (1.0 == 1.0)", kOne, kOne, kZfMask},
      {"Less than (1.0 < 2.0)", kOne, kTwo, kCfMask},
      {"Greater than (2.0 > 1.0)", kTwo, kOne, 0},
      {"Unordered (1.0 ? NaN)", kOne, kQNaN, kZfMask | kPfMask | kCfMask},
  };

  for (const auto& ft : kFlagsTests) {
    alignas(16) const uint32_t op_a[4] = {ft.a_val, 0, 0, 0};
    alignas(16) const uint32_t op_b[4] = {ft.b_val, 0, 0, 0};

    uint32_t flags = RunComiss(op_a, op_b) & kEflagsCompareMask;
    if (flags == ft.exp_flags) {
      TEST_PASS("comiss  %-22s flags: ZF=%d PF=%d CF=%d", ft.name,
                (flags & kZfMask) != 0, (flags & kPfMask) != 0,
                (flags & kCfMask) != 0);
    } else {
      TEST_FAIL_EX(
          TestFields("op", "comiss", "case", ft.name, "expected_eflags",
                     Hex32(ft.exp_flags), "actual_eflags", Hex32(flags)),
          "comiss  %-22s exp flags: 0x%02X got 0x%02X", ft.name, ft.exp_flags,
          flags);
      pass = false;
    }

    flags = RunUcomiss(op_a, op_b) & kEflagsCompareMask;
    if (flags == ft.exp_flags) {
      TEST_PASS("ucomiss %-22s flags: ZF=%d PF=%d CF=%d", ft.name,
                (flags & kZfMask) != 0, (flags & kPfMask) != 0,
                (flags & kCfMask) != 0);
    } else {
      TEST_FAIL_EX(
          TestFields("op", "ucomiss", "case", ft.name, "expected_eflags",
                     Hex32(ft.exp_flags), "actual_eflags", Hex32(flags)),
          "ucomiss %-22s exp flags: 0x%02X got 0x%02X", ft.name, ft.exp_flags,
          flags);
      pass = false;
    }
  }

  // 3. Verify comiss vs ucomiss on Quiet NaN setting MXCSR Invalid Operation
  // flag (bit 0)
  const uint32_t orig_mxcsr = ReadMxcsr();
  alignas(16) const uint32_t qnan_a[4] = {kOne, 0, 0, 0};
  alignas(16) const uint32_t qnan_b[4] = {kQNaN, 0, 0, 0};

  // ucomiss: clear sticky flags, execute with QNaN, verify IE (bit 0) is NOT
  // set
  WriteMxcsr(orig_mxcsr & ~0x3Fu);
  RunUcomiss(qnan_a, qnan_b);
  uint32_t ucom_mxcsr = ReadMxcsr();
  if ((ucom_mxcsr & 0x01u) == 0) {
    TEST_PASS("ucomiss with QNaN did not set MXCSR Invalid Flag (IE=0)");
  } else {
    TEST_FAIL("ucomiss with QNaN erroneously set MXCSR Invalid Flag (IE=1)");
    pass = false;
  }

  // comiss: clear sticky flags, execute with QNaN, verify IE (bit 0) IS set
  WriteMxcsr(orig_mxcsr & ~0x3Fu);
  RunComiss(qnan_a, qnan_b);
  uint32_t com_mxcsr = ReadMxcsr();
  if ((com_mxcsr & 0x01u) == 0x01u) {
    TEST_PASS("comiss with QNaN set MXCSR Invalid Flag (IE=1)");
  } else {
    TEST_FAIL("comiss with QNaN did not set MXCSR Invalid Flag (IE=0)");
    pass = false;
  }

  WriteMxcsr(orig_mxcsr);
  return pass;
}
