#pragma once

/** 
 * Factory for the board level input
 */

#include "Config/config.h"
#include "External/board_input.h"

class BoardFactory {
  public:
    BoardInput *CreateBoard(Config *config);
};