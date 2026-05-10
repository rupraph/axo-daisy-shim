/*
 * daisy_seed.h — Board configuration for bare Daisy Seed
 *
 * Default configuration: no extra hardware, just audio I/O and MIDI.
 * ADC channels are not mapped by default.
 *
 * ── ADC Pin Mapping ──────────────────────────────────────────────────────────
 *
 * Axoloti patcher uses STM32 port names (PA0, PB1, etc.) as pin selectors in
 * the ksoloti/gpio/in/analog object. These map to adcvalues[N] indices, which
 * in turn map to Daisy Seed ADC channels and physical pins:
 *
 *  Patcher pin │ adcvalues[] │ Daisy ADC │ Daisy pin
 *  ────────────┼─────────────┼───────────┼──────────
 *  PA0         │      [0]    │    A0     │   D15
 *  PA1         │      [1]    │    A1     │   D16
 *  PA2         │      [2]    │    A2     │   D17
 *  PA3         │      [3]    │    A3     │   D18
 *  PA4         │      [4]    │    A4     │   D19
 *  PA5         │      [5]    │    A5     │   D20
 *  PA6         │      [6]    │    A6     │   D21
 *  PA7         │      [7]    │    A7     │   D22
 *  PB0         │      [8]    │    A8     │   D23
 *  PB1         │      [9]    │    A9     │   D24
 *  PC1         │     [10]    │   A10     │   D25
 *  PC4         │     [11]    │   A11     │   D26
 *  PC5         │     [12]    │   ——      │   (no Daisy pin)
 *  PF6         │     [14]    │   ——      │   (Ksoloti Core only)
 *  PF7         │     [15]    │   ——      │   (Ksoloti Core only)
 *  PF8         │     [16]    │   ——      │   (Ksoloti Core only)
 *  PF9         │     [17]    │   ——      │   (Ksoloti Core only)
 *
 * Use PA0–PA7, PB0, PB1, PC1, PC4 in the patcher → connects to D15–D26.
 * Voltage range: 0–3.3V → 0..64 (frac32, 12-bit precision).
 * Do NOT use PC5 or PF6–PF9; those pins are not available on Daisy Seed.
 *
 * The number of ADC channels is auto-detected from xpatch.cpp by the Makefile
 * (NUM_ADC_CHANNELS). BOARD_NUM_ADC_CHANNELS below is currently unused.
 * ─────────────────────────────────────────────────────────────────────────────
 */
#ifndef __BOARD_DAISY_SEED_H
#define __BOARD_DAISY_SEED_H

/* Number of ADC channels exposed to patch parameters (0 = none, auto-detected
 * by Makefile) */
#define BOARD_NUM_ADC_CHANNELS 0

/* MIDI configuration */
#define BOARD_MIDI_UART_RX daisy::DaisySeed::Pin::D14 /* USART1 RX */

/* Audio: stereo in/out via onboard codec — no configuration needed */

#endif /* __BOARD_DAISY_SEED_H */
