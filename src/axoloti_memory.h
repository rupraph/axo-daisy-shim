/*
 * axoloti_memory.h — Daisy shim layer
 *
 * SDRAM allocator matching the Axoloti firmware API.
 * Uses Daisy Seed's external SDRAM (64MB at 0xC0000000).
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef AXOLOTI_MEMORY_H
#define AXOLOTI_MEMORY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void sdram_init(char *base_addr, char *end_addr);
void *sdram_malloc(size_t size);
void sdram_free(void *ptr);
int32_t sdram_get_free(void);

#ifdef __cplusplus
}
#endif

#endif /* AXOLOTI_MEMORY_H */
