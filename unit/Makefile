###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project
# HERD_TRUNK_DIR   - Location of the HERD repository
# FRAMEWORK_DIR    - Location of the MOOSE framework
#
###############################################################################
MOOSE_DIR          ?= $(shell dirname `pwd`)/../moose
FRAMEWORK_DIR      ?= $(MOOSE_DIR)/framework
ADDITIONAL_CPPFLAGS += -Wall -Wextra
###############################################################################

# Extra stuff for GTEST
ADDITIONAL_INCLUDES	:= -isystem $(FRAMEWORK_DIR)/contrib/gtest
ADDITIONAL_INCLUDES	:= -isystem $(MOOSE_DIR)/unit/include
ADDITIONAL_LIBS 	:= $(FRAMEWORK_DIR)/contrib/gtest/libgtest.la
###############################################################################

CURRENT_DIR        := $(shell pwd)

# framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

################################## MODULES ####################################
ALL_MODULES       := no
RICHARDS          := yes
TENSOR_MECHANICS  := yes
FLUID_PROPERTIES  := yes
include           $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# dep apps
APPLICATION_DIR    := $(CURRENT_DIR)/..
APPLICATION_NAME   := ba_hun
include            $(FRAMEWORK_DIR)/app.mk

APPLICATION_DIR    := $(CURRENT_DIR)
APPLICATION_NAME   := ba_hun-unit
BUILD_EXEC         := yes
DEP_APPS    ?= $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include $(FRAMEWORK_DIR)/app.mk

# Find all the BA_HUN unit test source files and include their dependencies.
ba_hun-unit_srcfiles := $(shell find $(CURRENT_DIR) -name "*.C")
ba_hun-unit_deps := $(patsubst %.C, %.$(obj-suffix).d, $(ba_hun-unit_srcfiles))
-include $(ba_hun-unit_deps)

###############################################################################
# Additional special case targets should be added here
