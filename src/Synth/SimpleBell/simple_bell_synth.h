#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

#include "Synth/synth.h"

// Bell Synth without reverb or the multiple options of BellSynth 
// Mostly used for debug

using namespace daisy;
using namespace daisysp;

class SimpleBellSynth : public Synth {
  public:
    static constexpr int kNumPartials = 4;

    SimpleBellSynth() {}
    void Init(float sample_rate);
    void Update();
    
    /* Trigger something - typically Button press or Encoder click */
    void Select1();
    void Select2();
    void Select3();

    /* Modifiers - typically a knob */
    void Mod1(float value);
    void Mod2(float value);
    void Mod3(float value);
    
    void Process(float &outL, float &outR);

    void NoteOn(NoteOnEvent m);
    void NoteOff(NoteOffEvent m);

  private:
    Oscillator partials_[kNumPartials];
    AdEnv envelopes_[kNumPartials];

    float base_freq_ = 880.0f; // A5

    static const float ratios_[kNumPartials];
    static const float amps_[kNumPartials];

    void Trigger();
    void SetBaseFreq(float freq);
};
