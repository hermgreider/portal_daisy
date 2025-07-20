#include "daisy_seed.h"
#include "daisysp.h"
#include <cmath>

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
Oscillator osc;

uint8_t midi_note = 50;

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t size)
{
    for (size_t i = 0; i < size; i += 2)
    {
        float sig = osc.Process();
        out[i] = out[i + 1] = sig;
    }
}

int main(void)
{
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);
    float sample_rate = hw.AudioSampleRate();

    // Oscillator
    osc.Init(sample_rate);
    osc.SetWaveform(Oscillator::WAVE_SIN);
    osc.SetAmp(0.5f);
    osc.SetFreq(mtof(midi_note));

    hw.StartAudio(AudioCallback);

    while (1) {}
}
