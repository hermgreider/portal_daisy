#include "pod_controls.h"

void PodControls::Init(DaisyPod* pod)
{
    pod_ = pod;
    prev_reverb_mix_ = -1.0f;
}

void PodControls::Update()
{
    pod_->ProcessAllControls();
}

bool PodControls::TriggerPressed()
{
    return pod_->button1.RisingEdge();
}

bool PodControls::NextTypePressed()
{
    return pod_->button2.RisingEdge();
}

float PodControls::GetPitchControl()
{
    return 220.0f * powf(2.0f, 2.0f * pod_->knob1.Process());
}

float PodControls::GetReverbFeedback()
{
    return 0.7f + 0.29f * pod_->knob2.Process();
}

float PodControls::UpdateReverbMix(float current_mix)
{
    float mix = current_mix + pod_->encoder.Increment() * 0.01f;
    return daisysp::fclamp(mix, 0.0f, 1.0f);
}

void PodControls::UpdateLEDs(int bell_type, float reverb_mix)
{
    if (fabs(reverb_mix - prev_reverb_mix_) > 0.01f)
    {
        float mix = daisysp::fclamp(reverb_mix, 0.0f, 1.0f);
        if (mix <= 0.5f)
            pod_->led1.Set(1.0f - 2.0f * mix, 2.0f * mix, 0.0f);
        else
            pod_->led1.Set(0.0f, 2.0f - 2.0f * mix, 2.0f * (mix - 0.5f));

        prev_reverb_mix_ = mix;
    }

    switch (bell_type)
    {
        case 0: pod_->led2.Set(1.0f, 0.0f, 0.0f); break; // Red
        case 1: pod_->led2.Set(0.0f, 1.0f, 0.0f); break; // Green
        case 2: pod_->led2.Set(0.0f, 0.0f, 1.0f); break; // Blue
        case 3: pod_->led2.Set(1.0f, 1.0f, 0.0f); break; // Yellow
        case 4: pod_->led2.Set(0.0f, 1.0f, 1.0f); break; // Cyan
        case 5: pod_->led2.Set(1.0f, 0.0f, 1.0f); break; // Magenta
    }

    pod_->UpdateLeds();
}
