/*
 * parameters.h — Daisy shim layer
 *
 * Parameter exchange types from the original Axoloti firmware.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __PARAMETERS_H
#define __PARAMETERS_H
#include <stdint.h>

typedef struct _ParameterExchange {
    int32_t value;
    int32_t modvalue;
    int32_t finalvalue;
    void (*pfunction)(struct _ParameterExchange *);
    uint32_t signals;
} ParameterExchange_t;

typedef ParameterExchange_t Parameter_t;

typedef struct {
    char name[8];
} Parameter_name_t;

typedef struct {
    int32_t parameterIndex;
    int32_t amount;
} PExModulationTarget_t;

typedef struct {
    int32_t pexIndex;
    int32_t value;
} PresetParamChange_t;

typedef struct {
    int32_t displaytype;
} Display_meta_t;

/* Stub implementations for parameter change dispatch */
static inline void PExModulationSourceChange(PExModulationTarget_t *modulation,
                                              uint16_t nTargets,
                                              ParameterExchange_t *parameters,
                                              int32_t *oldvalue,
                                              int32_t value)
{
    int i;
    *oldvalue = value;
    for (i = 0; i < nTargets; i++) {
        ParameterExchange_t *p = &parameters[modulation[i].parameterIndex];
        p->modvalue = p->value + (int32_t)(((int64_t)modulation[i].amount * value) >> 27);
        if (p->pfunction) {
            p->pfunction(p);
        } else {
            p->finalvalue = p->modvalue;
        }
    }
}

static inline void PExParameterChange(ParameterExchange_t *param, int32_t value,
                                       uint32_t signals)
{
    param->value = value;
    param->modvalue = value;
    if (param->pfunction) {
        param->pfunction(param);
    } else {
        param->finalvalue = value;
    }
    param->signals |= signals;
}

#endif /* __PARAMETERS_H */
