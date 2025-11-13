// ------------------ MIDI Event ------------------
#pragma once

#include "daisy_seed.h"

struct MyMidiEvent
{
    uint32_t time_ms;
    uint8_t type; // 1=NoteOn, 0=NoteOff
    uint8_t note;
    uint8_t vel;
};
