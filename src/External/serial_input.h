/**
 * Serial Handler
 */

#pragma once

#include "daisy_seed.h"
#include "external_input.h"

using namespace daisy;

class SerialInput : ExternalInput
{
public:
  void Init(Controller *controller);

  // Callback for main loop
  void Update();

  Controller *controller_;
  float scale;
};
