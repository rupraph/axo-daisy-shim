/*
 * patch.h — Daisy shim layer
 *
 * Patch metadata and function pointer types matching the Axoloti firmware API.
 * ChibiOS and STM32F4-specific includes are replaced with stubs.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __PATCH_H
#define __PATCH_H

#include <stdint.h>
#include "axoloti_defines.h"
#include "parameters.h"
#include "midi.h"

/* Function pointer types for patch entry points */
typedef void (*fptr_patch_init_t)(uint32_t fwID);
typedef void (*fptr_patch_dispose_t)(void);
typedef void (*fptr_patch_dsp_process_t)(int32_t *, int32_t *);
typedef void (*fptr_patch_midi_in_handler_t)(midi_device_t dev, uint8_t port,
                                             uint8_t, uint8_t, uint8_t);
typedef void (*fptr_patch_applyPreset_t)(int32_t);

typedef struct {
    fptr_patch_init_t fptr_patch_init;
    fptr_patch_dispose_t fptr_patch_dispose;
    fptr_patch_dsp_process_t fptr_dsp_process;
    fptr_patch_midi_in_handler_t fptr_MidiInHandler;
    fptr_patch_applyPreset_t fptr_applyPreset;
    uint32_t numPEx;
    ParameterExchange_t *pPExch;
    int32_t *pDisplayVector;
    uint32_t patchID;
    uint32_t initpreset_size;
    void *pInitpreset;
    uint32_t npresets;
    uint32_t npreset_entries;
    PresetParamChange_t *pPresets;
} patchMeta_t;

extern patchMeta_t patchMeta;

/* Stubs for firmware state variables that xpatch.cpp may reference */
extern uint32_t dspLoad200;

/* HID stubs — some patches poll USB HID state */
extern uint8_t hid_buttons[3];
extern uint8_t hid_mouse_x;
extern uint8_t hid_mouse_y;
extern uint8_t hid_keys[6];
extern uint8_t hid_key_modifiers;

#endif /* __PATCH_H */
