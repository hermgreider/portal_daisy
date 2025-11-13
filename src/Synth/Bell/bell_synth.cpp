#include "Config/config.h"
#include "bell_synth.h"

extern DaisySeed hw;
extern Config config;

const float BellSynth::ratios_[BellSynth::kNumTypes][BellSynth::kNumPartials] = {
    {1.0f, 2.7f, 3.9f, 5.4f},
    {1.0f, 2.0f, 2.5f, 7.0f},
    {0.5f, 1.0f, 1.5f, 2.1f},
    {1.0f, 1.41f, 1.8f, 2.9f},
    {1.0f, 2.3f, 3.1f, 4.0f},
    {1.0f, 1.6f, 2.7f, 3.3f}
};

const float BellSynth::amps_[BellSynth::kNumTypes][BellSynth::kNumPartials] = {
    {0.8f, 0.5f, 0.3f, 0.2f},
    {0.7f, 0.5f, 0.4f, 0.3f},
    {0.6f, 0.4f, 0.3f, 0.2f},
    {0.9f, 0.7f, 0.5f, 0.4f},
    {0.9f, 0.6f, 0.3f, 0.2f},
    {1.0f, 0.7f, 0.5f, 0.3f}
};


void BellSynth::Init(float sample_rate)
{
    bell_type_ = config.bell_type;

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

    reverb_.Init(sample_rate);
    reverb_.SetFeedback(REVERB_FEEDBACK);
    reverb_.SetLpFreq(REVERB_LOWPASS);

    hw.PrintLine("BellSynth: Init complete");
}

void BellSynth::Select1() 
{
    Trigger();
}

void BellSynth::Select2() 
{
    SetType();
    Trigger();
}

void BellSynth::Select3() 
{
    
}

// Pitch Control
void BellSynth::Mod1(float value)
{
    base_freq_ = 220.0f * powf(2.0f, 2.0f * value);
}

// Reverb Mix
void BellSynth::Mod2(float value)
{
    SetReverbMix(value);
}

// Reverb Feedback
void BellSynth::Mod3(float value)
{
    SetReverbFeedback(value);
}

void BellSynth::Update() 
{
}

void BellSynth::Trigger()
{
    for (int i = 0; i < kNumPartials; i++)
    {
        partials_[i].SetFreq(base_freq_ * ratios_[bell_type_][i]);
        envelopes_[i].Trigger();
    }
}

void BellSynth::SetType()
{
    bell_type_ = (bell_type_ + 1) % kNumTypes;
}

void BellSynth::SetReverbFeedback(float fb)
{
    hw.PrintLine("BellSynth: SetFeedback(%f)", fb);
    reverb_.SetFeedback(fb);
}

void BellSynth::SetReverbMix(float mix)
{
    reverb_mix_ = fclamp(mix, 0.0f, 1.0f);
}

void BellSynth::Process(float &outL, float &outR)
{
    float sig = 0.0f, wetL, wetR;

    for (int i = 0; i < kNumPartials; i++)
    {
        sig += partials_[i].Process() * envelopes_[i].Process() * amps_[bell_type_][i];
    }

    reverb_.Process(sig, sig, &wetL, &wetR);

    outL = sig * (1.0f - reverb_mix_) + wetL * reverb_mix_;
    outR = sig * (1.0f - reverb_mix_) + wetR * reverb_mix_;
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

static int changeOctave(int note, int octave_adjust)
{
    int transposed = note + 12 * config.octave_adjust;
    return transposed;
}

void BellSynth::NoteOn(NoteOnEvent m)
{
    uint8_t note = changeOctave(m.note, config.octave_adjust);
    note = foldNoteToRange(note, config.range_min, config.range_max);

    hw.PrintLine("BellSynth NoteOn: %d, fixed: %d, vel: %d", m.note, note, m.velocity);

    base_freq_ = 440.f * powf(2.f, (note - 69) / 12.f);
    Trigger();
}

void BellSynth::NoteOff(NoteOffEvent m)
{
}