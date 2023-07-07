
#################################
# Check for undefined variables
#################################

ERROR := "was not passed to the makefile in the folder 'arch/'"

ifndef ARCH
  $(error ARCH $(ERROR))
endif

ifndef BUILD_DIR
  $(error BUILD_DIR $(ERROR))
endif

ifndef CXX
  $(error CXX $(ERROR))
endif

ifndef CXXFLAGS
  $(error CXXFLAGS $(ERROR))
endif

ifndef NASM
  $(error LD $(ERROR))
endif

ifndef NASMFLAGS
  $(error LDFLAGS $(ERROR))
endif

#################################
# Build process
#################################

SOURCE_DIR := ./$(ARCH)


SOURCES := $(wildcard $(SOURCE_DIR)/**/*.cc  $(SOURCE_DIR)/*.cc) \
					 $(wildcard $(SOURCE_DIR)/**/*.asm $(SOURCE_DIR)/*.asm) \

OBJECTS := $(patsubst $(SOURCE_DIR)/%.cc,  $(BUILD_DIR)/arch/%.cc.o, $(SOURCES))
OBJECTS += $(patsubst $(SOURCE_DIR)/%.asm, $(BUILD_DIR)/arch/%.asm.o, $(SOURCES))

.PHONY: clean debug

all: $(OBJECTS)

$(BUILD_DIR)/arch/%.cc.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	@echo "Compiling arch/$(ARCH)/$<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/arch/%.asm.o: $(SOURCE_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo "Compiling arch/$(ARCH)/$<"
	@$(NASM) $(NASMFLAGS) $< -o $@

