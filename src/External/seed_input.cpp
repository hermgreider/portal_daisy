#include "seed_input.h"

void SeedInput::Init(Controller *controller)
{
    controller_ = controller;
    seed_.Init();
    seed_.SetAudioBlockSize(4);
}

void SeedInput::StartAudio(daisy::AudioHandle::AudioCallback audioCallback)
{
    seed_.StartAudio(audioCallback);
}

void SeedInput::Update()
{
}

float SeedInput::AudioSampleRate() 
{
    return seed_.AudioSampleRate();
}

