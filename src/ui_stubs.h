/*
 * ui_stubs.h — Daisy shim layer
 *
 * Stubs for KeyValuePair (KVP) system. On real Axoloti hardware,
 * KVPs provide a UI display/control system. On Daisy these are no-ops.
 */
#ifndef __UI_STUBS_H
#define __UI_STUBS_H

#include "parameters.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal KeyValuePair struct — just enough to compile */
typedef struct KeyValuePair {
  const char *keyname;
  void *parent;
  uint8_t kvptype;
  uint8_t _pad[64]; /* Union placeholder — never actually used on Daisy */
} KeyValuePair_s;

typedef KeyValuePair_s KeyValuePair;

/* Global KVP root — no-op, just needs to exist */
extern struct KeyValuePair *ObjectKvpRoot;

/* KVP registration functions — all no-ops on Daisy */
static inline void SetKVP_IPVP(struct KeyValuePair *kvp,
                               struct KeyValuePair *parent, const char *keyName,
                               ParameterExchange_t *PEx, int min, int max) {
  (void)kvp;
  (void)parent;
  (void)keyName;
  (void)PEx;
  (void)min;
  (void)max;
}

static inline void SetKVP_APVP(struct KeyValuePair *kvp,
                               struct KeyValuePair *parent, const char *keyName,
                               int length, struct KeyValuePair *array) {
  (void)kvp;
  (void)parent;
  (void)keyName;
  (void)length;
  (void)array;
}

static inline void SetKVP_AVP(struct KeyValuePair *kvp,
                              struct KeyValuePair *parent, const char *keyName,
                              int length, struct KeyValuePair *array) {
  (void)kvp;
  (void)parent;
  (void)keyName;
  (void)length;
  (void)array;
}

static inline void SetKVP_IVP(struct KeyValuePair *kvp,
                              struct KeyValuePair *parent, const char *keyName,
                              int *value, int min, int max) {
  (void)kvp;
  (void)parent;
  (void)keyName;
  (void)value;
  (void)min;
  (void)max;
}

static inline void KVP_RegisterObject(struct KeyValuePair *kvp) { (void)kvp; }

static inline void KVP_ClearObjects(void) {}

#ifdef __cplusplus
}
#endif

#endif /* __UI_STUBS_H */
