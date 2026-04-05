/*
 * axoloti_defines.h — Daisy shim layer
 *
 * Defines from the original Axoloti firmware, kept identical for compatibility.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __AXOLOTI_DEFINES_H
#define __AXOLOTI_DEFINES_H

#include <stdint.h>

#define PI_F 3.14159265359f
#define SAMPLERATE 48000
#define BUFSIZE 16
#define DOUBLE_BUFSIZE 32
#define BUFSIZE_POW 4

typedef int32_t int32buffer[BUFSIZE];

/* Firmware version: report as Ksoloti 1.1.0.0 for compatibility checks */
#define FWVERSION1 1
#define FWVERSION2 1
#define FWVERSION3 0
#define FWVERSION4 0

#endif /* __AXOLOTI_DEFINES_H */
