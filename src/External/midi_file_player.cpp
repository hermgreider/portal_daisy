#include <stdio.h>
#include <string.h>
#include "daisy_seed.h"
#include "fatfs.h"
#include "midi_file_player.h"
#include "Init/controller.h"

using namespace daisy;

extern DaisySeed hardware;

// ---------- SD Card --------------------
#define DSY_TEXT __attribute__((section(".text")))
DSY_TEXT FIL            SDFile;
DSY_TEXT FatFSInterface fsi;
SdmmcHandler   sd;

// ------------------ MIDI Event ------------------
struct MyMidiEvent
{
    uint32_t time_ms;
    uint8_t type; // 1=NoteOn, 0=NoteOff
    uint8_t note;
    uint8_t vel;
};

DSY_TEXT std::vector<MyMidiEvent> sequence;
size_t seq_index = 0;
uint32_t start_time_ms = 0;

const char* fname = "calm.mid";

void MidiFilePlayer::Init(Controller *controller)
{
    controller_ = controller;

    // Init SD Card
    SdmmcHandler::Config sd_cfg;
    sd_cfg.Defaults();
    sd.Init(sd_cfg);
    hardware.PrintLine("MidiFilePlayer: SD Init");

    // Link libdaisy i/o to fatfs driver.
    fsi.Init(FatFSInterface::Config::MEDIA_SD);
    hardware.PrintLine("MidiFilePlayer: SD Link to FATFS");

    // Mount SD Card
    /** mount the filesystem to the root directory */
    f_mount(&fsi.GetSDFileSystem(), "/", 1);

    if (LoadMidi("calm.mid")) 
    {
        hardware.PrintLine("MIDI loaded successfully");
        controller->DebugNote(80, 1);
        controller->DebugNote(81, 1);
        controller->DebugNote(82, 1);
    }
    else 
    {
        hardware.PrintLine("MIDI load failed");
        controller->DebugNote(60, 1);
        controller->DebugNote(61, 1);
        controller->DebugNote(62, 1);
    }

    start_time_ms = System::GetNow();
}

void MidiFilePlayer::Update()
{
    if (seq_index >= sequence.size()) 
    {
        hardware.PrintLine("MidiFilePlayer: Resetting Loop");
        seq_index = 0;
        start_time_ms = System::GetNow();
        return;
    } 
        
    uint32_t now = System::GetNow() - start_time_ms;

    if (sequence[seq_index].time_ms <= now)
    {
        hardware.PrintLine("MidiFilePlayer: Playing %d, size: %d", seq_index, sequence.size());
    
        auto &ev = sequence[seq_index];
        if(ev.type == 1)
        {
            NoteOnEvent note = { 0, ev.note, ev.vel };
            controller_->NoteOn(note);
        }
        else
        {
            NoteOffEvent note = { 0, ev.note, ev.vel };
            controller_->NoteOff(note);
        }
        seq_index++;
    }
}


// ------------------ MIDI Parser ------------------
static uint32_t read_varlen(FIL* f)
{
    uint32_t v = 0;
    uint8_t c;
    UINT br;
    do {
        f_read(f, &c, 1, &br);
        v = (v << 7) | (c & 0x7F);
    } while (c & 0x80);
    return v;
}

bool MidiFilePlayer::LoadMidi(const char* fname)
{
    if(f_open(&SDFile, fname, FA_READ) != FR_OK)
    {
        hardware.PrintLine("Error: Cannot open file %s", fname);
        return false;
    }
    hardware.PrintLine("Opened file %s", fname);

    // Minimal MIDI header check
    char id[50] = {0};
    UINT br;
    memset(id, 0, 50);

    f_read(&SDFile, id, 4, &br);
    if(strcmp(id, "MThd") != 0)
    {
        f_close(&SDFile);
        hardware.PrintLine("Error: Not a valid MIDI file");
        return false;
    }
    hardware.PrintLine("MIDI open successfully");
    
    // Proceed with reading events...
    f_lseek(&SDFile, 12); // skip header
    float us_per_qn = 500000.0f; // default 120 BPM
    int ticks_per_qn = 480;
    float ms_per_tick = (us_per_qn / 1000.0f) / ticks_per_qn;

    f_lseek(&SDFile, 22); // skip first track header

    uint32_t abs_ticks = 0;
    uint32_t extra_events = 0;
    uint8_t running = 0;
    DWORD filesize = f_size(&SDFile);

    while(f_tell(&SDFile) < filesize)
    {
        uint32_t delta = read_varlen(&SDFile);
        abs_ticks += delta;

        uint8_t status; f_read(&SDFile, &status, 1, &br);
        if(status < 0x80) { f_lseek(&SDFile, f_tell(&SDFile)-1); status = running; }
        else running = status;

        uint8_t type = status & 0xF0;

        if(type == 0x90 || type == 0x80)
        {
            uint8_t note, vel;
            f_read(&SDFile, &note, 1, &br);
            f_read(&SDFile, &vel, 1, &br);
            MyMidiEvent ev;
            ev.time_ms = abs_ticks * ms_per_tick;
            ev.note = note;
            ev.vel = vel;
            ev.type = (type == 0x90 && vel > 0) ? 1 : 0;
            sequence.push_back(ev);
        }
        else
        {
            extra_events++;
            // skip data bytes
            if(type == 0xC0 || type == 0xD0) f_lseek(&SDFile, f_tell(&SDFile)+1);
            else if(type == 0xA0 || type == 0xB0 || type == 0xE0) f_lseek(&SDFile, f_tell(&SDFile)+2);
            else if(status == 0xFF)
            {
                uint8_t mt;
                f_read(&SDFile, &mt, 1, &br);
                uint32_t len = read_varlen(&SDFile);
                f_lseek(&SDFile, f_tell(&SDFile) + len);
            }
            else f_lseek(&SDFile, f_tell(&SDFile)+1);
        }

        // if (sequence.size() > 4) break;
    }

    f_close(&SDFile);

    hardware.PrintLine("Loaded %d note events, %d additional events", (int)sequence.size(), (int)extra_events);
    return true;
}

