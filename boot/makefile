
#################################
# Check for undefined variables
#################################

ERROR := "was not passed to the makefile in the folder 'boot/'"

ifndef BUILD_DIR
  $(error BUILD_DIR $(ERROR))
endif

ifndef NASM
  $(error NASM $(ERROR))
endif

ifndef NASMFLAGS
  $(error NASMFLAGS $(ERROR))
endif


ifndef BOOTLOADER
  $(error BUILD_DIR $(ERROR))
endif

#################################
# Build Process
#################################

SOURCE_DIR := ./$(BOOTLOADER)
SOURCES := $(wildcard $(SOURCE_DIR)/*.asm)
OBJECTS := $(patsubst $(SOURCE_DIR)/%.asm, $(BUILD_DIR)/boot/%.asm.o, $(SOURCES))

all: $(OBJECTS)

$(BUILD_DIR)/boot/%.asm.o: $(SOURCE_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(NASM) $(NASMFLAGS) $< -o $@
