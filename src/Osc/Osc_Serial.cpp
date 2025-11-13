#include "daisy_seed.h"
#include "daisysp.h"
#include <cmath>

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
Oscillator osc;

UartHandler uart;
uint8_t rx_byte;

uint8_t midi_note = 50;

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t size)
{
    for (size_t i = 0; i < size; i += 2)
    {
        float sig = osc.Process();
        out[i] = out[i + 1] = sig;
    }
}

void InitSerial()
{
    UartHandler::Config cfg;
    cfg.periph = UartHandler::Config::Peripheral::USART_1;
    cfg.mode = UartHandler::Config::Mode::RX;
    cfg.pin_config.rx = seed::D14; // RX pin
    cfg.baudrate = 9600;
    uart.Init(cfg);
}

void Handle_Serial()
{
    if (uart.BlockingReceive(&rx_byte, 1, 10) == UartHandler::Result::OK)
    {
        // Example: interpret incoming byte as MIDI note number
        midi_note = 30 + static_cast<int>((rx_byte / 255.0f) * (80 - 30));
        osc.SetFreq(mtof(midi_note));
        hw.PrintLine("Output %d", rx_byte);
    }
}

int main(void)
{
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);
    float sample_rate = hw.AudioSampleRate();

    hw.StartLog();
    hw.PrintLine("Startup");

    // Oscillator
    osc.Init(sample_rate);
    osc.SetWaveform(Oscillator::WAVE_SIN);
    osc.SetAmp(0.5f);
    osc.SetFreq(mtof(midi_note));

    hw.StartAudio(AudioCallback);

    InitSerial();

    while (1)
    {
        Handle_Serial();
        System::Delay(50);
    }
}