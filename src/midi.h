/*
 * midi.h — Daisy shim layer
 *
 * MIDI types and function declarations matching the Axoloti firmware API.
 * Original copyright (C) 2013, 2014 Johannes Taelman (GPL v3)
 */
#ifndef __MIDI_H
#define __MIDI_H

#include <stdint.h>

/* MIDI Status bytes */
#define MIDI_NOTE_OFF         0x80
#define MIDI_NOTE_ON          0x90
#define MIDI_POLY_PRESSURE    0xa0
#define MIDI_CONTROL_CHANGE   0xb0
#define MIDI_PROGRAM_CHANGE   0xc0
#define MIDI_CHANNEL_PRESSURE 0xd0
#define MIDI_PITCH_BEND       0xe0
#define MIDI_SYSEX_START      0xf0
#define MIDI_MTC              0xf1
#define MIDI_SONG_POSITION    0xf2
#define MIDI_SONG_SELECT      0xf3
#define MIDI_TUNE_REQUEST     0xf6
#define MIDI_SYSEX_END        0xf7
#define MIDI_RESET            0xff
#define MIDI_META_EVENT       0xff
#define MIDI_TIMING_CLOCK     0xf8
#define MIDI_MEASURE_END      0xf9
#define MIDI_START            0xfa
#define MIDI_CONTINUE         0xfb
#define MIDI_STOP             0xfc
#define MIDI_ACTIVE_SENSE     0xfe

/* Controller Numbers */
#define MIDI_C_LSB            0x20
#define MIDI_C_GM_BANK        0x00
#define MIDI_C_MODULATION     0x01
#define MIDI_C_BREATH         0x02
#define MIDI_C_FOOT           0x04
#define MIDI_C_PORTA_TIME     0x05
#define MIDI_C_DATA_ENTRY     0x06
#define MIDI_C_MAIN_VOLUME    0x07
#define MIDI_C_BALANCE        0x08
#define MIDI_C_PAN            0x0a
#define MIDI_C_EXPRESSION     0x0b
#define MIDI_C_GENERAL_1      0x10
#define MIDI_C_GENERAL_2      0x11
#define MIDI_C_GENERAL_3      0x12
#define MIDI_C_GENERAL_4      0x13
#define MIDI_C_DAMPER         0x40
#define MIDI_C_PORTA          0x41
#define MIDI_C_SOSTENUTO      0x42
#define MIDI_C_SOFT_PEDAL     0x43
#define MIDI_C_HOLD_2         0x45
#define MIDI_C_TIMBRE         0x4a
#define MIDI_C_GENERAL_5      0x50
#define MIDI_C_GENERAL_6      0x51
#define MIDI_C_GENERAL_7      0x52
#define MIDI_C_GENERAL_8      0x53
#define MIDI_C_EFFECT_DEPTH   0x5b
#define MIDI_C_TREMELO_DEPTH  0x5c
#define MIDI_C_CHORUS_DEPTH   0x5d
#define MIDI_C_CELESTE_DEPTH  0x5e
#define MIDI_C_PHASER_DEPTH   0x5f
#define MIDI_C_DATA_INC       0x60
#define MIDI_C_DATA_DEC       0x61
#define MIDI_C_NONRPN_LSB    0x62
#define MIDI_C_NONRPN_MSB    0x63
#define MIDI_C_RPN_LSB        0x64
#define MIDI_C_RPN_MSB        0x65
#define MIDI_C_RESET          0x79
#define MIDI_C_LOCAL          0x7a
#define MIDI_C_ALL_NOTES_OFF  0x7b
#define MIDI_C_OMNI_OFF       0x7c
#define MIDI_C_OMNI_ON        0x7d
#define MIDI_C_MONO           0x7e
#define MIDI_C_POLY           0x7f

typedef enum {
    MIDI_DEVICE_OMNI = 0,
    MIDI_DEVICE_DIN,
    MIDI_DEVICE_USB_DEVICE,
    MIDI_DEVICE_USB_HOST,
    MIDI_DEVICE_DIGITAL_X1,
    MIDI_DEVICE_DIGITAL_X2,
    MIDI_DEVICE_INTERNAL = 0x0F
} midi_device_t;

#define MIDI_PORT_OMNI 0

#ifdef __cplusplus
extern "C" {
#endif

void midi_init(void);
void MidiInMsgHandler(midi_device_t dev, uint8_t port,
                      uint8_t b0, uint8_t b1, uint8_t b2);

void MidiSend1(midi_device_t dev, uint8_t port, uint8_t b0);
void MidiSend2(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1);
void MidiSend3(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1,
               uint8_t b2);
void MidiSendSysEx(midi_device_t dev, uint8_t port, uint8_t bytes[],
                   uint8_t len);

int MidiGetOutputBufferPending(midi_device_t dev);
int MidiGetOutputBufferAvailable(midi_device_t dev);

#ifdef __cplusplus
}
#endif

#endif /* __MIDI_H */
