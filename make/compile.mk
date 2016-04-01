# vim: set noexpandtab ts=4 sw=4 sts=4

# move everything to the build directory
INCLUDES += $(MODULE_INCLUDES)
SOURCES += $(MODULE_SOURCES)
INCLUDES := $(addprefix -I, $(INCLUDES))
DEFINES  := $(addprefix -D, $(DEFINES))

ASM_SOURCES  := $(filter %.s,$(SOURCES))
C_SOURCES	 := $(filter %.c,$(SOURCES))

OUTPUT_DIR := $(BUILDDIR)/$(PROJECT)
ASM_OBJECTS  := $(addprefix $(OUTPUT_DIR)/, $(subst .s,.o,$(ASM_SOURCES)))
C_OBJECTS    := $(addprefix $(OUTPUT_DIR)/, $(subst .c,.o,$(C_SOURCES)))
OUTPUT	 	 := $(addprefix $(OUTPUT_DIR)/, $(OUTPUT))

CC_NAME := $(shell basename $(CC))
LD_NAME := $(shell basename $(LD))
AS_NAME := $(shell basename $(AS))


#$(warning sources: $(SOURCES))
#$(warning includes: $(INCLUDES))
#$(warning asm_sources: $(ASM_SOURCES))
#$(warning asm_objects: $(ASM_OBJECTS))
#$(warning c_sources: $(ASM_SOURCES))
#$(warning c_objects: $(C_OBJECTS))
#$(warning ldscript: $(LDSCRIPT))

# The variables in front of the targets allow each project file to define and use the immediate
# values before clear.mk clears them all out again
$(OUTPUT): LD := $(LD)
$(OUTPUT): LDFLAGS := $(LDFLAGS)
$(OUTPUT): LDPATH := $(LDPATH)
$(OUTPUT): LDSCRIPT := $(LDSCRIPT)
$(OUTPUT): $(C_OBJECTS) $(CXX_OBJECTS) $(ASM_OBJECTS)
	$(ECHO) $(SIZE) -t --common $(sort $^)
	$(ECHO) echo "[$(LD_NAME)] linking $@ with $(LDSCRIPT)"
	$(ECHO) $(LD) $(LDFLAGS) -L $(LDPATH) -T $(LDSCRIPT) $^ -o $@

$(OUTPUT_DIR)/%.o: PROJECT_DEFINES := $(PROJECT_DEFINES)
$(OUTPUT_DIR)/%.o: DEFINES := $(DEFINES)
$(OUTPUT_DIR)/%.o: CFLAGS := $(CFLAGS)
$(OUTPUT_DIR)/%.o: INCLUDES := $(INCLUDES)
$(OUTPUT_DIR)/%.o: SIZE := $(SIZE)
$(OUTPUT_DIR)/%.o: CC := $(CC)
$(OUTPUT_DIR)/%.o: %.c
	@$(MKDIR)
	$(ECHO) echo "[$(CC_NAME)] compiling $<"
	$(ECHO) $(CC) $(PROJECT_DEFINES) $(DEFINES) $(CFLAGS) $(INCLUDES) -MD $< -c -o $@
	$(ECHO) $(SIZE) $@ >> $@.size


$(OUTPUT_DIR)/%.o: ASFLAGS := $(ASFLAGS)
$(OUTPUT_DIR)/%.o: AS := $(AS)
$(OUTPUT_DIR)/%.o: %.s
	@$(MKDIR)
	$(ECHO) echo "[$(AS_NAME)] compiling $<"
	$(ECHO) $(AS) $(DEFINES) $(ASFLAGS) $< -c -o $@

$(OUTPUT).bin: OBJCOPY := $(OBJCOPY)
$(OUTPUT).bin: $(OUTPUT)
	$(ECHO) echo "generating $@"
	$(ECHO) $(OBJCOPY) -O binary $< $@

$(OUTPUT).hex: OBJCOPY := $(OBJCOPY)
$(OUTPUT).hex: $(OUTPUT)
	$(ECHO) echo "generating $@"
	$(ECHO) $(OBJCOPY) -O ihex $< $@

$(OUTPUT).sym: NM := $(NM)
$(OUTPUT).sym: $(OUTPUT)
	$(ECHO) echo "generating $@"
	$(ECHO) $(NM) -nS $< > $@

$(OUTPUT).lst: OBJDUMP := $(OBJDUMP)
$(OUTPUT).lst: $(OUTPUT)
	$(ECHO) echo "generating $@"
	$(ECHO) $(OBJDUMP) -S $< > $@

$(OUTPUT).size: ASM_OBJECTS := $(ASM_OBJECTS)
$(OUTPUT).size: C_OBJECTS := $(C_OBJECTS)
$(OUTPUT).size: SIZE := $(SIZE)
$(OUTPUT).size: $(OUTPUT)
	$(ECHO) echo "generating $@"
	$(ECHO) $(SIZE) -dA $(ASM_OBJECTS) $(C_OBJECTS) $< > $@
	$(ECHO) $(SIZE) -d $(ASM_OBJECTS) $(C_OBJECTS) $< >> $@
	$(ECHO) $(SIZE) -d $< | tee -a $@

# .d files are generated via the -MD passed to gcc/clang in the c/cpp rules
-include $(C_OBJECTS:.o=.d)
-include $(CXX_OBJECTS:.o=.d)
