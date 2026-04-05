/*
 * axoloti_compat.h — Daisy shim layer
 *
 * Master include that replaces xpatch.h for Daisy compilation.
 * Pulls in all headers that xpatch.cpp expects to find.
 */
#ifndef __AXOLOTI_COMPAT_H
#define __AXOLOTI_COMPAT_H

extern "C" {
#include "axoloti.h"
#include "axoloti_memory.h"
#include "parameter_functions.h"
#include "patch.h"
#include "ui_stubs.h"
}

#include "arm_intrinsics.hpp"

/*
 * Linker symbols referenced by xpatch_init2().
 * On real Axoloti, these come from the linker script for dynamic patch loading.
 * On Daisy, we define them as dummy variables in main.cpp.
 */
extern "C" {
extern char _sdram_dyn_start;
extern char _sdram_dyn_end;
extern uint32_t _pbss_start;
extern uint32_t _pbss_end;

typedef void (*funcp_t)(void);
extern funcp_t __ctor_array_start;
extern funcp_t __ctor_array_end;
extern funcp_t __dtor_array_start;
extern funcp_t __dtor_array_end;
}

/*
 * Mutable Instruments library includes are commented out.
 * Uncomment and add MI source files to the build if your patch uses MI objects.
 *
 * #include "braids/parameter_interpolation.h"
 * #include "braids/resources.h"
 * #include "rings/dsp/string.h"
 * #include "rings/dsp/resonator.h"
 * #include "warps/dsp/modulator.h"
 * #include "warps/dsp/vocoder.h"
 * #include "elements/dsp/exciter.h"
 * #include "elements/dsp/tube.h"
 * #include "elements/dsp/string.h"
 * #include "elements/dsp/fx/diffuser.h"
 * #include "elements/dsp/fx/reverb.h"
 * #include "clouds/dsp/granular_processor.h"
 * #include "elements/dsp/part.h"
 * #include "elements/dsp/patch.h"
 * #include "streams/processor.h"
 */

#endif /* __AXOLOTI_COMPAT_H */
