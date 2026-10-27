/*
 * stm32_stubs_desktop.h — Desktop shim
 *
 * Minimal stubs for STM32/CMSIS types needed by the Axoloti shim headers
 * when compiling for desktop (x86/ARM macOS) instead of Cortex-M7.
 */
#ifndef __STM32_STUBS_DESKTOP_H
#define __STM32_STUBS_DESKTOP_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* CMSIS-style inline qualifier */
#ifndef __STATIC_INLINE
#define __STATIC_INLINE static inline
#endif

/* Dummy RNG peripheral — rand_s32() checks RNG->SR */
typedef struct {
  volatile uint32_t CR;
  volatile uint32_t SR;
  volatile uint32_t DR;
} RNG_TypeDef;

/* RNG status bits */
#define RNG_SR_DRDY 0x00000001U
#define RNG_CR_RNGEN 0x00000004U

/* Dummy RNG instance — never actually ready, so rand_s32 uses LFSR */
static RNG_TypeDef _rng_dummy = {0, 0, 0};
#define RNG (&_rng_dummy)

/* HAL macros — no-op on desktop */
#define __HAL_RCC_RNG_CLK_ENABLE() ((void)0)

/* GPIO stubs for patches that reference ChibiOS PAL */
typedef struct {
  uint32_t dummy;
} GPIO_TypeDef;
#define GPIOA ((GPIO_TypeDef *)0)
#define GPIOB ((GPIO_TypeDef *)0)
#define GPIOC ((GPIO_TypeDef *)0)
#define GPIOD ((GPIO_TypeDef *)0)
#define GPIOE ((GPIO_TypeDef *)0)

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

#define HAL_GPIO_WritePin(port, pin, val) ((void)0)
#define HAL_GPIO_ReadPin(port, pin) (0)
#define HAL_GPIO_TogglePin(port, pin) ((void)0)

/* ChibiOS PAL stubs */
#define palSetPad(port, pin) ((void)0)
#define palClearPad(port, pin) ((void)0)
#define palReadPad(port, pin) (0)
#define palTogglePad(port, pin) ((void)0)
#define palSetPadMode(port, pin, mode) ((void)0)

#define PAL_MODE_OUTPUT_PUSHPULL 0
#define PAL_MODE_INPUT 1
#define PAL_MODE_INPUT_PULLUP 2
#define PAL_MODE_INPUT_PULLDOWN 3

/* Section attributes — no-op on desktop */
#define __attribute_section(x)

/* Mach-O doesn't support ELF-style section(".name") attributes.
 * Redefine section() inside __attribute__ to be harmless.
 * xpatch.cpp uses: __attribute__ ((section(".boot")))
 * We define a macro 'section' that eats the argument. */
#ifdef __APPLE__
#define section(x) used
#endif

/* CMSIS DSP arm_sin/cos_q31 stubs — used by axoloti_filters.h */
static inline int32_t arm_sin_q31(int32_t x) {
  /* Quick approximation using the sine lookup table.
   * The real implementation is in axoloti_math_desktop.h via sin_q31().
   * This is called by biquad coefficient functions with phase in Q31. */
  double phase = (double)x / 2147483648.0 * 3.14159265358979323846;
  return (int32_t)(sin(phase) * 2147483647.0);
}

static inline int32_t arm_cos_q31(int32_t x) {
  double phase = (double)x / 2147483648.0 * 3.14159265358979323846;
  return (int32_t)(cos(phase) * 2147483647.0);
}

/* FatFS stub — ff.h not available on desktop */
#ifndef _FATFS
#define _FATFS
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef char TCHAR;
typedef DWORD FSIZE_t;
typedef enum { FR_OK = 0 } FRESULT;
typedef struct {
  BYTE dummy;
} FIL;
typedef struct {
  BYTE dummy;
} DIR_;
typedef struct {
  BYTE dummy;
} FILINFO;
#define FA_READ 0x01
#define FA_WRITE 0x02
#define FA_CREATE_ALWAYS 0x08
static inline FRESULT f_open(FIL *fp, const TCHAR *path, BYTE mode) {
  (void)fp;
  (void)path;
  (void)mode;
  return FR_OK;
}
static inline FRESULT f_close(FIL *fp) {
  (void)fp;
  return FR_OK;
}
static inline FRESULT f_read(FIL *fp, void *buff, UINT btr, UINT *br) {
  (void)fp;
  (void)buff;
  (void)btr;
  if (br)
    *br = 0;
  return FR_OK;
}
static inline FRESULT f_write(FIL *fp, const void *buff, UINT btw, UINT *bw) {
  (void)fp;
  (void)buff;
  (void)btw;
  if (bw)
    *bw = 0;
  return FR_OK;
}
static inline FRESULT f_lseek(FIL *fp, FSIZE_t ofs) {
  (void)fp;
  (void)ofs;
  return FR_OK;
}
static inline FSIZE_t f_size(FIL *fp) {
  (void)fp;
  return 0;
}
#endif /* _FATFS */

#endif /* __STM32_STUBS_DESKTOP_H */
