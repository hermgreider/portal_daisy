#pragma once

#include <vector>

#include "Synth/synth.h"
#include "Synth/Bell/bell_synth.h"
#include "Synth/BellModified/bell_synth_modified.h"
#include "Synth/DroneSynth/drone_synth.h"
#include "Synth/MoogySynth/moogy_synth.h"
#include "Synth/ShakerSynth/shaker_synth.h"
#include "Synth/PolySynth/poly_synth.h"

// enum class BoardType { POD, SIMPLEFIX, SEED };

struct Config
{
public:
  // const char *midi_fname = "PortalsAirways.mid"; // for Bells
  const char *midi_fname = "Portals_Brass5.mid"; // for Moogy
  //  const char* midi_fname = "PortalsShaker.mid"; // for Shaker
  //  const char* midi_fname = "PortalsInfinity.mid"; // for Bells

  // BellSynth bellSynth;
  // BellSynthModified bellSynth;
  PolySynth<MoogySynth> polySynth;

  // DroneSynth droneSynth;
  // MoogySynth moogySynth;
  // ShakerSynth shakerSynth;

  // BoardType boardType = BoardType::SEED;

  // std::vector<Synth *> synths = { &droneSynth, &bellSynth };
  // std::vector<Synth *> synths = {&moogySynth};
  // std::vector<Synth *> synths = {&bellSynth};
  std::vector<Synth *> synths = {&polySynth};
  //   std::vector<Synth *> synths = { &shakerSynth };
};
