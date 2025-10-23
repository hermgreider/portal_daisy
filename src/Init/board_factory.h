#pragma once

/** 
 * Factory for the board level input
 */

 // TODO: Probably eliminate this class

#include "Config/config.h"
#include "External/seed_input.h"

class BoardFactory {
  public:
    SeedInput *CreateBoard(Config *config);
};