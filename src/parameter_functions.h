/*
 * parameter_functions.h — Daisy shim layer
 *
 * Parameter processing functions from the Axoloti firmware.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef PARAMETER_FUNCTIONS_H
#define PARAMETER_FUNCTIONS_H

#include "parameters.h"
#include "axoloti_math.h"

static void pfun_signed_clamp(ParameterExchange_t *p) {
    p->finalvalue = __SSAT(p->modvalue, 28);
}

static void pfun_unsigned_clamp(ParameterExchange_t *p) {
    p->finalvalue = __USAT(p->modvalue, 27);
}

static void pfun_signed_clamp_fullrange(ParameterExchange_t *p) {
    p->finalvalue = __SSAT(p->modvalue, 28) << 4;
}

static void pfun_unsigned_clamp_fullrange(ParameterExchange_t *p) {
    p->finalvalue = __USAT(p->modvalue, 27) << 4;
}

static void pfun_signed_clamp_squarelaw(ParameterExchange_t *p) {
    int32_t psat = __SSAT(p->modvalue, 28) << 4;
    if (psat > 0)
        p->finalvalue = ___SMMUL(psat, psat) >> 3;
    else
        p->finalvalue = -___SMMUL(psat, psat) >> 3;
}

static void pfun_unsigned_clamp_squarelaw(ParameterExchange_t *p) {
    int32_t psat = __USAT(p->modvalue, 27) << 4;
    p->finalvalue = ___SMMUL(psat, psat) >> 3;
}

static void pfun_signed_clamp_fullrange_squarelaw(ParameterExchange_t *p) {
    int32_t psat = __SSAT(p->modvalue, 28) << 4;
    if (psat > 0)
        p->finalvalue = ___SMMUL(psat, psat) << 1;
    else
        p->finalvalue = -___SMMUL(psat, psat) << 1;
}

static void pfun_unsigned_clamp_fullrange_squarelaw(ParameterExchange_t *p) {
    int32_t psat = __USAT(p->modvalue, 27) << 4;
    p->finalvalue = ___SMMUL(psat, psat) << 1;
}

static void pfun_kexpltime(ParameterExchange_t *p) {
    int32_t in = (-p->modvalue);
    int32_t out;
    MTOF(in, out);
    p->finalvalue = out >> 2;
}

static void pfun_kexpdtime(ParameterExchange_t *p) {
    int32_t in = (-p->modvalue);
    int32_t out;
    MTOF(in, out);
    p->finalvalue = 0x7FFFFFFF - (out >> 2);
}

#endif /* PARAMETER_FUNCTIONS_H */
