TARGET = portal
PROJECT_NAME = $(TARGET)

USE_DAISYSP_LGPL = 1
LDFLAGS += -u _printf_float

# Sources
CPP_SOURCES += \
src/Init/controller.cpp \
src/Init/board_factory.cpp \
src/Synth/Bell/bell_synth.cpp \
src/Synth/DroneSynth/drone_synth.cpp \
src/External/pod_input.cpp \
src/External/seed_input.cpp \
src/External/simple_fix_input.cpp \
src/main.cpp 

# Library Locations
LIBDAISY_DIR = libDaisy
DAISYSP_DIR = DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

CPPFLAGS += -Isrc

