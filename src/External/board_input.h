#pragma once

#include "daisy_seed.h"
#include "External/external_input.h"

class Controller;

class BoardInput : public ExternalInput {
  public:
    // Return the sample rate
    virtual float AudioSampleRate() = 0;

    // Start Audio
    virtual void StartAudio(daisy::AudioHandle::AudioCallback audioCallback) = 0;

    // Get the Daisy Seed object
    virtual daisy::DaisySeed GetSeed() = 0;
  };