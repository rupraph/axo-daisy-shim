/*
 * gpio_mapping.h — Daisy shim layer
 *
 * Maps Axoloti GPIO calls (ChibiOS PAL) to Daisy GPIO API.
 * Include the appropriate board config to define pin mappings.
 */
#ifndef __GPIO_MAPPING_H
#define __GPIO_MAPPING_H

#include "daisy_seed.h"

/*
 * ChibiOS PAL compatibility macros.
 * These are used by GPIO objects in the Axoloti object library.
 * They map to Daisy's HAL GPIO functions.
 *
 * On Axoloti: palSetPad(GPIOA, 1) sets PA1 high
 * On Daisy:   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
 */

/* Map ChibiOS PAL pin set/clear to STM32 HAL directly */
#define palSetPad(port, pin) \
    HAL_GPIO_WritePin(port, (1U << (pin)), GPIO_PIN_SET)

#define palClearPad(port, pin) \
    HAL_GPIO_WritePin(port, (1U << (pin)), GPIO_PIN_RESET)

#define palReadPad(port, pin) \
    HAL_GPIO_ReadPin(port, (1U << (pin)))

#define palTogglePad(port, pin) \
    HAL_GPIO_TogglePin(port, (1U << (pin)))

/* Mode configuration stubs — GPIO objects call these during init */
#define palSetPadMode(port, pin, mode) \
    do { (void)(port); (void)(pin); (void)(mode); } while(0)

/* ChibiOS PAL mode constants (stubs) */
#define PAL_MODE_OUTPUT_PUSHPULL  0
#define PAL_MODE_INPUT            1
#define PAL_MODE_INPUT_PULLUP     2
#define PAL_MODE_INPUT_PULLDOWN   3

#endif /* __GPIO_MAPPING_H */
