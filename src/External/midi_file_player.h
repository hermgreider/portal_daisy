/** 
 * MIDI File Player - read MIDI file and play the notes
 */

#pragma once

#include <string.h>
#include <vector>
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
    bool LoadMidi(std::string fname);
    bool LoadSequence(std::vector<MyMidiEvent> events);
    void HandleMidiMessage(MidiEvent m);

    Controller *controller_;
};
