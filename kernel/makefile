
#################################
# Check for undefined variables
#################################

ERROR := "was not passed to the makefile in the folder 'kernel/'"

ifndef BUILD_DIR
  $(error BUILD_DIR $(ERROR))
endif

ifndef CXX
  $(error CXX $(ERROR))
endif

ifndef CXXFLAGS
  $(error CXXFLAGS $(ERROR))
endif

#################################
# Build process
#################################


SOURCE_DIR := .

SOURCES := $(wildcard ./**/*.cc ./*.cc)
OBJECTS := $(patsubst ./%.cc, $(BUILD_DIR)/kernel/%.cc.o, $(SOURCES))

.PHONY: clean debug

all: $(OBJECTS)

debug:
	$(OBJECTS)

$(BUILD_DIR)/kernel/%.cc.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@


