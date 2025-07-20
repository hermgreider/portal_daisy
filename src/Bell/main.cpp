#include "daisy_pod.h"
#include "bell_synth.h"
#include "pod_controls.h"

using namespace daisy;

DaisyPod pod;
BellSynth synth;
PodControls controls;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    float left, right;

    for (size_t i = 0; i < size; i++)
    {
        synth.Process(left, right);
        out[0][i] = left;
        out[1][i] = right;
    }
}

int main(void)
{
    pod.Init();
    pod.SetAudioBlockSize(4);
    float sample_rate = pod.AudioSampleRate();

    synth.Init(sample_rate);
    controls.Init(&pod);

    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while (1)
    {
        controls.Update();

        if (controls.TriggerPressed())
            synth.Trigger();

        if (controls.NextTypePressed())
        {
            synth.SetType((synth.GetType() + 1) % BellSynth::kNumTypes);
            synth.Trigger();
        }

        synth.SetBaseFreq(controls.GetPitchControl());
        synth.SetReverbFeedback(controls.GetReverbFeedback());

        float mix = controls.UpdateReverbMix(synth.GetReverbMix());
        synth.SetReverbMix(mix);

        controls.UpdateLEDs(synth.GetType(), mix);

        System::Delay(1);
    }
}
