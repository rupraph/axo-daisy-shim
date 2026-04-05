/*
 * daisy_patch.h — Board configuration for Daisy Patch
 *
 * Daisy Patch has: 4 CV inputs, 2 CV outputs, OLED, encoder, audio I/O.
 * CV inputs are mapped to the first 4 patch parameters.
 */
#ifndef __BOARD_DAISY_PATCH_H
#define __BOARD_DAISY_PATCH_H

#define BOARD_NUM_ADC_CHANNELS 4

/* Patch CV input pins */
#define BOARD_ADC_PIN_0 daisy::seed::A0  /* CV IN 1 */
#define BOARD_ADC_PIN_1 daisy::seed::A1  /* CV IN 2 */
#define BOARD_ADC_PIN_2 daisy::seed::A2  /* CV IN 3 */
#define BOARD_ADC_PIN_3 daisy::seed::A3  /* CV IN 4 */

/* CV outputs use DAC channels — DAC1: pin D22, DAC2: pin D23 */
#define BOARD_DAC_PIN_0 daisy::seed::D22  /* CV OUT 1 */
#define BOARD_DAC_PIN_1 daisy::seed::D23  /* CV OUT 2 */

/* OLED: SPI on D7 (MISO), D8 (MOSI), D10 (CS), D9 (SCK), D30 (DC), D29 (RST) */

#endif /* __BOARD_DAISY_PATCH_H */
