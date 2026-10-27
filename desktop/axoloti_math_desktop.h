/*
 * axoloti_math_desktop.h — Desktop shim
 *
 * Portable C replacements for all ARM inline asm intrinsics in axoloti_math.h.
 * Defines the same symbols so existing headers (axoloti_filters.h,
 * parameter_functions.h, etc.) work unmodified.
 *
 * MUST be included BEFORE any header that includes "axoloti_math.h" —
 * the include guard _AXOLOTI_MATH_H prevents the ARM version from loading.
 */
#ifndef _AXOLOTI_MATH_H
#define _AXOLOTI_MATH_H

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- Lookup table sizes and externs ---- */

#define SINE2TSIZE 4096
extern int32_t sine2t[SINE2TSIZE + 1];

#define WINDOWSIZE 1024
extern int16_t windowt[WINDOWSIZE + 1];

#define PITCHTSIZE 257
extern uint32_t pitcht[PITCHTSIZE];

#define EXPTSIZE 256
extern uint16_t expt[EXPTSIZE];

#define LOGTSIZE 256
#define LOGTSIZEN 8
extern uint16_t logt[LOGTSIZE];

typedef union {
  int32_t i;
  float f;
  struct {
    uint32_t mantissa : 23;
    uint32_t exponent : 8;
    uint32_t sign : 1;
  } parts;
} Float_t;

#ifdef __cplusplus
extern "C" {
#endif

void axoloti_math_init(void);

#ifdef __cplusplus
}
#endif

/* ---- CMSIS intrinsic replacements ---- */

static inline int32_t __SSAT(int32_t x, int bits) {
  int32_t max = (1 << (bits - 1)) - 1;
  int32_t min = -(1 << (bits - 1));
  if (x > max)
    return max;
  if (x < min)
    return min;
  return x;
}

static inline int32_t __USAT(int32_t x, int bits) {
  int32_t max = (1 << bits) - 1;
  if (x > max)
    return max;
  if (x < 0)
    return 0;
  return x;
}

static inline int32_t __QADD(int32_t a, int32_t b) {
  int64_t r = (int64_t)a + b;
  if (r > INT32_MAX)
    return INT32_MAX;
  if (r < INT32_MIN)
    return INT32_MIN;
  return (int32_t)r;
}

static inline int32_t __QSUB(int32_t a, int32_t b) {
  int64_t r = (int64_t)a - b;
  if (r > INT32_MAX)
    return INT32_MAX;
  if (r < INT32_MIN)
    return INT32_MIN;
  return (int32_t)r;
}

static inline int __CLZ(uint32_t x) {
  if (x == 0)
    return 32;
#if defined(__GNUC__) || defined(__clang__)
  return __builtin_clz(x);
#else
  int n = 0;
  if (x <= 0x0000FFFF) {
    n += 16;
    x <<= 16;
  }
  if (x <= 0x00FFFFFF) {
    n += 8;
    x <<= 8;
  }
  if (x <= 0x0FFFFFFF) {
    n += 4;
    x <<= 4;
  }
  if (x <= 0x3FFFFFFF) {
    n += 2;
    x <<= 2;
  }
  if (x <= 0x7FFFFFFF) {
    n += 1;
  }
  return n;
#endif
}

/* ---- ARM DSP instruction replacements ---- */

static inline int32_t ___ROR(int32_t op1, int32_t op2) {
  uint32_t a = (uint32_t)op1;
  int shift = op2 & 31;
  return (int32_t)((a >> shift) | (a << (32 - shift)));
}

/* Signed multiply, return high 32 bits: (a * b) >> 32 */
static inline int32_t ___SMMUL(int32_t op1, int32_t op2) {
  return (int32_t)(((int64_t)op1 * (int64_t)op2) >> 32);
}

/* Signed multiply-accumulate, high 32 bits: ((a * b) >> 32) + c */
static inline int32_t ___SMMLA(int32_t op1, int32_t op2, int32_t op3) {
  return (int32_t)(((int64_t)op1 * (int64_t)op2) >> 32) + op3;
}

/* Signed multiply-subtract, high 32 bits: c - ((a * b) >> 32) */
static inline int32_t ___SMMLS(int32_t op1, int32_t op2, int32_t op3) {
  return op3 - (int32_t)(((int64_t)op1 * (int64_t)op2) >> 32);
}

static inline int32_t ___SDIV(int32_t op1, int32_t op2) {
  if (op2 == 0)
    return 0;
  return op1 / op2;
}

static inline uint32_t ___UDIV(uint32_t op1, uint32_t op2) {
  if (op2 == 0)
    return 0;
  return op1 / op2;
}

/* ---- Floating-point VFP replacements (trivial on desktop) ---- */

static inline float ___VMULF(float op1, float op2) { return op1 * op2; }
static inline float ___VDIVF(float op1, float op2) { return op1 / op2; }
static inline float ___VMLAF(float op1, float op2, float op3) {
  return op3 + op1 * op2;
}
static inline float ___VMLSF(float op1, float op2, float op3) {
  return op3 - op1 * op2;
}
static inline float ___VSQRTF(float op1) { return sqrtf(op1); }
#define _VSQRTF ___VSQRTF

/* ---- Q27 fixed-point <-> float conversion ---- */

static inline float q27_to_float(int32_t op1) {
  return (float)op1 / 134217728.0f; /* 1 << 27 */
}

static inline int32_t float_to_q27(float fop1) {
  return (int32_t)(fop1 * 134217728.0f);
}

/* ---- Integer <-> fractional conversion ---- */

static inline int32_t ConvertIntToFrac(int i) { return i << 21; }
#define int_to_frac ConvertIntToFrac

static inline int32_t ConvertFracToInt(int i) { return i >> 21; }
#define frac_to_int ConvertFracToInt

static inline int32_t ConvertFloatToFrac(float f) {
  return (int32_t)(f * (1 << 21));
}
#define float_to_frac ConvertFloatToFrac

/* ---- Lookup-table interpolation functions ---- */

static inline int32_t sin_q31(int32_t phase) {
  uint32_t p = (uint32_t)phase;
  uint32_t pi = p >> 20;
  int32_t y1 = sine2t[pi];
  int32_t y2 = sine2t[1 + pi];
  int32_t pf = (p & 0xfffff) << 11;
  int32_t pfc = INT32_MAX - pf;
  int32_t rr;
  rr = ___SMMUL(y1, pfc);
  rr = ___SMMLA(y2, pf, rr);
  return rr << 1;
}

static inline uint32_t hann_q31(int32_t phase) {
  uint32_t p = (uint32_t)phase;
  uint32_t pi = p >> 22;
  int32_t y1 = windowt[pi];
  int32_t y2 = windowt[1 + pi];
  int32_t pf = (p & 0x3fffff) << 9;
  int32_t pfc = INT32_MAX - pf;
  int32_t rr;
  rr = ___SMMUL(y1 << 16, pfc);
  rr = ___SMMLA(y2 << 16, pf, rr);
  return rr << 1;
}

/* ---- Pitch / frequency conversion ---- */

static inline uint32_t mtof48k_q31(int32_t pitch) {
  int32_t p = __SSAT(pitch, 28);
  uint32_t pi = p >> 21;
  int32_t y1 = pitcht[128 + pi];
  int32_t y2 = pitcht[128 + 1 + pi];
  int32_t pf = (p & 0x1FFFFF) << 10;
  int32_t pfc = INT32_MAX - pf;
  uint32_t r;
  r = ___SMMUL(y1, pfc);
  r = ___SMMLA(y2, pf, r);
  return r << 1;
}

static inline uint32_t mtof48k_ext_q31(int32_t pitch) {
  int32_t p = __SSAT(pitch, 29);
  uint32_t pi = p >> 21;
  int32_t y1 = pitcht[128 + pi];
  int32_t y2 = pitcht[128 + 1 + pi];
  int32_t pf = (p & 0x1FFFFF) << 10;
  int32_t pfc = INT32_MAX - pf;
  uint32_t r;
  r = ___SMMUL(y1, pfc);
  r = ___SMMLA(y2, pf, r);
  return r << 1;
}

/* ---- Random number generation (LFSR, no hardware RNG) ---- */

#include "stm32_stubs_desktop.h"

static inline int32_t rand_s32(void) {
  static uint32_t randSeed = 22229;
  randSeed = (randSeed * 196314163) + 907633515;
  return (int32_t)randSeed;
}

#ifdef RAND_MAX
#undef RAND_MAX
#endif
#define RAND_MAX INT32_MAX

static inline int32_t rand_u32(void) { return ((uint32_t)rand_s32()) >> 1; }

uint32_t FastLog(uint32_t f);

/* ---- Deprecated macros (still used by generated code) ---- */
#define MTOF(pitch, frequency) frequency = mtof48k_q31(pitch);
#define MTOFEXTENDED(pitch, frequency) frequency = mtof48k_ext_q31(pitch);
#define MTOF_EXTENDED MTOFEXTENDED
#define SINE2TINTERP(phase, output) output = sin_q31(phase);
#define SINE2T_INTERP SINE2TINTERP
#define HANNING2TINTERP(phase, output) output = hann_q31(phase);
#define HANNING2T_INTERP HANNING2TINTERP

static inline uint32_t GenerateRandomNumber(void) { return rand_s32(); }

#endif /* _AXOLOTI_MATH_H */
