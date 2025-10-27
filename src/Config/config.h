#pragma once

#include <vector>

#include "Synth/synth.h"
#include "Synth/Bell/bell_synth.h"
#include "Synth/DroneSynth/drone_synth.h"

// enum class BoardType { POD, SIMPLEFIX, SEED };

struct Config {
public:
  BellSynth bellSynth;
  DroneSynth droneSynth;
  // BoardType boardType = BoardType::SEED; 
  // std::vector<Synth *> synths = { &droneSynth, &bellSynth };
  std::vector<Synth *> synths = { &droneSynth};
};
