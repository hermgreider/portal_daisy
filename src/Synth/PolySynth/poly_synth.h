// Synths/PolySynth/poly_synth.h
#pragma once
#include <array>
#include "daisy_seed.h"
#include "daisysp.h"
#include "Synth/synth.h"

using namespace daisy;
extern DaisySeed hw;

template <typename VoiceType, int MaxVoices = 8>
class PolySynth : public Synth
{
public:
  void Init(float sample_rate) override
  {
    hw.PrintLine("PolySynth Init: sample_rate = %f", sample_rate);

    for (auto &v : voices_)
      v.Init(sample_rate);
  }

  void NoteOn(NoteOnEvent m) override
  {
    // Find a free voice
    for (int i = 0; i < MaxVoices; i++)
    {
      if (!voice_active_[i])
      {
        voices_[i].NoteOn(m);
        note_numbers_[i] = m.note;
        voice_active_[i] = true;
        return;
      }
    }
    // Steal the first one if all busy
    voices_[0].NoteOn(m);
    note_numbers_[0] = m.note;
  }

  void NoteOff(NoteOffEvent m) override
  {
    for (int i = 0; i < MaxVoices; i++)
    {
      if (voice_active_[i] && note_numbers_[i] == m.note)
      {
        voices_[i].NoteOff(m);
        voice_active_[i] = false;
        return;
      }
    }
  }

  void Process(float &outL, float &outR) override
  {
    outL = outR = 0.0f;
    float l, r;

    for (int i = 0; i < MaxVoices; i++)
    {
      voices_[i].Process(l, r);
      outL += l;
      outR += r;
    }

    // Normalize to avoid clipping
    outL /= MaxVoices;
    outR /= MaxVoices;
  }

  bool IsActive() const override
  {
    for (bool active : voice_active_)
      if (active)
        return true;
    return false;
  }

private:
  std::array<VoiceType, MaxVoices> voices_;
  std::array<bool, MaxVoices> voice_active_{};    // all false initially
  std::array<uint8_t, MaxVoices> note_numbers_{}; // track which note each voice plays
};
