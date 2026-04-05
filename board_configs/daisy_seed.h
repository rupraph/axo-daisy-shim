/*
 * daisy_seed.h — Board configuration for bare Daisy Seed
 *
 * Default configuration: no extra hardware, just audio I/O and MIDI.
 * ADC channels are not mapped by default.
 */
#ifndef __BOARD_DAISY_SEED_H
#define __BOARD_DAISY_SEED_H

/* Number of ADC channels exposed to patch parameters (0 = none) */
#define BOARD_NUM_ADC_CHANNELS 0

/* MIDI configuration */
#define BOARD_MIDI_UART_RX daisy::DaisySeed::Pin::D14  /* USART1 RX */

/* Audio: stereo in/out via onboard codec — no configuration needed */

#endif /* __BOARD_DAISY_SEED_H */
