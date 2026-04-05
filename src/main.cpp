/*
 * main.cpp — Daisy shim layer entry point
 *
 * Initializes the Daisy Seed hardware, sets up audio and MIDI,
 * and bridges Daisy's float audio to Axoloti's Q27 fixed-point patch.
 */

#include "axoloti_compat.h"
#include "daisy_seed.h"
#include "hid/usb_midi.h"
#include "patch_wrapper.h"

using namespace daisy;

/* ---- Hardware ---- */
static DaisySeed hw;

/* ---- MIDI ---- */
static MidiUartHandler midi_uart;
static MidiUsbHandler midi_usb;

/* ---- Audio DMA buffers (Q27 fixed-point, stereo interleaved) ---- */
int32_t buf[DOUBLE_BUFSIZE];
int32_t buf2[DOUBLE_BUFSIZE];
int32_t rbuf[DOUBLE_BUFSIZE];
int32_t rbuf2[DOUBLE_BUFSIZE];

/* ---- Patch metadata (referenced by generated code) ---- */
patchMeta_t patchMeta;
uint32_t dspLoad200 = 0;

/* ---- HID stubs ---- */
uint8_t hid_buttons[3] = {0};
uint8_t hid_mouse_x = 0;
uint8_t hid_mouse_y = 0;
uint8_t hid_keys[6] = {0};
uint8_t hid_key_modifiers = 0;

/* ---- ADC values (read by gpio/in/analog objects) ---- */
uint16_t adcvalues[16] = {0};

/* ---- KVP stubs (UI system — no-op on Daisy) ---- */
struct KeyValuePair *ObjectKvpRoot = nullptr;

/*
 * Linker symbols referenced by xpatch_init2().
 * On real Axoloti, these delimit a dynamic-patch BSS section and
 * constructor/destructor arrays in SRAM. On Daisy the patch is
 * statically linked, so BSS is already zeroed by startup code and
 * constructors run normally.
 *
 * We alias every *_end symbol to its *_start counterpart so that:
 *   &start == &end  →  loop executes zero iterations  →  no memory corruption.
 *
 * _sdram_dyn_start / _sdram_dyn_end are plain char variables.
 * xpatch_init2 calls sdram_init() with their addresses, which temporarily
 * points the allocator at useless RAM.  We re-init SDRAM after xpatch_init
 * returns (see main()).
 */
extern "C" {
typedef void (*funcp_t)(void);

/* Patch BSS region — _pbss_end is aliased to _pbss_start via asm below */
uint32_t _pbss_start = 0;

/* Constructor / destructor arrays — ends aliased to starts via asm below */
funcp_t __ctor_array_start = nullptr;
funcp_t __dtor_array_start = nullptr;

/* SDRAM region markers — dummy variables; see re-init in main() */
char _sdram_dyn_start = 0;
char _sdram_dyn_end = 0;
}

/*
 * Assembly-level aliases: make every *_end symbol equal its *_start.
 * This causes the clearing/iteration loops in xpatch_init2 to execute
 * zero iterations, preventing memory corruption.
 */
asm(".global _pbss_end\n .set _pbss_end, _pbss_start");
asm(".global __ctor_array_end\n .set __ctor_array_end, __ctor_array_start");
asm(".global __dtor_array_end\n .set __dtor_array_end, __dtor_array_start");

/* ---- SDRAM allocator state ---- */
static char *sdram_heap_ptr = nullptr;
static char *sdram_heap_end = nullptr;
static char *sdram_last_alloc = nullptr;

void sdram_init(char *base_addr, char *end_addr) {
  sdram_heap_ptr = base_addr;
  sdram_heap_end = end_addr;
  sdram_last_alloc = nullptr;
}

void *sdram_malloc(size_t size) {
  /* Align to 4 bytes */
  size = (size + 3) & ~3;
  if (sdram_heap_ptr + size > sdram_heap_end) {
    return nullptr;
  }
  sdram_last_alloc = sdram_heap_ptr;
  void *ptr = sdram_heap_ptr;
  sdram_heap_ptr += size;
  return ptr;
}

void sdram_free(void *ptr) {
  /* Can only free the last allocation (stack allocator) */
  if (ptr == sdram_last_alloc) {
    sdram_heap_ptr = sdram_last_alloc;
    sdram_last_alloc = nullptr;
  }
}

int32_t sdram_get_free(void) {
  return (int32_t)(sdram_heap_end - sdram_heap_ptr);
}

/* ---- Lookup tables ---- */

/*
 * These tables are normally defined in axoloti_math.c in the firmware.
 * We generate them at startup using the same formulas.
 */
int32_t sine2t[SINE2TSIZE + 1];
int16_t windowt[WINDOWSIZE + 1];
uint32_t pitcht[PITCHTSIZE];
uint16_t expt[EXPTSIZE];
uint16_t logt[LOGTSIZE];
int16_t blept[BLEPSIZE];

static void generate_lookup_tables(void) {
  /* Sine table: Q31 sine over one full period */
  for (int i = 0; i <= SINE2TSIZE; i++) {
    sine2t[i] = (int32_t)(2147483647.0 * sin(2.0 * 3.14159265358979323846 * i /
                                             (double)SINE2TSIZE));
  }

  /* Hann window table: Q15 */
  for (int i = 0; i <= WINDOWSIZE; i++) {
    windowt[i] = (int16_t)(16383.0 *
                           (1.0 - cos(2.0 * 3.14159265358979323846 * i /
                                      (double)WINDOWSIZE)) /
                           2.0);
  }

  /* Pitch table: MIDI pitch to frequency (Q31 phase increment for 48kHz)
   * pitcht[128] = middle C (MIDI 60), covers ±128 semitones around
   * The table maps semitone index to phase increment:
   * freq = 440 * 2^((note-69)/12)
   * phase_inc = freq / 48000 * 2^32
   */
  for (int i = 0; i < PITCHTSIZE; i++) {
    double note = (double)(i - 128) + 64.0; /* offset so index 128 = MIDI 64 */
    double freq = 440.0 * pow(2.0, (note - 69.0) / 12.0);
    double phase_inc = freq / 48000.0 * 4294967296.0;
    if (phase_inc > 2147483647.0)
      phase_inc = 2147483647.0;
    pitcht[i] = (uint32_t)phase_inc;
  }

  /* Exponential table: 2^(i/256) scaled to Q16 */
  for (int i = 0; i < EXPTSIZE; i++) {
    expt[i] = (uint16_t)(65535.0 * pow(2.0, (double)i / 256.0) / 2.0);
  }

  /* Log table: log2 scaled to Q16 */
  for (int i = 0; i < LOGTSIZE; i++) {
    double v = (double)(i + 1) / (double)LOGTSIZE;
    logt[i] = (uint16_t)(65535.0 * (log(v) / log(2.0) + LOGTSIZEN) /
                         (double)LOGTSIZEN);
  }

  /* BLEP table: bandlimited step for anti-aliased oscillators */
  for (int i = 0; i < BLEPSIZE; i++) {
    double t = (double)i / (double)BLEPSIZE * 4.0 - 2.0;
    double sinc_val;
    if (fabs(t) < 1e-10) {
      sinc_val = 1.0;
    } else {
      sinc_val = sin(3.14159265358979323846 * t) / (3.14159265358979323846 * t);
    }
    /* Apply Hann window */
    double window =
        0.5 * (1.0 - cos(2.0 * 3.14159265358979323846 * i / (double)BLEPSIZE));
    /* Integrate and scale to Q15 */
    blept[i] = (int16_t)(sinc_val * window * 16384.0);
  }
}

/* ---- Logging ---- */
void LogTextMessage(const char *format, ...) {
  /* No-op in standalone mode. Could route to USB serial if desired. */
  (void)format;
}

/* ---- MIDI stubs ---- */
void midi_init(void) {}

void MidiInMsgHandler(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1,
                      uint8_t b2) {
  /* Forward to the patch's MIDI handler */
  PatchMidiInHandler(dev, port, b0, b1, b2);
}

/* Route MIDI output to the correct transport based on device type.
 * OMNI sends to both UART and USB. */
static inline void midi_send_routed(midi_device_t dev, uint8_t *data,
                                    size_t len) {
  if (dev == MIDI_DEVICE_DIN) {
    midi_uart.SendMessage(data, len);
  } else if (dev == MIDI_DEVICE_USB_DEVICE) {
    midi_usb.SendMessage(data, len);
  } else {
    /* OMNI or unspecified — send on both */
    midi_uart.SendMessage(data, len);
    midi_usb.SendMessage(data, len);
  }
}

void MidiSend1(midi_device_t dev, uint8_t port, uint8_t b0) {
  (void)port;
  uint8_t data[3] = {b0, 0, 0};
  midi_send_routed(dev, data, 1);
}

void MidiSend2(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1) {
  (void)port;
  uint8_t data[3] = {b0, b1, 0};
  midi_send_routed(dev, data, 2);
}

void MidiSend3(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1,
               uint8_t b2) {
  (void)port;
  uint8_t data[3] = {b0, b1, b2};
  midi_send_routed(dev, data, 3);
}

void MidiSendSysEx(midi_device_t dev, uint8_t port, uint8_t bytes[],
                   uint8_t len) {
  (void)port;
  midi_send_routed(dev, bytes, len);
}

int MidiGetOutputBufferPending(midi_device_t dev) {
  (void)dev;
  return 0;
}
int MidiGetOutputBufferAvailable(midi_device_t dev) {
  (void)dev;
  return 128;
}

/* ---- FastLog stub ---- */
uint32_t FastLog(uint32_t f) {
  if (f == 0)
    return 0;
  Float_t fl;
  fl.i = f;
  uint32_t mantissa = fl.parts.mantissa >> (23 - LOGTSIZEN);
  uint32_t exponent = fl.parts.exponent;
  return (exponent << 15) | logt[mantissa & (LOGTSIZE - 1)];
}

/* ---- Audio callback ---- */
static void AudioCallback(AudioHandle::InputBuffer in,
                          AudioHandle::OutputBuffer out, size_t size) {
#if NUM_ADC_CHANNELS > 0
  /* Update ADC values from Daisy's DMA-driven ADC.
   * Daisy returns 16-bit (0–65535); Axoloti expects 12-bit (0–4095). */
  for (int ch = 0; ch < NUM_ADC_CHANNELS; ch++) {
    adcvalues[ch] = hw.adc.Get(ch) >> 4;
  }
#endif

  /* Convert Daisy float input -> Axoloti Q31 interleaved buffer.
   * PatchProcess() does >>4 internally to get Q27. */
  for (size_t i = 0; i < size; i++) {
    rbuf[i * 2] = (int32_t)(in[0][i] * 2147483648.0f); /* Q31 */
    rbuf[i * 2 + 1] = (int32_t)(in[1][i] * 2147483648.0f);
  }

  /* Call the Axoloti patch DSP */
  PatchProcess(rbuf, buf);

  /* Convert Axoloti Q31 output -> Daisy float.
   * PatchProcess() does <<4 internally (Q27→Q31). */
  const float inv_q31 = 1.0f / 2147483648.0f; /* 1/(1<<31) */
  for (size_t i = 0; i < size; i++) {
    out[0][i] = (float)buf[i * 2] * inv_q31;
    out[1][i] = (float)buf[i * 2 + 1] * inv_q31;
  }
}

/* ---- Main ---- */
int main(void) {
  /* Initialize Daisy Seed hardware */
  hw.Init();

  /* Match Axoloti's audio configuration */
  hw.SetAudioBlockSize(BUFSIZE);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

  /* Initialize SDRAM allocator.
   * Daisy Seed has 64MB SDRAM at 0xC0000000.
   * Reserve the first 1MB for the patch heap.
   */
  sdram_init((char *)0xC0000000, (char *)0xC0000000 + (32 * 1024 * 1024));

  /* Generate lookup tables */
  generate_lookup_tables();

  /* Enable hardware RNG for rand_s32() */
  __HAL_RCC_RNG_CLK_ENABLE();
  RNG->CR |= RNG_CR_RNGEN;

#if NUM_ADC_CHANNELS > 0
  /* Initialize ADC — one channel per knob/CV input.
   * Channel 0 → pin A0 (Daisy D15), channel 1 → A1 (D16), etc.
   * Wire potentiometers: wiper→pin, ends→3V3 and GND. */
  {
    using namespace daisy::seed;
    static constexpr daisy::Pin adc_pins[] = {A0, A1, A2, A3, A4,  A5,
                                              A6, A7, A8, A9, A10, A11};
    daisy::AdcChannelConfig adc_cfg[NUM_ADC_CHANNELS];
    for (int i = 0; i < NUM_ADC_CHANNELS; i++) {
      adc_cfg[i].InitSingle(adc_pins[i]);
    }
    hw.adc.Init(adc_cfg, NUM_ADC_CHANNELS);
    hw.adc.Start();
  }
#endif

  /* Initialize UART MIDI (default pins) */
  MidiUartHandler::Config midi_cfg;
  midi_uart.Init(midi_cfg);
  midi_uart.StartReceive();

  /* Initialize USB MIDI Device (micro-USB connector) */
  MidiUsbHandler::Config usb_midi_cfg;
  usb_midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
  midi_usb.Init(usb_midi_cfg);
  midi_usb.StartReceive();

  /* Initialize the Axoloti patch.
   * PATCH_FWID must match the firmware ID in xpatch.cpp.
   * Extract it: grep -oP '0x[0-9A-Fa-f]+' xpatch.cpp | head -1
   * Or use 0 with the bypass below.
   */
#ifdef PATCH_FWID
  xpatch_init(PATCH_FWID);
#else
  /* Bypass fwid check: call xpatch_init2 directly.
   * The _pbss and __ctor_array symbols are defined as empty ranges,
   * so the clearing/ctor loops are harmless no-ops. */
  xpatch_init2(0);
#endif

  /* Re-initialize SDRAM allocator — xpatch_init2 called sdram_init() with
   * dummy _sdram_dyn addresses, corrupting our allocator state. Restore it.
   * This is safe because root.Init() above doesn't use sdram_malloc. */
  sdram_init((char *)0xC0000000, (char *)0xC0000000 + (32 * 1024 * 1024));

  /* Start audio processing */
  hw.StartAudio(AudioCallback);

  /* Main loop: poll MIDI (UART + USB) */
  while (1) {
    midi_uart.Listen();
    while (midi_uart.HasEvents()) {
      MidiEvent ev = midi_uart.PopEvent();
      /* Reconstruct MIDI status byte from Daisy's enum:
       * MidiMessageType enum: NoteOff=0,NoteOn=1,...PitchBend=6
       * MIDI status:          0x80,     0x90,    ...0xE0       */
      uint8_t status = ((uint8_t)ev.type << 4) | 0x80 | (ev.channel & 0x0F);
      PatchMidiInHandler(MIDI_DEVICE_DIN, 0, status, ev.data[0], ev.data[1]);
    }

    midi_usb.Listen();
    while (midi_usb.HasEvents()) {
      MidiEvent ev = midi_usb.PopEvent();
      uint8_t status = ((uint8_t)ev.type << 4) | 0x80 | (ev.channel & 0x0F);
      PatchMidiInHandler(MIDI_DEVICE_USB_DEVICE, 0, status, ev.data[0],
                         ev.data[1]);
    }
  }
}
