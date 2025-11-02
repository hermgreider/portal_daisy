#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
WhiteNoise noise;
AdEnv env;
Svf filter;

bool button_state = false;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    float sig, env_out, filt_out;
    for (size_t i = 0; i < size; i++)
    {
        // Envelope output
        env_out = env.Process();

        // Generate white noise
        sig = noise.Process() * env_out;

        // Bandpass filter for shaker tone
        filter.Process(sig);
        filt_out = filter.Band();

        // Output mono to both channels
        out[0][i] = filt_out * 0.5f;
        out[1][i] = filt_out * 0.5f;
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4);
    float samplerate = hw.AudioSampleRate();

    // Init modules
    noise.Init();
    env.Init(samplerate);
    filter.Init(samplerate);

    // Envelope: quick attack, short decay
    env.SetTime(ADENV_SEG_ATTACK, 0.002f);  // 2 ms
    // env.SetTime(ADENV_SEG_DECAY, 0.12f);    // 120 ms
    env.SetTime(ADENV_SEG_DECAY, 0.06f);    // 60 ms
    env.SetMax(1.0f);
    env.SetMin(0.0f);
    env.SetCurve(0.5f);                     // Slight exponential

    // Bandpass filter ~6kHz, moderate Q
    filter.SetFreq(6000.0f);
    filter.SetRes(1.2f);

    // Start audio
    hw.StartAudio(AudioCallback);

    // Button trigger loop
    while (1)
    {
        env.SetMax(1.0f);  // Quieter second hit
        env.Trigger();

        System::Delay(200);
        env.SetMax(0.5f);  // Quieter second hit
        env.Trigger();
        
        System::Delay(1000);
    }
}
