#include "daisy_seed.h"
#include "serial_input.h"
#include "Init/controller.h"

using namespace daisy;

// #define MIDI_USB

extern DaisySeed hw;

UartHandler uart;
uint8_t rx_byte;

void SerialInput::Init(Controller *controller)
{
    controller_ = controller;

    UartHandler::Config cfg;
    cfg.periph = UartHandler::Config::Peripheral::USART_1;
    cfg.mode = UartHandler::Config::Mode::RX;
    cfg.pin_config.rx = seed::D14; // RX pin
    cfg.baudrate = 9600;
    uart.Init(cfg);

    scale = 0.5f;
}

void SerialInput::Update()
{
    // handle Serial
    if (uart.BlockingReceive(&rx_byte, 1, 10) == UartHandler::Result::OK)
    {
        // Example: interpret incoming byte
        scale = rx_byte / 255.0f;
        scale = powf(2.0f, (scale - 0.5f) * 2.0f); // exponential

        // hw.PrintLine("Output %d", scale);
        controller_->Mod2(scale);
    }
}

/* voice.filter.SetFreq(cutoff_base *(env_f_out *voice.filter_amount) *
                     (1.0f - voice.follow) * scale);
voice.filter.SetRes(cutoff_base *(env_f_out *voice.filter_amount) *
                    (1.0f - voice.follow) * scale);
float filter_out = voice.filter.Process(sig); */