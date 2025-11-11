TARGET = portal
PROJECT_NAME = $(TARGET)

APP_TYPE = BOOT_SRAM

# USE_DAISYSP_LGPL = 1
LDFLAGS += -u _printf_float

# src/Synth/StringSynth/string_synth.cpp \
# src/Synth/StringSynth/drone_synth.cpp \

# Sources
CPP_SOURCES += \
src/External/ff_lfn_stub.cpp \
src/Init/controller.cpp \
src/External/midi_input.cpp \
src/Synth/MoogySynth/moogy_synth.cpp \
src/Synth/MoogySynth2/moogy_synth2.cpp \
src/Synth/Bell/bell_synth.cpp \
src/Synth/SimpleBell/simple_bell_synth.cpp \
src/Synth/ShakerSynth/shaker_synth.cpp \
src/Synth/Util/reverbsc.cpp \
src/Synth/Util/moogladder.cpp \
src/External/midi_file_player.cpp \
src/main.cpp 

# Library Locations
LIBDAISY_DIR = libDaisy
DAISYSP_DIR = DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

CPPFLAGS += -Isrc
