#include "daisy_seed.h"

#include "External/board_input.h"
#include "Init/board_factory.h"
#include "Init/controller.h"

#include "External/pod_input.h"

using namespace daisy;

Config config;
Controller controller;
BoardFactory boardSetup;
BoardInput *board;

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
    BoardInput *new_board = boardSetup.CreateBoard(&config);
    board = new_board;
    board->Init(&controller);

    controller.Init(board, &config);

    board->StartAudio(AudioCallback);
    board->GetSeed().PrintLine("Portal: Startup Complete");

    uint16_t count = 0;
    while (1)
    {
        controller.Update();
        System::Delay(1);
    }
}
