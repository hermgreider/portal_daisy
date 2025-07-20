#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class BellSynth {
  public:
    static constexpr int kNumPartials = 4;
    static constexpr int kNumTypes = 6;

    void Init(float sample_rate);
    void Trigger();
    void SetType(int type);
    void SetBaseFreq(float freq);
    void SetReverbFeedback(float fb);
    void SetReverbMix(float mix);
    void Process(float &outL, float &outR);

    int GetType() const { return bell_type_; }
    float GetReverbMix() const { return reverb_mix_; }

  private:
    Oscillator partials_[kNumPartials];
    AdEnv envelopes_[kNumPartials];
    ReverbSc reverb_;

    int bell_type_ = 0;
    float base_freq_ = 880.0f; // A5
    float reverb_mix_ = 0.3f;

    static const float ratios_[kNumTypes][kNumPartials];
    static const float amps_[kNumTypes][kNumPartials];
};
