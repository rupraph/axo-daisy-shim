# daisy-axoloti-shim

Run Axoloti/Ksoloti patches on Daisy Seed hardware. This is a work in progress. It has been tested working with a few patches on a daisyseed 1.2.

This shim layer lets Axoloti/Ksoloti/Akso-generated `xpatch.cpp` files compile and run unmodified on a Daisy Seed — no Axoloti board or firmware required.

## What You Need

- A [Daisy Seed](https://www.electro-smith.com/daisy/daisy) board
- [Ksoloti patcher](https://ksoloti.github.io/) installed (to generate `xpatch.cpp` from your `.axp` patches)
- [DaisyToolchain](https://github.com/electro-smith/DaisyToolchain/releases) installed (provides the ARM compiler, `make`, and `dfu-util`)

## Setup

### 1. Install the DaisyToolchain

Download and run the installer for your OS from the [DaisyToolchain releases page](https://github.com/electro-smith/DaisyToolchain/releases). This gives you `arm-none-eabi-gcc`, `make`, and `dfu-util`.

### 2. Clone and build libDaisy

libDaisy must be cloned **next to** this repository (the Makefile expects it at `../libDaisy`):

```
your-workspace/
├── libDaisy/              ← clone here
└── daisy-axoloti-shim/    ← this repo
```

```bash
cd /path/to/your-workspace
git clone https://github.com/electro-smith/libDaisy.git
cd libDaisy
git submodule update --init --recursive
make
```

### 3. Enable code generation in Ksoloti

The "Generate code" menu option is hidden by default. To enable it:

1. Quit the Ksoloti patcher if it's running
2. Open the preferences file in a text editor:
   - **macOS**: `Ksoloti.app/Contents/Resources/ksoloti.prefs`
   - **Windows**: `Ksoloti/ksoloti.prefs`
   - **Linux**: `ksoloti/ksoloti.prefs`
3. Change `<ExpertMode>false</ExpertMode>` to `<ExpertMode>true</ExpertMode>`
4. Save and restart the patcher

## Usage

### Generate code from your patch

1. Open your `.axp` patch in the Ksoloti patcher
2. Go to **Patch → Generate code** — this creates an `xpatch.cpp` file. The patcher's console output shows the file path.
3. Copy the generated `xpatch.cpp` into this directory:

```bash
cp /path/to/generated/xpatch.cpp /path/to/daisy-axoloti-shim/
```

### Build the daisy patch image

```bash
cd /path/to/daisy-axoloti-shim
make clean && make CPP_SOURCES="src/main.cpp xpatch.cpp"
```

The Makefile auto-detects the patch's firmware ID and ADC channel count from `xpatch.cpp` — no manual configuration needed.

When switching to a different patch, you must rebuild. You can either run `make clean` again, or for a faster rebuild that only recompiles the patch:

```bash
rm -f build/xpatch.o && make CPP_SOURCES="src/main.cpp xpatch.cpp"
```

### Flash to Daisy Seed

1. Connect the Daisy Seed via USB
2. Enter DFU mode: hold **BOOT**, press **RESET**, release **BOOT** — the LED should go dark
3. Flash:

```bash
make program-dfu
```

The board resets automatically after flashing. Audio starts immediately.

To flash a different patch, just replace `xpatch.cpp`, rebuild, put the board back in DFU mode, and flash again.

## MIDI

Both UART and USB MIDI work simultaneously:

- **USB MIDI** — the Daisy appears as a USB MIDI device to your computer/DAW via the micro-USB cable. No extra hardware needed.
- **UART MIDI** — for DIN/TRS MIDI circuits wired to Daisy pins D13/D14 (USART1).

MIDI output routing respects the `midi_device_t` parameter from the patch: `MIDI_DEVICE_DIN` sends to UART, `MIDI_DEVICE_USB_DEVICE` sends to USB, and `MIDI_DEVICE_OMNI` sends to both.

## What Works

| Category    | Examples                                    | Status |
| ----------- | ------------------------------------------- | ------ |
| Oscillators | sine, saw, square, tri, pulse, wavetable    | ✅     |
| Filters     | biquad LP/BP/HP, SVF, allpass, comb, 1-pole | ✅     |
| Envelopes   | ADSR, AD, AR, decay, follow                 | ✅     |
| LFOs        | sine, tri, square, saw                      | ✅     |
| Math        | arithmetic, scaling, comparison, bitwise    | ✅     |
| Logic       | gates, triggers, comparators, toggle        | ✅     |
| Mixing      | crossfade, gain, pan, VCA                   | ✅     |
| Delays      | echo, chorus, flanger (via SDRAM)           | ✅     |
| Noise       | white, pink (via hardware RNG)              | ✅     |
| MIDI in/out | note, CC, pitch bend, sysex                 | ✅     |
| ADC inputs  | knobs/CV via gpio/in/analog objects         | ✅     |

## What Doesn't Work (Yet)

| Category               | Reason                                           |
| ---------------------- | ------------------------------------------------ |
| GPIO digital in/out    | ChibiOS palReadPad/palSetPadMode not yet shimmed |
| USB host MIDI          | Needs USB-A connector soldered to PB14/PB15      |
| Display objects        | No LCD/OLED on bare Daisy Seed                   |
| SD card objects        | No SD slot without adapter                       |
| Live parameter control | Out of scope (no USB parameter protocol)         |

## Troubleshooting

**Build fails with "No rule to make target 'xpatch.cpp'"** — Make sure you copied `xpatch.cpp` into the repo directory.

**"region FLASH overflowed"** — The patch exceeds 128KB. Very complex patches may need the [Daisy bootloader](https://github.com/electro-smith/libDaisy/blob/master/doc/md/_a7_Getting-Started-Daisy-Bootloader.md) to run from QSPI flash (8MB).

**No audio output** — Verify the Daisy Seed's audio codec connections. The shim uses the default AK4556 codec configuration.

**MIDI not working** — Make sure you're sending to the correct USB MIDI device. For UART MIDI, check that your circuit is on pins D13/D14.

**`make program-dfu` fails** — The board isn't in DFU mode. Hold BOOT, press RESET, release BOOT, then try again.

## How It Works

The shim provides all the headers that `xpatch.cpp` expects (`axoloti_math.h`, `patch.h`, `midi.h`, etc.) backed by libDaisy. At the audio boundary, it converts between Daisy's float samples and Axoloti's Q27 fixed-point format. Lookup tables (sine, pitch, window, exp, log, BLEP) are generated at startup from the same formulas as the original firmware. A bump allocator over Daisy's 64MB SDRAM provides `sdram_malloc()` for delay lines and large buffers.

## License

This software is provided **as-is, without warranty of any kind**, express or implied. It is a work in progress — not all Axoloti objects and features have been tested. Use at your own risk.

It references APIs from:

- [libDaisy](https://github.com/electro-smith/libDaisy) (MIT License)
- [Ksoloti](https://github.com/ksoloti/ksoloti) / [Axoloti](https://github.com/axoloti/axoloti) (GPL v3)
