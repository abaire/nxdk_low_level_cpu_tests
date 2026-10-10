#include "sse_tests/test_sse_approximations.h"

#include <cmath>
#include <cstdint>
#include <cstring>

#include "sse_tests/sse_runner_case.h"
#include "sse_tests/sse_test_common.h"

static void RunRcpss(uint32_t out[4], const uint32_t in[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "rcpss xmm0, xmm0\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(in)
      : "xmm0", "memory");
}

static void RunRcpps(uint32_t out[4], const uint32_t in[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "rcpps xmm0, xmm0\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(in)
      : "xmm0", "memory");
}

static void RunRsqrtss(uint32_t out[4], const uint32_t in[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "rsqrtss xmm0, xmm0\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(in)
      : "xmm0", "memory");
}

static void RunRsqrtps(uint32_t out[4], const uint32_t in[4]) {
  __asm__ __volatile__(
      "movaps xmm0, xmmword ptr [%1]\n\t"
      "rsqrtps xmm0, xmm0\n\t"
      "movaps xmmword ptr [%0], xmm0\n\t"
      :
      : "r"(out), "r"(in)
      : "xmm0", "memory");
}

static float AsFloat(uint32_t u) {
  float f;
  std::memcpy(&f, &u, sizeof(f));
  return f;
}

static uint32_t AsUint32(float f) {
  uint32_t u;
  std::memcpy(&u, &f, sizeof(u));
  return u;
}

bool TestSseApproximations::Test() {
  LogMsg("\n============================================================\n");
  LogMsg("Test: SSE Approximations (rcp/rsqrt)\n");

  const uint32_t orig_mxcsr = ReadMxcsr();
  bool pass = true;

  alignas(16) uint32_t in[4] = {0};
  alignas(16) uint32_t out[4] = {0};

  // 1. rcpss relative error bound across test values (error bound <= 1.5 *
  // 2^-12 = 0.0003662)
  static constexpr float kRcpValues[] = {1.0f,  2.0f, 0.5f,  4.0f,
                                         0.25f, 3.0f, 10.0f, 100.0f};
  for (float val : kRcpValues) {
    in[0] = AsUint32(val);
    RunRcpss(out, in);
    float true_rcp = 1.0f / val;
    float res = AsFloat(out[0]);
    float rel_err = std::fabs((res - true_rcp) / true_rcp);
    if (rel_err <= 0.0003662109375f) {
      LogMsg("  [PASS] rcpss(%f) = %f (0x%08X), rel_err = %e\n", val, res,
             out[0], rel_err);
    } else {
      LogMsg("  [FAIL] rcpss(%f) rel_err %e exceeds 1.5*2^-12\n", val, rel_err);
      pass = false;
    }
  }

  // 2. rcpss exact hardware baseline bit patterns on Coppermine
  static constexpr struct {
    float input;
    uint32_t expected_hex;
  } kRcpBaselines[] = {
      {1.0f, 0x3F7FF000}, {2.0f, 0x3EFFF000},  {0.5f, 0x3FFFF000},
      {4.0f, 0x3E7FF000}, {0.25f, 0x407FF000},
  };
  for (const auto& item : kRcpBaselines) {
    in[0] = AsUint32(item.input);
    RunRcpss(out, in);
    if (out[0] == item.expected_hex) {
      LogMsg("  [PASS] rcpss(%f) hardware bit pattern exact: 0x%08X\n",
             item.input, out[0]);
    } else {
      LogMsg("  [FAIL] rcpss(%f) expected 0x%08X got 0x%08X\n", item.input,
             item.expected_hex, out[0]);
      pass = false;
    }
  }

  // 3. rcpss boundary conditions (+0.0 -> +Inf, -0.0 -> -Inf)
  // Per Intel reference: approximation instructions generate NO numeric
  // exceptions.
  WriteMxcsr(orig_mxcsr & ~0x3Fu);
  in[0] = AsUint32(+0.0f);
  RunRcpss(out, in);
  uint32_t sticky_flags = ReadMxcsr() & 0x3Fu;
  if (out[0] == 0x7F800000 && sticky_flags == 0) {
    LogMsg("  [PASS] rcpss(+0.0f) == +Inf (0x%08X) with no MXCSR exceptions\n",
           out[0]);
  } else {
    LogMsg("  [FAIL] rcpss(+0.0f) exp +Inf, flags=0 got 0x%08X flags=0x%02X\n",
           out[0], sticky_flags);
    pass = false;
  }

  WriteMxcsr(orig_mxcsr & ~0x3Fu);
  in[0] = AsUint32(-0.0f);
  RunRcpss(out, in);
  sticky_flags = ReadMxcsr() & 0x3Fu;
  if (out[0] == 0xFF800000 && sticky_flags == 0) {
    LogMsg("  [PASS] rcpss(-0.0f) == -Inf (0x%08X) with no MXCSR exceptions\n",
           out[0]);
  } else {
    LogMsg("  [FAIL] rcpss(-0.0f) exp -Inf, flags=0 got 0x%08X flags=0x%02X\n",
           out[0], sticky_flags);
    pass = false;
  }

  // 4. rsqrtss relative error bound across test values
  static constexpr float kRsqrtValues[] = {1.0f, 4.0f, 0.25f, 16.0f,
                                           2.0f, 3.0f, 10.0f};
  for (float val : kRsqrtValues) {
    in[0] = AsUint32(val);
    RunRsqrtss(out, in);
    float true_rsqrt = 1.0f / std::sqrt(val);
    float res = AsFloat(out[0]);
    float rel_err = std::fabs((res - true_rsqrt) / true_rsqrt);
    if (rel_err <= 0.0003662109375f) {
      LogMsg("  [PASS] rsqrtss(%f) = %f (0x%08X), rel_err = %e\n", val, res,
             out[0], rel_err);
    } else {
      LogMsg("  [FAIL] rsqrtss(%f) rel_err %e exceeds 1.5*2^-12\n", val,
             rel_err);
      pass = false;
    }
  }

  // 5. rsqrtss exact hardware baseline bit patterns on Coppermine
  static constexpr struct {
    float input;
    uint32_t expected_hex;
  } kRsqrtBaselines[] = {
      {1.0f, 0x3F7FF000},
      {4.0f, 0x3EFFF000},
      {0.25f, 0x3FFFF000},
      {16.0f, 0x3E7FF000},
  };
  for (const auto& item : kRsqrtBaselines) {
    in[0] = AsUint32(item.input);
    RunRsqrtss(out, in);
    if (out[0] == item.expected_hex) {
      LogMsg("  [PASS] rsqrtss(%f) hardware bit pattern exact: 0x%08X\n",
             item.input, out[0]);
    } else {
      LogMsg("  [FAIL] rsqrtss(%f) expected 0x%08X got 0x%08X\n", item.input,
             item.expected_hex, out[0]);
      pass = false;
    }
  }

  // 6. rsqrtss on negative number (e.g. -1.0f -> QNaN 0xFFC00000, no numeric
  // exceptions)
  WriteMxcsr(orig_mxcsr & ~0x3Fu);
  in[0] = AsUint32(-1.0f);
  RunRsqrtss(out, in);
  sticky_flags = ReadMxcsr() & 0x3Fu;
  if (out[0] == 0xFFC00000 && sticky_flags == 0) {
    LogMsg(
        "  [PASS] rsqrtss(-1.0f) == QNaN (0x%08X) with no MXCSR exceptions\n",
        out[0]);
  } else {
    LogMsg(
        "  [FAIL] rsqrtss(-1.0f) exp 0xFFC00000 flags=0 got 0x%08X "
        "flags=0x%02X\n",
        out[0], sticky_flags);
    pass = false;
  }

  // 7. Vector versions: rcpps and rsqrtps lane parity
  in[0] = AsUint32(1.0f);
  in[1] = AsUint32(2.0f);
  in[2] = AsUint32(3.0f);
  in[3] = AsUint32(4.0f);
  RunRcpps(out, in);
  alignas(16) uint32_t scalar_rcp[4] = {0};
  bool rcpps_pass = true;
  for (int i = 0; i < 4; ++i) {
    alignas(16) uint32_t sc_in[4] = {in[i], 0, 0, 0};
    RunRcpss(scalar_rcp, sc_in);
    if (out[i] != scalar_rcp[0]) {
      LogMsg("  [FAIL] rcpps lane %d (0x%08X) != rcpss (0x%08X)\n", i, out[i],
             scalar_rcp[0]);
      rcpps_pass = false;
      pass = false;
    }
  }
  if (rcpps_pass) {
    LogMsg("  [PASS] rcpps matches rcpss across all 4 vector lanes\n");
  }

  RunRsqrtps(out, in);
  bool rsqrtps_pass = true;
  for (int i = 0; i < 4; ++i) {
    alignas(16) uint32_t sc_in[4] = {in[i], 0, 0, 0};
    RunRsqrtss(scalar_rcp, sc_in);
    if (out[i] != scalar_rcp[0]) {
      LogMsg("  [FAIL] rsqrtps lane %d (0x%08X) != rsqrtss (0x%08X)\n", i,
             out[i], scalar_rcp[0]);
      rsqrtps_pass = false;
      pass = false;
    }
  }
  if (rsqrtps_pass) {
    LogMsg("  [PASS] rsqrtps matches rsqrtss across all 4 vector lanes\n");
  }

  WriteMxcsr(orig_mxcsr);
  return pass;
}
