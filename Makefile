TARGET = portal
PROJECT_NAME = $(TARGET)

APP_TYPE = BOOT_SRAM

# USE_DAISYSP_LGPL = 1
LDFLAGS += -u _printf_float

# Sources
CPP_SOURCES += \
src/External/ff_lfn_stub.cpp \
src/Init/controller.cpp \
src/External/midi_input.cpp \
src/Synth/DroneSynth/drone_synth.cpp \
src/Synth/Bell/bell_synth.cpp \
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
