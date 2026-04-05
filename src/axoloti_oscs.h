/*
 * axoloti_oscs.h — Daisy shim layer
 *
 * BLEP oscillator table declaration from the Axoloti firmware.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef _AXOLOTI_OSCS_H
#define _AXOLOTI_OSCS_H

#include "axoloti_math.h"

#define BLEPSIZE 2048
extern int16_t blept[BLEPSIZE];

#endif /* _AXOLOTI_OSCS_H */
