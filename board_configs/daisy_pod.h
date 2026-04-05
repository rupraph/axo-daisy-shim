/*
 * daisy_pod.h — Board configuration for Daisy Pod
 *
 * Daisy Pod has: 2 knobs, 2 buttons, 1 encoder, 2 LEDs, audio I/O, MIDI.
 * Knobs are mapped to the first 2 patch parameters.
 */
#ifndef __BOARD_DAISY_POD_H
#define __BOARD_DAISY_POD_H

#define BOARD_NUM_ADC_CHANNELS 2

/* Pod knob pins (directly connected ADC inputs) */
#define BOARD_ADC_PIN_0 daisy::seed::A0 /* Knob 1 */
#define BOARD_ADC_PIN_1 daisy::seed::A1 /* Knob 2 */

/* Pod encoder is on pins D26 (A), D27 (B), D28 (click) */
/* Pod buttons are on D27, D28 */
/* Pod LEDs are on D22 (red), D23 (green) */

#endif /* __BOARD_DAISY_POD_H */
