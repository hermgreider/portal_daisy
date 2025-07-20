// === main.cpp ===
#include "daisy_pod.h"
#include "drone_synth.h"
#include "pod_controls.h"

using namespace daisy;

DaisyPod pod;
DroneSynth synth;
PodControls controls;
int chord_index = 0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
    for (size_t i = 0; i < size; i++) {
        float sig = synth.Process();
        out[0][i] = sig;
        out[1][i] = sig;
    }
}

int main(void) {
    pod.Init();
    float samplerate = pod.AudioSampleRate();

    controls.Init(&pod);
    synth.Init(samplerate);

    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while (1) {
        controls.Process();

        float knob_val = controls.GetKnob1();
        synth.SetCutoffBase(knob_val * 7900.0f + 100.0f);

        int inc = controls.GetEncoderInc();
        synth.AdjustCutoff(inc * 100.0f);

        if (pod.button2.RisingEdge()) {
            chord_index = (chord_index + 1) % 5;
            synth.SetChord(chord_index);
        }

        controls.UpdateLED(synth.GetCutoffBase());
    }
}