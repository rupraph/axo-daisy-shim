/*
 * patch_wrapper.h — Daisy shim layer
 *
 * Glue layer that connects Daisy's audio callback to the Axoloti patch's
 * PatchProcess / xpatch_init / PatchMidiInHandler entry points.
 *
 * The generated xpatch.cpp defines these functions at global scope.
 * This header declares them so main.cpp can call them.
 */
#ifndef __PATCH_WRAPPER_H
#define __PATCH_WRAPPER_H

#include "axoloti_defines.h"
#include "midi.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Entry points defined by xpatch.cpp (generated code).
 * xpatch_init is placed in .boot section with C linkage.
 */
extern void xpatch_init(int fwid);

#ifdef __cplusplus
}
#endif

/*
 * These are C++ functions defined at global scope by xpatch.cpp.
 * They are NOT extern "C" — they have C++ linkage.
 */
extern void xpatch_init2(int fwid);
extern void PatchProcess(int32_t *inbuf, int32_t *outbuf);
extern void PatchMidiInHandler(midi_device_t dev, uint8_t port, uint8_t status,
                               uint8_t data1, uint8_t data2);
extern void ApplyPreset(int32_t i);
extern void PatchDispose(void);

#endif /* __PATCH_WRAPPER_H */
