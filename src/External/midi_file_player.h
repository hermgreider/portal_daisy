/** 
 * MIDI File Player - read MIDI file and play the notes
 */

#pragma once

#include "daisy_seed.h"
#include "external_input.h"

using namespace daisy;

class MidiFilePlayer : ExternalInput
{
  public:
    void Init(Controller *controller);

    // Callback for main loop
    void Update();

  private:
    bool LoadMidi(const char* fname);
    void HandleMidiMessage(MidiEvent m);

    Controller *controller_;
};
