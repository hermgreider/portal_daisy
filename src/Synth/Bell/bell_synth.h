#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

#include "Synth/synth.h"
#include "Synth/Util/reverbsc.h"

using namespace daisy;
using namespace daisysp;

// TODO: When included here, Reverb takes out printing functionality (and probably other 
// stuff) - would love to have it back

class BellSynth : public Synth {
  public:
    static constexpr int kNumPartials = 4;
    static constexpr int kNumTypes = 6;

    BellSynth() {}
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

    int GetType() const { return bell_type_; }
    float GetReverbMix() const { return reverb_mix_; }

    void NoteOn(NoteOnEvent m);
    void NoteOff(NoteOffEvent m);

  private:
    Oscillator partials_[kNumPartials];
    AdEnv envelopes_[kNumPartials];
    ReverbSc reverb_;

    int bell_type_ = 0;
    float base_freq_ = 880.0f; // A5
    float reverb_mix_ = 0.3f;

    static const float ratios_[kNumTypes][kNumPartials];
    static const float amps_[kNumTypes][kNumPartials];

    void Trigger();
    void SetType();
    void SetBaseFreq(float freq);
    void SetReverbFeedback(float fb);
    void SetReverbMix(float mix);
};
