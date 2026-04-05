/*
 * axoloti_math.h — Daisy shim layer
 *
 * Math functions, ARM intrinsics, and lookup tables from the Axoloti firmware.
 * ChibiOS includes replaced with CMSIS equivalents available in libDaisy.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef _AXOLOTI_MATH_H
#define _AXOLOTI_MATH_H

#include <stdint.h>
#include <limits.h>
#include <math.h>

/* CMSIS core and DSP — provided by libDaisy */
#include "arm_math.h"

/* Lookup table externs — defined in axoloti_math.c */
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

/* ---- ARM intrinsics (all available on Cortex-M7) ---- */

__attribute__((always_inline)) __STATIC_INLINE int32_t ___ROR(int32_t op1, int32_t op2) {
    int32_t result;
    __ASM volatile("ror %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t ___SMMUL(int32_t op1, int32_t op2) {
    int32_t result;
    __ASM volatile("smmul %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t ___SMMLA(int32_t op1, int32_t op2, int32_t op3) {
    int32_t result;
    __ASM volatile("smmla %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t ___SMMLS(int32_t op1, int32_t op2, int32_t op3) {
    int32_t result;
    __ASM volatile("smmls %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t ___SDIV(int32_t op1, int32_t op2) {
    int32_t result;
    __ASM volatile("sdiv %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE uint32_t ___UDIV(uint32_t op1, uint32_t op2) {
    uint32_t result;
    __ASM volatile("udiv %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
    return result;
}

/* ---- Floating-point VFP operations ---- */

__attribute__((always_inline)) __STATIC_INLINE float ___VMULF(float op1, float op2) {
    float result;
    __ASM volatile("vmul.f32 %0, %1, %2" : "=w"(result) : "w"(op1), "w"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE float ___VDIVF(float op1, float op2) {
    float result;
    __ASM volatile("vdiv.f32 %0, %1, %2" : "=w"(result) : "w"(op1), "w"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE float ___VMLAF(float op1, float op2, float op3) {
    float result = op3;
    __ASM volatile("vmla.f32 %0, %1, %2" : "+w"(result) : "w"(op1), "w"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE float ___VMLSF(float op1, float op2, float op3) {
    float result = op3;
    __ASM volatile("vmls.f32 %0, %1, %2" : "+w"(result) : "w"(op1), "w"(op2));
    return result;
}

__attribute__((always_inline)) __STATIC_INLINE float ___VSQRTF(float op1) {
    float result;
    __ASM volatile("vsqrt.f32 %0, %1" : "=w"(result) : "w"(op1));
    return result;
}
#define _VSQRTF ___VSQRTF

/* ---- Pitch / frequency conversion ---- */

__attribute__((always_inline)) __STATIC_INLINE uint32_t mtof48k_q31(int32_t pitch) {
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

__attribute__((always_inline)) __STATIC_INLINE uint32_t mtof48k_ext_q31(int32_t pitch) {
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

/* ---- Lookup-table interpolation functions ---- */

__attribute__((always_inline)) __STATIC_INLINE int32_t sin_q31(int32_t phase) {
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

__attribute__((always_inline)) __STATIC_INLINE uint32_t hann_q31(int32_t phase) {
    uint32_t p = phase;
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

/* ---- Q27 fixed-point <-> float conversion ---- */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

__attribute__((always_inline)) __STATIC_INLINE float q27_to_float(int32_t op1) {
    float fop1 = *(float *)(&op1);
    __ASM volatile("VCVT.F32.S32 %0, %0, 27" : "+w"(fop1));
    return fop1;
}

__attribute__((always_inline)) __STATIC_INLINE int32_t float_to_q27(float fop1) {
    __ASM volatile("VCVT.S32.F32 %0, %0, 27" : "+w"(fop1));
    int32_t r = *(int32_t *)(&fop1);
    return r;
}

#pragma GCC diagnostic pop

/* ---- Integer <-> fractional conversion ---- */

__attribute__((always_inline)) __STATIC_INLINE int32_t ConvertIntToFrac(int i) {
    return i << 21;
}
#define int_to_frac ConvertIntToFrac

__attribute__((always_inline)) __STATIC_INLINE int32_t ConvertFracToInt(int i) {
    return i >> 21;
}
#define frac_to_int ConvertFracToInt

__attribute__((always_inline)) __STATIC_INLINE int32_t ConvertFloatToFrac(float f) {
    return (int32_t)(f * (1 << 21));
}
#define float_to_frac ConvertFloatToFrac

/* ---- Random number generation ----
 * Uses the STM32H750 hardware RNG (same peripheral as STM32F4/H7).
 * The RNG clock must be enabled — Daisy's System::Init() handles this.
 */
#include "stm32h7xx.h"

__attribute__((always_inline)) __STATIC_INLINE int32_t rand_s32(void) {
    static uint32_t randSeed = 22229;
    if (RNG->SR & RNG_SR_DRDY) {
        return randSeed = (randSeed * 196314163) + RNG->DR;
    }
    return randSeed = (randSeed * 196314163) + 1;
}

#ifdef RAND_MAX
#undef RAND_MAX
#endif
#define RAND_MAX INT32_MAX

__attribute__((always_inline)) __STATIC_INLINE int32_t rand_u32(void) {
    return ((uint32_t)rand_s32()) >> 1;
}

uint32_t FastLog(uint32_t f);

/* ---- Deprecated macros (still used by generated code) ---- */
#define MTOF(pitch, frequency) frequency = mtof48k_q31(pitch);
#define MTOFEXTENDED(pitch, frequency) frequency = mtof48k_ext_q31(pitch);
#define MTOF_EXTENDED MTOFEXTENDED
#define SINE2TINTERP(phase, output) output = sin_q31(phase);
#define SINE2T_INTERP SINE2TINTERP
#define HANNING2TINTERP(phase, output) output = hann_q31(phase);
#define HANNING2T_INTERP HANNING2TINTERP

__attribute__((always_inline)) __STATIC_INLINE uint32_t GenerateRandomNumber(void) {
    return rand_s32();
}

#endif /* _AXOLOTI_MATH_H */
