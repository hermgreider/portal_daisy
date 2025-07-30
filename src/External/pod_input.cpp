#include "pod_input.h"

void PodInput::Init(Controller *controller)
{
    controller_ = controller;
    pod_.Init();
    pod_.SetAudioBlockSize(4);

    pod_.seed.StartLog();
    pod_.seed.PrintLine("Portal: Daisy Pod Startup");

    // prev_reverb_mix_ = -1.0f;
}

void PodInput::StartAudio(AudioHandle::AudioCallback audioCallback)
{
    pod_.StartAdc();
    pod_.StartAudio(audioCallback);
}

void PodInput::Update()
{
    pod_.ProcessAllControls();

    if (pod_.button1.RisingEdge()) 
    {
        controller_->Select1();
    }
    if (pod_.button2.RisingEdge()) 
    {
        controller_->Select2();
    }

    float cur_knob1 = pod_.knob1.Process();
    if (cur_knob1 != knob1_val_) {
        knob1_val_ = cur_knob1;
        controller_->Mod1(knob1_val_);
    } 

    float cur_knob2 = pod_.knob2.Process();
    if (cur_knob2 != knob2_val_) {
        knob2_val_ = cur_knob2;
        controller_->Mod2(knob2_val_);
    } 
}

float PodInput::AudioSampleRate() 
{
    return pod_.AudioSampleRate();
}

// float PodInput::GetReverbFeedback()
// {
//     return 0.7f + 0.29f * pod_->knob2.Process();
// }

// float PodInput::UpdateReverbMix(float current_mix)
// {
//     float mix = current_mix + pod_->encoder.Increment() * 0.01f;
//     return daisysp::fclamp(mix, 0.0f, 1.0f);
// }

// void PodInput::UpdateLEDs(int bell_type, float reverb_mix)
// {
//     if (fabs(reverb_mix - prev_reverb_mix_) > 0.01f)
//     {
//         float mix = daisysp::fclamp(reverb_mix, 0.0f, 1.0f);
//         if (mix <= 0.5f)
//             pod_->led1.Set(1.0f - 2.0f * mix, 2.0f * mix, 0.0f);
//         else
//             pod_->led1.Set(0.0f, 2.0f - 2.0f * mix, 2.0f * (mix - 0.5f));

//         prev_reverb_mix_ = mix;
//     }

//     switch (bell_type)
//     {
//         case 0: pod_->led2.Set(1.0f, 0.0f, 0.0f); break; // Red
//         case 1: pod_->led2.Set(0.0f, 1.0f, 0.0f); break; // Green
//         case 2: pod_->led2.Set(0.0f, 0.0f, 1.0f); break; // Blue
//         case 3: pod_->led2.Set(1.0f, 1.0f, 0.0f); break; // Yellow
//         case 4: pod_->led2.Set(0.0f, 1.0f, 1.0f); break; // Cyan
//         case 5: pod_->led2.Set(1.0f, 0.0f, 1.0f); break; // Magenta
//     }

//     pod_->UpdateLeds();
// }
