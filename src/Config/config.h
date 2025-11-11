#pragma once

#include <vector>

#include "Synth/synth.h"
#include "Synth/Bell/bell_synth.h"
#include "Synth/MoogySynth/moogy_synth.h"
#include "Synth/MoogySynth2/moogy_synth2.h"
#include "Synth/ShakerSynth/shaker_synth.h"

// enum class BoardType { POD, SIMPLEFIX, SEED };

struct Config {
public:
  const char* midi_fname = "PortalsAirways.mid"; // for Bells
  // const char* midi_fname = "Portals_Brass5.mid"; // for Moogy
  // const char* midi_fname = "PortalsShaker.mid"; // for Shaker
  // const char* midi_fname = "PortalsInfinity.mid"; // for low Bells
  // const char* midi_fname = "PortalsFireflies.mid";

  BellSynth bellSynth;
  // MoogySynth moogySynth;
  // MoogySynth2 moogySynth2;
  // ShakerSynth shakerSynth;

  // BoardType boardType = BoardType::SEED; 

  // std::vector<Synth *> synths = { &moogySynth };
  // std::vector<Synth *> synths = { &moogySynth2 };
  std::vector<Synth *> synths = { &bellSynth };
  // std::vector<Synth *> synths = { &shakerSynth };
};
