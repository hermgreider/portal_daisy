#include "External/external_input.h"
#include "controller.h"

extern DaisySeed hw;

void Controller::Init(Config *config)
{
    float sample_rate = hw.AudioSampleRate();

    synths_ = config->synths;
    for (auto *synth : synths_)
    {
        synth->Init(sample_rate);
    }
}

void Controller::Update()
{
    for (ExternalInput *input : inputs_)
    {
        input->Update();
    }

    synths_[current_synth_]->Update();
}

void Controller::Process(float &outL, float &outR)
{
    if (synths_.size() == 0)
        return;

    synths_[current_synth_]->Process(outL, outR);
}

void Controller::NoteOn(NoteOnEvent m)
{
    synths_[current_synth_]->NoteOn(m);
}

void Controller::NoteOff(NoteOffEvent m)
{
    synths_[current_synth_]->NoteOff(m);
}

void Controller::DebugNote(uint8_t val, uint8_t repeat)
{
    for (uint8_t i = 0; i < repeat; i++)
    {
        NoteOnEvent m = {0, val, 127};
        synths_[current_synth_]->NoteOn(m);
        System::Delay(200);
        NoteOffEvent o = {0, val, 0};
        synths_[current_synth_]->NoteOff(o);
        System::Delay(500);
    }
}

void Controller::Mod1(float val)
{
    synths_[current_synth_]->Mod1(val);
}

void Controller::Mod2(float val)
{
    synths_[current_synth_]->Mod2(val);
}

void Controller::Mod3(float val)
{
    synths_[current_synth_]->Mod2(val);
}

void Controller::Select(uint8_t val)
{
    current_synth_ = val;
    hw.PrintLine("Controller: Select Synth %d", current_synth_);
}

void Controller::Select1()
{
    synths_[current_synth_]->Select1();
    hw.PrintLine("Select1 pressed");
}

void Controller::Select2()
{
    synths_[current_synth_]->Select2();
    hw.PrintLine("Select2 pressed");
}

/* Encoder on Pod - switch to another synth */
void Controller::Select3()
{
    hw.PrintLine("Next Synth");
    current_synth_ = ((current_synth_ + 1) % synths_.size());
}

void Controller::Incr()
{
}
