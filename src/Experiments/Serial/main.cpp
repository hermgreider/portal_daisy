#include "daisy_seed.h"
using namespace daisy;

DaisySeed hw;
UartHandler uart;
uint8_t buffer[100];

void setup() {
    // Initialize the Daisy Seed board
    hw.Init();
    hw.StartLog();

    // Configure USART4
    UartHandler::Config uart_config;
    uart_config.periph = UartHandler::Config::Peripheral::USART_1;
    uart_config.mode = UartHandler::Config::Mode::TX_RX;
    uart_config.pin_config.tx = seed::D13;
    uart_config.pin_config.rx = seed::D14;
    uart_config.baudrate = 9600; // Adjust the baud rate according to your needs

    // Initialize the UART
    if (uart.Init(uart_config) == UartHandler::Result::ERR) {
        while (1) {
            // UART initialization failed, blink an LED or print an error message
            hw.SetLed(false);
            System::Delay(100);
            hw.SetLed(true);
            System::Delay(100);
        }
    }
}

void loop() {
    hw.PrintLine("Loop");

    // Send a string over USART1
    const char* message = "He";
    if (uart.PollTx((uint8_t*)message, strlen(message)) == UartHandler::Result::ERR) {
        // UART transmission failed, blink an LED or print an error message
        hw.SetLed(false);
        System::Delay(100);
        hw.SetLed(true);
        System::Delay(100);
        hw.SetLed(false);
        System::Delay(100);
        hw.SetLed(true);
        System::Delay(100);
    }
    
    hw.SetLed(false);
    System::Delay(50);
    hw.SetLed(true);
    System::Delay(50);
    hw.SetLed(false);
    System::Delay(50);
    hw.SetLed(true);
    System::Delay(50);
    hw.SetLed(false);
    System::Delay(50);

    if (uart.BlockingReceive(buffer, 1, 100) == UartHandler::Result::ERR) {
        hw.PrintLine("BlockingReceive error");
        // UART receive failed, blink an LED or print an error message
        hw.SetLed(false);
        System::Delay(100);
        hw.SetLed(true);
        System::Delay(100);
        hw.SetLed(false);
        System::Delay(100);
        hw.SetLed(true);
        System::Delay(100);
    }

    else {
        hw.PrintLine("Output %d", buffer[0]);
    }

    // Wait a bit before sending the next message
    System::Delay(1000); // 1 second delay
}

int main() {
    setup();

    while (true) {
        loop();
    }

    return 0;
}

