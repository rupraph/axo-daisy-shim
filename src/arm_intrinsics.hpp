/*
 * arm_intrinsics.hpp — Daisy shim layer
 *
 * C++ namespace wrappers for ARM DSP intrinsics.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef ARM_INTRINSICS_H_
#define ARM_INTRINSICS_H_

#include "arm_math.h"
#include <stdint.h>

namespace arm {

__attribute__((always_inline)) __STATIC_INLINE int32_t smmul(int32_t op1,
                                                             int32_t op2) {
  int32_t result;
  __ASM volatile("smmul %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smmla(int32_t op1,
                                                             int32_t op2,
                                                             int32_t op3) {
  int32_t result;
  __ASM volatile("smmla %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smmls(int32_t op1,
                                                             int32_t op2,
                                                             int32_t op3) {
  int32_t result;
  __ASM volatile("smmls %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smuad(int32_t op1,
                                                             int32_t op2) {
  int32_t result;
  __ASM volatile("smuad %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smuadx(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smuadx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smusd(int32_t op1,
                                                             int32_t op2) {
  int32_t result;
  __ASM volatile("smusd %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smusdx(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smusdx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smultt(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smultt %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smulbb(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smulbb %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smultb(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smultb %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smulbt(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smulbt %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smulwb(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smulwb %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smulwt(int32_t op1,
                                                              int32_t op2) {
  int32_t result;
  __ASM volatile("smulwt %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smlabb(int32_t op1,
                                                              int32_t op2,
                                                              int32_t op3) {
  int32_t result;
  __ASM volatile("smlabb %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smlabt(int32_t op1,
                                                              int32_t op2,
                                                              int32_t op3) {
  int32_t result;
  __ASM volatile("smlabt %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smlatt(int32_t op1,
                                                              int32_t op2,
                                                              int32_t op3) {
  int32_t result;
  __ASM volatile("smlatt %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smlatb(int32_t op1,
                                                              int32_t op2,
                                                              int32_t op3) {
  int32_t result;
  __ASM volatile("smlatb %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smlawt(int32_t op1,
                                                              int32_t op2,
                                                              int32_t op3) {
  int32_t result;
  __ASM volatile("smlawt %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t smlawb(int32_t op1,
                                                              int32_t op2,
                                                              int32_t op3) {
  int32_t result;
  __ASM volatile("smlawb %0, %1, %2, %3"
                 : "=r"(result)
                 : "r"(op1), "r"(op2), "r"(op3));
  return result;
}

__attribute__((always_inline)) __STATIC_INLINE float vsqrtf(float op1) {
  float result;
  __ASM volatile("vsqrt.f32 %0, %1" : "=w"(result) : "w"(op1));
  return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

template <int q>
__attribute__((always_inline)) __STATIC_INLINE float q_to_float(int32_t op1) {
  float fop1 = *(float *)(&op1);
  __ASM volatile("VCVT.F32.S32 %0, %0, %1" : "+w"(fop1) : "i"(q));
  return fop1;
}

template <int q>
__attribute__((always_inline)) __STATIC_INLINE int32_t float_to_q(float op1) {
  __ASM volatile("VCVT.S32.F32 %0, %0, %1" : "+w"(op1) : "i"(q));
  return *(int32_t *)(&op1);
}

#pragma GCC diagnostic pop

} /* namespace arm */

#endif /* ARM_INTRINSICS_H_ */
