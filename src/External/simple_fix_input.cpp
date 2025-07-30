#include "simple_fix_input.h"

void SimpleFixInput::Init(Controller *controller)
{
    controller_ = controller;
    seed_.Init();
    seed_.SetAudioBlockSize(4);
}

void SimpleFixInput::StartAudio(AudioHandle::AudioCallback audioCallback)
{
    seed_.StartAudio(audioCallback);
}

void SimpleFixInput::Update()
{
}

float SimpleFixInput::AudioSampleRate() 
{
    return seed_.AudioSampleRate();
}

