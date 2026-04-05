# Phase 1 Investigation Report: Axoloti → Daisy Portability

## 1. Generated Code Analysis (xpatch.cpp)

### Structure

The Ksoloti/Akso patcher generates a single `xpatch.cpp` file containing:

- A `rootc` class with methods: `Init()`, `dsp()`, `Dispose()`, `MidiInHandler()`, `ApplyPreset()`
- A static instance: `static rootc root;`
- Entry point: `xpatch_init(int32_t fwid)` (C linkage, `.boot` section)
- DSP entry: `PatchProcess(int32_t* inbuf, int32_t* outbuf)` — stereo interleaved Q27, 16 samples
- MIDI entry: `PatchMidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2)`
- Audio buffer copy pattern:
  - Input: `AudioInputLeft[i] = inbuf[i*2] >> 4;`
  - Output: `outbuf[i*2] = __SSAT(AudioOutputLeft[i], 28) << 4;`
- Parameter array: `ParameterExchange_t PExch[nparams]` with function pointers for clamping/scaling

### Required Headers (via xpatch.h)

| Header                  | Purpose                                                                       | ChibiOS Dependency      |
| ----------------------- | ----------------------------------------------------------------------------- | ----------------------- |
| `axoloti_defines.h`     | SAMPLERATE=48000, BUFSIZE=16, int32buffer typedef                             | None                    |
| `axoloti_math.h`        | ARM intrinsics, lookup tables (sine2t, pitcht, windowt), sin_q31, mtof48k_q31 | None                    |
| `axoloti_filters.h`     | biquad_state/coefficients, biquad_dsp, biquad_lp/bp/hp_coefs                  | None                    |
| `parameter_functions.h` | pfun_signed_clamp, pfun_unsigned_clamp, etc.                                  | None                    |
| `codec.h`               | buf[32]/rbuf[32] externs, computebufI decl                                    | None                    |
| `midi.h`                | midi_device_t enum, MIDI status/controller defines, MidiSend\*                | None                    |
| `patch.h`               | patchMeta_t, ParameterExchange_t, function pointer typedefs                   | **firmware layer only** |
| `axoloti_memory.h`      | sdram_malloc/free                                                             | None                    |
| `axoloti.h`             | LogTextMessage, master include file                                           | None                    |

**Key finding: Patches have ZERO ChibiOS dependencies.** ChibiOS is used only by the firmware layer (thread mgmt, USB stack, scheduler). The generated xpatch.cpp never calls any ChibiOS function.

## 2. ARM Intrinsic Compatibility (Cortex-M4F → Cortex-M7)

| Intrinsic             | Used For                         | M7 Status                           |
| --------------------- | -------------------------------- | ----------------------------------- |
| `___SMMUL(a,b)`       | Q31 multiply, keep upper 32 bits | ✅ Available (SMMUL instruction)    |
| `___SMMLA(a,b,c)`     | Q31 multiply-accumulate          | ✅ Available                        |
| `___SMMLS(a,b,c)`     | Q31 multiply-subtract            | ✅ Available                        |
| `__SSAT(v,n)`         | Signed saturate to n bits        | ✅ Available                        |
| `__USAT(v,n)`         | Unsigned saturate                | ✅ Available                        |
| `__CLZ(v)`            | Count leading zeros              | ✅ Available                        |
| `___SDIV` / `___UDIV` | Signed/unsigned divide           | ✅ Available                        |
| `VCVT` (via FPU)      | Float↔int conversion             | ✅ fpv5-d16 superset of fpv4-sp-d16 |

**Conclusion: 100% compatible.** No intrinsic rewrites needed.

## 3. Daisy Seed Hardware Comparison

| Feature      | Axoloti (STM32F427)          | Akso (STM32H743)  | Daisy Seed (STM32H750)       |
| ------------ | ---------------------------- | ----------------- | ---------------------------- |
| Core         | Cortex-M4F, 168MHz           | Cortex-M7, 480MHz | Cortex-M7, 480MHz            |
| FPU          | fpv4-sp-d16                  | fpv5-d16          | fpv5-d16                     |
| Flash        | 2MB                          | 2MB               | 128KB (8MB QSPI)             |
| SRAM         | 256KB                        | 1MB               | 512KB + 128KB DTCM           |
| SDRAM        | 8MB                          | 32MB              | 64MB                         |
| Audio        | ADAU1961 codec               | ADAU1961          | WM8731 (via SAI)             |
| Sample Rate  | 48kHz fixed                  | 48kHz             | 48kHz (configurable)         |
| Block Size   | 16 samples                   | 16 samples        | Configurable (set to 16)     |
| Audio Format | Q27 fixed-point, interleaved | Q27 fixed-point   | Float, non-interleaved       |
| MIDI         | DIN + USB                    | DIN + USB         | UART (DIN via TRS)           |
| RNG          | STM32 TRNG                   | STM32 TRNG        | STM32 TRNG (same peripheral) |

## 4. Daisy Audio Interface

```cpp
// Daisy callback signature:
void AudioCallback(const float* const* in, float** out, size_t size);
// in[0][i] = left float, in[1][i] = right float (-1.0 to 1.0)

// Axoloti expects:
void PatchProcess(int32_t* inbuf, int32_t* outbuf);
// inbuf[i*2] = left Q27, inbuf[i*2+1] = right Q27, 16 samples
```

Conversion is straightforward:

- Float → Q27: `(int32_t)(float_val * 134217728.0f)` (1 << 27)
- Q27 → Float: `float_val * (1.0f / 134217728.0f)`

## 5. Object Library Categorization

### CLEAN (~53%, ~190 objects) — No firmware dependencies

- **math/**: all arithmetic, scaling, comparison, bitwise ops
- **osc/**: sine, saw, square, tri, pulse, noise (most), wavetable
- **filter/**: biquad LP/BP/HP, SVF, allpass, comb, 1-pole, DC blocker
- **env/**: ADSR, AD, AR, decay, follow (all use Q27 math)
- **lfo/**: sine, tri, square, saw LFOs (use lookup tables)
- **table/**: read, write, interpolate (if SDRAM-backed → NEEDS_MAPPING)
- **mix/**: crossfade, gain, pan, VCA (pure arithmetic)
- **logic/**: gates, triggers, comparators, toggle, boolean ops
- **mux/**: multiplexers, demuxers (pure routing)
- **conv/**: type converters (interp, bipolar↔unipolar)

### NEEDS_MAPPING (~33%, ~130 objects) — Require shim adaptation

- **ctrl/**: knob/slider objects → read Daisy ADC
- **midi/in/**: MIDI note, CC, pitch bend → MidiInHandler routing
- **midi/out/**: MIDI send → MidiSend\* functions
- **gpio/**: digital in/out → Daisy GPIO
- **delay/**: echo, chorus, flanger → `sdram_malloc` for delay lines
- **noise/**: white/pink noise → `rand_s32()` via STM32 RNG
- **audio/**: in/out routing → codec.h buf[]/rbuf[] buffers
- **disp/**: scope, meter → display vectors (functional without physical display)

### BLOCKED (~14%, ~50 objects) — Missing hardware/firmware

- **display/**: LCD, OLED objects (no display on Daisy Seed)
- **sd/**: SD card read/write (no SD slot without adapter)
- **usb/**: USB audio passthrough, USB host MIDI, USB gamepad
- **i2c/**: specific I2C peripherals (codec config, custom I2C)

## 6. Memory Layout

**Daisy Seed memory map:**

- `0x08000000` — 128KB internal flash (bootloader + firmware)
- `0x20000000` — 512KB SRAM
- `0x24000000` — 128KB DTCM
- `0x90000000` — 8MB QSPI flash (user code, if bootloader used)
- `0xC0000000` — 64MB SDRAM

Axoloti's `sdram_malloc` allocates from SDRAM. We can implement this identically using Daisy's SDRAM region. The 64MB of Daisy SDRAM is 8× the original Axoloti's 8MB.

## 7. Build System

Daisy uses a `core/Makefile` include pattern:

```makefile
TARGET = my_project
CPP_SOURCES = main.cpp
LIBDAISY_DIR = path/to/libDaisy
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
```

Compiler: `arm-none-eabi-gcc 10.3`
Flags: `-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -std=gnu++14`
Linker script: `core/STM32H750IB_flash.lds`
