/*
 * arm_intrinsics_desktop.hpp — Desktop shim
 *
 * Portable C++ replacements for ARM DSP intrinsics in namespace arm::.
 * Uses the same include guard as the original arm_intrinsics.hpp so
 * existing headers that #include it get this version instead.
 */
#ifndef ARM_INTRINSICS_H_
#define ARM_INTRINSICS_H_

#include <cmath>
#include <stdint.h>

/* Ensure portable ___SMMUL etc. are available */
#include "axoloti_math_desktop.h"

namespace arm {

/* ---- Multiply, high 32 bits ---- */

static inline int32_t smmul(int32_t op1, int32_t op2) {
  return (int32_t)(((int64_t)op1 * (int64_t)op2) >> 32);
}

static inline int32_t smmla(int32_t op1, int32_t op2, int32_t op3) {
  return (int32_t)(((int64_t)op1 * (int64_t)op2) >> 32) + op3;
}

static inline int32_t smmls(int32_t op1, int32_t op2, int32_t op3) {
  return op3 - (int32_t)(((int64_t)op1 * (int64_t)op2) >> 32);
}

/* ---- Dual 16-bit multiply ---- */

/* SMUAD: (top16(a)*top16(b)) + (bot16(a)*bot16(b)) */
static inline int32_t smuad(int32_t op1, int32_t op2) {
  int16_t a_lo = (int16_t)(op1 & 0xFFFF);
  int16_t a_hi = (int16_t)(op1 >> 16);
  int16_t b_lo = (int16_t)(op2 & 0xFFFF);
  int16_t b_hi = (int16_t)(op2 >> 16);
  return (int32_t)a_lo * b_lo + (int32_t)a_hi * b_hi;
}

/* SMUADX: (top16(a)*bot16(b)) + (bot16(a)*top16(b)) — cross */
static inline int32_t smuadx(int32_t op1, int32_t op2) {
  int16_t a_lo = (int16_t)(op1 & 0xFFFF);
  int16_t a_hi = (int16_t)(op1 >> 16);
  int16_t b_lo = (int16_t)(op2 & 0xFFFF);
  int16_t b_hi = (int16_t)(op2 >> 16);
  return (int32_t)a_lo * b_hi + (int32_t)a_hi * b_lo;
}

/* SMUSD: (bot16(a)*bot16(b)) - (top16(a)*top16(b)) */
static inline int32_t smusd(int32_t op1, int32_t op2) {
  int16_t a_lo = (int16_t)(op1 & 0xFFFF);
  int16_t a_hi = (int16_t)(op1 >> 16);
  int16_t b_lo = (int16_t)(op2 & 0xFFFF);
  int16_t b_hi = (int16_t)(op2 >> 16);
  return (int32_t)a_lo * b_lo - (int32_t)a_hi * b_hi;
}

/* SMUSDX: cross variant */
static inline int32_t smusdx(int32_t op1, int32_t op2) {
  int16_t a_lo = (int16_t)(op1 & 0xFFFF);
  int16_t a_hi = (int16_t)(op1 >> 16);
  int16_t b_lo = (int16_t)(op2 & 0xFFFF);
  int16_t b_hi = (int16_t)(op2 >> 16);
  return (int32_t)a_lo * b_hi - (int32_t)a_hi * b_lo;
}

/* ---- 16-bit multiplies ---- */

/* SMULTT: top16(a) * top16(b) */
static inline int32_t smultt(int32_t op1, int32_t op2) {
  int16_t a = (int16_t)(op1 >> 16);
  int16_t b = (int16_t)(op2 >> 16);
  return (int32_t)a * b;
}

/* SMULBB: bot16(a) * bot16(b) */
static inline int32_t smulbb(int32_t op1, int32_t op2) {
  int16_t a = (int16_t)(op1 & 0xFFFF);
  int16_t b = (int16_t)(op2 & 0xFFFF);
  return (int32_t)a * b;
}

/* SMULTB: top16(a) * bot16(b) */
static inline int32_t smultb(int32_t op1, int32_t op2) {
  int16_t a = (int16_t)(op1 >> 16);
  int16_t b = (int16_t)(op2 & 0xFFFF);
  return (int32_t)a * b;
}

/* SMULBT: bot16(a) * top16(b) */
static inline int32_t smulbt(int32_t op1, int32_t op2) {
  int16_t a = (int16_t)(op1 & 0xFFFF);
  int16_t b = (int16_t)(op2 >> 16);
  return (int32_t)a * b;
}

/* SMULWB: word * bot16(b), result >> 16 */
static inline int32_t smulwb(int32_t op1, int32_t op2) {
  int16_t b = (int16_t)(op2 & 0xFFFF);
  return (int32_t)(((int64_t)op1 * b) >> 16);
}

/* SMULWT: word * top16(b), result >> 16 */
static inline int32_t smulwt(int32_t op1, int32_t op2) {
  int16_t b = (int16_t)(op2 >> 16);
  return (int32_t)(((int64_t)op1 * b) >> 16);
}

/* ---- 16-bit multiply-accumulate ---- */

static inline int32_t smlabb(int32_t op1, int32_t op2, int32_t op3) {
  return smulbb(op1, op2) + op3;
}

static inline int32_t smlabt(int32_t op1, int32_t op2, int32_t op3) {
  return smulbt(op1, op2) + op3;
}

static inline int32_t smlatt(int32_t op1, int32_t op2, int32_t op3) {
  return smultt(op1, op2) + op3;
}

static inline int32_t smlatb(int32_t op1, int32_t op2, int32_t op3) {
  return smultb(op1, op2) + op3;
}

static inline int32_t smlawt(int32_t op1, int32_t op2, int32_t op3) {
  return smulwt(op1, op2) + op3;
}

static inline int32_t smlawb(int32_t op1, int32_t op2, int32_t op3) {
  return smulwb(op1, op2) + op3;
}

/* ---- Float ---- */

static inline float vsqrtf(float op1) { return sqrtf(op1); }

/* ---- Q-format conversion (templated) ---- */

template <int q> static inline float q_to_float(int32_t op1) {
  return (float)op1 / (float)(1 << q);
}

template <int q> static inline int32_t float_to_q(float op1) {
  return (int32_t)(op1 * (float)(1 << q));
}

} /* namespace arm */

#endif /* ARM_INTRINSICS_H_ */
