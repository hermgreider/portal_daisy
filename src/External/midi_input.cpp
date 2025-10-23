#include "daisy_seed.h"
#include "midi_input.h"
#include "Init/controller.h"

using namespace daisy;

#define MIDI_UART
// #define MIDI_USB

extern DaisySeed hw;

#ifdef MIDI_USB 
MidiUsbHandler midi;
#else
MidiUartHandler midi;
#endif

/** FIFO to hold messages as we're ready to print them */
#define DSY_TEXT __attribute__((section(".text")))
DSY_TEXT FIFO<MidiEvent, 128> event_log;

void MidiInput::Init(Controller *controller)
{
    controller_ = controller;

    #ifdef MIDI_USB 
    MidiUsbHandler::Config midi_config;
    midi_config.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    
    #else
    MidiUartHandler::Config midi_config;
    #endif
    
    midi.Init(midi_config);
    midi.StartReceive();
}

void MidiInput::Update()
{        
    // handle MIDI Events
    midi.Listen();
    while(midi.HasEvents())
    {
        // hardware.PrintLine("Got one");
        HandleMidiMessage(midi.PopEvent());
    }    
}

void MidiInput::HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            hw.PrintLine("MidiInput NoteOn: %d", p.note);
            controller_->NoteOn(p);
            break;
        }
        case NoteOff:
        {
            NoteOffEvent p = m.AsNoteOff();
            hw.PrintLine("MidiInput NoteOff: %d", p.note);
            controller_->NoteOff(p);
            break;
        } 
        
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            hw.PrintLine("MidiInput CC: %d, Val: %d", p.control_number, p.value);

            switch(p.control_number)
            {
                case 80: // Type Knob = Synth select
                    if (p.value == 0) {
                        controller_->Select(0);
                    }
                    else if (p.value == 25) {
                        controller_->Select(1);
                    }
                    else if (p.value == 51) {
                    }
                    else if (p.value == 76) {
                    }
                    else if (p.value == 102) {
                    }
                    else if (p.value == 127) {
                    }

                    break;

                case 81:
                    // Reface Drive (81) = Mod2
                    controller_->Mod2((float)p.value / 127.0f);
                    break;

                case 17: 
					// Reface Tremelo Switch
					controller_->Select2();
                    break;

                default:
                    break;
            }
        }
        default:
            break;
    }
}
