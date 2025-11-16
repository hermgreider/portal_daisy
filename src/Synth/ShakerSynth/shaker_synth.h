#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

#include "Synth/Util/reverbsc.h"
#include "Synth/synth.h"

using namespace daisy;
using namespace daisysp;

class ShakerSynth : public Synth
{
public:
  ShakerSynth() {}
  void Init(float sample_rate);
  void Update();

  /* Trigger something - typically Button press or Encoder click */
  void Select1();
  void Select2();
  void Select3();

  void Mod2(float val);

  void Process(float &outL, float &outR);

  void NoteOn(NoteOnEvent m);
  void NoteOff(NoteOffEvent m);

private:
  WhiteNoise noise;
  AdEnv env;
  Svf filter;
  ReverbSc reverb;

  float reverb_mix_ = 0.3f;
};
