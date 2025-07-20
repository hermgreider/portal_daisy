#include "bell_synth.h"

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
    reverb_.SetFeedback(0.85f);
    reverb_.SetLpFreq(12000.0f);
}

void BellSynth::Trigger()
{
    for (int i = 0; i < kNumPartials; i++)
    {
        partials_[i].SetFreq(base_freq_ * ratios_[bell_type_][i]);
        envelopes_[i].Trigger();
    }
}

void BellSynth::SetType(int type)
{
    bell_type_ = type % kNumTypes;
}

void BellSynth::SetBaseFreq(float freq)
{
    base_freq_ = freq;
}

void BellSynth::SetReverbFeedback(float fb)
{
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
