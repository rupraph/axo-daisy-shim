
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define MIDICHANNEL 0 // DEPRECATED!
void xpatch_init2(int fwid);
extern "C" __attribute__ ((section(".boot"))) void xpatch_init(int fwid) {
  xpatch_init2(fwid);
}

void PatchMidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2);

int32buffer AudioInputLeft;
int32buffer AudioInputRight;
int32buffer AudioOutputLeft;
int32buffer AudioOutputRight;
typedef enum { A_STEREO, A_MONO, A_BALANCED } AudioModeType;
AudioModeType AudioOutputMode = A_STEREO;
AudioModeType AudioInputMode = A_STEREO;

static void PropagateToSub(ParameterExchange_t *origin) {
  ParameterExchange_t *pex = (ParameterExchange_t *)origin->finalvalue;
  PExParameterChange(pex,origin->modvalue,0xFFFFFFEE);
}

class rootc {
  public:
    static const uint16_t NPEXCH = 1;
    ParameterExchange_t PExch[NPEXCH];
    int32_t displayVector[5];
    static const uint8_t NPRESETS = 0;
    static const uint8_t NPRESET_ENTRIES = 0;
    static const uint8_t NMODULATIONSOURCES = 0;
    static const uint8_t NMODULATIONTARGETS = 0;
    int32_t PExModulationPrevVal[1][NMODULATIONSOURCES];
/* modsource defines */
/* parameter instance indices */
static const int PARAM_INDEX_osc__1_pitch = 0;
/* controller classes */
/* object classes */
class instancekeyb1 {
  public: // v1
  rootc *parent;
int8_t _note;
int32_t _gate;
int32_t _gate2;
uint8_t _velo;
uint8_t _rvelo;

  public: void Init(rootc * _parent) {
parent = _parent;
_gate = 0;
_note = 0;

}
  public: void Dispose() {
}
  public: void dsp (int32_t  & outlet_note,
bool  & outlet_gate,
bool  & outlet_gate2,
int32_t  & outlet_velocity,
int32_t  & outlet_releaseVelocity  ){
outlet_note= _note<<21;
outlet_gate= _gate;
outlet_gate2= _gate2;
_gate2 = _gate;
outlet_velocity= _velo<<20;
outlet_releaseVelocity= _rvelo<<20;

}
void MidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
{
if ((status == MIDI_NOTE_ON + 0) && (data2)) {
  _velo = data2;
  _note = data1-64;
  _gate = 1<<27;
  _gate2 = 0;
} else if (((status == MIDI_NOTE_ON + 0) && (!data2))||
          (status == MIDI_NOTE_OFF + 0)) {
  if (_note == data1-64) {
    _rvelo = data2;
    _gate = 0;
  }
} else if ((status == 0 + MIDI_CONTROL_CHANGE)&&(data1 == MIDI_C_ALL_NOTES_OFF)) {
  _gate = 0;
}
}
}

}
;class instanceosc__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instanceosc__1_pitch;
uint32_t Phase;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_osc__1_pitch].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instanceosc__1_pitch,ObjectKvpRoot, "osc_1" ,&parent->PExch[PARAM_INDEX_osc__1_pitch], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instanceosc__1_pitch);
Phase = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_pitch,
const int32buffer  inlet_freq,
const int32buffer  inlet_phase,
int32buffer  & outlet_wave,
int param_pitch  ){
   int32_t freq;
   MTOFEXTENDED(param_pitch + inlet_pitch,freq);

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
Phase += freq + inlet_freq[buffer_index];
int32_t r;
int32_t p2 = Phase + (inlet_phase[buffer_index]<<4);
SINE2TINTERP(p2,r)
outlet_wave[buffer_index]= (r>>4);

}
}

}
;class instance_star___1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_a,
const int32_t  inlet_b,
int32buffer  & outlet_result  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_result[buffer_index]= ___SMMUL(inlet_a[buffer_index]<<3,inlet_b<<2);

}
}

}
;class instancedac_tilde___1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_vuLeft,
int32_t & disp_vuRight) {
parent = _parent;
disp_vuLeft = 0;
disp_vuRight = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_left,
const int32buffer  inlet_right,
int32_t & disp_vuLeft,
int32_t & disp_vuRight  ){
int j;
for(j=0;j<BUFSIZE;j++){
   AudioOutputLeft[j] += __SSAT(inlet_left[j],28);
   AudioOutputRight[j] += __SSAT(inlet_right[j],28);
}
disp_vuLeft=inlet_left[0];
disp_vuRight=inlet_right[0];

}

}
;/* controller instances */
/* object instances */
     instancekeyb1 instancekeyb1_i;
     instanceosc__1 instanceosc__1_i;
     instance_star___1 instance_star___1_i;
     instancedac_tilde___1 instancedac_tilde___1_i;
/* net latches */
static const uint8_t polyIndex = 0;
   static int32_t * GetInitParams(void) {
      static const int32_t p[1]= {
      0
      };
      return (int32_t *)&p[0];
   }   static const int32_t * GetPresets(void) {
      static const int32_t p[NPRESETS][NPRESET_ENTRIES][2] = {
      };
   return &p[0][0][0];
   };
void ApplyPreset(uint8_t index) {
   if (!index) {
     int32_t *p = GetInitParams();
     uint16_t i; for (i=0; i<NPEXCH; i++) {
        PExParameterChange(&PExch[i],p[i],0xFFEF);
     }
   }
   index--;
   if (index < NPRESETS) {
     PresetParamChange_t *pa = (PresetParamChange_t *)(GetPresets());
     PresetParamChange_t *p = &pa[index*NPRESET_ENTRIES];
       uint8_t i; for (i=0; i<NPRESET_ENTRIES; i++) {
         PresetParamChange_t *pp = &p[i];
         if ((pp->pexIndex>=0)&&(pp->pexIndex<NPEXCH)) {
           PExParameterChange(&PExch[pp->pexIndex],pp->value,0xFFEF);         }
         else break;
       }
   }
}
   static PExModulationTarget_t * GetModulationTable(void) {
    static const PExModulationTarget_t PExModulationSources[NMODULATIONSOURCES][NMODULATIONTARGETS] = 
{};
   return (PExModulationTarget_t *)&PExModulationSources[0][0];
   };
/* init */
void Init() {
   int i;
   int j;
   const int32_t *p;
   p = GetInitParams();
   for (j=0; j<1; j++) {
      PExch[j].value = p[j];
      PExch[j].modvalue = p[j];
      PExch[j].signals = 0;
      PExch[j].pfunction = 0;
   }
   int32_t *pp = &PExModulationPrevVal[0][0];
   for (j=0; j<1*NMODULATIONSOURCES; j++) {
      *pp = 0; pp++;
   }
     displayVector[0] = 0x446F7841;
     displayVector[1] = 0;
     displayVector[2] = 2;
   instancekeyb1_i.Init(this );
   instanceosc__1_i.Init(this );
   instance_star___1_i.Init(this );
   instancedac_tilde___1_i.Init(this, displayVector[3], displayVector[4] );
      uint16_t k; for (k=0; k<NPEXCH; k++) {
        if (PExch[k].pfunction) {
          (PExch[k].pfunction)(&PExch[k]);
        } else {
          PExch[k].finalvalue = PExch[k].value;
        }
      }
}

/* dispose */
void Dispose() {
   instancedac_tilde___1_i.Dispose();
   instance_star___1_i.Dispose();
   instanceosc__1_i.Dispose();
   instancekeyb1_i.Dispose();
}

/* krate */
void dsp (void) {
  uint8_t i;
  for (i=0; i<BUFSIZE; i++) AudioOutputLeft[i] = 0;
  for (i=0; i<BUFSIZE; i++) AudioOutputRight[i] = 0;
//--------- <nets> -----------//
    int32buffer  net0;
    int32_t  net1;
    int32buffer  net2;
    bool  net3;
//--------- </nets> ----------//
//--------- <zero> ----------//
  int32_t UNCONNECTED_OUTPUT;
  static const int32_t UNCONNECTED_INPUT = 0;
  static const int32buffer zerobuffer = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int32buffer UNCONNECTED_OUTPUT_BUFFER;
//--------- </zero> ----------//
//--------- <controller calls> ----------//
//--------- <object calls> ----------//
  instancekeyb1_i.dsp(net1, net3, (bool &)UNCONNECTED_OUTPUT, UNCONNECTED_OUTPUT, UNCONNECTED_OUTPUT);
  instanceosc__1_i.dsp(net1, zerobuffer, zerobuffer, net2, PExch[PARAM_INDEX_osc__1_pitch].finalvalue);
  instance_star___1_i.dsp(net2, (net3?(1<<27)-1:0), net0);
  instancedac_tilde___1_i.dsp(net0, net0, displayVector[3], displayVector[4]);
//--------- </object calls> ----------//
//--------- <net latch copy> ----------//
//--------- </net latch copy> ----------//
}

void MidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
instancekeyb1_i.MidiInHandler(dev, port, status, data1, data2);
}

};

static rootc root;
void PatchProcess( int32_t * inbuf, int32_t * outbuf) {
  uint8_t i; for (i=0; i<BUFSIZE; i++) {
    AudioInputLeft[i] = inbuf[i*2]>>4;
    switch (AudioInputMode) {
       case A_MONO:
             AudioInputRight[i] = AudioInputLeft[i]; break;
       case A_BALANCED:
             AudioInputLeft[i] = (AudioInputLeft[i] - (inbuf[i*2+1]>>4) ) >> 1;
             AudioInputRight[i] = AudioInputLeft[i];             break;
       case A_STEREO:
       default:
             AudioInputRight[i] = inbuf[i*2+1]>>4;
     }
  }
  root.dsp();
  for (i=0; i<BUFSIZE; i++) {
    outbuf[i*2] = __SSAT(AudioOutputLeft[i],28)<<4;
    switch (AudioOutputMode) {
      case A_MONO:
        outbuf[i*2+1] = 0; break;
      case A_BALANCED:
        outbuf[i*2+1] = ~ outbuf[i*2]; break;
      case A_STEREO:
      default:
        outbuf[i*2+1] = __SSAT(AudioOutputRight[i],28)<<4;
    }
  }
}

void ApplyPreset(int32_t i) {
   root.ApplyPreset(i);
}

void PatchMidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
  root.MidiInHandler(dev, port, status, data1, data2);
}

typedef void (*funcp_t)(void);
typedef funcp_t * funcpp_t;
extern funcp_t __ctor_array_start;
extern funcp_t __ctor_array_end;extern funcp_t __dtor_array_start;
extern funcp_t __dtor_array_end;void PatchDispose( ) {
  root.Dispose();
  {
    funcpp_t fpp = &__dtor_array_start;
    while (fpp < &__dtor_array_end) {
      (*fpp)();
      fpp++;
    }
  }
}

void xpatch_init2(int fwid)
{
  if (fwid != 0xEB8B6CD0) {
    return;  }
  extern uint32_t _pbss_start;
  extern uint32_t _pbss_end;
  volatile uint32_t *p;
  for (p=&_pbss_start; p<&_pbss_end; p++) *p = 0;
  {
    funcpp_t fpp = &__ctor_array_start;
    while (fpp < &__ctor_array_end) {
      (*fpp)();
      fpp++;
    }
  }
  patchMeta.npresets = 0;
  patchMeta.npreset_entries = 0;
  patchMeta.pPresets = (PresetParamChange_t*) root.GetPresets();
  patchMeta.pPExch = &root.PExch[0];
  patchMeta.pDisplayVector = &root.displayVector[0];
  patchMeta.numPEx = 1;
  patchMeta.patchID = -6875425;
  extern char _sdram_dyn_start;
  extern char _sdram_dyn_end;
  sdram_init(&_sdram_dyn_start,&_sdram_dyn_end);
  root.Init();
  patchMeta.fptr_applyPreset = ApplyPreset;
  patchMeta.fptr_patch_dispose = PatchDispose;
  patchMeta.fptr_MidiInHandler = PatchMidiInHandler;
  patchMeta.fptr_dsp_process = PatchProcess;
}
