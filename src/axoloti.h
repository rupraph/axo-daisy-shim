/*
 * axoloti.h — Daisy shim layer
 *
 * Master include header matching the Axoloti firmware API.
 * Replaces ChibiOS-dependent includes with Daisy-compatible equivalents.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __AXOLOTI_H
#define __AXOLOTI_H

#include <stdint.h>
#include <limits.h>

#include "axoloti_math.h"
#include "axoloti_oscs.h"
#include "axoloti_filters.h"
#include "axoloti_defines.h"
#include "midi.h"

/* Memory section attributes — map to Daisy equivalents */
#define SECTION_SRAM2   __attribute__((section(".bss")))
#define SECTION_SRAM3   __attribute__((section(".bss")))
#define SECTION_CCMSRAM __attribute__((section(".dtcmram_bss")))
#define SECTION_SDRAM   __attribute__((section(".sdram_bss")))

#ifdef __cplusplus
extern "C" {
#endif

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
