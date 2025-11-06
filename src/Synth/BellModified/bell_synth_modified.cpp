#include "bell_synth_modified.h"

extern DaisySeed hw;

const float BellSynthModified::ratios_[BellSynthModified::kNumTypes][BellSynthModified::kNumPartials] = {
    {1.0f, 2.7f, 3.9f, 5.4f},
    {1.0f, 2.0f, 2.5f, 7.0f},
    {0.5f, 1.0f, 1.5f, 2.1f},
    {1.0f, 1.41f, 1.8f, 2.9f},
    {1.0f, 2.3f, 3.1f, 4.0f},
    {1.0f, 1.6f, 2.7f, 3.3f}};

const float BellSynthModified::amps_[BellSynthModified::kNumTypes][BellSynthModified::kNumPartials] = {
    {0.8f, 0.5f, 0.3f, 0.2f},
    {0.7f, 0.5f, 0.4f, 0.3f},
    {0.6f, 0.4f, 0.3f, 0.2f},
    {0.9f, 0.7f, 0.5f, 0.4f},
    {0.9f, 0.6f, 0.3f, 0.2f},
    {1.0f, 0.7f, 0.5f, 0.3f}};

void BellSynthModified::Init(float sample_rate)
{
    for (int i = 0; i < kNumPartials; i++)
    {
        partials_[i].Init(sample_rate);
        partials_[i].SetWaveform(Oscillator::WAVE_SIN);

        envelopes_[i].Init(sample_rate);
        envelopes_[i].SetTime(ADSR_SEG_ATTACK, 0.001f);
        envelopes_[i].SetTime(ADSR_SEG_DECAY, 1.2f);
        envelopes_[i].SetTime(ADSR_SEG_IDLE, 1.0f);
        envelopes_[i].SetTime(ADSR_SEG_RELEASE, 1.2f);
        // envelopes_[i].SetMax(1.0f);
        //  envelopes_[i].SetMin(0.0f);
    }

    reverb_.Init(sample_rate);
    reverb_.SetFeedback(0.85f);
    reverb_.SetLpFreq(12000.0f);

    hw.PrintLine("BellSynthModified: Init complete");
}

void BellSynthModified::Select1()
{
    Trigger();
}

void BellSynthModified::Select2()
{
    SetType();
    Trigger();
}

void BellSynthModified::Select3()
{
}

// Pitch Control
void BellSynthModified::Mod1(float value)
{
    base_freq_ = 220.0f * powf(2.0f, 2.0f * value);
}

// Reverb Mix
void BellSynthModified::Mod2(float value)
{
    SetReverbMix(value);
}

// Reverb Feedback
void BellSynthModified::Mod3(float value)
{
    SetReverbFeedback(value);
}

void BellSynthModified::Update()
{
}

void BellSynthModified::Trigger()
{
    gate_ = true;
    for (int i = 0; i < kNumPartials; i++)
    {
        partials_[i].SetFreq(base_freq_ * ratios_[bell_type_][i]);
        envelopes_[i].Retrigger(gate_);
    }
}

bool BellSynthModified::IsActive()
{
    // return true if any envelope is still running
    for (int i = 0; i < kNumPartials; i++)
    {
        if (envelopes_[i].IsRunning())
            return true;
    }
    return false;
}

void BellSynthModified::SetType()
{
    bell_type_ = (bell_type_ + 1) % kNumTypes;
}

void BellSynthModified::SetReverbFeedback(float fb)
{
    hw.PrintLine("BellSynthModified: SetFeedback(%f)", fb);
    reverb_.SetFeedback(fb);
}

void BellSynthModified::SetReverbMix(float mix)
{
    reverb_mix_ = fclamp(mix, 0.0f, 1.0f);
}

void BellSynthModified::Process(float &outL, float &outR)
{
    float sig = 0.0f, wetL, wetR;

    for (int i = 0; i < kNumPartials; i++)
    {
        sig += partials_[i].Process() * envelopes_[i].Process(gate_) * amps_[bell_type_][i];
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

void BellSynthModified::NoteOn(NoteOnEvent m)
{
    uint8_t note = foldNoteToRange(m.note, 40, 92);

    hw.PrintLine("BellSynthModified NoteOn: %d, fixed: %d, vel: %d", m.note, note, m.velocity);

    base_freq_ = 440.f * powf(2.f, (note - 69) / 12.f);
    Trigger();
}

void BellSynthModified::NoteOff(NoteOffEvent m)
{
    hw.PrintLine("Note off");
    gate_ = false;
}