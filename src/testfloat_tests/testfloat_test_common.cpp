#include "testfloat_test_common.h"

#include <cstdarg>
#include <cstdio>

#include "logger.h"
#include "printf/printf.h"

extern "C" {
#include "genCases.h"
#include "subjfloat.h"
#include "testLoops.h"
#include "verCases.h"
}

static std::deque<std::string> g_on_screen_log;
static constexpr size_t kMaxOnScreenLogLines = 18;

void LogMsg(const char* fmt, ...) {
  char buf[512];
  va_list args;
  va_start(args, fmt);
  vsnprintf_(buf, sizeof(buf), fmt, args);
  va_end(args);

  DbgPrint("%s", buf);

  if (Logger::IsInitialized()) {
    Logger::Log() << buf;
  }

  std::string s(buf);
  while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
    s.pop_back();
  }
  if (!s.empty()) {
    g_on_screen_log.push_back(s);
    if (g_on_screen_log.size() > kMaxOnScreenLogLines) {
      g_on_screen_log.pop_front();
    }
  }
}

const std::deque<std::string>& GetOnScreenLog() { return g_on_screen_log; }

void ClearOnScreenLog() { g_on_screen_log.clear(); }

void InitializeTestFloatEnvironment() {
  testLoops_trueFlagsPtr = &softfloat_exceptionFlags;
  testLoops_subjFlagsFunction = subjfloat_clearExceptionFlags;
  genCases_setLevel(1);
  verCases_maxErrorCount = 20;
  verCases_errorStop = false;
  verCases_checkNaNs = false;
  verCases_checkInvInts = false;
}

static const uint_fast8_t kStandardRoundingModes[] = {
    softfloat_round_near_even,
    softfloat_round_minMag,
    softfloat_round_min,
    softfloat_round_max,
};

static const char* RoundingModeName(uint_fast8_t mode) {
  switch (mode) {
    case softfloat_round_near_even:
      return "near_even";
    case softfloat_round_minMag:
      return "minMag";
    case softfloat_round_min:
      return "min";
    case softfloat_round_max:
      return "max";
    default:
      return "unknown";
  }
}

bool RunTestFloatBinaryF32(const char* name,
                           float32_t (*true_fn)(float32_t, float32_t),
                           float32_t (*subj_fn)(float32_t, float32_t)) {
  bool all_pass = true;
  verCases_functionNamePtr = name;

  for (uint_fast8_t mode : kStandardRoundingModes) {
    softfloat_roundingMode = mode;
    subjfloat_setRoundingMode(mode);
    verCases_errorCount = 0;
    verCases_tenThousandsCount = 0;

    test_abz_f32(true_fn, subj_fn);

    if (verCases_errorCount > 0) {
      LogMsg("  [FAIL] %s (%s): %u error(s)\n", name, RoundingModeName(mode),
             static_cast<unsigned>(verCases_errorCount));
      all_pass = false;
    }
  }

  if (all_pass) {
    LogMsg("  [PASS] %s across all rounding modes\n", name);
  }
  return all_pass;
}

bool RunTestFloatUnaryF32(const char* name, float32_t (*true_fn)(float32_t),
                          float32_t (*subj_fn)(float32_t)) {
  bool all_pass = true;
  verCases_functionNamePtr = name;

  for (uint_fast8_t mode : kStandardRoundingModes) {
    softfloat_roundingMode = mode;
    subjfloat_setRoundingMode(mode);
    verCases_errorCount = 0;
    verCases_tenThousandsCount = 0;

    test_az_f32(true_fn, subj_fn);

    if (verCases_errorCount > 0) {
      LogMsg("  [FAIL] %s (%s): %u error(s)\n", name, RoundingModeName(mode),
             static_cast<unsigned>(verCases_errorCount));
      all_pass = false;
    }
  }

  if (all_pass) {
    LogMsg("  [PASS] %s across all rounding modes\n", name);
  }
  return all_pass;
}

bool RunTestFloatCompareF32(const char* name,
                            bool (*true_fn)(float32_t, float32_t),
                            bool (*subj_fn)(float32_t, float32_t)) {
  verCases_functionNamePtr = name;
  softfloat_roundingMode = softfloat_round_near_even;
  subjfloat_setRoundingMode(softfloat_round_near_even);
  verCases_errorCount = 0;
  verCases_tenThousandsCount = 0;

  test_ab_f32_z_bool(true_fn, subj_fn);

  if (verCases_errorCount > 0) {
    LogMsg("  [FAIL] %s: %u error(s)\n", name,
           static_cast<unsigned>(verCases_errorCount));
    return false;
  }
  LogMsg("  [PASS] %s\n", name);
  return true;
}

bool RunTestFloatBinaryF64(const char* name,
                           float64_t (*true_fn)(float64_t, float64_t),
                           float64_t (*subj_fn)(float64_t, float64_t)) {
  bool all_pass = true;
  verCases_functionNamePtr = name;

  for (uint_fast8_t mode : kStandardRoundingModes) {
    softfloat_roundingMode = mode;
    subjfloat_setRoundingMode(mode);
    verCases_errorCount = 0;
    verCases_tenThousandsCount = 0;

    test_abz_f64(true_fn, subj_fn);

    if (verCases_errorCount > 0) {
      LogMsg("  [FAIL] %s (%s): %u error(s)\n", name, RoundingModeName(mode),
             static_cast<unsigned>(verCases_errorCount));
      all_pass = false;
    }
  }

  if (all_pass) {
    LogMsg("  [PASS] %s across all rounding modes\n", name);
  }
  return all_pass;
}

bool RunTestFloatUnaryF64(const char* name, float64_t (*true_fn)(float64_t),
                          float64_t (*subj_fn)(float64_t)) {
  bool all_pass = true;
  verCases_functionNamePtr = name;

  for (uint_fast8_t mode : kStandardRoundingModes) {
    softfloat_roundingMode = mode;
    subjfloat_setRoundingMode(mode);
    verCases_errorCount = 0;
    verCases_tenThousandsCount = 0;

    test_az_f64(true_fn, subj_fn);

    if (verCases_errorCount > 0) {
      LogMsg("  [FAIL] %s (%s): %u error(s)\n", name, RoundingModeName(mode),
             static_cast<unsigned>(verCases_errorCount));
      all_pass = false;
    }
  }

  if (all_pass) {
    LogMsg("  [PASS] %s across all rounding modes\n", name);
  }
  return all_pass;
}

bool RunTestFloatCompareF64(const char* name,
                            bool (*true_fn)(float64_t, float64_t),
                            bool (*subj_fn)(float64_t, float64_t)) {
  verCases_functionNamePtr = name;
  softfloat_roundingMode = softfloat_round_near_even;
  subjfloat_setRoundingMode(softfloat_round_near_even);
  verCases_errorCount = 0;
  verCases_tenThousandsCount = 0;

  test_ab_f64_z_bool(true_fn, subj_fn);

  if (verCases_errorCount > 0) {
    LogMsg("  [FAIL] %s: %u error(s)\n", name,
           static_cast<unsigned>(verCases_errorCount));
    return false;
  }
  LogMsg("  [PASS] %s\n", name);
  return true;
}

bool RunTestFloatBinaryExtF80(
    const char* name,
    void (*true_fn)(const extFloat80_t*, const extFloat80_t*, extFloat80_t*),
    void (*subj_fn)(const extFloat80_t*, const extFloat80_t*, extFloat80_t*)) {
  bool all_pass = true;
  verCases_functionNamePtr = name;
  static const uint_fast8_t kPrecisions[] = {32, 64, 80};

  for (uint_fast8_t prec : kPrecisions) {
    extF80_roundingPrecision = prec;
    subjfloat_setExtF80RoundingPrecision(prec);
    verCases_roundingPrecision = prec;

    for (uint_fast8_t mode : kStandardRoundingModes) {
      softfloat_roundingMode = mode;
      subjfloat_setRoundingMode(mode);
      verCases_errorCount = 0;
      verCases_tenThousandsCount = 0;

      test_abz_extF80(true_fn, subj_fn);

      if (verCases_errorCount > 0) {
        LogMsg("  [FAIL] %s (prec %u, %s): %u error(s)\n", name,
               static_cast<unsigned>(prec), RoundingModeName(mode),
               static_cast<unsigned>(verCases_errorCount));
        all_pass = false;
      }
    }
  }

  if (all_pass) {
    LogMsg("  [PASS] %s across all precision & rounding modes\n", name);
  }
  return all_pass;
}

bool RunTestFloatUnaryExtF80(
    const char* name, void (*true_fn)(const extFloat80_t*, extFloat80_t*),
    void (*subj_fn)(const extFloat80_t*, extFloat80_t*)) {
  bool all_pass = true;
  verCases_functionNamePtr = name;
  static const uint_fast8_t kPrecisions[] = {32, 64, 80};

  for (uint_fast8_t prec : kPrecisions) {
    extF80_roundingPrecision = prec;
    subjfloat_setExtF80RoundingPrecision(prec);
    verCases_roundingPrecision = prec;

    for (uint_fast8_t mode : kStandardRoundingModes) {
      softfloat_roundingMode = mode;
      subjfloat_setRoundingMode(mode);
      verCases_errorCount = 0;
      verCases_tenThousandsCount = 0;

      test_az_extF80(true_fn, subj_fn);

      if (verCases_errorCount > 0) {
        LogMsg("  [FAIL] %s (prec %u, %s): %u error(s)\n", name,
               static_cast<unsigned>(prec), RoundingModeName(mode),
               static_cast<unsigned>(verCases_errorCount));
        all_pass = false;
      }
    }
  }

  if (all_pass) {
    LogMsg("  [PASS] %s across all precision & rounding modes\n", name);
  }
  return all_pass;
}

bool RunTestFloatCompareExtF80(
    const char* name, bool (*true_fn)(const extFloat80_t*, const extFloat80_t*),
    bool (*subj_fn)(const extFloat80_t*, const extFloat80_t*)) {
  verCases_functionNamePtr = name;
  softfloat_roundingMode = softfloat_round_near_even;
  subjfloat_setRoundingMode(softfloat_round_near_even);
  extF80_roundingPrecision = 80;
  subjfloat_setExtF80RoundingPrecision(80);
  verCases_errorCount = 0;
  verCases_tenThousandsCount = 0;

  test_ab_extF80_z_bool(true_fn, subj_fn);

  if (verCases_errorCount > 0) {
    LogMsg("  [FAIL] %s: %u error(s)\n", name,
           static_cast<unsigned>(verCases_errorCount));
    return false;
  }
  LogMsg("  [PASS] %s\n", name);
  return true;
}
