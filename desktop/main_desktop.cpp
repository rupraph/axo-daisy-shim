/*
 * main_desktop.cpp — Desktop audio preview
 *
 * Standalone entry point that compiles xpatch.cpp for macOS/Linux
 * and plays audio through the default output device using miniaudio.
 * Output-only (no microphone input).
 *
 * Build:
 *   clang++ -std=c++14 -O2 \
 *     -include desktop/axoloti_compat_desktop.h \
 *     -I desktop/ -I src/ \
 *     desktop/main_desktop.cpp xpatch.cpp \
 *     -framework CoreAudio -framework AudioToolbox -framework CoreFoundation \
 *     -lpthread -o /tmp/axo_preview
 */

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/* axoloti_compat_desktop.h is force-included via -include flag.
 * It pulls in all the shim headers. */

/* ---- Audio DMA buffers (Q31 fixed-point, stereo interleaved) ---- */
int32_t buf[DOUBLE_BUFSIZE];
int32_t buf2[DOUBLE_BUFSIZE];
int32_t rbuf[DOUBLE_BUFSIZE];
int32_t rbuf2[DOUBLE_BUFSIZE];

/* ---- Patch metadata ---- */
patchMeta_t patchMeta;
uint32_t dspLoad200 = 0;

/* ---- HID stubs ---- */
uint8_t hid_buttons[3] = {0};
uint8_t hid_mouse_x = 0;
uint8_t hid_mouse_y = 0;
uint8_t hid_keys[6] = {0};
uint8_t hid_key_modifiers = 0;

/* ---- ADC values (all zero on desktop — no knobs) ---- */
uint16_t adcvalues[16] = {0};

/* ---- KVP stubs ---- */
struct KeyValuePair *ObjectKvpRoot = nullptr;

/* ---- Linker symbol stubs ---- */

/*
 * SDRAM heap via a large static array.
 * xpatch_init2() calls sdram_init(&_sdram_dyn_start, &_sdram_dyn_end)
 * then IMMEDIATELY calls root.Init() which allocates delay buffers.
 * _sdram_dyn_start must be at the beginning and _sdram_dyn_end at the
 * end of a real memory region, otherwise allocations corrupt memory.
 *
 * We place _sdram_dyn_start as the first byte of a 32 MB array and
 * _sdram_dyn_end as the last, so &_sdram_dyn_start/&_sdram_dyn_end
 * span valid heap memory.
 */
#define SDRAM_HEAP_SIZE (32 * 1024 * 1024)

extern "C" {
typedef void (*funcp_t)(void);

uint32_t _pbss_start = 0;
funcp_t __ctor_array_start = nullptr;
funcp_t __dtor_array_start = nullptr;

/* These are referenced by xpatch_init2 but their addresses don't matter —
 * sdram_init ignores its arguments on desktop (see below). */
char _sdram_dyn_start = 0;
char _sdram_dyn_end = 0;
}

/*
 * Alias *_end to *_start so xpatch_init2's clearing/constructor loops
 * execute zero iterations. Mach-O (macOS) adds a leading underscore
 * to C symbol names at the assembly level.
 */
#ifdef __APPLE__
asm(".globl __pbss_end\n .set __pbss_end, __pbss_start");
asm(".globl ___ctor_array_end\n .set ___ctor_array_end, ___ctor_array_start");
asm(".globl ___dtor_array_end\n .set ___dtor_array_end, ___dtor_array_start");
#else
asm(".globl _pbss_end\n .set _pbss_end, _pbss_start");
asm(".globl __ctor_array_end\n .set __ctor_array_end, __ctor_array_start");
asm(".globl __dtor_array_end\n .set __dtor_array_end, __dtor_array_start");
#endif

/* ---- SDRAM allocator (uses static array on desktop) ---- */
static char sdram_heap_storage[SDRAM_HEAP_SIZE];
static char *sdram_heap_ptr = nullptr;
static char *sdram_heap_end = nullptr;

/*
 * Always point at our static heap, ignoring arguments.
 * xpatch_init2 calls sdram_init(&_sdram_dyn_start, &_sdram_dyn_end) which
 * would be garbage addresses. By ignoring them, the subsequent
 * sdram_malloc calls during root.Init() allocate from valid memory.
 */
void sdram_init(char *base_addr, char *end_addr) {
  (void)base_addr;
  (void)end_addr;
  sdram_heap_ptr = sdram_heap_storage;
  sdram_heap_end = sdram_heap_storage + SDRAM_HEAP_SIZE;
}

void *sdram_malloc(size_t size) {
  size = (size + 3) & ~3;
  if (sdram_heap_ptr + size > sdram_heap_end) {
    return nullptr;
  }
  void *ptr = sdram_heap_ptr;
  sdram_heap_ptr += size;
  return ptr;
}

void sdram_free(void *) {
  /* Stack allocator — no individual free on desktop preview */
}

int32_t sdram_get_free(void) {
  return (int32_t)(sdram_heap_end - sdram_heap_ptr);
}

/* ---- Lookup tables ---- */
int32_t sine2t[SINE2TSIZE + 1];
int16_t windowt[WINDOWSIZE + 1];
uint32_t pitcht[PITCHTSIZE];
uint16_t expt[EXPTSIZE];
uint16_t logt[LOGTSIZE];
int16_t blept[BLEPSIZE];

static void generate_lookup_tables(void) {
  const double PI = 3.14159265358979323846;

  for (int i = 0; i <= SINE2TSIZE; i++) {
    sine2t[i] =
        (int32_t)(2147483647.0 * sin(2.0 * PI * i / (double)SINE2TSIZE));
  }

  for (int i = 0; i <= WINDOWSIZE; i++) {
    windowt[i] =
        (int16_t)(16383.0 * (1.0 - cos(2.0 * PI * i / (double)WINDOWSIZE)) /
                  2.0);
  }

  for (int i = 0; i < PITCHTSIZE; i++) {
    double note = (double)(i - 128) + 64.0;
    double freq = 440.0 * pow(2.0, (note - 69.0) / 12.0);
    double phase_inc = freq / 48000.0 * 4294967296.0;
    if (phase_inc > 2147483647.0)
      phase_inc = 2147483647.0;
    pitcht[i] = (uint32_t)phase_inc;
  }

  for (int i = 0; i < EXPTSIZE; i++) {
    expt[i] = (uint16_t)(65535.0 * pow(2.0, (double)i / 256.0) / 2.0);
  }

  for (int i = 0; i < LOGTSIZE; i++) {
    double v = (double)(i + 1) / (double)LOGTSIZE;
    logt[i] = (uint16_t)(65535.0 * (log(v) / log(2.0) + LOGTSIZEN) /
                         (double)LOGTSIZEN);
  }

  for (int i = 0; i < BLEPSIZE; i++) {
    double t = (double)i / (double)BLEPSIZE * 4.0 - 2.0;
    double sinc_val;
    if (fabs(t) < 1e-10) {
      sinc_val = 1.0;
    } else {
      sinc_val = sin(PI * t) / (PI * t);
    }
    double window = 0.5 * (1.0 - cos(2.0 * PI * i / (double)BLEPSIZE));
    blept[i] = (int16_t)(sinc_val * window * 16384.0);
  }
}

/* ---- Logging ---- */
void LogTextMessage(const char *format, ...) { (void)format; }

/* ---- MIDI stubs (no MIDI on desktop preview) ---- */
void midi_init(void) {}

void MidiSend1(midi_device_t dev, uint8_t port, uint8_t b0) {
  (void)dev;
  (void)port;
  (void)b0;
}
void MidiSend2(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1) {
  (void)dev;
  (void)port;
  (void)b0;
  (void)b1;
}
void MidiSend3(midi_device_t dev, uint8_t port, uint8_t b0, uint8_t b1,
               uint8_t b2) {
  (void)dev;
  (void)port;
  (void)b0;
  (void)b1;
  (void)b2;
}
void MidiSendSysEx(midi_device_t dev, uint8_t port, uint8_t bytes[],
                   uint8_t len) {
  (void)dev;
  (void)port;
  (void)bytes;
  (void)len;
}
int MidiGetOutputBufferPending(midi_device_t dev) {
  (void)dev;
  return 0;
}
int MidiGetOutputBufferAvailable(midi_device_t dev) {
  (void)dev;
  return 128;
}

/* ---- FastLog ---- */
uint32_t FastLog(uint32_t f) {
  if (f == 0)
    return 0;
  Float_t fl;
  fl.i = (int32_t)f;
  uint32_t mantissa = fl.parts.mantissa >> (23 - LOGTSIZEN);
  uint32_t exponent = fl.parts.exponent;
  return (exponent << 15) | logt[mantissa & (LOGTSIZE - 1)];
}

/* ---- Audio callback ---- */

/*
 * Keep a persistent cursor into the DSP output buffer so we never
 * drop samples when miniaudio's frameCount isn't a multiple of BUFSIZE.
 * Without this, PatchProcess advances the oscillator Phase for 16 samples
 * but only a partial chunk gets output → waveform discontinuities → distortion.
 */
static ma_uint32 dsp_cursor =
    BUFSIZE; /* start at BUFSIZE to trigger first fill */

static void audio_callback(ma_device *pDevice, void *pOutput,
                           const void *pInput, ma_uint32 frameCount) {
  (void)pDevice;
  (void)pInput;

  float *out = (float *)pOutput;
  const float inv_q31 = 1.0f / 2147483648.0f;

  for (ma_uint32 i = 0; i < frameCount; i++) {
    /* Refill the DSP buffer when we've consumed all BUFSIZE samples */
    if (dsp_cursor >= BUFSIZE) {
      memset(rbuf, 0, sizeof(rbuf));
      PatchProcess(rbuf, buf);
      dsp_cursor = 0;
    }

    float l = (float)buf[dsp_cursor * 2] * inv_q31;
    float r = (float)buf[dsp_cursor * 2 + 1] * inv_q31;

    /* Clamp to [-1, 1] */
    if (l > 1.0f)
      l = 1.0f;
    if (l < -1.0f)
      l = -1.0f;
    if (r > 1.0f)
      r = 1.0f;
    if (r < -1.0f)
      r = -1.0f;

    out[i * 2] = l;
    out[i * 2 + 1] = r;
    dsp_cursor++;
  }
}

/* ---- Signal handling for clean shutdown ---- */
static volatile bool g_running = true;

static void signal_handler(int sig) {
  (void)sig;
  g_running = false;
}

/* ---- Main ---- */
int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  /* Generate lookup tables */
  generate_lookup_tables();

  /* Initialize the patch.
   * xpatch_init2 calls sdram_init(&_sdram_dyn_start, &_sdram_dyn_end)
   * which correctly sets up the allocator over sdram_heap_storage[],
   * then root.Init() allocates delay buffers via sdram_malloc(). */
#ifdef PATCH_FWID
  xpatch_init(PATCH_FWID);
#else
  xpatch_init2(0);
#endif

  /* Set up miniaudio playback device */
  ma_device_config config = ma_device_config_init(ma_device_type_playback);
  config.playback.format = ma_format_f32;
  config.playback.channels = 2;
  config.sampleRate = SAMPLERATE;
  config.dataCallback = audio_callback;
  config.pUserData = nullptr;

  ma_device device;
  if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
    fprintf(stderr, "Failed to initialize audio device\n");
    return 1;
  }

  if (ma_device_start(&device) != MA_SUCCESS) {
    fprintf(stderr, "Failed to start audio device\n");
    ma_device_uninit(&device);
    return 1;
  }

  fprintf(stderr, "axo-preview: audio running (%d Hz stereo). Kill to stop.\n",
          SAMPLERATE);

  /* Install signal handlers for clean shutdown */
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  /* Block until killed */
  while (g_running) {
    ma_sleep(100);
  }

  /* Clean up */
  ma_device_uninit(&device);

  return 0;
}
