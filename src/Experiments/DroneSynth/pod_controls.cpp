#include "pod_controls.h"

void PodControls::Init(DaisyPod* p) {
    pod = p;
    last_encoder = pod->encoder.Increment();
}

void PodControls::Process() {
    pod->ProcessAllControls();
}

float PodControls::GetKnob1() const {
    return pod->GetKnobValue(DaisyPod::KNOB_1);
}

int PodControls::GetEncoderInc() {
    int current = pod->encoder.Increment();
    int inc = current - last_encoder;
    last_encoder = current;
    return inc;
}

bool PodControls::Button2Pressed() {
    return pod->button2.RisingEdge();
}

void PodControls::UpdateLED(float brightness) {
    brightness = fminf(fmaxf(brightness / 10000.0f, 0.f), 1.f);
    pod->led1.Set(brightness, brightness, brightness);
    pod->UpdateLeds();
}
