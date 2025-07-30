#pragma once

#include <vector>

#include "Synth/synth.h"
#include "Synth/Bell/bell_synth.h"
#include "Synth/DroneSynth/drone_synth.h"

enum class BoardType { POD, SIMPLEFIX, SEED };

struct Config {
public:
  BoardType boardType = BoardType::POD; 
  std::vector<Synth *> synths = { new BellSynth(), new DroneSynth() };
};
