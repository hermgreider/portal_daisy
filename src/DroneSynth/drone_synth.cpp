// === drone_synth.cpp ===
#include "drone_synth.h"
#include <cmath>

void DroneSynth::Init(float sr) {
    samplerate = sr;
    cutoff_base = 1000.0f;

    lfo.Init(sr);
    lfo.SetFreq(0.1f);
    lfo.SetAmp(1.0f);
    lfo.SetWaveform(Oscillator::WAVE_TRI);

    SetChord(0);  // Initialize to first chord
}

void DroneSynth::SetChord(int chord_index) {
    chord_index = chord_index % kNumChords;
    for (int i = 0; i < kNumVoices; ++i) {
        InitVoice(voices[i], chords[chord_index][i]);
    }
}

void DroneSynth::InitVoice(Voice &v, float freq) {
    v.freq = freq;
    v.drift1 = v.drift2 = v.driftSub = 0.0f;

    v.osc1.Init(samplerate);
    v.osc2.Init(samplerate);
    v.subOsc.Init(samplerate);

    v.osc1.SetWaveform(Oscillator::WAVE_SAW);
    v.osc2.SetWaveform(Oscillator::WAVE_SAW);
    v.subOsc.SetWaveform(Oscillator::WAVE_SAW);

    v.filter.Init(samplerate);
    v.filter.SetRes(0.5f);

    v.env.Init(samplerate);
    v.env.SetTime(ADSR_SEG_ATTACK, 2.0f);
    v.env.SetTime(ADSR_SEG_DECAY, 0.5f);
    v.env.SetSustainLevel(0.8f);
    v.env.SetTime(ADSR_SEG_RELEASE, 3.0f);
}

float DroneSynth::randWalk(float &val, float amt, float range) {
    val += (rand() / (float)RAND_MAX - 0.5f) * amt;
    val = fminf(fmaxf(val, -range), range);
    return val;
}

float DroneSynth::Process() {
    float mix = 0.0f;
    lfo_cutoff = cutoff_base + (lfo.Process() * 300.0f);

    for (int v = 0; v < kNumVoices; ++v) {
        Voice &voice = voices[v];

        float d1 = randWalk(voice.drift1, 0.00001f, 0.02f);
        float d2 = randWalk(voice.drift2, 0.00001f, 0.02f);
        float ds = randWalk(voice.driftSub, 0.00001f, 0.01f);

        voice.osc1.SetFreq(voice.freq * 0.995f + d1);
        voice.osc2.SetFreq(voice.freq * 1.005f + d2);
        voice.subOsc.SetFreq(voice.freq * 0.5f + ds);

        voice.osc1.SetAmp(0.3f);
        voice.osc2.SetAmp(0.3f);
        voice.subOsc.SetAmp(0.4f);

        float sig = voice.osc1.Process() + voice.osc2.Process() + voice.subOsc.Process();

        float cutoff = fminf(fmaxf(lfo_cutoff, 80.0f), 8000.0f);
        voice.filter.SetFreq(cutoff);
        float filtered = tanh(voice.filter.Process(sig) * 1.5f);

        float envOut = voice.env.Process(true);
        mix += filtered * envOut;
    }
    return mix * 0.2f;
}

void DroneSynth::SetCutoffBase(float val) {
    cutoff_base = fminf(fmaxf(val, 100.0f), 8000.0f);
}

void DroneSynth::AdjustCutoff(float delta) {
    SetCutoffBase(cutoff_base + delta);
}

float DroneSynth::GetCutoffBase() const {
    return cutoff_base;
}