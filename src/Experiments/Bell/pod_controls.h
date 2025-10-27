#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;

class PodControls {
  public:
    void Init(DaisyPod* pod);
    void Update();

    bool TriggerPressed();
    bool NextTypePressed();
    float GetPitchControl();
    float GetReverbFeedback();
    float UpdateReverbMix(float current_mix);
    void UpdateLEDs(int bell_type, float reverb_mix);

  private:
    DaisyPod* pod_;
    float prev_reverb_mix_ = -1.0f; // Force first update
};
