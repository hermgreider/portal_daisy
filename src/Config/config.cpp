#include "External/midi_event.h"
#include "Synth/synth.h"
#include "Synth/Bell/bell_synth.h"
#include "Synth/MoogySynth/moogy_synth.h"
#include "Synth/MoogySynth2/moogy_synth2.h"
#include "Synth/ShakerSynth/shaker_synth.h"
#include "Synth/StringSynth/string_synth.h"

#include "config.h"

// Fsus4 - F Bb C Eb
std::vector<MyMidiEvent> fallback_sequence = {
{0, 1, 65, 127}, {2000, 0, 65, 0}, // F
{2000, 1, 77, 127}, {3000, 0, 77, 0}, // F
{3000, 1, 60, 127}, {3500, 0, 60, 0}, // C
{3500, 1, 63, 127}, {4000, 0, 60, 0}, // Eb
{4000, 1, 77, 20}, {5000, 0, 77, 0}, // F
{5000, 1, 72, 20}, {6000, 0, 72, 0}, // C
{6000, 1, 70, 127}, {6500, 0, 70, 0}, // Bb
{6500, 1, 75, 127}, {7000, 0, 75, 0},
{7000, 1, 70, 127}, {8000, 0, 70, 0},
};

// Pick configuration by uncommenting one of these
// #define PANE4_HORIZ_MOOGY
// #define PANE1_BELL
// #define PANE4_SHAKER
// #define PANE4_MOOGY
// #define PANE8_ORIG_BELL
// #define PANE8_NARROW_SHAKER
#define PANE8_TALL_MOOGY_BASS_NO_SD
// #define PANE6_MOOGY_BASS
// #define STRING_TEST

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

#elif defined(PANE8_ORIG_BELL)

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

MoogySynth2 synth;
Config config = { 
  &synth, 
  "PortalsInfinity.mid", 
  500000.0f,   // default 120 BPM
  0,           // octaves_adjust
  45,          // range_min
  127,         // range_max
  0,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(PANE8_NARROW_SHAKER)

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

#elif defined(PANE4_MOOGY)

// TODO: This one was closed. Not reprogrammed yet.

MoogySynth synth;
Config config = { 
  &synth, 
  "Portals_Brass5.mid", 
  500000.0f,   // default 120 BPM
  0,           // octaves_adjust
  55,          // range_min
  95,          // range_max
  0,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(PANE4_HORIZ_MOOGY)

MoogySynth synth;
Config config = { 
  &synth, 
  "Portals_Brass5.mid", 
  500000.0f,   // default 120 BPM
  0,           // octaves_adjust
  55,          // range_min
  95,          // range_max
  0,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(PANE8_TALL_MOOGY_BASS_NO_SD)

MoogySynth2 synth;
Config config = { 
  &synth, 
  "", 
  500000.0f,   // default 120 BPM
  0,           // octaves_adjust
  0,           // range_min
  127,         // range_max
  0,           // bell_type
  0, 0, 0, 0,  // shaker
  fallback_sequence
};

#elif defined(STRING_TEST)

// TODO: This one was closed. Not reprogrammed yet.

StringSynth synth;
Config config = { 
  &synth, 
  "Portals_Brass5.mid", 
  500000.0f,   // default 120 BPM
  0,           // octaves_adjust
  55,          // range_min
  95,          // range_max
  0,           // bell_type
  0, 0, 0, 0,  // shaker
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


