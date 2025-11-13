#pragma once

#include <string>
#include <vector>
#include "daisy_seed.h"
#include "Synth/synth.h"
#include "External/midi_event.h"

using namespace daisy;

class Config {
public:
  Synth *synth;
  std::string midi_fname;

  float us_per_qn;

  // integer - 0 = no change, 1 = 1 octave up, 2 = 2 octaves up, -1 = one octave down
  int8_t octave_adjust;

  uint8_t range_min;
  uint8_t range_max;

  // BellSynth only
  uint8_t bell_type;

  // ShakerSynth Only
  float shaker_attack;
  float shaker_decay;
  float shaker_freq;
  float shaker_filter_type;

  // Sequence to use if no file
  std::vector<MyMidiEvent> fallback_sequence;
};

