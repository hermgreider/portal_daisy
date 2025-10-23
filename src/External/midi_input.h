/** 
 * MIDI Handler - simple MIDI input to take noteon/noteoff from keyboard
 */

#pragma once

#include "daisy_seed.h"
#include "external_input.h"

using namespace daisy;

// MIDI
#define MIDI_CHANNEL_ONE 0
#define MIDI_CHANNEL_ALL 17
#define MIDI_VELOCITY_MAX 127

class MidiInput : ExternalInput
{
  public:
    void Init(Controller *controller);

    // Callback for main loop
    void Update();

  private:
    void HandleMidiMessage(MidiEvent m);

    Controller *controller_;
};
