#include "Config/config.h"
#include "shaker_synth.h"

extern DaisySeed hw;
extern Config config;

// static const float attack = 0.002f;
// static const float decay = 0.06f;
// static const float freq = 4000.f;
// static const float filter_type = 0; // band

// static const float attack = 0.015f;
// static const float decay = 0.06f;
// static const float freq = 6000.f;
// static const float filter_type = 1; // high

void ShakerSynth::Init(float sr)
{
    noise.Init();
    env.Init(sr);
    filter.Init(sr);

    // Envelope: quick attack, short decay
    env.SetTime(ADENV_SEG_ATTACK, config.shaker_attack);
    env.SetTime(ADENV_SEG_DECAY, config.shaker_decay);
    env.SetMax(1.0f);
    env.SetMin(0.0f);
    env.SetCurve(0.5f); // Slight exponential

    filter.SetFreq(config.shaker_freq);
    filter.SetRes(0.0f); // 1.2f

    reverb.Init(sr);
    reverb.SetFeedback(0.8f);
    reverb.SetLpFreq(12000.0f);
}

void ShakerSynth::Update() {}
void ShakerSynth::Select1() {}
void ShakerSynth::Select2() {}
void ShakerSynth::Select3() {}

void ShakerSynth::Mod2(float val)
{
    // Val is between 0 and 1

    // float curved = powf(val, 2.0f); // exponential

    // Set values between min and max - note that Resonance and cutoff go in opposite directions
    // float res = resMin + (resMax - resMin) * (1.0f - curved);
    // float cutoff = cutoffMin + (cutoffMax - cutoffMin) * (1.0f - curved);
    // SetCutoffBase(cutoff);

    // filter.SetRes(res)
    float attack = fmaxf(val / 8, config.shaker_attack);
    float decay = fmaxf(val / 2, config.shaker_decay);
    env.SetTime(ADENV_SEG_ATTACK, attack);
    env.SetTime(ADENV_SEG_DECAY, decay);
    // hw.PrintLine("Shaker::Mod2, val %f attack: %f, decay: %f", val, attack, decay);
}

void ShakerSynth::Process(float &outL, float &outR)
{
    float sig, env_out, filt_out, wetL, wetR;
    // Envelope output
    env_out = env.Process();

    // Generate white noise
    sig = noise.Process() * env_out;

    // Bandpass filter = 0
    filter.Process(sig);
    if (config.shaker_filter_type == 0)
        filt_out = filter.Band();
    else
    {
        filt_out = filter.High();
    }

    reverb.Process(filt_out * 0.5f, filt_out * 0.5f, &wetL, &wetR);

    outL = filt_out * 0.5f * (1.0f - reverb_mix_) + wetL * reverb_mix_;
    outR = filt_out * 0.5f * (1.0f - reverb_mix_) + wetR * reverb_mix_;
}

void ShakerSynth::NoteOn(NoteOnEvent m)
{
    // 20 is max velocity
    float env_val = m.velocity / 20.0f + 0.2;
    if (env_val > 1.0f)
    {
        env_val = 1.0f;
    }

    env.SetMax(env_val);
    env.Trigger();
}

void ShakerSynth::NoteOff(NoteOffEvent m)
{
}
