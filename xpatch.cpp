
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
    static const uint16_t NPEXCH = 3;
    ParameterExchange_t PExch[NPEXCH];
    int32_t displayVector[37];
    static const uint8_t NPRESETS = 0;
    static const uint8_t NPRESET_ENTRIES = 0;
    static const uint8_t NMODULATIONSOURCES = 0;
    static const uint8_t NMODULATIONTARGETS = 0;
    int32_t PExModulationPrevVal[1][NMODULATIONSOURCES];
/* modsource defines */
/* parameter instance indices */
static const int PARAM_INDEX_osc__1_pitch = 0;
static const int PARAM_INDEX__star_c__1_amp = 1;
static const int PARAM_INDEX_FREEZE_b = 2;
/* controller classes */
/* object classes */
class instanceanalog__1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (int32_t  & outlet_out  ){
outlet_out = adcvalues[0]<<15;
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
;class instance_star_c__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instance_star_c__1_amp;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX__star_c__1_amp].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instance_star_c__1_amp,ObjectKvpRoot, "*c_1" ,&parent->PExch[PARAM_INDEX__star_c__1_amp], 0, 1<<27);
  KVP_RegisterObject(&KVP_instance_star_c__1_amp);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32buffer  & outlet_out,
int param_amp  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index]= ___SMMUL(param_amp,inlet_in[buffer_index])<<1;

}
}

}
;class instancedac__11 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_vu) {
parent = _parent;
disp_vu = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_wave,
int32_t & disp_vu  ){
int j;
for(j=0;j<BUFSIZE;j++){
   AudioOutputLeft[j] += __SSAT(inlet_wave[j],28);
}
disp_vu=inlet_wave[0];

}

}
;class instancedac__21 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_vu) {
parent = _parent;
disp_vu = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_wave,
int32_t & disp_vu  ){
int j;
for(j=0;j<BUFSIZE;j++){
   AudioOutputRight[j] += __SSAT(inlet_wave[j],28);
}
disp_vu=inlet_wave[0];

}

}
;class instanceFREEZE {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instanceFREEZE_b;
  public: void Init(rootc * _parent) {
parent = _parent;
    parent->PExch[PARAM_INDEX_FREEZE_b].signals = 0;
    SetKVP_IPVP(&KVP_instanceFREEZE_b,ObjectKvpRoot, "KVP_instanceFREEZE_b" ,&parent->PExch[PARAM_INDEX_FREEZE_b],0,65535);
  KVP_RegisterObject(&KVP_instanceFREEZE_b);
}
  public: void Dispose() {
}
  public: void dsp (bool  & outlet_o,
int param_b  ){
outlet_o=param_b;

}

}
;class instancescope__128s__trig1 {
  public: // v1
  rootc *parent;
int8_t t[128];
int index;
int32_t pval;

  public: void Init(rootc * _parent,
int8_t * disp_scope) {
parent = _parent;
{
   int _i;
   for(_i=0;_i<128;_i++)
   disp_scope[_i] = 0;
}
int i;
for(i=0;i<128;i++) t[i]=0;
index = 0;
pval = 0;

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
const bool  inlet_hold,
int8_t * disp_scope  ){
int i;
if (index<0) {
   for(i=0;i<16;i++){
      int32_t val = (inlet_in[i])>(1<<19);
      if (val&&(!pval)){
         index=0; break;
      }
      pval = val;
   }
   if (index == 0){
      for(;i<16;i++){
         t[index++]=(uint8_t)(inlet_in[i]>>21);
      }
   }
} else {    for(i=0;i<16;i++){
      if(index==128) {
         index=-1;
         break;
      }
      t[index++]=(uint8_t)(inlet_in[i]>>21);
   }
   if ((index==-1)&&(!inlet_hold)){
     for(i=0;i<128;i++)
       disp_scope[i]=t[i];
     pval = (inlet_in[BUFSIZE-1])>(1<<19);
   }
}

}

}
;/* controller instances */
/* object instances */
     instanceanalog__1 instanceanalog__1_i;
     instanceosc__1 instanceosc__1_i;
     instance_star_c__1 instance_star_c__1_i;
     instancedac__11 instancedac__11_i;
     instancedac__21 instancedac__21_i;
     instanceFREEZE instanceFREEZE_i;
     instancescope__128s__trig1 instancescope__128s__trig1_i;
/* net latches */
static const uint8_t polyIndex = 0;
   static int32_t * GetInitParams(void) {
      static const int32_t p[3]= {
      2097152,
      26214400,
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
   for (j=0; j<3; j++) {
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
     displayVector[2] = 34;
   instanceanalog__1_i.Init(this );
   instanceosc__1_i.Init(this );
   instance_star_c__1_i.Init(this );
   instancedac__11_i.Init(this, displayVector[3] );
   instancedac__21_i.Init(this, displayVector[4] );
   instanceFREEZE_i.Init(this );
   instancescope__128s__trig1_i.Init(this, (int8_t *)(&displayVector[5]) );
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
   instancescope__128s__trig1_i.Dispose();
   instanceFREEZE_i.Dispose();
   instancedac__21_i.Dispose();
   instancedac__11_i.Dispose();
   instance_star_c__1_i.Dispose();
   instanceosc__1_i.Dispose();
   instanceanalog__1_i.Dispose();
}

/* krate */
void dsp (void) {
  uint8_t i;
  for (i=0; i<BUFSIZE; i++) AudioOutputLeft[i] = 0;
  for (i=0; i<BUFSIZE; i++) AudioOutputRight[i] = 0;
//--------- <nets> -----------//
    int32buffer  net0;
    int32buffer  net1;
    bool  net2;
    int32_t  net3;
//--------- </nets> ----------//
//--------- <zero> ----------//
  int32_t UNCONNECTED_OUTPUT;
  static const int32_t UNCONNECTED_INPUT = 0;
  static const int32buffer zerobuffer = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int32buffer UNCONNECTED_OUTPUT_BUFFER;
//--------- </zero> ----------//
//--------- <controller calls> ----------//
//--------- <object calls> ----------//
  instanceanalog__1_i.dsp(net3);
  instanceosc__1_i.dsp(net3, zerobuffer, zerobuffer, net1, PExch[PARAM_INDEX_osc__1_pitch].finalvalue);
  instance_star_c__1_i.dsp(net1, net0, PExch[PARAM_INDEX__star_c__1_amp].finalvalue);
  instancedac__11_i.dsp(net0, displayVector[3]);
  instancedac__21_i.dsp(net0, displayVector[4]);
  instanceFREEZE_i.dsp(net2, PExch[PARAM_INDEX_FREEZE_b].finalvalue);
  instancescope__128s__trig1_i.dsp(net0, net2, (int8_t *)(&displayVector[5]));
//--------- </object calls> ----------//
//--------- <net latch copy> ----------//
//--------- </net latch copy> ----------//
}

void MidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
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
  patchMeta.numPEx = 3;
  patchMeta.patchID = -775857493;
  extern char _sdram_dyn_start;
  extern char _sdram_dyn_end;
  sdram_init(&_sdram_dyn_start,&_sdram_dyn_end);
  root.Init();
  patchMeta.fptr_applyPreset = ApplyPreset;
  patchMeta.fptr_patch_dispose = PatchDispose;
  patchMeta.fptr_MidiInHandler = PatchMidiInHandler;
  patchMeta.fptr_dsp_process = PatchProcess;
}
