extern "C" {
#include "platform.h"
#include "softfloat.h"
#include "subjfloat.h"
#include "subjfloat_config.h"
}

void subjfloat_setRoundingMode(uint_fast8_t roundingMode) {
  uint16_t cw = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(cw));
  cw &= ~0x0C00;
  switch (roundingMode) {
    case softfloat_round_near_even:
      cw |= 0x0000;
      break;
    case softfloat_round_minMag:
      cw |= 0x0C00;
      break;
    case softfloat_round_min:
      cw |= 0x0400;
      break;
    case softfloat_round_max:
      cw |= 0x0800;
      break;
    default:
      break;
  }
  __asm__ __volatile__("fldcw word ptr %0" : : "m"(cw));
}

void subjfloat_setExtF80RoundingPrecision(uint_fast8_t roundingPrecision) {
  uint16_t cw = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(cw));
  cw &= ~0x0300;
  switch (roundingPrecision) {
    case 32:
      cw |= 0x0000;
      break;
    case 64:
      cw |= 0x0200;
      break;
    case 80:
      cw |= 0x0300;
      break;
    default:
      break;
  }
  __asm__ __volatile__("fldcw word ptr %0" : : "m"(cw));
}

uint_fast8_t subjfloat_clearExceptionFlags(void) {
  uint16_t sw = 0;
  __asm__ __volatile__("fnstsw word ptr %0" : "=m"(sw));
  __asm__ __volatile__("fnclex");

  uint_fast8_t flags = 0;
  if (sw & 0x01) flags |= softfloat_flag_invalid;
  if (sw & 0x04) flags |= softfloat_flag_infinite;
  if (sw & 0x08) flags |= softfloat_flag_overflow;
  if (sw & 0x10) flags |= softfloat_flag_underflow;
  if (sw & 0x20) flags |= softfloat_flag_inexact;
  return flags;
}

// -----------------------------------------------------------------------------
// Float32 Operations
// -----------------------------------------------------------------------------

float32_t subj_ui32_to_f32(uint32_t a) {
  uint64_t a64 = a;
  float32_t z;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a64));
  return z;
}

float32_t subj_ui64_to_f32(uint64_t a) {
  if (static_cast<int64_t>(a) >= 0) {
    return subj_i64_to_f32(static_cast<int64_t>(a));
  }
  int64_t rem = static_cast<int64_t>(a - 0x8000000000000000ULL);
  static const double kTwo63 = 9223372036854775808.0;
  float32_t z;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fadd qword ptr %2\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(rem), "m"(kTwo63));
  return z;
}

float32_t subj_i32_to_f32(int32_t a) {
  float32_t z;
  __asm__ __volatile__(
      "fild dword ptr %1\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

float32_t subj_i64_to_f32(int64_t a) {
  float32_t z;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

uint_fast32_t subj_f32_to_ui32_rx_minMag(float32_t a) {
  uint16_t orig_cw, cw;
  int64_t z64 = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z64)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return static_cast<uint32_t>(z64);
}

uint_fast64_t subj_f32_to_ui64_rx_minMag(float32_t a) {
  uint16_t orig_cw, cw;
  int64_t z64 = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z64)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return static_cast<uint64_t>(z64);
}

int_fast32_t subj_f32_to_i32_rx_minMag(float32_t a) {
  uint16_t orig_cw, cw;
  int32_t z = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fistp dword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return z;
}

int_fast64_t subj_f32_to_i64_rx_minMag(float32_t a) {
  uint16_t orig_cw, cw;
  int64_t z = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return z;
}

float32_t subj_f32_add(float32_t a, float32_t b) {
  float32_t z;
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fadd dword ptr %2\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float32_t subj_f32_sub(float32_t a, float32_t b) {
  float32_t z;
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fsub dword ptr %2\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float32_t subj_f32_mul(float32_t a, float32_t b) {
  float32_t z;
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fmul dword ptr %2\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float32_t subj_f32_div(float32_t a, float32_t b) {
  float32_t z;
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fdiv dword ptr %2\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float32_t subj_f32_sqrt(float32_t a) {
  float32_t z;
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fsqrt\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

bool subj_f32_eq(float32_t a, float32_t b) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld dword ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fucompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(a), "m"(b));
  return (sw & 0x4500) == 0x4000;
}

bool subj_f32_le(float32_t a, float32_t b) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld dword ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fcompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(a), "m"(b));
  uint16_t flags = sw & 0x4500;
  return (flags == 0x4000) || (flags == 0x0100);
}

bool subj_f32_lt(float32_t a, float32_t b) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld dword ptr %2\n\t"
      "fld dword ptr %1\n\t"
      "fcompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(a), "m"(b));
  return (sw & 0x4500) == 0x0100;
}

float64_t subj_f32_to_f64(float32_t a) {
  float64_t z;
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

void subj_f32_to_extF80M(float32_t a, extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld dword ptr %1\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(a));
}

// -----------------------------------------------------------------------------
// Float64 Operations
// -----------------------------------------------------------------------------

float64_t subj_ui32_to_f64(uint32_t a) {
  uint64_t a64 = a;
  float64_t z;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a64));
  return z;
}

float64_t subj_ui64_to_f64(uint64_t a) {
  if (static_cast<int64_t>(a) >= 0) {
    return subj_i64_to_f64(static_cast<int64_t>(a));
  }
  int64_t rem = static_cast<int64_t>(a - 0x8000000000000000ULL);
  static const double kTwo63 = 9223372036854775808.0;
  float64_t z;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fadd qword ptr %2\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(rem), "m"(kTwo63));
  return z;
}

float64_t subj_i32_to_f64(int32_t a) {
  float64_t z;
  __asm__ __volatile__(
      "fild dword ptr %1\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

float64_t subj_i64_to_f64(int64_t a) {
  float64_t z;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

float32_t subj_f64_to_f32(float64_t a) {
  float32_t z;
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

void subj_f64_to_extF80M(float64_t a, extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(a));
}

uint_fast32_t subj_f64_to_ui32_rx_minMag(float64_t a) {
  uint16_t orig_cw, cw;
  int64_t z64 = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z64)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return static_cast<uint32_t>(z64);
}

uint_fast64_t subj_f64_to_ui64_rx_minMag(float64_t a) {
  uint16_t orig_cw, cw;
  int64_t z64 = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z64)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return static_cast<uint64_t>(z64);
}

int_fast32_t subj_f64_to_i32_rx_minMag(float64_t a) {
  uint16_t orig_cw, cw;
  int32_t z = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fistp dword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return z;
}

int_fast64_t subj_f64_to_i64_rx_minMag(float64_t a) {
  uint16_t orig_cw, cw;
  int64_t z = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z)
      : "m"(a), "m"(cw), "m"(orig_cw));
  return z;
}

float64_t subj_f64_add(float64_t a, float64_t b) {
  float64_t z;
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fadd qword ptr %2\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float64_t subj_f64_sub(float64_t a, float64_t b) {
  float64_t z;
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fsub qword ptr %2\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float64_t subj_f64_mul(float64_t a, float64_t b) {
  float64_t z;
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fmul qword ptr %2\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float64_t subj_f64_div(float64_t a, float64_t b) {
  float64_t z;
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fdiv qword ptr %2\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a), "m"(b));
  return z;
}

float64_t subj_f64_sqrt(float64_t a) {
  float64_t z;
  __asm__ __volatile__(
      "fld qword ptr %1\n\t"
      "fsqrt\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(a));
  return z;
}

bool subj_f64_eq(float64_t a, float64_t b) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld qword ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fucompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(a), "m"(b));
  return (sw & 0x4500) == 0x4000;
}

bool subj_f64_le(float64_t a, float64_t b) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld qword ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fcompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(a), "m"(b));
  uint16_t flags = sw & 0x4500;
  return (flags == 0x4000) || (flags == 0x0100);
}

bool subj_f64_lt(float64_t a, float64_t b) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld qword ptr %2\n\t"
      "fld qword ptr %1\n\t"
      "fcompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(a), "m"(b));
  return (sw & 0x4500) == 0x0100;
}

// -----------------------------------------------------------------------------
// ExtFloat80 Operations
// -----------------------------------------------------------------------------

void subj_ui32_to_extF80M(uint32_t a, extFloat80_t* zPtr) {
  uint64_t a64 = a;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(a64));
}

void subj_ui64_to_extF80M(uint64_t a, extFloat80_t* zPtr) {
  if (static_cast<int64_t>(a) >= 0) {
    subj_i64_to_extF80M(static_cast<int64_t>(a), zPtr);
    return;
  }
  int64_t rem = static_cast<int64_t>(a - 0x8000000000000000ULL);
  static const double kTwo63 = 9223372036854775808.0;
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fadd qword ptr %2\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(rem), "m"(kTwo63));
}

void subj_i32_to_extF80M(int32_t a, extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fild dword ptr %1\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(a));
}

void subj_i64_to_extF80M(int64_t a, extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fild qword ptr %1\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(a));
}

float32_t subj_extF80M_to_f32(const extFloat80_t* aPtr) {
  float32_t z;
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fstp dword ptr %0\n\t"
      : "=m"(z)
      : "m"(*aPtr));
  return z;
}

float64_t subj_extF80M_to_f64(const extFloat80_t* aPtr) {
  float64_t z;
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fstp qword ptr %0\n\t"
      : "=m"(z)
      : "m"(*aPtr));
  return z;
}

uint_fast32_t subj_extF80M_to_ui32_rx_minMag(const extFloat80_t* aPtr) {
  uint16_t orig_cw, cw;
  int64_t z64 = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z64)
      : "m"(*aPtr), "m"(cw), "m"(orig_cw));
  return static_cast<uint32_t>(z64);
}

uint_fast64_t subj_extF80M_to_ui64_rx_minMag(const extFloat80_t* aPtr) {
  uint16_t orig_cw, cw;
  int64_t z64 = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z64)
      : "m"(*aPtr), "m"(cw), "m"(orig_cw));
  return static_cast<uint64_t>(z64);
}

int_fast32_t subj_extF80M_to_i32_rx_minMag(const extFloat80_t* aPtr) {
  uint16_t orig_cw, cw;
  int32_t z = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fistp dword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z)
      : "m"(*aPtr), "m"(cw), "m"(orig_cw));
  return z;
}

int_fast64_t subj_extF80M_to_i64_rx_minMag(const extFloat80_t* aPtr) {
  uint16_t orig_cw, cw;
  int64_t z = 0;
  __asm__ __volatile__("fnstcw word ptr %0" : "=m"(orig_cw));
  cw = (orig_cw & ~0x0C00) | 0x0C00;
  __asm__ __volatile__(
      "fldcw word ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fistp qword ptr %0\n\t"
      "fldcw word ptr %3\n\t"
      : "=m"(z)
      : "m"(*aPtr), "m"(cw), "m"(orig_cw));
  return z;
}

void subj_extF80M_add(const extFloat80_t* aPtr, const extFloat80_t* bPtr,
                      extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fld tbyte ptr %2\n\t"
      "faddp st(1), st(0)\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(*aPtr), "m"(*bPtr));
}

void subj_extF80M_sub(const extFloat80_t* aPtr, const extFloat80_t* bPtr,
                      extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fld tbyte ptr %2\n\t"
      "fsubp st(1), st(0)\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(*aPtr), "m"(*bPtr));
}

void subj_extF80M_mul(const extFloat80_t* aPtr, const extFloat80_t* bPtr,
                      extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fld tbyte ptr %2\n\t"
      "fmulp st(1), st(0)\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(*aPtr), "m"(*bPtr));
}

void subj_extF80M_div(const extFloat80_t* aPtr, const extFloat80_t* bPtr,
                      extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fld tbyte ptr %2\n\t"
      "fdivp st(1), st(0)\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(*aPtr), "m"(*bPtr));
}

void subj_extF80M_sqrt(const extFloat80_t* aPtr, extFloat80_t* zPtr) {
  __asm__ __volatile__(
      "fld tbyte ptr %1\n\t"
      "fsqrt\n\t"
      "fstp tbyte ptr %0\n\t"
      : "=m"(*zPtr)
      : "m"(*aPtr));
}

bool subj_extF80M_eq(const extFloat80_t* aPtr, const extFloat80_t* bPtr) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld tbyte ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fucompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(*aPtr), "m"(*bPtr));
  return (sw & 0x4500) == 0x4000;
}

bool subj_extF80M_le(const extFloat80_t* aPtr, const extFloat80_t* bPtr) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld tbyte ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fcompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(*aPtr), "m"(*bPtr));
  uint16_t flags = sw & 0x4500;
  return (flags == 0x4000) || (flags == 0x0100);
}

bool subj_extF80M_lt(const extFloat80_t* aPtr, const extFloat80_t* bPtr) {
  uint16_t sw = 0;
  __asm__ __volatile__(
      "fld tbyte ptr %2\n\t"
      "fld tbyte ptr %1\n\t"
      "fcompp\n\t"
      "fnstsw ax\n\t"
      : "=a"(sw)
      : "m"(*aPtr), "m"(*bPtr));
  return (sw & 0x4500) == 0x0100;
}
