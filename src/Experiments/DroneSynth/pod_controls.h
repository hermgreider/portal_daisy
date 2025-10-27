#pragma once
#include "daisy_pod.h"

using namespace daisy;

class PodControls {
  public:
    void Init(DaisyPod* p);
    void Process();

    float GetKnob1() const;
    int GetEncoderInc();
    bool Button2Pressed();

    void UpdateLED(float brightness);

  private:
    DaisyPod* pod;
    int last_encoder;
};
