# Axoloti-to-Daisy Shim Layer
# Based on libDaisy example Makefile structure

# Project Name
TARGET = axo_patch

# Sources - main.cpp + the generated xpatch.cpp
CPP_SOURCES = src/main.cpp

# Uncomment the line below and point to your xpatch.cpp to compile a real patch:
# CPP_SOURCES += xpatch.cpp

# To compile the test sine instead:
# CPP_SOURCES += test/test_sine.cpp

# Library Locations - adjust these to your local paths
LIBDAISY_DIR ?= ../libDaisy
# DAISYSP_DIR ?= ../DaisySP

# Additional include paths for Axoloti shim headers
C_INCLUDES += -Isrc

# Number of ADC channels — auto-detected from xpatch.cpp.
# Finds the highest adcvalues[N] index and adds 1.
# Override on command line if needed: make NUM_ADC_CHANNELS=4
NUM_ADC_CHANNELS ?= $(shell grep -oE 'adcvalues\[([0-9]+)\]' xpatch.cpp 2>/dev/null | grep -oE '[0-9]+' | sort -n | tail -1 | awk '{print $$1+1}')
ifeq ($(NUM_ADC_CHANNELS),)
    NUM_ADC_CHANNELS = 0
endif
C_DEFS += -DNUM_ADC_CHANNELS=$(NUM_ADC_CHANNELS)

# Auto-extract firmware ID from xpatch.cpp for the init check.
# The fwid appears in "if (fwid != 0xNNNNNNNN)" in xpatch_init2().
PATCH_FWID := $(shell grep -oE 'fwid != (0x[0-9A-Fa-f]+)' xpatch.cpp 2>/dev/null | grep -oE '0x[0-9A-Fa-f]+')
ifneq ($(PATCH_FWID),)
    C_DEFS += -DPATCH_FWID=$(PATCH_FWID)
endif

# Optimization: match Axoloti's aggressive DSP optimization
OPT = -O3

# Additional flags to match Axoloti's DSP behavior
C_DEFS += -ffast-math -ffp-contract=off -fno-math-errno

# Use our patched linker script that maps Axoloti's .sdram section to SDRAM
LDSCRIPT = STM32H750IB_flash.lds

# Core location, and generic Makefile
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Force-include the shim compatibility header AFTER libDaisy's Makefile
# so that CMSIS headers (-include stm32h7xx.h) are already available.
# We append to CPPFLAGS here because the include above just set it.
CPPFLAGS += -include src/axoloti_compat.h
