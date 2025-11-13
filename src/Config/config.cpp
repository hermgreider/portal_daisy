#include "External/midi_event.h"
#include "Synth/synth.h"
#include "Synth/Bell/bell_synth.h"
#include "Synth/MoogySynth/moogy_synth.h"
#include "Synth/MoogySynth2/moogy_synth2.h"
#include "Synth/ShakerSynth/shaker_synth.h"
#include "Synth/StringSynth/string_synth.h"

#include "config.h"

std::vector<MyMidiEvent> fallback_sequence = {
{0, 1, 48, 127}, {1000, 1, 50, 127}, 
{1100, 0, 48, 0}, {2000, 1, 52, 127}, 
{2100, 0, 50, 0}, {3000, 0, 52, 0}};

// Pick configuration by uncommenting one of these
// #define PANE1_BELL
// #define PANE8_BELL
#define PANE6_MOOGY_BASS
// #define PANE8_SHAKER
// #define PANE4_SHAKER

#ifdef PANE1_BELL_SYNTH

// TODO: This one was closed. Not reprogrammed yet.

BellSynth synth;
Config config = { 
  &synth, 
  "Bell.mid", 
  500000.0f,   // 120 BPM
  1,           // octaves_adjust
  55,          // range_min
  120,         // range_max
  1,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(PANE8_BELL)

BellSynth synth;
Config config = { 
  &synth, 
  "Bell.mid", 
  1000000.0f,  // 60 BPM
  0,           // octaves_adjust
  45,          // range_min
  120,         // range_max
  5,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(PANE6_MOOGY_BASS)

// TODO: This one was closed. Not reprogrammed yet.

MoogySynth2 synth;
Config config = { 
  &synth, 
  "PortalsInfinity.mid", 
  500000.0f,   // default 120 BPM
  0,           // octaves_adjust
  35,          // range_min
  127,         // range_max
  0,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(PANE8_SHAKER)

ShakerSynth synth;
Config config = { 
  &synth, 
  "PortalsShaker.mid", 
  1000000.0f,  // default 60 BPM
  -2,          // octaves_adjust
  0,           // range_min
  127,         // range_max
  0,           // bell_type
  0.002f, 0.06f, 4000.0f, 0, // shaker attack, decay, freq, filter_type (band)
  fallback_sequence
};

#elif defined(PANE4_SHAKER)

ShakerSynth synth;
Config config = { 
  &synth, 
  "PortalsShaker.mid", 
  500000.0f,  // 120 BPM
  0,          // octaves_adjust
  0,          // range_min
  127,        // range_max
  0,          // bell_type
  0.015f, 0.06f, 6000.0f, 1, // shaker attack, decay, freq, filter_type (high)
  fallback_sequence
};

#endif

// NOT DONE YET!

// 4 pane portal
// Moogy - 120BPM, no octave
// Config pane4_moogy = { 
//   &synth, 
//   "Portals_Brass5.mid", 
//   500000.0f,  // default 120 BPM
//   0,          // octaves_adjust
//   45,         // range_min
//   90,         // range_max
//   0,          // bell_type
//   fallback_sequence
// };

// // 4 pane portal - Horizontal Panes
// // Moogy - 120BPM, no octave
// Config pane4_horiz_moogy = { 
//   &synth, 
//   "Portals_Brass5.mid", 
//   500000.0f,  // default 120 BPM
//   0,          // octaves_adjust
//   45,         // range_min
//   90,         // range_max
//   0,          // bell_type
//   fallback_sequence
// };




  // const char* midi_fname = "PortalsAirways.mid"; // for Bells
  // const char* midi_fname = "Portals_Brass5.mid"; // for Moogy
  // const char* midi_fname = "PortalsShaker.mid"; // for Shaker
  // const char* midi_fname = "PortalsInfinity.mid"; // for low Bells
  // const char* midi_fname = "PortalsFireflies.mid";


