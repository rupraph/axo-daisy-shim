/*
 * axoloti.h — Daisy shim layer
 *
 * Master include header matching the Axoloti firmware API.
 * Replaces ChibiOS-dependent includes with Daisy-compatible equivalents.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __AXOLOTI_H
#define __AXOLOTI_H

#include <limits.h>
#include <stdint.h>

#include "axoloti_defines.h"
#include "axoloti_filters.h"
#include "axoloti_math.h"
#include "axoloti_oscs.h"
#include "midi.h"

/* Memory section attributes — map to Daisy equivalents */
#define SECTION_SRAM2 __attribute__((section(".bss")))
#define SECTION_SRAM3 __attribute__((section(".bss")))
#define SECTION_CCMSRAM __attribute__((section(".dtcmram_bss")))
#define SECTION_SDRAM __attribute__((section(".sdram_bss")))

/* Number of ADC channels to scan.
 * Daisy Seed v1.1 has 12 ADC-capable pins (A0–A11).
 * Set NUM_ADC_CHANNELS to however many knobs/CVs you wire up (max 12).
 * Default: 0 (no ADC). Override in Makefile with -DNUM_ADC_CHANNELS=N.
 */
#ifndef NUM_ADC_CHANNELS
#define NUM_ADC_CHANNELS 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ADC values array — 12-bit (0–4095), same format as Axoloti.
 * In the patcher, gpio/in/analog channel N reads adcvalues[N].
 * Channel 0 = Daisy pin A0 (D15), channel 1 = A1 (D16), etc. */
extern uint16_t adcvalues[16];

/* Debug logging — outputs via Daisy's USB serial (if connected) */
void LogTextMessage(const char *format, ...);

/* Stub for codec register writes — not applicable on Daisy */
static inline void ADAU1961_WriteRegister(uint16_t RegisterAddr,
                                          uint8_t RegisterValue) {
  (void)RegisterAddr;
  (void)RegisterValue;
}

#ifdef __cplusplus
}
#endif

#endif /* __AXOLOTI_H */
