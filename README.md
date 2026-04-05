# daisy-axoloti-shim

Run Axoloti/Ksoloti patches on Daisy Seed hardware.

This shim layer provides the API surface that Axoloti/Ksoloti/Akso-generated `xpatch.cpp` files expect, backed by Electro-Smith's libDaisy. Patches compile unmodified and run standalone on Daisy Seed — no Axoloti board or firmware required.

## What It Does

- Bridges Daisy's float audio to Axoloti's Q27 fixed-point format at the audio callback boundary
- Provides all headers that `xpatch.cpp` includes (`axoloti_math.h`, `patch.h`, `midi.h`, etc.)
- Implements SDRAM allocation, lookup table generation, UART MIDI, and hardware RNG
- Compiles with the standard libDaisy build system (`arm-none-eabi-gcc`, GNU++14)

## Prerequisites

- [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) (arm-none-eabi-gcc 10.3+)
- [libDaisy](https://github.com/electro-smith/libDaisy) — cloned and built adjacent to this directory
- A Daisy Seed board
- [Ksoloti](https://ksoloti.github.io/) or Akso patcher (to generate `xpatch.cpp`)

## Directory Layout

```
daisy-axoloti-shim/
├── Makefile                      # Build config
├── src/
│   ├── main.cpp                  # Daisy entry point + runtime
│   ├── axoloti_compat.h          # Master include (replaces xpatch.h)
│   └── ... (shim headers)        # API-compatible replacements
├── test/
│   └── test_sine.cpp             # Built-in test patch (440Hz sine)
├── board_configs/                # Pin mappings for Daisy variants
└── investigation/                # Research reports
```

## Quick Start

### 1. Clone and build libDaisy

```bash
cd /path/to/your/workspace
git clone https://github.com/electro-smith/libDaisy.git
cd libDaisy
git submodule update --init --recursive
make
cd ..
```

### 2. Build the test patch

```bash
cd daisy-axoloti-shim
make CPP_SOURCES="src/main.cpp test/test_sine.cpp"
```

This compiles a 440Hz sine oscillator that responds to MIDI note-on messages.

### 3. Flash to Daisy Seed

Enter DFU mode (hold BOOT, press RESET, release BOOT), then:

```bash
make program-dfu
```

Or use the Daisy Web Programmer with `build/axo_patch.bin`.

### 4. Use your own patch

1. Open your patch in the Ksoloti/Akso patcher
2. Generate code (the patcher produces `xpatch.cpp`)
3. Copy `xpatch.cpp` into this directory
4. Build:

```bash
make CPP_SOURCES="src/main.cpp xpatch.cpp"
```

5. Flash and run

## How It Works

### Audio Path

```
Daisy SAI (float, -1.0 to 1.0, non-interleaved, 16 samples)
    ↓ float_to_q27()
Axoloti patch (Q27 fixed-point, interleaved stereo, 16 samples)
    ↓ q27_to_float()
Daisy SAI output
```

The conversion happens in `AudioCallback` in `main.cpp`. The patch's `PatchProcess()` function sees exactly the same buffer format it expects on real Axoloti hardware.

### MIDI

UART MIDI on Daisy's default MIDI pins (USART1). The main loop polls for MIDI events and converts them to `PatchMidiInHandler()` calls with the same signature the patch expects.

### Memory

A bump allocator over Daisy's 64MB SDRAM (at `0xC0000000`) provides `sdram_malloc()`. This is the same allocation pattern the original Axoloti firmware uses for delay lines and large buffers.

### Lookup Tables

Sine, pitch-to-frequency, window, exponential, log, and BLEP tables are generated at startup from the same formulas as the original firmware. These are used by oscillator, filter, and math objects.

## What Works

| Category | Examples | Status |
|----------|----------|--------|
| Oscillators | sine, saw, square, tri, pulse, wavetable | ✅ |
| Filters | biquad LP/BP/HP, SVF, allpass, comb, 1-pole | ✅ |
| Envelopes | ADSR, AD, AR, decay, follow | ✅ |
| LFOs | sine, tri, square, saw | ✅ |
| Math | arithmetic, scaling, comparison, bitwise | ✅ |
| Logic | gates, triggers, comparators, toggle | ✅ |
| Mixing | crossfade, gain, pan, VCA | ✅ |
| Delays | echo, chorus, flanger (via SDRAM) | ✅ |
| Noise | white, pink (via hardware RNG) | ✅ |
| MIDI in/out | note, CC, pitch bend, sysex | ✅ |
| Parameters | knobs, faders, parameter exchange | ✅ |

## What Doesn't Work

| Category | Reason |
|----------|--------|
| Display objects | No LCD/OLED on Daisy Seed |
| SD card objects | No SD slot without adapter |
| USB audio passthrough | Different USB stack |
| USB host MIDI | Not available on Daisy Seed |
| Live parameter control | Out of scope (no USB parameter protocol) |

## Build Configuration

The Makefile uses libDaisy's `core/Makefile` include pattern. Key settings:

- **Target MCU**: STM32H750 (Cortex-M7, 480MHz)
- **FPU**: fpv5-d16 (hardware float, single+double)
- **Optimization**: `-O3 -ffast-math`
- **Standard**: GNU++14
- **Audio**: 48kHz, 16-sample blocks, stereo

To adjust the libDaisy path:

```bash
make LIBDAISY_DIR=/path/to/libDaisy CPP_SOURCES="src/main.cpp xpatch.cpp"
```

## Board Variants

Header files in `board_configs/` provide pin mappings for:

- **Daisy Seed** — bare board, default MIDI pins
- **Daisy Pod** — 2 knobs mapped to first 2 ADC params, encoder to param 3
- **Daisy Patch** — 4 CV inputs, 2 CV outputs

To use a board config, include it before `axoloti_compat.h` in your build or add `-include board_configs/daisy_pod.h` to CPPFLAGS.

## Troubleshooting

**"undefined reference to `xpatch_init`"** — You need to add your `xpatch.cpp` to CPP_SOURCES.

**"region FLASH overflowed"** — Complex patches may exceed 128KB. Use the Daisy bootloader to run from QSPI flash (8MB). See [Daisy bootloader docs](https://github.com/electro-smith/libDaisy/blob/master/doc/md/_a7_Getting-Started-Daisy-Bootloader.md).

**No audio output** — Check that your Daisy Seed's audio codec connections are correct. The shim expects the default WM8731 codec configuration.

**MIDI not working** — MIDI uses USART1 by default. Ensure your MIDI circuit is on the correct pins (Daisy Seed pins 13/14).

## Roadmap

- [ ] USB MIDI support (in addition to UART)
- [ ] Daisy Pod knob → parameter mapping
- [ ] QSPI flash bootloader support for large patches
- [ ] ADC → Axoloti ctrl/ object mapping
- [ ] Display object support via Daisy Patch OLED
- [ ] Test with complex real-world patches
- [ ] CI build testing

## License

This shim layer is provided as-is for personal/educational use. It references APIs from:
- [libDaisy](https://github.com/electro-smith/libDaisy) (MIT License)
- [Ksoloti](https://github.com/ksoloti/ksoloti) / [Axoloti](https://github.com/axoloti/axoloti) (GPL v3)
