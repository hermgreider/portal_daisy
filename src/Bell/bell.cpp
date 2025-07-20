/**
 * Bell Sound program for Daisy Pod with:
 * 1. Six different bell types - Button 2 cycles through each
 * 2. Button 1 triggers the current bell
 * 3. Knob1 controls pitch (A4 to A6)
 * 4. Knob2 controls reverb feedback
 * 5. Encoder controls reverb wet/dry mix
*/

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod pod;

constexpr int NUM_PARTIALS = 4;
constexpr int NUM_TYPES = 6;

Oscillator partials[NUM_PARTIALS];
AdEnv envelopes[NUM_PARTIALS];
ReverbSc reverb;

int bell_type = 0;
float base_freq = 880.0f; // A5 by default
float reverb_mix = 0.3f;

// Bell definitions
float bell_ratios[NUM_TYPES][NUM_PARTIALS] = {
    {1.0f, 2.7f, 3.9f, 5.4f}, // Classic bell
    {1.0f, 2.0f, 2.5f, 7.0f}, // Wind chime
    {0.5f, 1.0f, 1.5f, 2.1f}, // Temple bell
    {1.0f, 1.41f, 1.8f, 2.9f}, // Gamelan
    {1.0f, 2.3f, 3.1f, 4.0f},  // Cowbell
    {1.0f, 1.6f, 2.7f, 3.3f}   // Ship bell
};

float bell_amps[NUM_TYPES][NUM_PARTIALS] = {
    {0.8f, 0.5f, 0.3f, 0.2f},
    {0.7f, 0.5f, 0.4f, 0.3f},
    {0.6f, 0.4f, 0.3f, 0.2f},
    {0.9f, 0.7f, 0.5f, 0.4f},
    {0.9f, 0.6f, 0.3f, 0.2f},
    {1.0f, 0.7f, 0.5f, 0.3f}
};

void TriggerBell()
{
    for (int i = 0; i < NUM_PARTIALS; i++)
    {
        partials[i].SetFreq(base_freq * bell_ratios[bell_type][i]);
        envelopes[i].Trigger();
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    float sig, wetL, wetR;

    for (size_t i = 0; i < size; i++)
    {
        sig = 0.0f;

        for (int p = 0; p < NUM_PARTIALS; p++)
        {
            sig += partials[p].Process() * envelopes[p].Process() * bell_amps[bell_type][p];
        }

        reverb.Process(sig, sig, &wetL, &wetR);
        out[0][i] = sig * (1.0f - reverb_mix) + wetL * reverb_mix;
        out[1][i] = sig * (1.0f - reverb_mix) + wetR * reverb_mix;
    }
}

int main(void)
{
    pod.Init();
    pod.SetAudioBlockSize(4);
    float sample_rate = pod.AudioSampleRate();

    for (int i = 0; i < NUM_PARTIALS; i++)
    {
        partials[i].Init(sample_rate);
        partials[i].SetWaveform(Oscillator::WAVE_SIN);

        envelopes[i].Init(sample_rate);
        envelopes[i].SetTime(ADENV_SEG_ATTACK, 0.001f);
        envelopes[i].SetTime(ADENV_SEG_DECAY, 1.2f);
        envelopes[i].SetMax(1.0f);
        envelopes[i].SetMin(0.0f);
    }

    reverb.Init(sample_rate);
    reverb.SetFeedback(0.85f);
    reverb.SetLpFreq(12000.0f);

    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    float previous_reverb_mix = reverb_mix;  // To track if reverb_mix has changed significantly

    while (1)
    {
        pod.ProcessAllControls();

        // Button 1: trigger bell
        if (pod.button1.RisingEdge())
            TriggerBell();

        // Button 2: switch bell type and trigger
        if (pod.button2.RisingEdge())
        {
            bell_type = (bell_type + 1) % NUM_TYPES;
            TriggerBell();
        }

        // Encoder click changes reverb mix
        reverb_mix += pod.encoder.Increment() * 0.01f;
        reverb_mix = fclamp(reverb_mix, 0.0f, 1.0f);

        // Knob1 controls pitch (A4 to A6)
        base_freq = 220.0f * powf(2.0f, 2.0f * pod.knob1.Process());

        // Knob2 controls reverb feedback
        float fb = 0.7f + 0.29f * pod.knob2.Process();
        reverb.SetFeedback(fb);

        // Only update LED 1 if reverb_mix has changed significantly
        if (fabs(reverb_mix - previous_reverb_mix) > 0.01f)
        {
            // LED 1 smoothly transitions from red to green to blue as reverb mix changes
            float mix_brightness = fclamp(reverb_mix, 0.0f, 1.0f);

            // Red to Green
            if (mix_brightness <= 0.5f)
            {
                pod.led1.Set(1.0f - 2.0f * mix_brightness, 2.0f * mix_brightness, 0.0f);
            }
            // Green to Blue
            else
            {
                pod.led1.Set(0.0f, 2.0f - 2.0f * mix_brightness, 2.0f * (mix_brightness - 0.5f));
            }

            previous_reverb_mix = reverb_mix;  // Update the previous reverb mix value
        }

        // LED 2 shows different color for each bell type
        switch (bell_type)
        {
            case 0: // Classic bell
                pod.led2.Set(1.0f, 0.0f, 0.0f); // Red
                break;
            case 1: // Wind chime
                pod.led2.Set(0.0f, 1.0f, 0.0f); // Green
                break;
            case 2: // Temple bell
                pod.led2.Set(0.0f, 0.0f, 1.0f); // Blue
                break;
            case 3: // Gamelan
                pod.led2.Set(1.0f, 1.0f, 0.0f); // Yellow
                break;
            case 4: // Cowbell
                pod.led2.Set(0.0f, 1.0f, 1.0f); // Cyan
                break;
            case 5: // Ship bell
                pod.led2.Set(1.0f, 0.0f, 1.0f); // Magenta
                break;
        }

        pod.UpdateLeds();
        System::Delay(1);
    }
}
