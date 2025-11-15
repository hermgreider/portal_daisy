// === moogy_synth.cpp ===
#include <cmath>
#include "Config/config.h"
#include "moogy_synth.h"

extern DaisySeed hw;
extern Config config;

const float cutoffMin = 2000.0f;
const float cutoffMax = 8000.0f;
const float resMin = 0.1f;
const float resMax = 0.4f;

void MoogySynth::Init(float sr) 
{
    current_res = resMin; // print only

    samplerate = sr;
    cutoff_base = cutoffMin;

    lfo.Init(sr);
    lfo.SetFreq(0.05f);
    lfo.SetAmp(1.0f);
    lfo.SetWaveform(Oscillator::WAVE_TRI);

    InitVoices();
}

void MoogySynth::Update() {}    
void MoogySynth::Select1() {}
void MoogySynth::Select2() {}
void MoogySynth::Select3() {}

void MoogySynth::InitVoices() 
{
    for (int i = 0; i < kNumVoices; ++i) 
    {
        InitVoice(voices[i]);
    }
}

void MoogySynth::Mod2(float val)
{
    // Val is between 0 and 1

    float curved = powf(val, 2.0f); // exponential

    // Set values between min and max - note that Resonance and cutoff go in opposite directions
    float res = resMin + (resMax - resMin) * (1.0f - curved);
    float cutoff = cutoffMin + (cutoffMax - cutoffMin) * (1.0f - curved);
    SetCutoffBase(cutoff);

    current_res = res; // print only
    hw.PrintLine("Moogy::Mod2, val %f, cutoff %f, res %f ", val, cutoff_base, res);

    for (int v = 0; v < kNumVoices; ++v)
    {
        Voice &voice = voices[v];
        voice.filter.SetRes(res);
    }
}

void MoogySynth::InitVoice(Voice &v)
{
    v.freq = 0;
    v.drift1 = v.drift2 = v.driftSub = 0.0f;

    v.osc1.Init(samplerate);
    v.osc2.Init(samplerate);
    v.subOsc.Init(samplerate);

    v.osc1.SetWaveform(Oscillator::WAVE_SAW);
    v.osc2.SetWaveform(Oscillator::WAVE_SQUARE);
    v.osc2.SetPw(0.3);
    v.subOsc.SetWaveform(Oscillator::WAVE_TRI);

    v.filter.Init(samplerate);
    v.filter.SetRes(0.1f);
    v.filter_amount = 1.0;

    v.amp_env.Init(samplerate);
    v.amp_env.SetTime(ADSR_SEG_ATTACK, 0.5f);
    v.amp_env.SetTime(ADSR_SEG_DECAY, 0.5f);
    v.amp_env.SetSustainLevel(0.8f);
    v.amp_env.SetTime(ADSR_SEG_RELEASE, 1.0f);

    v.filter_env.Init(samplerate);
    v.filter_env.SetTime(ADSR_SEG_ATTACK, 0.7f);
    v.filter_env.SetTime(ADSR_SEG_DECAY, 0.3f);
    v.filter_env.SetSustainLevel(0.5f);
    v.filter_env.SetTime(ADSR_SEG_RELEASE, 0.8f);
}

float MoogySynth::randWalk(float &val, float amt, float range) 
{
    val += (rand() / (float)RAND_MAX - 0.5f) * amt;
    val = fminf(fmaxf(val, -range), range);
    return val;
}

void MoogySynth::Process(float &outL, float &outR)
{
    float mix = 0.0f;
    lfo_cutoff = cutoff_base + (lfo.Process() * 300.0f);

    for (int v = 0; v < kNumVoices; ++v) 
    {
        Voice &voice = voices[v];

        float d1 = randWalk(voice.drift1, 0.00001f, 0.02f);
        float d2 = randWalk(voice.drift2, 0.00001f, 0.02f);
        float ds = randWalk(voice.driftSub, 0.00001f, 0.01f);

        voice.osc1.SetFreq(voice.freq * 0.995f + d1);
        voice.osc2.SetFreq(voice.freq * 1.005f + d2);
        voice.subOsc.SetFreq(voice.freq * 2.0f + ds);

        voice.osc1.SetAmp(0.8f);
        voice.osc2.SetAmp(0.6f);
        voice.subOsc.SetAmp(0.4f);

        float sig = voice.osc1.Process() + voice.osc2.Process() + voice.subOsc.Process();

        float env_f_out = voice.filter_env.Process(voice.active);

        voice.follow = 1.0f - (float(voice.note) / 84.0f);

    	voice.filter.SetFreq(cutoff_base * 
            (env_f_out * voice.filter_amount) * 
            (1.0f - voice.follow));
		float filter_out = voice.filter.Process(sig);

        float envOut = voice.amp_env.Process(voice.active);
        mix += filter_out * envOut;
    }
    outL = outR = mix * 0.4f;
}

void MoogySynth::SetCutoffBase(float val) 
{
    cutoff_base = fminf(fmaxf(val, 100.0f), cutoffMax);
}

void MoogySynth::AdjustCutoff(float delta) 
{
    SetCutoffBase(cutoff_base + delta);
}

float MoogySynth::GetCutoffBase() const 
{
    return cutoff_base;
}

void MoogySynth::NoteOn(NoteOnEvent m)
{
    voices[current_voice].NoteOn(m.note);
    current_voice = (current_voice + 1) % kNumVoices;
    
    // hw.PrintLine("Moogy2::NoteOn, cutoff %f, res %f ", cutoff_base, current_res);
}

void MoogySynth::NoteOff(NoteOffEvent m)
{
    for (int v = 0; v < kNumVoices; v++)
    {
        voices[v].NoteOff(m.note);
    }
}

static int changeOctave(int note, int octave_adjust)
{
    int transposed = note + 12 * config.octave_adjust;
    return transposed;
}

static int foldNoteToRange(int note, int minNote, int maxNote)
{
    // Shift up or down by 12s to land in the right octave
    int transposed = note;
    if (transposed < minNote)
        transposed += 12 * ((minNote - transposed + 11) / 12);
    else if (transposed > maxNote)
        transposed -= 12 * ((transposed - maxNote + 11) / 12);
    return transposed;
}

void MoogySynth::Voice::NoteOn(int midinote)
{
    note = changeOctave(midinote, config.octave_adjust);
    note = foldNoteToRange(midinote, config.range_min, config.range_max);

    hw.PrintLine("Moogy NoteOn: %d, fixed: %d", midinote, note);

    freq = 440.f * powf(2.f, (note - 69) / 12.f);
    active = true;
}

void MoogySynth::Voice::NoteOff(int midinote)
{
    note = changeOctave(midinote, config.octave_adjust);
    note = foldNoteToRange(midinote, config.range_min, config.range_max);
    if (note == midinote)
    {
        active = false;
    }
}
