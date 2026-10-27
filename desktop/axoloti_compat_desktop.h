/*
 * axoloti_compat_desktop.h — Desktop shim
 *
 * Master force-include for compiling xpatch.cpp on desktop (macOS/Linux).
 * Equivalent to src/axoloti_compat.h but uses portable math/intrinsics
 * instead of ARM inline assembly.
 *
 * Usage: clang++ -std=c++14 -include desktop/axoloti_compat_desktop.h ...
 */
#ifndef __AXOLOTI_COMPAT_DESKTOP_H
#define __AXOLOTI_COMPAT_DESKTOP_H

/* 1. STM32/CMSIS type stubs — must come first */
#include "stm32_stubs_desktop.h"

/* 2. Portable math (same guard as axoloti_math.h — blocks ARM version) */
#include "axoloti_math_desktop.h"

/* 3. Existing headers that DON'T contain ARM asm */
extern "C" {
#include "axoloti_defines.h"
#include "codec.h"
#include "midi.h"
#include "parameter_functions.h"
#include "parameters.h"
#include "patch.h"
#include "patch_wrapper.h"
#include "ui_stubs.h"
}

/* 4. axoloti.h — includes axoloti_math.h (guarded out), axoloti_filters.h,
 *    axoloti_oscs.h, axoloti_defines.h, midi.h */
extern "C" {
#include "axoloti.h"
}

/* 5. Portable arm:: namespace intrinsics (same guard as arm_intrinsics.hpp) */
#include "arm_intrinsics_desktop.hpp"

/* 6. SDRAM memory allocator API */
extern "C" {
#include "axoloti_memory.h"
}

/*
 * Linker symbols referenced by xpatch_init2().
 * On real hardware these come from the linker script.
 * On desktop, main_desktop.cpp defines them as aliases so the
 * clearing/constructor loops run zero iterations.
 */
extern "C" {
extern uint32_t _pbss_start;
extern uint32_t _pbss_end;
extern char _sdram_dyn_start;
extern char _sdram_dyn_end;

typedef void (*funcp_t)(void);
extern funcp_t __ctor_array_start;
extern funcp_t __ctor_array_end;
extern funcp_t __dtor_array_start;
extern funcp_t __dtor_array_end;
}

#endif /* __AXOLOTI_COMPAT_DESKTOP_H */
