#include "External/external_input.h"
#include "controller.h"

extern BoardInput *board;

void Controller::Init(BoardInput *board, Config *config) 
{
    inputs_.push_back(board);
    float sample_rate = board->AudioSampleRate();

    synths_ = config->synths;
    for (Synth *synth : synths_) 
    {
        synth->Init(sample_rate);
    }
}

void Controller::Update() 
{
    for (ExternalInput *input : inputs_) {
        input->Update();
    }

    synths_[current_synth_]->Update();
}

void Controller::Process(float &outL, float &outR)
{
    if (synths_.size() == 0) return;

    synths_[current_synth_]->Process(outL, outR);
}

void Controller::NoteOn()
{
}

void Controller::NoteOff()
{
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
}

void Controller::Select1()
{
    synths_[current_synth_]->Select1();
    board->GetSeed().PrintLine("Select1 pressed");
}

void Controller::Select2()
{
    synths_[current_synth_]->Select2();
    board->GetSeed().PrintLine("Select2 pressed");
}

/* Encoder on Pod - switch to another synth */
void Controller::Select3()
{
    board->GetSeed().PrintLine("Next Synth");
    current_synth_ = ((current_synth_ + 1) % synths_.size());
}

void Controller::Incr()
{
}
