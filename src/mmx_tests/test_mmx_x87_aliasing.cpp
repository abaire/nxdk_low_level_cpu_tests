#include "mmx_tests/test_mmx_x87_aliasing.h"

#include <cstdint>
#include <cstring>

#include "mmx_tests/mmx_test_common.h"

struct X87Env {
  uint32_t control_word;
  uint32_t status_word;
  uint32_t tag_word;
  uint32_t fip;
  uint16_t fcs;
  uint16_t opcode;
  uint32_t fdp;
  uint16_t fds;
  uint16_t reserved;
};

static void RunFninit() { __asm__ __volatile__("fninit" ::: "memory"); }

static void SaveX87Env(X87Env& env) {
  __asm__ __volatile__("fnstenv [%0]\n\t" : : "r"(&env) : "memory");
}

static void SaveFxsave(uint8_t fx_buf[512]) {
  __asm__ __volatile__("fxsave [%0]\n\t" : : "r"(fx_buf) : "memory");
}

bool TestMmxX87Aliasing::Test() {
  LogMsg("\n============================================================\n");
  LogMsg("Test: MMX/x87 Aliasing & EMMS\n");

  bool pass = true;

  // 1. Direct physical register aliasing (MM0 -> FP0, MM7 -> FP7) and exponent
  // = 0xFFFF in FXSAVE
  {
    RunFninit();
    const uint64_t kPatternMm0 = 0x1122334455667788ULL;
    const uint64_t kPatternMm7 = 0x8877665544332211ULL;

    __asm__ __volatile__(
        "movq mm0, %0\n\t"
        "movq mm7, %1\n\t"
        :
        : "m"(kPatternMm0), "m"(kPatternMm7)
        : "mm0", "mm7");

    alignas(16) uint8_t fx_buf[512] = {0};
    SaveFxsave(fx_buf);
    Emms();

    uint64_t fp0_mantissa = *reinterpret_cast<uint64_t*>(&fx_buf[32]);
    uint16_t fp0_exp = *reinterpret_cast<uint16_t*>(&fx_buf[40]);
    uint64_t fp7_mantissa = *reinterpret_cast<uint64_t*>(&fx_buf[144]);
    uint16_t fp7_exp = *reinterpret_cast<uint16_t*>(&fx_buf[152]);

    if (fp0_mantissa == kPatternMm0 && fp0_exp == 0xFFFF) {
      LogMsg("  [PASS] MM0 aliases FP0 mantissa 0x%016llX, exp=0x%04X\n",
             fp0_mantissa, fp0_exp);
    } else {
      LogMsg(
          "  [FAIL] FP0 expected mantissa 0x%016llX exp 0xFFFF, got "
          "0x%016llX exp 0x%04X\n",
          kPatternMm0, fp0_mantissa, fp0_exp);
      pass = false;
    }

    if (fp7_mantissa == kPatternMm7 && fp7_exp == 0xFFFF) {
      LogMsg("  [PASS] MM7 aliases FP7 mantissa 0x%016llX, exp=0x%04X\n",
             fp7_mantissa, fp7_exp);
    } else {
      LogMsg(
          "  [FAIL] FP7 expected mantissa 0x%016llX exp 0xFFFF, got "
          "0x%016llX exp 0x%04X\n",
          kPatternMm7, fp7_mantissa, fp7_exp);
      pass = false;
    }
  }

  // 2. Load x87 stack value (fld1) and read physical FP7 via MM7
  {
    RunFninit();
    uint64_t mm7_read = 0;

    __asm__ __volatile__(
        "fld1\n\t"
        "movq %0, mm7\n\t"
        "emms\n\t"
        : "=m"(mm7_read)
        :
        : "mm7", "memory");

    // 1.0 in 80-bit extended has explicit integer bit 1, so mantissa is
    // 0x8000000000000000ULL
    if (mm7_read == 0x8000000000000000ULL) {
      LogMsg("  [PASS] fld1 mantissa read from MM7: 0x%016llX\n", mm7_read);
    } else {
      LogMsg("  [FAIL] MM7 read expected 0x8000000000000000 got 0x%016llX\n",
             mm7_read);
      pass = false;
    }
  }

  // 3. x87 Tag Word and TOP transitions under MMX and EMMS
  {
    RunFninit();
    X87Env env_init = {0};
    SaveX87Env(env_init);
    alignas(16) uint8_t fx_init[512] = {0};
    SaveFxsave(fx_init);

    uint16_t tag_init = env_init.tag_word & 0xFFFF;
    uint8_t fx_tag_init = fx_init[4];
    uint16_t top_init = (env_init.status_word >> 11) & 0x7;

    if (tag_init == 0xFFFF && fx_tag_init == 0x00 && top_init == 0) {
      LogMsg("  [PASS] fninit: FNSTENV tag=0x%04X, FXSAVE tag=0x%02X, TOP=%d\n",
             tag_init, fx_tag_init, top_init);
    } else {
      LogMsg(
          "  [FAIL] fninit tag expected 0xFFFF/0x00/0 got 0x%04X/0x%02X/%d\n",
          tag_init, fx_tag_init, top_init);
      pass = false;
    }

    // Execute an MMX instruction: all tags should become valid (00b / 0x0000;
    // FXSAVE=0xFF)
    __asm__ __volatile__("pxor mm0, mm0\n\t" ::: "mm0");

    X87Env env_mmx = {0};
    SaveX87Env(env_mmx);
    alignas(16) uint8_t fx_mmx[512] = {0};
    SaveFxsave(fx_mmx);

    uint16_t tag_mmx = env_mmx.tag_word & 0xFFFF;
    uint8_t fx_tag_mmx = fx_mmx[4];
    uint16_t top_mmx = (env_mmx.status_word >> 11) & 0x7;

    bool no_empty_tags = true;
    for (int i = 0; i < 8; ++i) {
      if (((tag_mmx >> (2 * i)) & 0x3) == 0x3) {
        no_empty_tags = false;
        break;
      }
    }

    if (no_empty_tags && fx_tag_mmx == 0xFF && top_mmx == 0) {
      LogMsg(
          "  [PASS] MMX: FNSTENV tag=0x%04X (all non-empty), FXSAVE "
          "tag=0x%02X, "
          "TOP=%d\n",
          tag_mmx, fx_tag_mmx, top_mmx);
    } else {
      LogMsg(
          "  [FAIL] MMX tag expected non-empty/0xFF/0 got 0x%04X/0x%02X/%d\n",
          tag_mmx, fx_tag_mmx, top_mmx);
      pass = false;
    }

    // Execute EMMS: all tags should become empty (11b / 0xFFFF; FXSAVE=0x00)
    Emms();

    X87Env env_emms = {0};
    SaveX87Env(env_emms);
    alignas(16) uint8_t fx_emms[512] = {0};
    SaveFxsave(fx_emms);

    uint16_t tag_emms = env_emms.tag_word & 0xFFFF;
    uint8_t fx_tag_emms = fx_emms[4];
    uint16_t top_emms = (env_emms.status_word >> 11) & 0x7;

    if (tag_emms == 0xFFFF && fx_tag_emms == 0x00 && top_emms == 0) {
      LogMsg("  [PASS] EMMS: FNSTENV tag=0x%04X, FXSAVE tag=0x%02X, TOP=%d\n",
             tag_emms, fx_tag_emms, top_emms);
    } else {
      LogMsg("  [FAIL] EMMS tag expected 0xFFFF/0x00/0 got 0x%04X/0x%02X/%d\n",
             tag_emms, fx_tag_emms, top_emms);
      pass = false;
    }
  }

  // 4. Data retention across EMMS
  {
    const uint64_t kPatternMm3 = 0xCAFEBABE12345678ULL;
    uint64_t read_back = 0;

    __asm__ __volatile__(
        "movq mm3, %1\n\t"
        "emms\n\t"
        "movq %0, mm3\n\t"
        "emms\n\t"
        : "=m"(read_back)
        : "m"(kPatternMm3)
        : "mm3");

    if (read_back == kPatternMm3) {
      LogMsg("  [PASS] MM3 data retained across EMMS: 0x%016llX\n", read_back);
    } else {
      LogMsg("  [FAIL] MM3 expected 0x%016llX got 0x%016llX\n", kPatternMm3,
             read_back);
      pass = false;
    }
  }

  // 5. Stack overflow fault (SF=1, IE=1, C1=1) without EMMS
  {
    RunFninit();
    // MMX sets all tags to valid (00b)
    __asm__ __volatile__("pxor mm0, mm0\n\t" ::: "mm0");

    // fld1 pushes to stack (TOP decrements to 7, where tag is already valid) ->
    // Stack Overflow
    uint16_t sw = 0;
    __asm__ __volatile__(
        "fld1\n\t"
        "fnstsw %0\n\t"
        : "=m"(sw)
        :
        : "memory");

    Emms();
    RunFninit();

    // Mask for IE (bit 0 = 0x0001), SF (bit 6 = 0x0040), C1 (bit 9 = 0x0200)
    bool has_fault = (sw & 0x0241) == 0x0241;
    if (has_fault) {
      LogMsg(
          "  [PASS] fld1 without EMMS raised Stack Overflow Fault: "
          "FSW=0x%04X\n",
          sw);
    } else {
      LogMsg(
          "  [FAIL] fld1 without EMMS expected FSW bits 0x0241 (IE+SF+C1), got "
          "0x%04X\n",
          sw);
      pass = false;
    }
  }

  // 6. Clean stack push after EMMS
  {
    RunFninit();
    __asm__ __volatile__("pxor mm0, mm0\n\t" ::: "mm0");
    Emms();

    uint16_t sw = 0;
    __asm__ __volatile__(
        "fld1\n\t"
        "fnstsw %0\n\t"
        : "=m"(sw)
        :
        : "memory");

    RunFninit();
    Emms();

    bool clean_push = (sw & 0x0241) == 0;
    if (clean_push) {
      LogMsg("  [PASS] fld1 after EMMS pushed cleanly: FSW=0x%04X\n", sw);
    } else {
      LogMsg("  [FAIL] fld1 after EMMS had exception flags in FSW: 0x%04X\n",
             sw);
      pass = false;
    }
  }

  return pass;
}
