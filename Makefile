include make/clear.mk
include make/defines.mk
ALL :=

TOOLCHAIN_PATH := third_party/gcc-arm-none-eabi-5_2-2015q4

# If clean is specified then short circuit everything
ifeq ($(MAKECMDGOALS),clean)
clean:
	rm -rf $(BUILDDIR)
else

# Every project listed on the command line should match a corresponding .mk
# file under projects/. By including them their deps and such will be setup
# and then $(ALL) receives all the targets
# TODO: Make this support more than 1 thing, since I just broke it so I could
# easily grab $(PROJECT) for the build step.

ifneq ($(MAKECMDGOALS),)
PROJECT := $(firstword $(MAKECMDGOALS))
include $(addprefix projects/, $(addsuffix .mk, $(PROJECT)))
endif

$(MAKECMDGOALS) all: $(ALL)
endif

.PHONY: all clean

# vim: set noexpandtab ts=4 sw=4 sts=4
