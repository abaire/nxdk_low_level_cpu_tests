#include "test_testfloat_f64.h"

extern "C" {
#include "softfloat.h"
#include "subjfloat.h"
}
#include "testfloat_test_common.h"

// Reference oracle for x87 double-precision multiplication.
// Models x87's 80-bit register intermediate (PC=64 / 53-bit precision)
// followed by store to 64-bit memory (FSTP qword).
static float64_t x87_oracle_f64_mul(float64_t a, float64_t b) {
  extFloat80_t extA, extB, extZ;
  f64_to_extF80M(a, &extA);
  f64_to_extF80M(b, &extB);

  uint_fast8_t orig_prec = extF80_roundingPrecision;
  extF80_roundingPrecision = 64;
  softfloat_exceptionFlags = 0;
  extF80M_mul(&extA, &extB, &extZ);
  uint_fast8_t op_flags = softfloat_exceptionFlags;

  softfloat_exceptionFlags = 0;
  float64_t z = extF80M_to_f64(&extZ);
  uint_fast8_t store_flags = softfloat_exceptionFlags;
  extF80_roundingPrecision = orig_prec;

  // On x87 with masked underflow exceptions:
  // FMUL/FDIV operates in 80-bit registers where numbers < 2^-1022 are normal,
  // so the arithmetic stage never raises the underflow (#U) flag.
  // The memory store (FSTP) only sets #U if the store operation itself
  // introduces rounding error (inexactness) during denormalization.
  uint_fast8_t final_flags = op_flags | store_flags;
  if (!(store_flags & softfloat_flag_inexact)) {
    final_flags &= ~softfloat_flag_underflow;
  }
  softfloat_exceptionFlags = final_flags;
  return z;
}

// Reference oracle for x87 double-precision division.
static float64_t x87_oracle_f64_div(float64_t a, float64_t b) {
  extFloat80_t extA, extB, extZ;
  f64_to_extF80M(a, &extA);
  f64_to_extF80M(b, &extB);

  uint_fast8_t orig_prec = extF80_roundingPrecision;
  extF80_roundingPrecision = 64;
  softfloat_exceptionFlags = 0;
  extF80M_div(&extA, &extB, &extZ);
  uint_fast8_t op_flags = softfloat_exceptionFlags;

  softfloat_exceptionFlags = 0;
  float64_t z = extF80M_to_f64(&extZ);
  uint_fast8_t store_flags = softfloat_exceptionFlags;
  extF80_roundingPrecision = orig_prec;

  uint_fast8_t final_flags = op_flags | store_flags;
  if (!(store_flags & softfloat_flag_inexact)) {
    final_flags &= ~softfloat_flag_underflow;
  }
  softfloat_exceptionFlags = final_flags;
  return z;
}

bool TestFloatF64::Test() {
  InitializeTestFloatEnvironment();
  bool pass = true;

  if (!RunTestFloatBinaryF64("f64_add", f64_add, subj_f64_add)) {
    pass = false;
  }
  if (!RunTestFloatBinaryF64("f64_sub", f64_sub, subj_f64_sub)) {
    pass = false;
  }
  if (!RunTestFloatBinaryF64("f64_mul", x87_oracle_f64_mul, subj_f64_mul)) {
    pass = false;
  }
  if (!RunTestFloatBinaryF64("f64_div", x87_oracle_f64_div, subj_f64_div)) {
    pass = false;
  }
  if (!RunTestFloatUnaryF64("f64_sqrt", f64_sqrt, subj_f64_sqrt)) {
    pass = false;
  }
  if (!RunTestFloatCompareF64("f64_eq", f64_eq, subj_f64_eq)) {
    pass = false;
  }
  if (!RunTestFloatCompareF64("f64_le", f64_le, subj_f64_le)) {
    pass = false;
  }
  if (!RunTestFloatCompareF64("f64_lt", f64_lt, subj_f64_lt)) {
    pass = false;
  }

  return pass;
}
