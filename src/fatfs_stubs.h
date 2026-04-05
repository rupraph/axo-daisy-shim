/*
 * fatfs_stubs.h — Daisy shim layer
 *
 * Provides FatFS types and functions for patches that use file I/O
 * (e.g. flash/wave playback objects). libDaisy ships with FatFS, so we
 * just include its header. File operations will fail at runtime unless
 * an SD card is wired up and the filesystem is mounted.
 */
#ifndef _FATFS_STUBS_H
#define _FATFS_STUBS_H

#include "ff.h"

#endif /* _FATFS_STUBS_H */
