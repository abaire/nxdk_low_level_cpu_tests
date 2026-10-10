#include "test_testfloat_extf80.h"

extern "C" {
#include "softfloat.h"
#include "subjfloat.h"
}
#include "testfloat_test_common.h"

bool TestFloatExtF80::Test() {
  LogMsg("\n============================================================\n");
  LogMsg("Test: TestFloat ExtF80 Operations\n");

  InitializeTestFloatEnvironment();
  bool pass = true;

  if (!RunTestFloatBinaryExtF80("extF80_add", extF80M_add, subj_extF80M_add)) {
    pass = false;
  }
  if (!RunTestFloatBinaryExtF80("extF80_sub", extF80M_sub, subj_extF80M_sub)) {
    pass = false;
  }
  if (!RunTestFloatBinaryExtF80("extF80_mul", extF80M_mul, subj_extF80M_mul)) {
    pass = false;
  }
  if (!RunTestFloatBinaryExtF80("extF80_div", extF80M_div, subj_extF80M_div)) {
    pass = false;
  }
  if (!RunTestFloatUnaryExtF80("extF80_sqrt", extF80M_sqrt,
                               subj_extF80M_sqrt)) {
    pass = false;
  }
  if (!RunTestFloatCompareExtF80("extF80_eq", extF80M_eq, subj_extF80M_eq)) {
    pass = false;
  }
  if (!RunTestFloatCompareExtF80("extF80_le", extF80M_le, subj_extF80M_le)) {
    pass = false;
  }
  if (!RunTestFloatCompareExtF80("extF80_lt", extF80M_lt, subj_extF80M_lt)) {
    pass = false;
  }

  return pass;
}
