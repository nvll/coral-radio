# included from the main makefile to include a set of rules.mk to satisfy
# the current MODULE list. If as a byproduct of including the rules.mk
# more stuff shows up on the MODULE list, recurse

# sort and filter out any modules that have already been included
# Shamelessly adapted from the Little Kernel project.
MODULES := $(sort $(MODULES))
MODULES := $(filter-out $(ALLMODULES),$(MODULES))

ifneq ($(MODULES),)

SOURCES += $(MODULE_SOURCES)
INCLUDES += $(MODULES) $(MODULE_INCLUDES)

ALLMODULES += $(MODULES)
ALLMODULES := $(sort $(ALLMODULES))
INCMODULES := $(MODULES)
MODULES :=
$(foreach INC, $(INCMODULES), $(info including $(INC)))
include $(addsuffix /module.mk,$(INCMODULES))


include make/module.mk
endif
