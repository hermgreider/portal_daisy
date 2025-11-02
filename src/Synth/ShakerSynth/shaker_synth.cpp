// === drone_synth.cpp ===
#include "shaker_synth.h"

extern DaisySeed hw;

void ShakerSynth::Init(float sr) 
{
    noise.Init();   
    env.Init(sr);
    filter.Init(sr);

    // Envelope: quick attack, short decay
    env.SetTime(ADENV_SEG_ATTACK, 0.002f);  // 2 ms
    env.SetTime(ADENV_SEG_DECAY, 0.06f);    // 60 ms
    env.SetMax(1.0f);
    env.SetMin(0.0f);
    env.SetCurve(0.5f);                     // Slight exponential

    // Bandpass filter ~6kHz, moderate Q
    filter.SetFreq(6000.0f);
    filter.SetRes(1.2f);

    reverb.Init(sr);
    reverb.SetFeedback(0.8f);
    reverb.SetLpFreq(12000.0f);    
}

void ShakerSynth::Update() {}    
void ShakerSynth::Select1() {}
void ShakerSynth::Select2() {}
void ShakerSynth::Select3() {}

void ShakerSynth::Process(float &outL, float &outR)
{
    float sig, env_out, filt_out, wetL, wetR;
    // Envelope output
    env_out = env.Process();

    // Generate white noise
    sig = noise.Process() * env_out;

    // Bandpass filter for shaker tone
    filter.Process(sig);
    filt_out = filter.Band();

    reverb.Process(filt_out * 0.5f, filt_out * 0.5f, &wetL, &wetR);

    outL = filt_out * 0.5f * (1.0f - reverb_mix_) + wetL * reverb_mix_;
    outR = filt_out * 0.5f * (1.0f - reverb_mix_) + wetR * reverb_mix_;
}

void ShakerSynth::NoteOn(NoteOnEvent m)
{
    // 20 is max velocity
    float env_val = m.velocity / 20.0f + 0.2;
    if (env_val > 1.0f) {
        env_val = 1.0f;
    }

    env.SetMax(env_val);
    env.Trigger();
}

void ShakerSynth::NoteOff(NoteOffEvent m)
{
}

