#pragma once

#include "daisy_seed.h"

using namespace daisy;

class Synth {
  public:
    virtual void Init(float sample_rate) = 0;

    // Callback for main loop - On 1 msec
    virtual void Update() {};

    // From Audiocallback to produce a note
    virtual void Process(float &outL, float &outR) {};

    /* Trigger something - typically Button press or Encoder click */
    virtual void Select1() {};
    virtual void Select2() {};
    virtual void Select3() {};    

    /* Knob value change */
    virtual void Mod1(float val) {};
    virtual void Mod2(float val) {};
    virtual void Mod3(float val) {};

    virtual void NoteOn(NoteOnEvent m) {};
    virtual void NoteOff(NoteOffEvent m) {};
};