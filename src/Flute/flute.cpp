#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;
Adsr        env;
Oscillator  sine_main, sine_detune, vibrato;
Svf         tone_filter, breath_filter;
WhiteNoise  noise;
ReverbSc    reverb;

float base_freq = 440.0f;
bool gate = false;
float reverb_mix = 0.25f;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    float sig, tone, breath = 0.0f, rev_l, rev_r;

    for (size_t i = 0; i < size; i++)
    {
        // Envelope processing (for the gate control)
        float env_val = env.Process(gate);

        // Vibrato modulation (subtle pitch modulation)
        float vib = vibrato.Process() * 0.01f;
        float freq = base_freq * powf(2.0f, vib);  // Apply vibrato to frequency
        sine_main.SetFreq(freq);  // Main oscillator frequency set
        sine_detune.SetFreq(freq * 1.003f);  // Slight detune for shimmer effect

        // Generate the primary flute tone using both sine waves for richness
        tone = 0.6f * sine_main.Process() + 0.4f * sine_detune.Process();
        tone *= env_val;  // Apply envelope to the tone for dynamic control

        // Filter the tone using a low-pass filter for warmth and natural sound
        tone_filter.Process(tone);  // Process the tone through the filter
        float filtered_tone = tone_filter.Low();  // Get the filtered tone output

        // Breath noise — only added during attack/release
        float breath_env = env_val * (1.0f - env_val);  // Envelope shaping for breath (more noise at attack)
        float noise_sample = noise.Process();  // Generate breath noise
        breath_filter.Process(noise_sample * 0.02f * breath_env);  // Filter the breath noise
        breath = breath_filter.Low();  // Get low-pass output for breath noise

        // Combine filtered tone and breath noise for the final sound
        sig = filtered_tone + breath;

        // Apply reverb to the combined signal
        reverb.Process(sig, sig, &rev_l, &rev_r);
        out[0][i] = (1.0f - reverb_mix) * sig + reverb_mix * rev_l;  // Left channel
        out[1][i] = (1.0f - reverb_mix) * sig + reverb_mix * rev_r;  // Right channel
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48);
    float sample_rate = hw.AudioSampleRate();

    // Oscillators
    sine_main.Init(sample_rate);
    sine_main.SetWaveform(Oscillator::WAVE_SIN);
    sine_detune.Init(sample_rate);
    sine_detune.SetWaveform(Oscillator::WAVE_SIN);
    vibrato.Init(sample_rate);
    vibrato.SetWaveform(Oscillator::WAVE_SIN);
    vibrato.SetFreq(5.0f);

    // Envelope
    env.Init(sample_rate);
    env.SetTime(ADSR_SEG_ATTACK, 0.04f);
    env.SetTime(ADSR_SEG_DECAY, 0.3f);
    env.SetSustainLevel(0.8f);
    env.SetTime(ADSR_SEG_RELEASE, 0.5f);

    // Filters
    tone_filter.Init(sample_rate);
    tone_filter.SetRes(0.1f);
    tone_filter.SetFreq(1800.0f);

    breath_filter.Init(sample_rate);
    breath_filter.SetRes(0.6f);
    breath_filter.SetFreq(1200.0f);

    // Noise
    noise.Init();

    // Reverb
    reverb.Init(sample_rate);
    reverb.SetFeedback(0.85f);
    reverb.SetLpFreq(12000.0f);

    hw.StartAdc();
    hw.StartAudio(AudioCallback);

    while (true)
    {
        hw.ProcessAnalogControls();
        hw.ProcessDigitalControls();
        hw.encoder.Debounce();

        if (hw.button1.RisingEdge())
        {
            gate = true;
        }

        if (hw.button1.FallingEdge())
        {
            gate = false;
        }

        // Pitch (Knob 1)
        float knob1 = hw.knob1.Process();
        base_freq = mtof(60 + knob1 * 24); // MIDI 60–84

        // Reverb feedback (Knob 2)
        float feedback = hw.knob2.Process();
        feedback = fclamp(feedback, 0.2f, 0.95f);
        reverb.SetFeedback(feedback);

        // Reverb mix (encoder)
        reverb_mix += 0.01f * hw.encoder.Increment();
        reverb_mix = fclamp(reverb_mix, 0.0f, 1.0f);

        // LED feedback
        hw.led1.Set(reverb_mix, 0.0f, 1.0f - reverb_mix);
        bool active = env.GetCurrentSegment() != ADSR_SEG_RELEASE;
        hw.led2.Set(active ? 0.1f : 0.0f, active ? 0.8f : 0.1f, 0.0f);
        hw.UpdateLeds();
    }
}
