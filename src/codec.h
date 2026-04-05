/*
 * codec.h — Daisy shim layer
 *
 * Audio buffer declarations matching the Axoloti firmware API.
 * The Daisy audio callback fills/reads these buffers.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __CODEC_H
#define __CODEC_H

#include <stdint.h>
#include "axoloti_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Interleaved stereo audio DMA buffers (Q27 fixed-point) */
extern int32_t buf[DOUBLE_BUFSIZE];
extern int32_t buf2[DOUBLE_BUFSIZE];
extern int32_t rbuf[DOUBLE_BUFSIZE];
extern int32_t rbuf2[DOUBLE_BUFSIZE];

/* DSP process function — called by the audio callback */
extern void computebufI(int32_t *inp, int32_t *outp);

#ifdef __cplusplus
}
#endif

#endif /* __CODEC_H */
