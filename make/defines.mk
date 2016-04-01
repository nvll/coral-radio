# This file sets global values that are included in all projects as
# a base configuration

# It feels ridiculous, but this is how you represent a space in Make
_EMPTY :=
SPACE := $(_EMPTY) $(_EMPTY)

# Macros
MKDIR = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
GET_PARENT = $(patsubst %/, %, $(dir $(lastword $(MAKEFILE_LIST))))
HOST_OS := $(subst $(SPACE),-,$(shell uname -sm))

# Config vars
BUILDDIR 	:= build
ECHO 		?= @
ALL			:=

# vim: set noexpandtab ts=4 sw=4 sts=4
