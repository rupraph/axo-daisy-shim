
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
    static const uint16_t NPEXCH = 19;
    ParameterExchange_t PExch[NPEXCH];
    int32_t displayVector[11];
    static const uint8_t NPRESETS = 0;
    static const uint8_t NPRESET_ENTRIES = 0;
    static const uint8_t NMODULATIONSOURCES = 0;
    static const uint8_t NMODULATIONTARGETS = 0;
    int32_t PExModulationPrevVal[1][NMODULATIONSOURCES];
/* modsource defines */
static const int MODULATOR_instanceamp = 0;
/* parameter instance indices */
static const int PARAM_INDEX_sine__1_pitch = 0;
static const int PARAM_INDEX_comp__1_tresh = 1;
static const int PARAM_INDEX_comp__1_ratio = 2;
static const int PARAM_INDEX_square__1_pitch = 3;
static const int PARAM_INDEX__star_c__2_amp = 4;
static const int PARAM_INDEX_fdn4__1_g = 5;
static const int PARAM_INDEX_mix__1_gain1 = 6;
static const int PARAM_INDEX_mix__1_gain2 = 7;
static const int PARAM_INDEX__star_c__1_amp = 8;
static const int PARAM_INDEX_mix__3_gain1 = 9;
static const int PARAM_INDEX_mix__3_gain2 = 10;
static const int PARAM_INDEX_hp_space_1_pitch = 11;
static const int PARAM_INDEX_hp_space_1_reso = 12;
static const int PARAM_INDEX_mix__2_gain1 = 13;
static const int PARAM_INDEX_mix__2_gain2 = 14;
static const int PARAM_INDEX_mix__4_gain1 = 15;
static const int PARAM_INDEX_mix__4_gain2 = 16;
static const int PARAM_INDEX_hp_space_2_pitch = 17;
static const int PARAM_INDEX_hp_space_2_reso = 18;
/* controller classes */
/* object classes */
class instancein__1 {
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
;class instancefollower__1 {
  public: // v1
  rootc *parent;
int32_t accu;

  public: void Init(rootc * _parent) {
parent = _parent;
accu = 0;

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32_t  & outlet_amp  ){
 int sabs = 0;
int i;  for(i=0;i<BUFSIZE;i++){    int32_t v = inlet_in[i];
    sabs += v>0?v:-v;
  }
  accu -= accu>>4;
  accu += sabs>>(4+4);
  outlet_amp = accu;

}

}
;class instancemuls__3 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in,
int32_t  & outlet_out  ){
outlet_out= __SSAT(inlet_in,27)<<1;
}

}
;class instanceamp {
  public: // v1
  rootc *parent;
int ntrig;

  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_v,
const bool  inlet_trig  ){
if ((inlet_trig>0) && !ntrig) {
  PExModulationSourceChange(
    &parent->GetModulationTable()[parent->MODULATOR_instanceamp*NMODULATIONTARGETS],
    NMODULATIONTARGETS,
    &parent->PExch[0],
    &parent->PExModulationPrevVal[parent->polyIndex][parent->MODULATOR_instanceamp],
    inlet_v);  ntrig=1;
}
if (!(inlet_trig>0)) ntrig=0;

}

}
;class instancevu__2 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_v) {
parent = _parent;
disp_v = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32_t & disp_v  ){
disp_v=inlet_in[0];

}

}
;class instancesine__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancesine__1_pitch;
uint32_t Phase;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_sine__1_pitch].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instancesine__1_pitch,ObjectKvpRoot, "sine_1" ,&parent->PExch[PARAM_INDEX_sine__1_pitch], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instancesine__1_pitch);
Phase = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_pitch,
int32_t  & outlet_wave,
int param_pitch  ){
   {      int32_t freq;
      MTOFEXTENDED(param_pitch + inlet_pitch,freq);
      Phase += freq>>2;
      int32_t r;
      SINE2TINTERP(Phase,r)
      outlet_wave= (r>>4);
   }
}

}
;class instancecomp__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancecomp__1_tresh;
KeyValuePair KVP_instancecomp__1_ratio;
int32_t frac_log(int32_t a) {
	Float_t f;
	f.f = a;
	int32_t r1 = ((f.parts.exponent&0x7F)-18) << 24;
	int32_t r3 = logt[f.parts.mantissa>>15]<<10;
	return r1 + r3;
}

int32_t frac_exp(int32_t a) {
	int8_t s = (a>>24)+4;
	uint8_t ei = a>>16;
	if (s>=0)
	     return expt[ei]<<s;
	else return expt[ei]>>(-s);
}

  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_comp__1_tresh].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancecomp__1_tresh,ObjectKvpRoot, "comp_1:tresh" ,&parent->PExch[PARAM_INDEX_comp__1_tresh], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancecomp__1_tresh);
parent->PExch[PARAM_INDEX_comp__1_ratio].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancecomp__1_ratio,ObjectKvpRoot, "comp_1:ratio" ,&parent->PExch[PARAM_INDEX_comp__1_ratio], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancecomp__1_ratio);
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in,
int32_t  & outlet_out,
int param_tresh,
int param_ratio  ){
int32_t inlog = frac_log(inlet_in);
int32_t treshlog = frac_log(param_tresh);
int32_t over = inlog-treshlog;
int32_t gain;
if (over<0){
	gain = 0x80000;
} else {
	gain = frac_exp(-___SMMUL(over,param_ratio)<<5);
}
outlet_out = gain<<8;//___SMMUL(inlet_in,gain);

}

}
;class instancechart__1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_v) {
parent = _parent;
disp_v = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_in,
int32_t & disp_v  ){
disp_v=inlet_in;

}

}
;class instancesquare__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancesquare__1_pitch;
int32_t Phase;
uint32_t r;

  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_square__1_pitch].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instancesquare__1_pitch,ObjectKvpRoot, "square_1" ,&parent->PExch[PARAM_INDEX_square__1_pitch], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instancesquare__1_pitch);
   Phase = 0;
   r = 1;

}
  public: void Dispose() {
}
  public: void dsp (const int32_t  inlet_pitch,
const bool  inlet_reset,
bool  & outlet_wave,
int param_pitch  ){
   {
      if (inlet_reset && r) {
         Phase = 0;
         r = 0;
      } else {
         if (!inlet_reset) r = 1;
         int32_t freq;
         MTOFEXTENDED(param_pitch + inlet_pitch,freq);
         Phase += freq>>2;
      }
      outlet_wave= (Phase>0)?1:0;
   }
}

}
;class instance_star_c__2 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instance_star_c__2_amp;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX__star_c__2_amp].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instance_star_c__2_amp,ObjectKvpRoot, "*c_2" ,&parent->PExch[PARAM_INDEX__star_c__2_amp], 0, 1<<27);
  KVP_RegisterObject(&KVP_instance_star_c__2_amp);
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
;class instancefdn4__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancefdn4__1_g;
int32_t d1d[700];
int32_t d2d[829];
int32_t d3d[1244];
int32_t d4d[277];
uint32_t d1p;
uint32_t d2p;
uint32_t d3p;
uint32_t d4p;

  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_fdn4__1_g].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancefdn4__1_g,ObjectKvpRoot, "fdn4_1" ,&parent->PExch[PARAM_INDEX_fdn4__1_g], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancefdn4__1_g);
int i;for(i=0;i<700;i++) d1d[i]=0;
for(i=0;i<829;i++) d2d[i]=0;
for(i=0;i<1244;i++) d3d[i]=0;
for(i=0;i<277;i++) d4d[i]=0;
d1p = 0;
d2p = 0;
d3p = 0;
d4p = 0;

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
const int32buffer  inlet_in3,
const int32buffer  inlet_in4,
int32buffer  & outlet_out1,
int32buffer  & outlet_out2,
int32buffer  & outlet_out3,
int32buffer  & outlet_out4,
int param_g  ){
int32_t g2 = 755299123+param_g;

int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t i1 = inlet_in1[buffer_index] + d1d[d1p];
int32_t i2 = inlet_in2[buffer_index] + d2d[d2p];
int32_t i3 = inlet_in3[buffer_index] + d3d[d3p];
int32_t i4 = inlet_in4[buffer_index] + d4d[d4p];
int32_t t1 = i2 + i3;
int32_t t2 = -i1 - i4;
int32_t t3 = i1 - i4;
int32_t t4 = i2 - i3;
int32_t o1 = ___SMMLA(t1,g2,t1>>1);
int32_t o2 = ___SMMLA(t2,g2,t2>>1);
int32_t o3 = ___SMMLA(t3,g2,t3>>1);
int32_t o4 = ___SMMLA(t4,g2,t4>>1);
d1d[d1p] = o1;
d2d[d2p] = o2;
d3d[d3p] = o3;
d4d[d4p] = o4;
d1p++;
d2p++;
d3p++;
d4p++;
if (d1p == 700) d1p = 0;
if (d2p == 829) d2p = 0;
if (d3p == 1244) d3p = 0;
if (d4p == 277) d4p = 0;
outlet_out1[buffer_index] = o1;
outlet_out2[buffer_index] = o2;
outlet_out3[buffer_index] = o3;
outlet_out4[buffer_index] = o4;

}
}

}
;class instancemix__1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancemix__1_gain1;
KeyValuePair KVP_instancemix__1_gain2;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_mix__1_gain1].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__1_gain1,ObjectKvpRoot, "mix_1:gain1" ,&parent->PExch[PARAM_INDEX_mix__1_gain1], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__1_gain1);
parent->PExch[PARAM_INDEX_mix__1_gain2].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__1_gain2,ObjectKvpRoot, "mix_1:gain2" ,&parent->PExch[PARAM_INDEX_mix__1_gain2], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__1_gain2);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_bus__in,
const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
int32buffer  & outlet_out,
int param_gain1,
int param_gain2  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   int32_t accum = ___SMMUL(inlet_in1[buffer_index],param_gain1);
   accum = ___SMMLA(inlet_in2[buffer_index],param_gain2,accum);
;   outlet_out[buffer_index]=  __SSAT(inlet_bus__in[buffer_index] + (accum<<1),28);

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
  public: void dsp (const int32_t  inlet_in,
int32_t  & outlet_out,
int param_amp  ){
outlet_out= ___SMMUL(param_amp,inlet_in)<<1;

}

}
;class instancemix__3 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancemix__3_gain1;
KeyValuePair KVP_instancemix__3_gain2;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_mix__3_gain1].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__3_gain1,ObjectKvpRoot, "mix_3:gain1" ,&parent->PExch[PARAM_INDEX_mix__3_gain1], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__3_gain1);
parent->PExch[PARAM_INDEX_mix__3_gain2].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__3_gain2,ObjectKvpRoot, "mix_3:gain2" ,&parent->PExch[PARAM_INDEX_mix__3_gain2], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__3_gain2);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_bus__in,
const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
int32buffer  & outlet_out,
int param_gain1,
int param_gain2  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   int32_t accum = ___SMMUL(inlet_in1[buffer_index],param_gain1);
   accum = ___SMMLA(inlet_in2[buffer_index],param_gain2,accum);
;   outlet_out[buffer_index]=  __SSAT(inlet_bus__in[buffer_index] + (accum<<1),28);

}
}

}
;class instancehp_space_1 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancehp_space_1_pitch;
KeyValuePair KVP_instancehp_space_1_reso;
biquad_state bs;biquad_coefficients bc;

  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_hp_space_1_pitch].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instancehp_space_1_pitch,ObjectKvpRoot, "hp 1:pitch" ,&parent->PExch[PARAM_INDEX_hp_space_1_pitch], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instancehp_space_1_pitch);
parent->PExch[PARAM_INDEX_hp_space_1_reso].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancehp_space_1_reso,ObjectKvpRoot, "hp 1:reso" ,&parent->PExch[PARAM_INDEX_hp_space_1_reso], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancehp_space_1_reso);
biquad_clearstate(&bs);

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
const int32_t  inlet_pitch,
const int32_t  inlet_reso,
int32buffer  & outlet_out,
int param_pitch,
int param_reso  ){
      int32_t freq;
      MTOF(param_pitch + inlet_pitch,freq);
      biquad_hp_coefs(&bc,freq,INT_MAX - (__USAT(inlet_reso + param_reso,27)<<4));
      biquad_dsp(&bs,&bc,inlet_in,outlet_out);

}

}
;class instancevca__1 {
  public: // v1
  rootc *parent;
   int32_t prev;
    int32_t step;

  public: void Init(rootc * _parent) {
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
;class instancemuls__1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32buffer  & outlet_out  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index]= __SSAT(inlet_in[buffer_index],25)<<3;
}
}

}
;class instancevu__3 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_v) {
parent = _parent;
disp_v = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32_t & disp_v  ){
disp_v=inlet_in[0];

}

}
;class instancemix__2 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancemix__2_gain1;
KeyValuePair KVP_instancemix__2_gain2;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_mix__2_gain1].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__2_gain1,ObjectKvpRoot, "mix_2:gain1" ,&parent->PExch[PARAM_INDEX_mix__2_gain1], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__2_gain1);
parent->PExch[PARAM_INDEX_mix__2_gain2].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__2_gain2,ObjectKvpRoot, "mix_2:gain2" ,&parent->PExch[PARAM_INDEX_mix__2_gain2], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__2_gain2);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_bus__in,
const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
int32buffer  & outlet_out,
int param_gain1,
int param_gain2  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   int32_t accum = ___SMMUL(inlet_in1[buffer_index],param_gain1);
   accum = ___SMMLA(inlet_in2[buffer_index],param_gain2,accum);
;   outlet_out[buffer_index]=  __SSAT(inlet_bus__in[buffer_index] + (accum<<1),28);

}
}

}
;class instancesoft__1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32buffer  & outlet_out  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
int32_t ts = __SSAT(inlet_in[buffer_index],28);
int32_t tsq31 = ts<<3;
int32_t tsq31p3 = ___SMMUL(tsq31,___SMMUL(tsq31,tsq31));
outlet_out[buffer_index] = ts + (ts>>1) - (tsq31p3);

}
}

}
;class instancevu__1 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_v) {
parent = _parent;
disp_v = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32_t & disp_v  ){
disp_v=inlet_in[0];

}

}
;class instancemix__4 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancemix__4_gain1;
KeyValuePair KVP_instancemix__4_gain2;
  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_mix__4_gain1].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__4_gain1,ObjectKvpRoot, "mix_4:gain1" ,&parent->PExch[PARAM_INDEX_mix__4_gain1], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__4_gain1);
parent->PExch[PARAM_INDEX_mix__4_gain2].pfunction = pfun_unsigned_clamp_fullrange;
  SetKVP_IPVP(&KVP_instancemix__4_gain2,ObjectKvpRoot, "mix_4:gain2" ,&parent->PExch[PARAM_INDEX_mix__4_gain2], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancemix__4_gain2);
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_bus__in,
const int32buffer  inlet_in1,
const int32buffer  inlet_in2,
int32buffer  & outlet_out,
int param_gain1,
int param_gain2  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
   int32_t accum = ___SMMUL(inlet_in1[buffer_index],param_gain1);
   accum = ___SMMLA(inlet_in2[buffer_index],param_gain2,accum);
;   outlet_out[buffer_index]=  __SSAT(inlet_bus__in[buffer_index] + (accum<<1),28);

}
}

}
;class instanceout__1 {
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
;class instancehp_space_2 {
  public: // v1
  rootc *parent;
KeyValuePair KVP_instancehp_space_2_pitch;
KeyValuePair KVP_instancehp_space_2_reso;
biquad_state bs;biquad_coefficients bc;

  public: void Init(rootc * _parent) {
parent = _parent;
parent->PExch[PARAM_INDEX_hp_space_2_pitch].pfunction = pfun_signed_clamp;
  SetKVP_IPVP(&KVP_instancehp_space_2_pitch,ObjectKvpRoot, "hp 2:pitch" ,&parent->PExch[PARAM_INDEX_hp_space_2_pitch], -1<<27, 1<<27);
  KVP_RegisterObject(&KVP_instancehp_space_2_pitch);
parent->PExch[PARAM_INDEX_hp_space_2_reso].pfunction = pfun_unsigned_clamp;
  SetKVP_IPVP(&KVP_instancehp_space_2_reso,ObjectKvpRoot, "hp 2:reso" ,&parent->PExch[PARAM_INDEX_hp_space_2_reso], 0, 1<<27);
  KVP_RegisterObject(&KVP_instancehp_space_2_reso);
biquad_clearstate(&bs);

}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
const int32_t  inlet_pitch,
const int32_t  inlet_reso,
int32buffer  & outlet_out,
int param_pitch,
int param_reso  ){
      int32_t freq;
      MTOF(param_pitch + inlet_pitch,freq);
      biquad_hp_coefs(&bc,freq,INT_MAX - (__USAT(inlet_reso + param_reso,27)<<4));
      biquad_dsp(&bs,&bc,inlet_in,outlet_out);

}

}
;class instancevca__2 {
  public: // v1
  rootc *parent;
   int32_t prev;
    int32_t step;

  public: void Init(rootc * _parent) {
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
;class instancemuls__2 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent) {
parent = _parent;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32buffer  & outlet_out  ){
int buffer_index;
for(buffer_index=0;buffer_index<BUFSIZE;buffer_index++) {
outlet_out[buffer_index]= __SSAT(inlet_in[buffer_index],25)<<3;
}
}

}
;class instancevu__4 {
  public: // v1
  rootc *parent;
  public: void Init(rootc * _parent,
int32_t & disp_v) {
parent = _parent;
disp_v = 0;
}
  public: void Dispose() {
}
  public: void dsp (const int32buffer  inlet_in,
int32_t & disp_v  ){
disp_v=inlet_in[0];

}

}
;/* controller instances */
/* object instances */
     instancein__1 instancein__1_i;
     instancefollower__1 instancefollower__1_i;
     instancemuls__3 instancemuls__3_i;
     instanceamp instanceamp_i;
     instancevu__2 instancevu__2_i;
     instancesine__1 instancesine__1_i;
     instancecomp__1 instancecomp__1_i;
     instancechart__1 instancechart__1_i;
     instancesquare__1 instancesquare__1_i;
     instance_star_c__2 instance_star_c__2_i;
     instancefdn4__1 instancefdn4__1_i;
     instancemix__1 instancemix__1_i;
     instance_star_c__1 instance_star_c__1_i;
     instancemix__3 instancemix__3_i;
     instancehp_space_1 instancehp_space_1_i;
     instancevca__1 instancevca__1_i;
     instancemuls__1 instancemuls__1_i;
     instancevu__3 instancevu__3_i;
     instancemix__2 instancemix__2_i;
     instancesoft__1 instancesoft__1_i;
     instancevu__1 instancevu__1_i;
     instancemix__4 instancemix__4_i;
     instanceout__1 instanceout__1_i;
     instancehp_space_2 instancehp_space_2_i;
     instancevca__2 instancevca__2_i;
     instancemuls__2 instancemuls__2_i;
     instancevu__4 instancevu__4_i;
/* net latches */
    int32buffer  net3Latch;
    int32buffer  net7Latch;
    int32buffer  net18Latch;
    bool  net19Latch;
static const uint8_t polyIndex = 0;
   static int32_t * GetInitParams(void) {
      static const int32_t p[19]= {
      -134217728,
      102760448,
      6291456,
      132120576,
      42991616,
      124780544,
      44040192,
      48234496,
      18874368,
      44040192,
      48234496,
      27262976,
      93323264,
      133169152,
      41943040,
      133169152,
      41943040,
      27262976,
      93323264
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
   for (j=0; j<19; j++) {
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
     displayVector[2] = 8;
   instancein__1_i.Init(this, displayVector[3] );
   instancefollower__1_i.Init(this );
   instancemuls__3_i.Init(this );
   instanceamp_i.Init(this );
   instancevu__2_i.Init(this, displayVector[4] );
   instancesine__1_i.Init(this );
   instancecomp__1_i.Init(this );
   instancechart__1_i.Init(this, displayVector[5] );
   instancesquare__1_i.Init(this );
   instance_star_c__2_i.Init(this );
   instancefdn4__1_i.Init(this );
   instancemix__1_i.Init(this );
   instance_star_c__1_i.Init(this );
   instancemix__3_i.Init(this );
   instancehp_space_1_i.Init(this );
   instancevca__1_i.Init(this );
   instancemuls__1_i.Init(this );
   instancevu__3_i.Init(this, displayVector[6] );
   instancemix__2_i.Init(this );
   instancesoft__1_i.Init(this );
   instancevu__1_i.Init(this, displayVector[7] );
   instancemix__4_i.Init(this );
   instanceout__1_i.Init(this, displayVector[8], displayVector[9] );
   instancehp_space_2_i.Init(this );
   instancevca__2_i.Init(this );
   instancemuls__2_i.Init(this );
   instancevu__4_i.Init(this, displayVector[10] );
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
   instancevu__4_i.Dispose();
   instancemuls__2_i.Dispose();
   instancevca__2_i.Dispose();
   instancehp_space_2_i.Dispose();
   instanceout__1_i.Dispose();
   instancemix__4_i.Dispose();
   instancevu__1_i.Dispose();
   instancesoft__1_i.Dispose();
   instancemix__2_i.Dispose();
   instancevu__3_i.Dispose();
   instancemuls__1_i.Dispose();
   instancevca__1_i.Dispose();
   instancehp_space_1_i.Dispose();
   instancemix__3_i.Dispose();
   instance_star_c__1_i.Dispose();
   instancemix__1_i.Dispose();
   instancefdn4__1_i.Dispose();
   instance_star_c__2_i.Dispose();
   instancesquare__1_i.Dispose();
   instancechart__1_i.Dispose();
   instancecomp__1_i.Dispose();
   instancesine__1_i.Dispose();
   instancevu__2_i.Dispose();
   instanceamp_i.Dispose();
   instancemuls__3_i.Dispose();
   instancefollower__1_i.Dispose();
   instancein__1_i.Dispose();
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
    int32_t  net4;
    int32_t  net5;
    int32buffer  net6;
    int32buffer  net7;
    int32buffer  net8;
    int32buffer  net9;
    int32buffer  net10;
    int32buffer  net11;
    int32buffer  net12;
    int32buffer  net13;
    int32buffer  net14;
    int32buffer  net15;
    int32buffer  net16;
    int32buffer  net17;
    int32buffer  net18;
    bool  net19;
    int32_t  net20;
    int32_t  net21;
    int32_t  net22;
//--------- </nets> ----------//
//--------- <zero> ----------//
  int32_t UNCONNECTED_OUTPUT;
  static const int32_t UNCONNECTED_INPUT = 0;
  static const int32buffer zerobuffer = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int32buffer UNCONNECTED_OUTPUT_BUFFER;
//--------- </zero> ----------//
//--------- <controller calls> ----------//
//--------- <object calls> ----------//
  instancein__1_i.dsp(net0, displayVector[3]);
  instancefollower__1_i.dsp(net0, net20);
  instancemuls__3_i.dsp(net20, net21);
  instanceamp_i.dsp(net21, net19Latch);
  instancevu__2_i.dsp(net3Latch, displayVector[4]);
  instancesine__1_i.dsp(0 , net4, PExch[PARAM_INDEX_sine__1_pitch].finalvalue);
  instancecomp__1_i.dsp((net0[0]), net22, PExch[PARAM_INDEX_comp__1_tresh].finalvalue, PExch[PARAM_INDEX_comp__1_ratio].finalvalue);
  instancechart__1_i.dsp(net22, displayVector[5]);
  instancesquare__1_i.dsp(0 , 0, net19, PExch[PARAM_INDEX_square__1_pitch].finalvalue);
  instance_star_c__2_i.dsp(net0, net6, PExch[PARAM_INDEX__star_c__2_amp].finalvalue);
  instancefdn4__1_i.dsp(net6, zerobuffer, zerobuffer, zerobuffer, net1, net11, net12, net13, PExch[PARAM_INDEX_fdn4__1_g].finalvalue);
  instancemix__1_i.dsp(zerobuffer, net1, net11, net3, PExch[PARAM_INDEX_mix__1_gain1].finalvalue, PExch[PARAM_INDEX_mix__1_gain2].finalvalue);
  instance_star_c__1_i.dsp(net4, net5, PExch[PARAM_INDEX__star_c__1_amp].finalvalue);
  instancemix__3_i.dsp(zerobuffer, net12, net13, net16, PExch[PARAM_INDEX_mix__3_gain1].finalvalue, PExch[PARAM_INDEX_mix__3_gain2].finalvalue);
  instancehp_space_1_i.dsp(net3, net5, 0 , net8, PExch[PARAM_INDEX_hp_space_1_pitch].finalvalue, PExch[PARAM_INDEX_hp_space_1_reso].finalvalue);
  instancevca__1_i.dsp(net22, net8, net10);
  instancemuls__1_i.dsp(net10, net9);
  instancevu__3_i.dsp(net9, displayVector[6]);
  instancemix__2_i.dsp(zerobuffer, net9, net7Latch, net2, PExch[PARAM_INDEX_mix__2_gain1].finalvalue, PExch[PARAM_INDEX_mix__2_gain2].finalvalue);
  instancesoft__1_i.dsp(net0, net7);
  instancevu__1_i.dsp(net8, displayVector[7]);
  instancemix__4_i.dsp(zerobuffer, net18Latch, net7, net17, PExch[PARAM_INDEX_mix__4_gain1].finalvalue, PExch[PARAM_INDEX_mix__4_gain2].finalvalue);
  instanceout__1_i.dsp(net2, net17, displayVector[8], displayVector[9]);
  instancehp_space_2_i.dsp(net16, net5, 0 , net14, PExch[PARAM_INDEX_hp_space_2_pitch].finalvalue, PExch[PARAM_INDEX_hp_space_2_reso].finalvalue);
  instancevca__2_i.dsp(net22, net14, net15);
  instancemuls__2_i.dsp(net15, net18);
  instancevu__4_i.dsp(net14, displayVector[10]);
//--------- </object calls> ----------//
//--------- <net latch copy> ----------//
   for(i=0;i<BUFSIZE;i++)
      net3Latch[i]=net3[i];
   for(i=0;i<BUFSIZE;i++)
      net7Latch[i]=net7[i];
   for(i=0;i<BUFSIZE;i++)
      net18Latch[i]=net18[i];
net19Latch = net19;
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
  patchMeta.numPEx = 19;
  patchMeta.patchID = 939856505;
  extern char _sdram_dyn_start;
  extern char _sdram_dyn_end;
  sdram_init(&_sdram_dyn_start,&_sdram_dyn_end);
  root.Init();
  patchMeta.fptr_applyPreset = ApplyPreset;
  patchMeta.fptr_patch_dispose = PatchDispose;
  patchMeta.fptr_MidiInHandler = PatchMidiInHandler;
  patchMeta.fptr_dsp_process = PatchProcess;
}
