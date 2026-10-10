#include "sse_tests/test_sse_min_max.h"

#include <cstring>

#include "sse_tests/sse_runner_case.h"
#include "sse_tests/sse_test_common.h"

static void RunMinss(uint32_t out[4], const uint32_t a[4],
                     const uint32_t b[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "minss xmm0, xmm1\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
}

static void RunMaxss(uint32_t out[4], const uint32_t a[4],
                     const uint32_t b[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "maxss xmm0, xmm1\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
}

static void RunMinps(uint32_t out[4], const uint32_t a[4],
                     const uint32_t b[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "minps xmm0, xmm1\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
}

static void RunMaxps(uint32_t out[4], const uint32_t a[4],
                     const uint32_t b[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "movaps xmm1, xmmword ptr [%2]\n\t"
      "maxps xmm0, xmm1\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(a), "r"(b)
      : "xmm0", "xmm1", "memory");
}

bool TestSseMinMax::Test() {
  LogMsg("\n============================================================\n");
  LogMsg("Test: SSE Min/Max Non-IEEE Rules\n");

  static constexpr uint32_t kNaN = 0x7FC00000;      // Quiet NaN
  static constexpr uint32_t kOne = 0x3F800000;      // 1.0f
  static constexpr uint32_t kPosZero = 0x00000000;  // +0.0f
  static constexpr uint32_t kNegZero = 0x80000000;  // -0.0f

  bool pass = true;

  alignas(16) uint32_t a[4];
  alignas(16) uint32_t b[4];
  alignas(16) uint32_t out[4];

  // minss: NaN vs 1.0f -> 1.0f (SRC2)
  a[0] = kNaN;
  a[1] = a[2] = a[3] = 0;
  b[0] = kOne;
  b[1] = b[2] = b[3] = 0;
  RunMinss(out, a, b);
  if (out[0] == kOne) {
    LogMsg("  [PASS] minss(NaN, 1.0f) == 1.0f (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] minss(NaN, 1.0f) expected 0x%08X got 0x%08X\n", kOne,
           out[0]);
    pass = false;
  }

  // minss: 1.0f vs NaN -> NaN (SRC2)
  a[0] = kOne;
  b[0] = kNaN;
  RunMinss(out, a, b);
  if (out[0] == kNaN) {
    LogMsg("  [PASS] minss(1.0f, NaN) == NaN (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] minss(1.0f, NaN) expected 0x%08X got 0x%08X\n", kNaN,
           out[0]);
    pass = false;
  }

  // minss: +0.0f vs -0.0f -> -0.0f (SRC2)
  a[0] = kPosZero;
  b[0] = kNegZero;
  RunMinss(out, a, b);
  if (out[0] == kNegZero) {
    LogMsg("  [PASS] minss(+0.0f, -0.0f) == -0.0f (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] minss(+0.0f, -0.0f) expected 0x%08X got 0x%08X\n",
           kNegZero, out[0]);
    pass = false;
  }

  // minss: -0.0f vs +0.0f -> +0.0f (SRC2)
  a[0] = kNegZero;
  b[0] = kPosZero;
  RunMinss(out, a, b);
  if (out[0] == kPosZero) {
    LogMsg("  [PASS] minss(-0.0f, +0.0f) == +0.0f (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] minss(-0.0f, +0.0f) expected 0x%08X got 0x%08X\n",
           kPosZero, out[0]);
    pass = false;
  }

  // maxss: NaN vs 1.0f -> 1.0f (SRC2)
  a[0] = kNaN;
  b[0] = kOne;
  RunMaxss(out, a, b);
  if (out[0] == kOne) {
    LogMsg("  [PASS] maxss(NaN, 1.0f) == 1.0f (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] maxss(NaN, 1.0f) expected 0x%08X got 0x%08X\n", kOne,
           out[0]);
    pass = false;
  }

  // maxss: 1.0f vs NaN -> NaN (SRC2)
  a[0] = kOne;
  b[0] = kNaN;
  RunMaxss(out, a, b);
  if (out[0] == kNaN) {
    LogMsg("  [PASS] maxss(1.0f, NaN) == NaN (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] maxss(1.0f, NaN) expected 0x%08X got 0x%08X\n", kNaN,
           out[0]);
    pass = false;
  }

  // maxss: +0.0f vs -0.0f -> -0.0f (SRC2)
  a[0] = kPosZero;
  b[0] = kNegZero;
  RunMaxss(out, a, b);
  if (out[0] == kNegZero) {
    LogMsg("  [PASS] maxss(+0.0f, -0.0f) == -0.0f (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] maxss(+0.0f, -0.0f) expected 0x%08X got 0x%08X\n",
           kNegZero, out[0]);
    pass = false;
  }

  // maxss: -0.0f vs +0.0f -> +0.0f (SRC2)
  a[0] = kNegZero;
  b[0] = kPosZero;
  RunMaxss(out, a, b);
  if (out[0] == kPosZero) {
    LogMsg("  [PASS] maxss(-0.0f, +0.0f) == +0.0f (0x%08X)\n", out[0]);
  } else {
    LogMsg("  [FAIL] maxss(-0.0f, +0.0f) expected 0x%08X got 0x%08X\n",
           kPosZero, out[0]);
    pass = false;
  }

  // minps: vector lanes [NaN, 1.0f, +0.0f, -0.0f] vs [1.0f, NaN, -0.0f, +0.0f]
  a[0] = kNaN;
  a[1] = kOne;
  a[2] = kPosZero;
  a[3] = kNegZero;
  b[0] = kOne;
  b[1] = kNaN;
  b[2] = kNegZero;
  b[3] = kPosZero;
  RunMinps(out, a, b);
  if (out[0] == kOne && out[1] == kNaN && out[2] == kNegZero &&
      out[3] == kPosZero) {
    LogMsg(
        "  [PASS] minps vector [1.0f, NaN, -0.0f, +0.0f] "
        "(0x%08X,0x%08X,0x%08X,0x%08X)\n",
        out[0], out[1], out[2], out[3]);
  } else {
    LogMsg("  [FAIL] minps vector mismatch got (0x%08X,0x%08X,0x%08X,0x%08X)\n",
           out[0], out[1], out[2], out[3]);
    pass = false;
  }

  // maxps: vector lanes [NaN, 1.0f, +0.0f, -0.0f] vs [1.0f, NaN, -0.0f, +0.0f]
  RunMaxps(out, a, b);
  if (out[0] == kOne && out[1] == kNaN && out[2] == kNegZero &&
      out[3] == kPosZero) {
    LogMsg(
        "  [PASS] maxps vector [1.0f, NaN, -0.0f, +0.0f] "
        "(0x%08X,0x%08X,0x%08X,0x%08X)\n",
        out[0], out[1], out[2], out[3]);
  } else {
    LogMsg("  [FAIL] maxps vector mismatch got (0x%08X,0x%08X,0x%08X,0x%08X)\n",
           out[0], out[1], out[2], out[3]);
    pass = false;
  }

  return pass;
}
