#pragma once

#include "daisy_seed.h"
#include "daisy_pod.h"
#include "Init/controller.h"
#include "External/board_input.h"

using namespace daisy;

class PodInput : public BoardInput {
  public:
    void Init(Controller *controller);
    void Update();
    float AudioSampleRate();
    void StartAudio(AudioHandle::AudioCallback audioCallback);
    DaisySeed GetSeed() { return pod_.seed; }

  private:
    DaisyPod pod_;
    Controller *controller_;

    float knob1_val_;
    float knob2_val_;
};
