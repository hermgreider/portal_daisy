#include "daisy_seed.h"
#include "daisysp.h"
#include <cmath>

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

#define NUM_VOICES 4

struct Voice
{
    Oscillator osc1, osc2, subOsc;
    Adsr       amp_env;
    Adsr       filt_env;
    MoogLadder flt;
    float      base_freq;
    float      gate;
    float      pan;
    float      drift1, drift2, driftSub;
};

Voice voices[NUM_VOICES];

float sample_rate;
float base_cutoff = 1000.0f;
float env_amt = 900.0f;

// A short note sequence: low–high–mid–high, then chords
struct NoteEvent
{
    float freqs[NUM_VOICES];
    float duration_ms;
};

NoteEvent sequence[] = {
    // single notes
    {{196.00f, 0, 0, 0}, 400},   // G3
    {{392.00f, 0, 0, 0}, 400},   // G4
    {{261.63f, 0, 0, 0}, 400},   // C4
    {{523.25f, 0, 0, 0}, 400},   // C5
    // chords
    {{261.63f, 329.63f, 392.00f, 523.25f}, 800}, // C major
    {{220.00f, 277.18f, 349.23f, 440.00f}, 800}, // A minor
    {{233.08f, 293.66f, 392.00f, 466.16f}, 1000}, // Bb major
};
const int num_notes = sizeof(sequence) / sizeof(NoteEvent);
int seq_index = 0;
uint32_t note_start_time = 0;
bool note_on = false;

// ---------- Audio callback ----------
static void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        float left = 0.0f, right = 0.0f;

        for(int v = 0; v < NUM_VOICES; v++)
        {
            Voice &voice = voices[v];
            if(voice.base_freq <= 0.0f)
                continue;

            float amp = voice.amp_env.Process(voice.gate);
            float filt_mod = voice.filt_env.Process();

            float cutoff = base_cutoff + (env_amt * filt_mod);
            if(cutoff > 5000.0f)
                cutoff = 5000.0f;
            voice.flt.SetFreq(cutoff);

            // mild analog drift
            voice.drift_phase += 0.00003f + 0.00002f * v;
            float drift = 0.0008f * sinf(voice.drift_phase);
            voice.osc1.SetFreq(voice.base_freq * (1.0f - 0.003f + drift));
            voice.osc2.SetFreq(voice.base_freq * (1.0f + 0.003f - drift));

            float sig = (voice.osc1.Process() + 0.8f * voice.osc2.Process()) * 0.5f;
            sig = voice.flt.Process(sig) * amp * 0.7f;

            left  += sig * (1.0f - voice.pan);
            right += sig * voice.pan;
        }

        out[0][i] = left;
        out[1][i] = right;
    }
}

// ---------- Helper to trigger notes ----------
void TriggerNoteSet(NoteEvent &note)
{
    for(int v = 0; v < NUM_VOICES; v++)
    {
        Voice &voice = voices[v];
        float f = note.freqs[v];
        if(f > 0.0f)
        {
            voice.base_freq = f;
            voice.gate = 1.0f;
            voice.filt_env.Trigger();
        }
        else
        {
            voice.gate = 0.0f;
        }
    }
    note_on = true;
    note_start_time = System::GetNow();
}

void ReleaseNotes()
{
    for(int v = 0; v < NUM_VOICES; v++)
        voices[v].gate = 0.0f;
    note_on = false;
}

void InitVoice(Voice &v, float freq) {
    v.freq = freq;
    v.drift1 = v.drift2 = v.driftSub = 0.0f;

    v.osc1.Init(samplerate);
    v.osc2.Init(samplerate);
    v.subOsc.Init(samplerate);

    v.osc1.SetWaveform(Oscillator::WAVE_SAW);
    v.osc2.SetWaveform(Oscillator::WAVE_SAW);
    v.subOsc.SetWaveform(Oscillator::WAVE_SAW);

    v.filter.Init(samplerate);
    v.filter.SetRes(0.5f);

    v.env.Init(samplerate);
    v.env.SetTime(ADSR_SEG_ATTACK, 2.0f);
    v.env.SetTime(ADSR_SEG_DECAY, 0.5f);
    v.env.SetSustainLevel(0.8f);
    v.env.SetTime(ADSR_SEG_RELEASE, 3.0f);
}


// ---------- Main ----------
int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48);
    sample_rate = hw.AudioSampleRate();

    for(int v = 0; v < NUM_VOICES; v++)
    {
        InitVoice(voices[v], )
        Voice &voice = voices[v];
        voice.osc1.Init(sample_rate);
        voice.osc2.Init(sample_rate);
        voice.osc1.SetWaveform(Oscillator::WAVE_SAW);
        voice.osc2.SetWaveform(Oscillator::WAVE_SAW);

        voice.flt.Init(sample_rate);
        voice.flt.SetRes(0.65f); // darker, more nasal tone

        voice.amp_env.Init(sample_rate);
        voice.amp_env.SetTime(ADSR_SEG_ATTACK, 0.01f);
        voice.amp_env.SetTime(ADSR_SEG_DECAY, 0.25f);
        voice.amp_env.SetSustainLevel(0.5f);
        voice.amp_env.SetTime(ADSR_SEG_RELEASE, 0.3f);

        voice.filt_env.Init(sample_rate);
        voice.filt_env.SetTime(ADENV_SEG_ATTACK, 0.01f);
        voice.filt_env.SetTime(ADENV_SEG_DECAY, 0.25f);
        voice.filt_env.SetMin(0.0f);
        voice.filt_env.SetMax(1.0f);

        voice.base_freq = 0.0f;
        voice.gate = 0.0f;
        voice.pan = 0.25f + 0.15f * v;
        voice.drift_phase = v * 1.1f;
    }

    hw.StartAudio(AudioCallback);
    System::Delay(1000);

    seq_index = 0;
    TriggerNoteSet(sequence[seq_index]);

    while(1)
    {
        uint32_t now = System::GetNow();
        NoteEvent &n = sequence[seq_index];

        if(note_on && now - note_start_time > n.duration_ms * 0.6f)
        {
            ReleaseNotes();
        }
        if(now - note_start_time > n.duration_ms)
        {
            seq_index = (seq_index + 1) % num_notes;
            TriggerNoteSet(sequence[seq_index]);
        }

        System::Delay(1);
    }
}
