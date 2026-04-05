
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
    static const uint16_t NPEXCH = 9;
    ParameterExchange_t PExch[NPEXCH];
    int32_t displayVector[6];
    static const uint8_t NPRESETS = 0;
    static const uint8_t NPRESET_ENTRIES = 0;
    static const uint8_t NMODULATIONSOURCES = 0;
    static const uint8_t NMODULATIONTARGETS = 0;
    int32_t PExModulationPrevVal[1][NMODULATIONSOURCES];
/* modsource defines */
/* parameter instance indices */
static const int PARAM_INDEX__star_c__1_amp = 0;
static const int PARAM_INDEX_patcher__1_lspread = 1;
static const int PARAM_INDEX_patcher__1_fb = 2;
static const int PARAM_INDEX_patcher__1_range = 3;
static const int PARAM_INDEX_patcher__1_offset = 4;
static const int PARAM_INDEX_patcher__1_length = 5;
static const int PARAM_INDEX_patcher__1_length__ = 6;
static const int PARAM_INDEX_mix__1_gain1 = 7;
static const int PARAM_INDEX_mix__2_gain1 = 8;
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
outlet_out= adcvalues[0]<<15;
}

}
;class instanceecho__1 {
  public: // v1
  rootc *parent;
uint32_t delaywindex;
uint32_t delayrindex;
int16_t delayline[4453];

  public: void Init(rootc * _parent) {
parent = _parent;
   int i;
   for(i=0;i<4453;i++) delayline[i] = 0;
   delaywindex = 0;
   delayrindex = 1;

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
const int32_t  inlet_feedback,
int32buffer  & outlet_out  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
 int32_t rd = delayline[delayrindex++];
delayline[delaywindex++] = __SSAT((inlet_in[buffer_index]>>14) + ___SMMUL(rd<<5,inlet_feedback),16);
if (delayrindex == 4453) delayrindex = 0;
if (delaywindex == 4453) delaywindex = 0;
outlet_out[buffer_index] = rd<<14;

}
}

}
;class instanceadc_tilde_1__1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_vu) {
parent = _parent;
disp_vu = 0;
}
  public: void Dispose() {
}
  public: void dsp (int32buffer  & outlet_wave,
int32_t & disp_vu  ){
int j;
for(j=0;j<BUFSIZE;j++){
   outlet_wave[j] = AudioInputLeft[j];
}
disp_vu=outlet_wave[0];

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
;class instancepatcher__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancepatcher__1_lspread;
KeyValuePair KVP_instancepatcher__1_fb;
KeyValuePair KVP_instancepatcher__1_range;
KeyValuePair KVP_instancepatcher__1_offset;
KeyValuePair KVP_instancepatcher__1_length;
KeyValuePair KVP_instancepatcher__1_length__;
   static int32_t * GetInitParams(void) {
      static const int32_t p[6]= {
      0,
      0,
      0,
      0,
      10485760,
      10485760
      };
      return (int32_t *)&p[0];
   }    static const uint16_t NPEXCH = 6;
    ParameterExchange_t PExch[NPEXCH];
    int32_t displayVector[3];
    static const uint8_t NPRESETS = 0;
    static const uint8_t NPRESET_ENTRIES = 0;
    static const uint8_t NMODULATIONSOURCES = 0;
    static const uint8_t NMODULATIONTARGETS = 0;
    int32_t PExModulationPrevVal[10][NMODULATIONSOURCES];
/* parameter instance indices */
static const int PARAM_INDEX_lspread_value = 0;
static const int PARAM_INDEX_fb_value = 1;
static const int PARAM_INDEX_range_value = 2;
static const int PARAM_INDEX_offset_value = 3;
static const int PARAM_INDEX_length_d = 4;
static const int PARAM_INDEX_length___d = 5;
   static const int32_t * GetPresets(void) {
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
class voice {
  public:
  uint8_t polyIndex;
    static const uint16_t NPEXCH = 6;
    ParameterExchange_t PExch[NPEXCH];
    int32_t displayVector[3];
    static const uint8_t NPRESETS = 0;
    static const uint8_t NPRESET_ENTRIES = 0;
    static const uint8_t NMODULATIONSOURCES = 0;
    static const uint8_t NMODULATIONTARGETS = 0;
/* modsource defines */
/* parameter instance indices */
static const int PARAM_INDEX_lspread_value = 0;
static const int PARAM_INDEX_fb_value = 1;
static const int PARAM_INDEX_range_value = 2;
static const int PARAM_INDEX_offset_value = 3;
static const int PARAM_INDEX_length_d = 4;
static const int PARAM_INDEX_length___d = 5;
/* controller classes */
/* object classes */
class instancelspread {
  public: // v1
  voice *parent;
KeyValuePair KVP_instancelspread_value;
  public: void Init(voice * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_lspread_value].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancelspread_value,ObjectKvpRoot, "lspread" ,&parent->PExch[PARAM_INDEX_lspread_value], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancelspread_value);
}
  public: void Dispose() {
}
  public: void dsp (int32_t  & outlet_out,
int param_value  ){
outlet_out= param_value;

}

}
;class instancefb {
  public: // v1
  voice *parent;
KeyValuePair KVP_instancefb_value;
  public: void Init(voice * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_fb_value].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancefb_value,ObjectKvpRoot, "fb" ,&parent->PExch[PARAM_INDEX_fb_value], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancefb_value);
}
  public: void Dispose() {
}
  public: void dsp (int32_t  & outlet_out,
int param_value  ){
outlet_out= param_value;

}

}
;class instancerange {
  public: // v1
  voice *parent;
KeyValuePair KVP_instancerange_value;
  public: void Init(voice * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_range_value].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancerange_value,ObjectKvpRoot, "range" ,&parent->PExch[PARAM_INDEX_range_value], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancerange_value);
}
  public: void Dispose() {
}
  public: void dsp (int32_t  & outlet_out,
int param_value  ){
outlet_out= param_value;

}

}
;class instanceoffset {
  public: // v1
  voice *parent;
KeyValuePair KVP_instanceoffset_value;
  public: void Init(voice * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_offset_value].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instanceoffset_value,ObjectKvpRoot, "offset" ,&parent->PExch[PARAM_INDEX_offset_value], 0, 1<<27);
  KVP_RegisterObject(&KVP_instanceoffset_value);
}
  public: void Dispose() {
}
  public: void dsp (int32_t  & outlet_out,
int param_value  ){
outlet_out= param_value;

}

}
;class instancerandtrig__1 {
  public: // v1
  voice *parent;
int32_t val;
int ntrig;

  public: void Init(voice * _parent) {
parent = _parent;
val = (int32_t)(GenerateRandomNumber())>>4;
ntrig = 0;

}
  public: void Dispose() {
}
  public: void dsp (const bool  inlet_trig,
int32_t  & outlet_rand  ){
   if ((inlet_trig>0) && !ntrig) { val = (int32_t)(GenerateRandomNumber())>>4; ntrig=1;}
   else if (!(inlet_trig>0)) ntrig=0;
   outlet_rand= val;

}

}
;class instance_star___1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_a,
const int32_t  inlet_b,
int32_t  & outlet_result  ){
outlet_result= ___SMMUL(inlet_a<<3,inlet_b<<2);

}

}
;class instancelength {
  public: // v1
  voice *parent;
KeyValuePair KVP_instancelength_d;
int32_t val;
int ntrig;

  public: void Init(voice * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_length_d].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instancelength_d,ObjectKvpRoot, "length" ,&parent->PExch[PARAM_INDEX_length_d], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instancelength_d);
val = 0;
ntrig = 0;

}
  public: void Dispose() {
}
  public: void dsp (const bool  inlet_trig,
const int32_t  inlet_d,
int32_t  & outlet_env,
int param_d  ){
if ((inlet_trig > 0) && !ntrig) {
  val = 1 << 27;
  ntrig = 1;
}
else {
  if (!(inlet_trig > 0))
    ntrig = 0;
  int32_t t;
  int32_t dt = param_d + inlet_d;
  MTOF(-dt,t);
  val -= t>>6;
  if (val < 0)
    val = 0;
}
outlet_env = val;

}

}
;class instance_gt___1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in1,
const int32_t  inlet_in2,
bool  & outlet_out  ){
outlet_out= inlet_in1>inlet_in2 ;
}

}
;class instanceinv__1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const bool  inlet_i,
bool  & outlet_o  ){
outlet_o = (inlet_i>0)?0:1;

}

}
;class instance_star___5 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_a,
const int32_t  inlet_b,
int32_t  & outlet_result  ){
outlet_result= ___SMMUL(inlet_a<<3,inlet_b<<2);

}

}
;class instance_plus___3 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in1,
const int32_t  inlet_in2,
int32_t  & outlet_out  ){
outlet_out= inlet_in1+inlet_in2 ;
}

}
;class instancediv__1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in,
int32_t  & outlet_out  ){
outlet_out= inlet_in >>1;
}

}
;class instancec__1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (int32_t & outlet_o  ){
outlet_o= 64;

}

}
;class instanceto__1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_i,
int32_t  & outlet_o  ){
   outlet_o= inlet_i;

}

}
;class instanceplay__1 {
  public: // v1
  voice *parent;
   int pstart;
   int pstop;
   int pos;

  public: void Init(voice * _parent) {
parent = _parent;
pos = 0;
pstart = 0;
pstop = 1;

}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_pos,
const bool  inlet_start,
const bool  inlet_stop,
int32buffer  & outlet_wave  ){
   if ((inlet_start>0) && !pstart) {
      pstart = 1;
      pstop = 0;
      uint32_t asat = __USAT(inlet_pos,27);
      pos = asat>>(27-parent->common->parent->instancet_i.LENGTHPOW);
   } else if (!(inlet_start>0)) {
      pstart = 0;
   }
  if ((inlet_stop>0) && !pstop) {
      pstop = 1;
      pstart = 0;
   } 


int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   if (!pstop) {
       if (pos< parent->common->parent->instancet_i.LENGTH)
              outlet_wave[buffer_index] = parent->common->parent->instancet_i.array[pos++]<<parent->common->parent->instancet_i.GAIN;
	else outlet_wave[buffer_index] = 0;
   } else outlet_wave[buffer_index] = 0;

}
}

}
;class instancein {
  public: // v1
  voice *parent;
int32buffer _inlet;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (int32buffer  & outlet_inlet  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_inlet[buffer_index] = _inlet[buffer_index];
}
}

}
;class instancewindow__1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_phase,
int32_t  & outlet_win  ){
   {
       int32_t r;
       HANNING2TINTERP(inlet_phase<<5,r)
       outlet_win= (r>>4);
   }
}

}
;class instance_star___4 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_a,
const int32_t  inlet_b,
int32_t  & outlet_result  ){
outlet_result= ___SMMUL(inlet_a<<3,inlet_b<<2);

}

}
;class instance_dash___1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in1,
const int32_t  inlet_in2,
int32_t  & outlet_out  ){
outlet_out= inlet_in1-inlet_in2 ;
}

}
;class instancevca_tilde___2 {
  public: // v1
  voice *parent;
   int32_t prev;
    int32_t step;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const int32buffer  inlet_a,
int32buffer  & outlet_o  ){
   step = (inlet_v - prev)>>4;
   int32_t i = prev;
   prev = inlet_v;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_o[buffer_index] =  ___SMMUL(inlet_a[buffer_index],i)<<5;
   i += step;

}
}

}
;class instancevca_tilde___1 {
  public: // v1
  voice *parent;
   int32_t prev;
    int32_t step;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const int32buffer  inlet_a,
int32buffer  & outlet_o  ){
   step = (inlet_v - prev)>>4;
   int32_t i = prev;
   prev = inlet_v;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_o[buffer_index] =  ___SMMUL(inlet_a[buffer_index],i)<<5;
   i += step;

}
}

}
;class instanceL {
  public: // v1
  voice *parent;
int32buffer _outlet;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_outlet  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   _outlet[buffer_index] = inlet_outlet[buffer_index];

}
}

}
;class instancevca_tilde___3 {
  public: // v1
  voice *parent;
   int32_t prev;
    int32_t step;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const int32buffer  inlet_a,
int32buffer  & outlet_o  ){
   step = (inlet_v - prev)>>4;
   int32_t i = prev;
   prev = inlet_v;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_o[buffer_index] =  ___SMMUL(inlet_a[buffer_index],i)<<5;
   i += step;

}
}

}
;class instance_plus___1 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
int32buffer  & outlet_out  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index]= inlet_in1[buffer_index]+inlet_in2[buffer_index] ;
}
}

}
;class instancetabrecord2_tilde___1 {
  public: // v1
  voice *parent;
   int pstart;
   int pstop;
   int pos;

  public: void Init(voice * _parent) {
parent = _parent;
pos = 0;
pstart = 0;
pstop = 1;

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_wave,
const int32_t  inlet_pos,
const bool  inlet_start,
const bool  inlet_stop  ){
   if ((inlet_start>0) && !pstart) {
      pstart = 1;
      pstop = 0;
      uint32_t asat = __USAT(inlet_pos,27);
      pos = asat>>(27-parent->common->parent->instancet_i.LENGTHPOW);
  } else if (!(inlet_start>0)) {
      pstart = 0;
  }
  if ((inlet_stop>0) && !pstop) {
      pstop = 1;
      pstart = 0;
  } 

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   if (!pstop)  {
       if (pos< parent->common->parent->instancet_i.LENGTH)
              parent->common->parent->instancet_i.array[pos++] = __SSAT(inlet_wave[buffer_index],28)>>parent->common->parent->instancet_i.GAIN;
   }
}
}

}
;class instancerandtrig__2 {
  public: // v1
  voice *parent;
int32_t val;
int ntrig;

  public: void Init(voice * _parent) {
parent = _parent;
val = (int32_t)(GenerateRandomNumber())>>4;
ntrig = 0;

}
  public: void Dispose() {
}
  public: void dsp (const bool  inlet_trig,
int32_t  & outlet_rand  ){
   if ((inlet_trig>0) && !ntrig) { val = (int32_t)(GenerateRandomNumber())>>4; ntrig=1;}
   else if (!(inlet_trig>0)) ntrig=0;
   outlet_rand= val;

}

}
;class instance_star___2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_a,
const int32_t  inlet_b,
int32_t  & outlet_result  ){
outlet_result= ___SMMUL(inlet_a<<3,inlet_b<<2);

}

}
;class instancelength__ {
  public: // v1
  voice *parent;
KeyValuePair KVP_instancelength___d;
int32_t val;
int ntrig;

  public: void Init(voice * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_length___d].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instancelength___d,ObjectKvpRoot, "length_" ,&parent->PExch[PARAM_INDEX_length___d], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instancelength___d);
val = 0;
ntrig = 0;

}
  public: void Dispose() {
}
  public: void dsp (const bool  inlet_trig,
const int32_t  inlet_d,
int32_t  & outlet_env,
int param_d  ){
if ((inlet_trig > 0) && !ntrig) {
  val = 1 << 27;
  ntrig = 1;
}
else {
  if (!(inlet_trig > 0))
    ntrig = 0;
  int32_t t;
  int32_t dt = param_d + inlet_d;
  MTOF(-dt,t);
  val -= t>>6;
  if (val < 0)
    val = 0;
}
outlet_env = val;

}

}
;class instance_gt___2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in1,
const int32_t  inlet_in2,
bool  & outlet_out  ){
outlet_out= inlet_in1>inlet_in2 ;
}

}
;class instanceinv__2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const bool  inlet_i,
bool  & outlet_o  ){
outlet_o = (inlet_i>0)?0:1;

}

}
;class instance_star___6 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_a,
const int32_t  inlet_b,
int32_t  & outlet_result  ){
outlet_result= ___SMMUL(inlet_a<<3,inlet_b<<2);

}

}
;class instance_plus___4 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in1,
const int32_t  inlet_in2,
int32_t  & outlet_out  ){
outlet_out= inlet_in1+inlet_in2 ;
}

}
;class instancediv__2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in,
int32_t  & outlet_out  ){
outlet_out= inlet_in >>1;
}

}
;class instancec__2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (int32_t & outlet_o  ){
outlet_o= 64;

}

}
;class instanceto__2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_i,
int32_t  & outlet_o  ){
   outlet_o= inlet_i;

}

}
;class instanceplay__2 {
  public: // v1
  voice *parent;
   int pstart;
   int pstop;
   int pos;

  public: void Init(voice * _parent) {
parent = _parent;
pos = 0;
pstart = 0;
pstop = 1;

}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_pos,
const bool  inlet_start,
const bool  inlet_stop,
int32buffer  & outlet_wave  ){
   if ((inlet_start>0) && !pstart) {
      pstart = 1;
      pstop = 0;
      uint32_t asat = __USAT(inlet_pos,27);
      pos = asat>>(27-parent->common->parent->instancet_i.LENGTHPOW);
   } else if (!(inlet_start>0)) {
      pstart = 0;
   }
  if ((inlet_stop>0) && !pstop) {
      pstop = 1;
      pstart = 0;
   } 


int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   if (!pstop) {
       if (pos< parent->common->parent->instancet_i.LENGTH)
              outlet_wave[buffer_index] = parent->common->parent->instancet_i.array[pos++]<<parent->common->parent->instancet_i.GAIN;
	else outlet_wave[buffer_index] = 0;
   } else outlet_wave[buffer_index] = 0;

}
}

}
;class instancewindow__2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_phase,
int32_t  & outlet_win  ){
   {
       int32_t r;
       HANNING2TINTERP(inlet_phase<<5,r)
       outlet_win= (r>>4);
   }
}

}
;class instance_star___3 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_a,
const int32_t  inlet_b,
int32_t  & outlet_result  ){
outlet_result= ___SMMUL(inlet_a<<3,inlet_b<<2);

}

}
;class instance_dash___2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in1,
const int32_t  inlet_in2,
int32_t  & outlet_out  ){
outlet_out= inlet_in1-inlet_in2 ;
}

}
;class instancevca_tilde___4 {
  public: // v1
  voice *parent;
   int32_t prev;
    int32_t step;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const int32buffer  inlet_a,
int32buffer  & outlet_o  ){
   step = (inlet_v - prev)>>4;
   int32_t i = prev;
   prev = inlet_v;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_o[buffer_index] =  ___SMMUL(inlet_a[buffer_index],i)<<5;
   i += step;

}
}

}
;class instancevca_tilde___6 {
  public: // v1
  voice *parent;
   int32_t prev;
    int32_t step;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const int32buffer  inlet_a,
int32buffer  & outlet_o  ){
   step = (inlet_v - prev)>>4;
   int32_t i = prev;
   prev = inlet_v;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_o[buffer_index] =  ___SMMUL(inlet_a[buffer_index],i)<<5;
   i += step;

}
}

}
;class instanceR {
  public: // v1
  voice *parent;
int32buffer _outlet;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_outlet  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   _outlet[buffer_index] = inlet_outlet[buffer_index];

}
}

}
;class instancevca_tilde___5 {
  public: // v1
  voice *parent;
   int32_t prev;
    int32_t step;

  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const int32buffer  inlet_a,
int32buffer  & outlet_o  ){
   step = (inlet_v - prev)>>4;
   int32_t i = prev;
   prev = inlet_v;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   outlet_o[buffer_index] =  ___SMMUL(inlet_a[buffer_index],i)<<5;
   i += step;

}
}

}
;class instance_plus___2 {
  public: // v1
  voice *parent;
  public: void Init(voice * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
int32buffer  & outlet_out  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index]= inlet_in1[buffer_index]+inlet_in2[buffer_index] ;
}
}

}
;class instancetabrecord2_tilde___2 {
  public: // v1
  voice *parent;
   int pstart;
   int pstop;
   int pos;

  public: void Init(voice * _parent) {
parent = _parent;
pos = 0;
pstart = 0;
pstop = 1;

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_wave,
const int32_t  inlet_pos,
const bool  inlet_start,
const bool  inlet_stop  ){
   if ((inlet_start>0) && !pstart) {
      pstart = 1;
      pstop = 0;
      uint32_t asat = __USAT(inlet_pos,27);
      pos = asat>>(27-parent->common->parent->instancet_i.LENGTHPOW);
  } else if (!(inlet_start>0)) {
      pstart = 0;
  }
  if ((inlet_stop>0) && !pstop) {
      pstop = 1;
      pstart = 0;
  } 

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   if (!pstop)  {
       if (pos< parent->common->parent->instancet_i.LENGTH)
              parent->common->parent->instancet_i.array[pos++] = __SSAT(inlet_wave[buffer_index],28)>>parent->common->parent->instancet_i.GAIN;
   }
}
}

}
;/* controller instances */
/* object instances */
     instancelspread instancelspread_i;
     instancefb instancefb_i;
     instancerange instancerange_i;
     instanceoffset instanceoffset_i;
     instancerandtrig__1 instancerandtrig__1_i;
     instance_star___1 instance_star___1_i;
     instancelength instancelength_i;
     instance_gt___1 instance_gt___1_i;
     instanceinv__1 instanceinv__1_i;
     instance_star___5 instance_star___5_i;
     instance_plus___3 instance_plus___3_i;
     instancediv__1 instancediv__1_i;
     instancec__1 instancec__1_i;
     instanceto__1 instanceto__1_i;
     instanceplay__1 instanceplay__1_i;
     instancein instancein_i;
     instancewindow__1 instancewindow__1_i;
     instance_star___4 instance_star___4_i;
     instance_dash___1 instance_dash___1_i;
     instancevca_tilde___2 instancevca_tilde___2_i;
     instancevca_tilde___1 instancevca_tilde___1_i;
     instanceL instanceL_i;
     instancevca_tilde___3 instancevca_tilde___3_i;
     instance_plus___1 instance_plus___1_i;
     instancetabrecord2_tilde___1 instancetabrecord2_tilde___1_i;
     instancerandtrig__2 instancerandtrig__2_i;
     instance_star___2 instance_star___2_i;
     instancelength__ instancelength___i;
     instance_gt___2 instance_gt___2_i;
     instanceinv__2 instanceinv__2_i;
     instance_star___6 instance_star___6_i;
     instance_plus___4 instance_plus___4_i;
     instancediv__2 instancediv__2_i;
     instancec__2 instancec__2_i;
     instanceto__2 instanceto__2_i;
     instanceplay__2 instanceplay__2_i;
     instancewindow__2 instancewindow__2_i;
     instance_star___3 instance_star___3_i;
     instance_dash___2 instance_dash___2_i;
     instancevca_tilde___4 instancevca_tilde___4_i;
     instancevca_tilde___6 instancevca_tilde___6_i;
     instanceR instanceR_i;
     instancevca_tilde___5 instancevca_tilde___5_i;
     instance_plus___2 instance_plus___2_i;
     instancetabrecord2_tilde___2 instancetabrecord2_tilde___2_i;
/* net latches */
    bool  net1Latch;
    bool  net13Latch;
    bool  net25Latch;
  instancepatcher__1 *common;
  void Init(voice *parent) {
    uint16_t i; for (i=0; i<NPEXCH; i++) {
      PExch[i].pfunction = 0;
    }
   instancelspread_i.Init(parent );
   instancefb_i.Init(parent );
   instancerange_i.Init(parent );
   instanceoffset_i.Init(parent );
   instancerandtrig__1_i.Init(parent );
   instance_star___1_i.Init(parent );
   instancelength_i.Init(parent );
   instance_gt___1_i.Init(parent );
   instanceinv__1_i.Init(parent );
   instance_star___5_i.Init(parent );
   instance_plus___3_i.Init(parent );
   instancediv__1_i.Init(parent );
   instancec__1_i.Init(parent );
   instanceto__1_i.Init(parent );
   instanceplay__1_i.Init(parent );
   instancein_i.Init(parent );
   instancewindow__1_i.Init(parent );
   instance_star___4_i.Init(parent );
   instance_dash___1_i.Init(parent );
   instancevca_tilde___2_i.Init(parent );
   instancevca_tilde___1_i.Init(parent );
   instanceL_i.Init(parent );
   instancevca_tilde___3_i.Init(parent );
   instance_plus___1_i.Init(parent );
   instancetabrecord2_tilde___1_i.Init(parent );
   instancerandtrig__2_i.Init(parent );
   instance_star___2_i.Init(parent );
   instancelength___i.Init(parent );
   instance_gt___2_i.Init(parent );
   instanceinv__2_i.Init(parent );
   instance_star___6_i.Init(parent );
   instance_plus___4_i.Init(parent );
   instancediv__2_i.Init(parent );
   instancec__2_i.Init(parent );
   instanceto__2_i.Init(parent );
   instanceplay__2_i.Init(parent );
   instancewindow__2_i.Init(parent );
   instance_star___3_i.Init(parent );
   instance_dash___2_i.Init(parent );
   instancevca_tilde___4_i.Init(parent );
   instancevca_tilde___6_i.Init(parent );
   instanceR_i.Init(parent );
   instancevca_tilde___5_i.Init(parent );
   instance_plus___2_i.Init(parent );
   instancetabrecord2_tilde___2_i.Init(parent );
      uint16_t k; for (k=0; k<NPEXCH; k++) {
        if (PExch[k].pfunction) {
          (PExch[k].pfunction)(&PExch[k]);
        } else {
          PExch[k].finalvalue = PExch[k].value;
        }
      }
}

void dsp(void) {
 int i;
//--------- <nets> -----------//
    int32_t  net0;
    bool  net1;
    int32buffer  net2;
    int32buffer  net3;
    bool  net4;
    int32_t net5;
    int32_t  net6;
    int32_t  net7;
    int32buffer  net8;
    int32buffer  net9;
    int32buffer  net10;
    int32buffer  net11;
    int32_t  net12;
    bool  net13;
    int32buffer  net14;
    int32buffer  net15;
    int32_t net16;
    int32_t  net17;
    int32_t  net18;
    int32buffer  net19;
    int32buffer  net20;
    int32buffer  net21;
    int32buffer  net22;
    int32_t  net23;
    bool  net24;
    bool  net25;
    int32_t  net26;
    int32_t  net27;
    int32_t  net28;
    int32_t  net29;
    int32_t  net30;
    int32_t  net31;
    int32_t  net32;
    int32_t  net33;
    int32_t net34;
    int32buffer  net35;
    int32buffer  net36;
    int32_t  net37;
    int32buffer  net38;
    int32buffer  net39;
    int32buffer  net40;
    int32_t  net41;
    int32_t  net42;
    int32_t  net43;
    int32_t  net44;
    int32_t  net45;
    int32_t  net46;
    int32_t  net47;
    int32_t  net48;
    int32_t  net49;
    int32_t  net50;
    int32_t  net51;
    int32_t  net52;
//--------- </nets> ----------//
//--------- <zero> ----------//
  int32_t UNCONNECTED_OUTPUT;
  static const int32_t UNCONNECTED_INPUT = 0;
  static const int32buffer zerobuffer = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int32buffer UNCONNECTED_OUTPUT_BUFFER;
//--------- </zero> ----------//
//--------- <controller calls> ----------//
//--------- <object calls> ----------//
  instancelspread_i.dsp(net27, PExch[PARAM_INDEX_lspread_value].finalvalue);
  instancefb_i.dsp(net44, PExch[PARAM_INDEX_fb_value].finalvalue);
  instancerange_i.dsp(net46, PExch[PARAM_INDEX_range_value].finalvalue);
  instanceoffset_i.dsp(net47, PExch[PARAM_INDEX_offset_value].finalvalue);
  instancerandtrig__1_i.dsp(net1Latch, net28);
  instance_star___1_i.dsp(net28, net27, net26);
  instancelength_i.dsp(net1Latch, net26, net0, PExch[PARAM_INDEX_length_d].finalvalue);
  instance_gt___1_i.dsp(net0, 0 , net4);
  instanceinv__1_i.dsp(net4, net1);
  instance_star___5_i.dsp(net28, net46, net45);
  instance_plus___3_i.dsp(net47, net45, net51);
  instancediv__1_i.dsp(net51, net50);
  instancec__1_i.dsp(net5);
  instanceto__1_i.dsp((net5<<21), net6);
  instanceplay__1_i.dsp(net50, net4, 0, net2);
  instancein_i.dsp(net9);
  instancewindow__1_i.dsp(net0, net43);
  instance_star___4_i.dsp(net44, net43, net42);
  instance_dash___1_i.dsp(net6, net42, net7);
  instancevca_tilde___2_i.dsp(net7, net2, net8);
  instancevca_tilde___1_i.dsp(net43, net2, net3);
  instanceL_i.dsp(net3);
  instancevca_tilde___3_i.dsp(net43, net9, net11);
  instance_plus___1_i.dsp(net8, net11, net10);
  instancetabrecord2_tilde___1_i.dsp(net10, net50, net4, 0);
  instancerandtrig__2_i.dsp(net25Latch, net29);
  instance_star___2_i.dsp(net29, net27, net30);
  instancelength___i.dsp(net25Latch, net30, net23, PExch[PARAM_INDEX_length___d].finalvalue);
  instance_gt___2_i.dsp(net23, 0 , net24);
  instanceinv__2_i.dsp(net24, net25);
  instance_star___6_i.dsp(net29, net46, net49);
  instance_plus___4_i.dsp(net47, net49, net52);
  instancediv__2_i.dsp(net52, net37);
  instancec__2_i.dsp(net34);
  instanceto__2_i.dsp((net34<<21), net33);
  instanceplay__2_i.dsp(net37, net24, 0, net35);
  instancewindow__2_i.dsp(net23, net31);
  instance_star___3_i.dsp(net44, net31, net32);
  instance_dash___2_i.dsp(net33, net32, net41);
  instancevca_tilde___4_i.dsp(net41, net35, net38);
  instancevca_tilde___6_i.dsp(net31, net35, net36);
  instanceR_i.dsp(net36);
  instancevca_tilde___5_i.dsp(net31, net9, net39);
  instance_plus___2_i.dsp(net38, net39, net40);
  instancetabrecord2_tilde___2_i.dsp(net40, net37, net24, 0);
//--------- </object calls> ----------//
//--------- <net latch copy> ----------//
net1Latch = net1;
net13Latch = net13;
net25Latch = net25;
//--------- </net latch copy> ----------//
}
void dispose(void) {
 int i;
   instancetabrecord2_tilde___2_i.Dispose();
   instance_plus___2_i.Dispose();
   instancevca_tilde___5_i.Dispose();
   instanceR_i.Dispose();
   instancevca_tilde___6_i.Dispose();
   instancevca_tilde___4_i.Dispose();
   instance_dash___2_i.Dispose();
   instance_star___3_i.Dispose();
   instancewindow__2_i.Dispose();
   instanceplay__2_i.Dispose();
   instanceto__2_i.Dispose();
   instancec__2_i.Dispose();
   instancediv__2_i.Dispose();
   instance_plus___4_i.Dispose();
   instance_star___6_i.Dispose();
   instanceinv__2_i.Dispose();
   instance_gt___2_i.Dispose();
   instancelength___i.Dispose();
   instance_star___2_i.Dispose();
   instancerandtrig__2_i.Dispose();
   instancetabrecord2_tilde___1_i.Dispose();
   instance_plus___1_i.Dispose();
   instancevca_tilde___3_i.Dispose();
   instanceL_i.Dispose();
   instancevca_tilde___1_i.Dispose();
   instancevca_tilde___2_i.Dispose();
   instance_dash___1_i.Dispose();
   instance_star___4_i.Dispose();
   instancewindow__1_i.Dispose();
   instancein_i.Dispose();
   instanceplay__1_i.Dispose();
   instanceto__1_i.Dispose();
   instancec__1_i.Dispose();
   instancediv__1_i.Dispose();
   instance_plus___3_i.Dispose();
   instance_star___5_i.Dispose();
   instanceinv__1_i.Dispose();
   instance_gt___1_i.Dispose();
   instancelength_i.Dispose();
   instance_star___1_i.Dispose();
   instancerandtrig__1_i.Dispose();
   instanceoffset_i.Dispose();
   instancerange_i.Dispose();
   instancefb_i.Dispose();
   instancelspread_i.Dispose();
}
void MidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
}

};

static voice * getVoices(void) {
  static voice v[10];
  return v;
}
static void PropagateToVoices(ParameterExchange_t *origin) {
  ParameterExchange_t *pex = (ParameterExchange_t *)origin->finalvalue;
  uint8_t vi; for (vi = 0; vi < 10; vi++) {
    PExParameterChange(pex,origin->modvalue,0xFFFFFFEE);
    pex = (ParameterExchange_t *)((int)pex + sizeof(voice)); // dirty trick...
  }}
uint8_t voiceNotePlaying[10];
uint32_t voicePriority[10];
uint32_t priority;
uint8_t sustain;
uint8_t pressed[10];

  public: void Init(rootc * _parent) {
parent = _parent;
// on Parent: propagate lspread false patcher__1lspread_value
parent->PExch[PARAM_INDEX_patcher__1_lspread].pfunction = PropagateToSub;
parent->PExch[PARAM_INDEX_patcher__1_lspread].finalvalue = (int32_t)(&(parent->instancepatcher__1_i.PExch[instancepatcher__1::PARAM_INDEX_lspread_value]));
// on Parent: propagate fb false patcher__1fb_value
parent->PExch[PARAM_INDEX_patcher__1_fb].pfunction = PropagateToSub;
parent->PExch[PARAM_INDEX_patcher__1_fb].finalvalue = (int32_t)(&(parent->instancepatcher__1_i.PExch[instancepatcher__1::PARAM_INDEX_fb_value]));
// on Parent: propagate range false patcher__1range_value
parent->PExch[PARAM_INDEX_patcher__1_range].pfunction = PropagateToSub;
parent->PExch[PARAM_INDEX_patcher__1_range].finalvalue = (int32_t)(&(parent->instancepatcher__1_i.PExch[instancepatcher__1::PARAM_INDEX_range_value]));
// on Parent: propagate offset false patcher__1offset_value
parent->PExch[PARAM_INDEX_patcher__1_offset].pfunction = PropagateToSub;
parent->PExch[PARAM_INDEX_patcher__1_offset].finalvalue = (int32_t)(&(parent->instancepatcher__1_i.PExch[instancepatcher__1::PARAM_INDEX_offset_value]));
// on Parent: propagate length false patcher__1length_d
parent->PExch[PARAM_INDEX_patcher__1_length].pfunction = PropagateToSub;
parent->PExch[PARAM_INDEX_patcher__1_length].finalvalue = (int32_t)(&(parent->instancepatcher__1_i.PExch[instancepatcher__1::PARAM_INDEX_length_d]));
// on Parent: propagate length_ false patcher__1length___d
parent->PExch[PARAM_INDEX_patcher__1_length__].pfunction = PropagateToSub;
parent->PExch[PARAM_INDEX_patcher__1_length__].finalvalue = (int32_t)(&(parent->instancepatcher__1_i.PExch[instancepatcher__1::PARAM_INDEX_length___d]));
   int i;
   int j;
   const int32_t *p;
   p = GetInitParams();
   for (j=0; j<6; j++) {
      PExch[j].value = p[j];
      PExch[j].modvalue = p[j];
      PExch[j].signals = 0;
      PExch[j].pfunction = 0;
   }
   int32_t *pp = &PExModulationPrevVal[0][0];
   for (j=0; j<10*NMODULATIONSOURCES; j++) {
      *pp = 0; pp++;
   }
     displayVector[0] = 0x446F7841;
     displayVector[1] = 0;
     displayVector[2] = 0;
uint16_t k; for (k=0; k<NPEXCH; k++) {
  PExch[k].pfunction = PropagateToVoices;
  PExch[k].finalvalue = (int32_t) (&(getVoices()[0].PExch[k]));
}

uint8_t vi; for (vi=0; vi<10; vi++) {
  voice *v = &getVoices()[vi];
  v->polyIndex = vi;
  v->common = this;
  v->Init(&getVoices()[vi]);
  voiceNotePlaying[vi] = 0;
  voicePriority[vi] = 0;
  for (j = 0; j < v->NPEXCH; j++) {
    v->PExch[j].value = 0;
    v->PExch[j].modvalue = 0;
  }
}

for (k = 0; k < NPEXCH; k++) {
  if (PExch[k].pfunction) {
    (PExch[k].pfunction)(&PExch[k]);
  } else {
    PExch[k].finalvalue = PExch[k].value;
  }
}

priority = 0;
sustain = 0;

}
  public: void Dispose() {
uint8_t vi; for (vi=0; vi<10; vi++) {
  voice *v = &getVoices()[vi];
  v->dispose();
}

}
  public: void dsp (const int32buffer  inlet_in,
int32buffer  & outlet_L,
int32buffer  & outlet_R  ){
{
  uint8_t j; for (j=0; j<BUFSIZE; j++) outlet_L[j] = 0;
}
{
  uint8_t j; for (j=0; j<BUFSIZE; j++) outlet_R[j] = 0;
}
uint8_t vi; for (vi=0; vi<10; vi++) {{
  uint8_t j; for (j=0; j<BUFSIZE; j++) getVoices()[vi].instancein_i._inlet[j] = inlet_in[j];
}
getVoices()[vi].dsp();
{
  uint8_t j; for (j=0; j<BUFSIZE; j++) outlet_L[j] += getVoices()[vi].instanceL_i._outlet[j];
}
{
  uint8_t j; for (j=0; j<BUFSIZE; j++) outlet_R[j] += getVoices()[vi].instanceR_i._outlet[j];
}
}

}
void MidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
{
if (0 > 0 && dev > 0 && 0 != dev) return;
if (0 > 0 && port > 0 && 0 != port) return;

if ((status == MIDI_NOTE_ON + 0) && (data2)) {
  int min = 1<<30;
  int min_i = 0;
  uint8_t i; for (i=0; i<10; i++) {
    if (voicePriority[i] < min) {
      min = voicePriority[i];
      min_i = i;
    }
  }

  voicePriority[min_i] = 100000 + priority++;
  voiceNotePlaying[min_i] = data1;
  pressed[min_i] = 1;

  getVoices()[min_i].MidiInHandler(dev, port, status, data1, data2);
} else if (((status == MIDI_NOTE_ON + 0) && (!data2)) || (status == MIDI_NOTE_OFF + 0)) {
  uint8_t i; for (i=0; i<10; i++) {
    if ((voiceNotePlaying[i] == data1) && pressed[i]) {
      voicePriority[i] = priority++;
      pressed[i] = 0;
      if (!sustain)
        getVoices()[i].MidiInHandler(dev, port, status, data1, data2);
      }
  }
} else if (status == 0 + MIDI_CONTROL_CHANGE) {
  uint8_t i; for (i=0; i<10; i++) getVoices()[i].MidiInHandler(dev, port, status, data1, data2);
  if (data1 == 64) {
    if (data2>0) {
      sustain = 1;
    } else if (sustain == 1) {
      sustain = 0;
      for (i=0; i<10; i++) {
        if (pressed[i] == 0) {
          getVoices()[i].MidiInHandler(dev, port, MIDI_NOTE_ON + 0, voiceNotePlaying[i], 0);
        }
      }
    }
  }
} else {  uint8_t i; for (i=0; i<10; i++) getVoices()[i].MidiInHandler(dev, port, status, data1, data2);
}
}
}

}
;class instancemix__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancemix__1_gain1;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_mix__1_gain1].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancemix__1_gain1,ObjectKvpRoot, "mix_1" ,&parent->PExch[PARAM_INDEX_mix__1_gain1], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__1_gain1);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_bus__in,
const int32buffer  inlet_in1,
int32buffer  & outlet_out,
int param_gain1  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
{   int32_t accum = ___SMMUL(inlet_in1[buffer_index],param_gain1);
   outlet_out[buffer_index]=  __SSAT(inlet_bus__in[buffer_index] + (accum<<5),28);
}

}
}

}
;class instancet {
  public: // v1
  rootc *parent;
static const uint32_t LENGTHPOW = 16;
static const uint32_t LENGTH = 1<<16;
static const uint32_t LENGTHMASK = (1<<16)-1;
static const uint32_t BITS = 32;
static const uint32_t GAIN = 0;
int32_t *array;

  public: void Init(rootc * _parent) {
parent = _parent;
static int32_t _array[1][LENGTH] __attribute__ ((section (".sdram")));
array = &_array[parent->polyIndex][0];
{ 
  int i;
  for(i=0;i<LENGTH;i++) array[i]=0;
}

}
  public: void Dispose() {
}
  public: void dsp (  ){
}

}
;class instancemix__2 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancemix__2_gain1;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_mix__2_gain1].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancemix__2_gain1,ObjectKvpRoot, "mix_2" ,&parent->PExch[PARAM_INDEX_mix__2_gain1], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__2_gain1);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_bus__in,
const int32buffer  inlet_in1,
int32buffer  & outlet_out,
int param_gain1  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
{   int32_t accum = ___SMMUL(inlet_in1[buffer_index],param_gain1);
   outlet_out[buffer_index]=  __SSAT(inlet_bus__in[buffer_index] + (accum<<5),28);
}

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
     instanceanalog__1 instanceanalog__1_i;
     instanceecho__1 instanceecho__1_i;
     instanceadc_tilde_1__1 instanceadc_tilde_1__1_i;
     instance_star_c__1 instance_star_c__1_i;
     instancepatcher__1 instancepatcher__1_i;
     instancemix__1 instancemix__1_i;
     instancet instancet_i;
     instancemix__2 instancemix__2_i;
     instancedac_tilde___1 instancedac_tilde___1_i;
/* net latches */
    int32buffer  net1Latch;
static const uint8_t polyIndex = 0;
   static int32_t * GetInitParams(void) {
      static const int32_t p[9]= {
      28311552,
      11534336,
      1468006,
      41943040,
      60817408,
      14680064,
      20971520,
      31457280,
      31457280
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
   for (j=0; j<9; j++) {
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
     displayVector[2] = 3;
   instanceanalog__1_i.Init(this );
   instanceecho__1_i.Init(this );
   instanceadc_tilde_1__1_i.Init(this, displayVector[3] );
   instance_star_c__1_i.Init(this );
   instancepatcher__1_i.Init(this );
   instancemix__1_i.Init(this );
   instancet_i.Init(this );
   instancemix__2_i.Init(this );
   instancedac_tilde___1_i.Init(this, displayVector[4], displayVector[5] );
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
   instancemix__2_i.Dispose();
   instancet_i.Dispose();
   instancemix__1_i.Dispose();
   instancepatcher__1_i.Dispose();
   instance_star_c__1_i.Dispose();
   instanceadc_tilde_1__1_i.Dispose();
   instanceecho__1_i.Dispose();
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
    int32buffer  net2;
    int32buffer  net3;
    int32buffer  net4;
    int32buffer  net5;
    int32buffer  net6;
    int32_t  net7;
//--------- </nets> ----------//
//--------- <zero> ----------//
  int32_t UNCONNECTED_OUTPUT;
  static const int32_t UNCONNECTED_INPUT = 0;
  static const int32buffer zerobuffer = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int32buffer UNCONNECTED_OUTPUT_BUFFER;
//--------- </zero> ----------//
//--------- <controller calls> ----------//
//--------- <object calls> ----------//
  instanceanalog__1_i.dsp(net7);
  instanceecho__1_i.dsp(net1Latch, net7, net6);
  instanceadc_tilde_1__1_i.dsp(net0, displayVector[3]);
  instance_star_c__1_i.dsp(net0, net5, PExch[PARAM_INDEX__star_c__1_amp].finalvalue);
  instancepatcher__1_i.dsp(net5, net3, net4);
  instancemix__1_i.dsp(net0, net3, net1, PExch[PARAM_INDEX_mix__1_gain1].finalvalue);
  instancet_i.dsp();
  instancemix__2_i.dsp(net0, net4, net2, PExch[PARAM_INDEX_mix__2_gain1].finalvalue);
  instancedac_tilde___1_i.dsp(net6, net2, displayVector[4], displayVector[5]);
//--------- </object calls> ----------//
//--------- <net latch copy> ----------//
   for(i=0;i<BUFSIZE;i++)
      net1Latch[i]=net1[i];
//--------- </net latch copy> ----------//
}

void MidiInHandler(midi_device_t dev, uint8_t port, uint8_t status, uint8_t data1, uint8_t data2) {
instancepatcher__1_i.MidiInHandler(dev, port, status, data1, data2);
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
  patchMeta.numPEx = 9;
  patchMeta.patchID = 77665021;
  extern char _sdram_dyn_start;
  extern char _sdram_dyn_end;
  sdram_init(&_sdram_dyn_start,&_sdram_dyn_end);
  root.Init();
  patchMeta.fptr_applyPreset = ApplyPreset;
  patchMeta.fptr_patch_dispose = PatchDispose;
  patchMeta.fptr_MidiInHandler = PatchMidiInHandler;
  patchMeta.fptr_dsp_process = PatchProcess;
}
