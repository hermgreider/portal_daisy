#pragma once

class Controller;

class ExternalInput {
  public:
    virtual void Init(Controller *controller) = 0;

    // Callback for main loop - On 1 msec
    virtual void Update() = 0;
};