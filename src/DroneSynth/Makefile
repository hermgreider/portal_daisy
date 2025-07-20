TARGET = drone_synth
PROJECT_NAME = $(TARGET)

USE_DAISYSP_LGPL = 1
LDFLAGS += -u _printf_float

# Sources
CPP_SOURCES = drone_synth.cpp pod_controls.cpp main.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

