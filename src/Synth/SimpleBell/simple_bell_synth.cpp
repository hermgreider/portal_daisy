#include "simple_bell_synth.h"

extern DaisySeed hw;

const float SimpleBellSynth::ratios_[SimpleBellSynth::kNumPartials] = {1.0f, 2.7f, 3.9f, 5.4f};
const float SimpleBellSynth::amps_[SimpleBellSynth::kNumPartials] = {0.8f, 0.5f, 0.3f, 0.2f};


void SimpleBellSynth::Init(float sample_rate)
{
    for (int i = 0; i < kNumPartials; i++)
    {
        partials_[i].Init(sample_rate);
        partials_[i].SetWaveform(Oscillator::WAVE_SIN);

        envelopes_[i].Init(sample_rate);
        envelopes_[i].SetTime(ADENV_SEG_ATTACK, 0.001f);
        envelopes_[i].SetTime(ADENV_SEG_DECAY, 1.2f);
        envelopes_[i].SetMax(1.0f);
        envelopes_[i].SetMin(0.0f);
    }

    hw.PrintLine("SimpleBellSynth: Init complete");
}

void SimpleBellSynth::Select1() 
{
    Trigger();
}

void SimpleBellSynth::Select2() 
{
    Trigger();
}

void SimpleBellSynth::Select3() 
{
    
}

// Pitch Control
void SimpleBellSynth::Mod1(float value)
{
    base_freq_ = 220.0f * powf(2.0f, 2.0f * value);
}

void SimpleBellSynth::Mod2(float value)
{
}

void SimpleBellSynth::Mod3(float value)
{
}

void SimpleBellSynth::Update() 
{
}

void SimpleBellSynth::Trigger()
{
    for (int i = 0; i < kNumPartials; i++)
    {
        partials_[i].SetFreq(base_freq_ * ratios_[i]);
        envelopes_[i].Trigger();
    }
}

void SimpleBellSynth::Process(float &outL, float &outR)
{
    float sig = 0.0f;

    for (int i = 0; i < kNumPartials; i++)
    {
        sig += partials_[i].Process() * envelopes_[i].Process() * amps_[i];
    }

    outL = outR = sig;
}

static int foldNoteToRange(int note, int minNote, int maxNote)
{
    // Shift up or down by 12s to land in the right octave
    int transposed = note;
    if (transposed < minNote)
        transposed += 12 * ((minNote - transposed + 11) / 12);
    else if (transposed > maxNote)
        transposed -= 12 * ((transposed - maxNote + 11) / 12);
    return transposed;
}

void SimpleBellSynth::NoteOn(NoteOnEvent m)
{
    uint8_t note = foldNoteToRange(m.note, 40, 92);

    hw.PrintLine("SimpleBellSynth NoteOn: %d, fixed: %d, vel: %d", m.note, note, m.velocity);

    base_freq_ = 440.f * powf(2.f, (note - 69) / 12.f);
    Trigger();
}

void SimpleBellSynth::NoteOff(NoteOffEvent m)
{
}