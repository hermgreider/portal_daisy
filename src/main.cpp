#include "daisy_seed.h"
#include "daisysp.h"

#include "External/midi_input.h"
#include "External/midi_file_player.h"
#include "Init/controller.h"

using namespace daisy;

Config config;
Controller controller;
// MidiInput midiInput;
MidiFilePlayer midiFilePlayer;

DaisySeed hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    float left, right;

    for (size_t i = 0; i < size; i++)
    {
        left = right = 0;
        controller.Process(left, right);

        out[0][i] = left;
        out[1][i] = right;
    }
}

int main(void)
{
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);
    hw.StartLog();

    System::Delay(5000);

    hw.PrintLine("Startup");

    controller.Init(&config);

    hw.StartAudio(AudioCallback);
    System::Delay(500);
    hw.PrintLine("Portal: Before MIDI");

    controller.DebugNote(80, 3);
    System::Delay(500);

    // Initialize Midi -- TODO: where should this go? Controller?
    // midiInput.Init(&controller);
    midiFilePlayer.Init(&controller);

    controller.DebugNote(69, 3);
    System::Delay(500);

    hw.PrintLine("Portal: Startup Complete");

    while (1)
    {
        controller.Update();
        // midiInput.Update(); // TODO: Move this
        midiFilePlayer.Update();

        // hw.PrintLine("Loop");
        System::Delay(1);
    }
}
