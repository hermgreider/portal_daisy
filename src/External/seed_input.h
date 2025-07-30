#pragma once

/** 
 * Input for the plain Daisy Seed without any other hardware - perhaps unused at some point
 */

#include "daisy_seed.h"
#include "Init/controller.h"
#include "External/board_input.h"

using namespace daisy;

class SeedInput : public BoardInput {
  public:
    void Init(Controller *controller);
    void Update();
    float AudioSampleRate();
    void StartAudio(daisy::AudioHandle::AudioCallback audioCallback);
    DaisySeed GetSeed() { return seed_; }

  private:
    DaisySeed seed_;
    Controller *controller_;
};
