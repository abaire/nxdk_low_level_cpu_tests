#include "test_testfloat_f32.h"

extern "C" {
#include "softfloat.h"
#include "subjfloat.h"
}
#include "testfloat_test_common.h"

bool TestFloatF32::Test() {
  InitializeTestFloatEnvironment();
  bool pass = true;

  if (!RunTestFloatBinaryF32("f32_add", f32_add, subj_f32_add)) {
    pass = false;
  }
  if (!RunTestFloatBinaryF32("f32_sub", f32_sub, subj_f32_sub)) {
    pass = false;
  }
  if (!RunTestFloatBinaryF32("f32_mul", f32_mul, subj_f32_mul)) {
    pass = false;
  }
  if (!RunTestFloatBinaryF32("f32_div", f32_div, subj_f32_div)) {
    pass = false;
  }
  if (!RunTestFloatUnaryF32("f32_sqrt", f32_sqrt, subj_f32_sqrt)) {
    pass = false;
  }
  if (!RunTestFloatCompareF32("f32_eq", f32_eq, subj_f32_eq)) {
    pass = false;
  }
  if (!RunTestFloatCompareF32("f32_le", f32_le, subj_f32_le)) {
    pass = false;
  }
  if (!RunTestFloatCompareF32("f32_lt", f32_lt, subj_f32_lt)) {
    pass = false;
  }

  return pass;
}
