#pragma once

#include "daisy_seed.h"
#include <vector>

#include "Synth/synth.h"
#include "Config/config.h"
#include "External/external_input.h"

using namespace daisy;

class Controller {
  public:
    void Init(Config *config);

    // Called from main loop (~1KHz)
    void Update();

    // Called from AudioCallback at samplerate (44KHz) to create sound
    void Process(float &outL, float &outR);

    void NoteOn(NoteOnEvent m);
    void NoteOff(NoteOffEvent m);
    void DebugNote(uint8_t val, uint8_t repeat);
    void Mod1(float val);
    void Mod2(float val);
    void Mod3(float val);
    void Select(uint8_t val);
    void Select1();
    void Select2();
    void Select3();
    void Incr();

  private:
    uint8_t current_synth_ = 0;

    std::vector<ExternalInput*> inputs_;
    std::vector<Synth*> synths_;
    Synth *debugSynth;
};