#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

#include "Synth/Util/moogladder.h"
#include "Synth/synth.h"

using namespace daisy;
using namespace daisysp;
using namespace infrasonic;

class MoogySynth : public Synth
{
public:
  MoogySynth() {}
  void Init(float sample_rate);
  void Update();

  /* Trigger something - typically Button press or Encoder click */
  void Select1();
  void Select2();
  void Select3();

  void Process(float &outL, float &outR);

  void NoteOn(NoteOnEvent m);
  void NoteOff(NoteOffEvent m);

  void SetCutoffBase(float val);
  void AdjustCutoff(float delta);
  float GetCutoffBase() const;

private:
  static const int kNumVoices = 5;

  class Voice
  {
  public:
    void NoteOn(int midinote);
    void NoteOff(int midinote);

    uint8_t note;
    bool active = 0;
    Oscillator osc1, osc2, subOsc;
    MoogLadder filter;
    Adsr amp_env;
    Adsr filter_env;
    float freq;
    float drift1, drift2, driftSub;
    float filter_amount;
    float follow;
  };
  Voice voices[kNumVoices];
  uint8_t current_voice = 0;

  Oscillator lfo;
  float cutoff_base;
  float lfo_cutoff;
  float samplerate;

  float randWalk(float &val, float amt, float range);
  void InitVoices();
  void InitVoice(Voice &v);
};
