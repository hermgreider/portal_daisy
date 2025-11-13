#include "External/external_input.h"
#include "Config/config.h"
#include "controller.h"

extern DaisySeed hw;
extern Config config;

void Controller::Init() 
{
    float sample_rate = hw.AudioSampleRate();

    config.synth->Init(sample_rate);
}

void Controller::Update() 
{
    for (ExternalInput *input : inputs_) {
        input->Update();
    }

    config.synth->Update();
}

void Controller::Process(float &outL, float &outR)
{
    config.synth->Process(outL, outR);
}

void Controller::NoteOn(NoteOnEvent m)
{
    config.synth->NoteOn(m);
}

void Controller::NoteOff(NoteOffEvent m)
{
    config.synth->NoteOff(m);
}

void Controller::DebugNote(uint8_t val, uint8_t repeat) 
{
    for (uint8_t i = 0; i<repeat; i++) {
        NoteOnEvent m = { 0, val, 127 };
        config.synth->NoteOn(m);
        System::Delay(200);
        NoteOffEvent o = { 0, val, 0 };
        config.synth->NoteOff(o);
        System::Delay(500);
    }
}

void Controller::Mod1(float val)
{
    config.synth->Mod1(val);
}

void Controller::Mod2(float val)
{
    config.synth->Mod2(val);
}

void Controller::Mod3(float val)
{
    config.synth->Mod2(val);
}

void Controller::Select(uint8_t val)
{
}

void Controller::Select1()
{
    config.synth->Select1();
    hw.PrintLine("Select1 pressed");
}

void Controller::Select2()
{
    config.synth->Select2();
    hw.PrintLine("Select2 pressed");
}

/* Encoder on Pod - switch to another synth */
void Controller::Select3()
{
}

void Controller::Incr()
{
}
