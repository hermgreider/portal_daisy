// === drone_synth.h ===
#pragma once

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class DroneSynth {
  public:
    void Init(float samplerate);
    float Process();

    void SetCutoffBase(float val);
    void AdjustCutoff(float delta);
    float GetCutoffBase() const;

    void SetChord(int chord_index);

  private:
    static const int kNumVoices = 5;
    static const int kNumChords = 5;

    struct Voice {
        Oscillator osc1, osc2, subOsc;
        MoogLadder filter;
        Adsr env;
        float freq;
        float drift1, drift2, driftSub;
    };

    Voice voices[kNumVoices];
    float chords[kNumChords][kNumVoices] = {
        {130.81f, 196.00f, 523.25f, 622.25f, 783.99f},    // Cm
        {196.00f, 233.08f, 466.16f, 739.99f, 932.33f},    // Gm(add9)
        {207.65f, 246.94f, 392.00f, 622.25f, 830.61f},    // Abmaj7
        {220.00f, 174.61f, 329.63f, 440.00f, 659.25f},    // A half-dim (A C Eb G B)
        {196.00f, 261.63f, 349.23f, 440.00f, 698.46f}     // Gsus4 to G (G C F A D)
    };

    Oscillator lfo;
    float cutoff_base;
    float lfo_cutoff;
    float samplerate;

    float randWalk(float &val, float amt, float range);
    void InitVoice(Voice &v, float freq);
};