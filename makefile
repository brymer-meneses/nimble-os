MODE := debug
ARCH := x86_64

CXX  := clang++
NASM := nasm
LD	 := $(ARCH)-elf-ld
QEMU := qemu-system-$(ARCH)

DEPS_DIR := build/deps
LIBS_DIR := lib
SUPPORTED_ARCHS = x86_64

ifneq ($(CXX), clang++)
  $(error Only clang++ is supported for now)
endif

ifeq ($(filter $(ARCH),$(SUPPORTED_ARCHS)),)
  $(error ARCH "$(ARCH)" is not supported. Supported architectures: $(SUPPORTED_ARCHS))
endif

CXXFLAGS := \
	--target=x86_64-unknown-elf \
	-std=c++20 \
	-Wall \
	-Wfatal-errors \
	-Wshadow \
	-Wextra \
	-Wpedantic \
	-pedantic-errors \
	-ffreestanding \
	-fPIE \
	-fno-stack-protector \
	-fno-unwind-tables \
	-fno-asynchronous-unwind-tables \
	-fno-rtti \
	-fno-exceptions \
	-fno-lto \
	-m64 \
	-mcmodel=kernel \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-MMD \
	-I lib/thirdparty/libc++/include \
	-I lib/thirdparty/ \
	-I lib/ \
	-I kernel/ \
	-I . \

NASMFLAGS := \
	-Wall \
	-f elf64 \

LDFLAGS := \
	-m elf_${ARCH} \
	-nostdlib \
	-static \
	-pie \
	--no-dynamic-linker \
	-z max-page-size=0x1000 \
	-T kernel/arch/$(ARCH)/linker.ld

QEMUFLAGS := \
	-m 512M \
	-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
	-smp cpus=2 \
	-serial stdio 

ifeq ($(MODE), debug)
	BUILD_DIR := build/debug
	NASMFLAGS += -g -dwarf 
	CXXFLAGS += \
		-g -O0 \
		-DDEBUG
	MACROS := -DDEBUG
	QEMUFLAGS += \
		-D qemu-log.txt \
		-d int -M smm=off
else ifeq ($(MODE), test)
	BUILD_DIR := build/test
	NASMFLAGS += -g -dwarf 
	CXXFLAGS += \
		-g -O3 \
		-DENABLE_TESTS \
		-DDEBUG
	MACROS := -DDEBUG -DENABLE_TESTS
	QEMUFLAGS += \
		-D qemu-log.txt \
		-display none \
		-d int -M smm=off
else ifeq ($(MODE), release)
	BUILD_DIR := build/release
	MACROS := -DRELEASE
	CXXFLAGS += -O3 -DRELEASE
else 
  $(error Invalid argument $(mode) for variable mode. Must be either debug, release, or test.)
endif

CXX_SOURCES  := $(shell find . -type f -name '*.cc'  | grep -v $(DEPS_DIR))
NASM_SOURCES := $(shell find . -type f -name '*.asm' | grep -v $(DEPS_DIR))

OBJECTS := $(patsubst ./%.cc,  $(BUILD_DIR)/%.cc.o,  $(CXX_SOURCES)) \
           $(patsubst ./%.asm, $(BUILD_DIR)/%.asm.o, $(NASM_SOURCES))

# Ignore the other object files for other architectures
OBJECTS := $(filter-out $(foreach arch,$(filter-out $(ARCH),$(SUPPORTED_ARCHS)), $(BUILD_DIR)/arch/$(arch)/%.o), $(OBJECTS))

# Do not build test objects if we're not on `test` mode
ifneq ($(MODE), test)
  OBJECTS := $(filter-out $(BUILD_DIR)/tests/%.cc.o, $(OBJECTS))
endif

HEADER_DEPS := $(OBJECTS:%.o=%.d)

-include $(HEADER_DEPS)

.PHONY: clean .clangd build

all: run

clean:
	$(RM) -r build/debug build/release build/test

distclean:
	$(RM) -rf build .clangd qemu-log.txt

slocs:
	tokei . --exclude=assets/fonts/pixeloperator.cc

debug-address:
	echo $(address) | x86_64-elf-addr2line -e $(BUILD_DIR)/kernel.elf

run: build
	@$(QEMU) $(QEMUFLAGS) -cdrom $(BUILD_DIR)/nimble-os.iso || true

build: .clangd dependencies $(OBJECTS)
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/kernel.elf
	@mkdir -p $(BUILD_DIR)/isoroot
	@cp $(BUILD_DIR)/kernel.elf \
		limine.cfg \
		$(DEPS_DIR)/limine/limine-bios.sys \
		$(DEPS_DIR)/limine/limine-bios-cd.bin \
		$(DEPS_DIR)/limine/limine-uefi-cd.bin \
		$(BUILD_DIR)/isoroot
	@mkdir -p $(BUILD_DIR)/isoroot/EFI/BOOT
	@cp $(DEPS_DIR)/limine/BOOT*.EFI $(BUILD_DIR)/isoroot/EFI/BOOT/
	@xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(BUILD_DIR)/isoroot -o $(BUILD_DIR)/nimble-os.iso
	@$(DEPS_DIR)/limine/limine bios-install $(BUILD_DIR)/nimble-os.iso
	@$(RM) -r $(BUILD_DIR)/isoroot

dependencies:
	@mkdir -p $(DEPS_DIR)/limine
	@echo "Downloading Limine ..."
	@-git clone https://github.com/limine-bootloader/limine --depth=1 --branch=v5.x-branch-binary $(DEPS_DIR)/limine
	@$(MAKE) -C $(DEPS_DIR)/limine
	@cp $(DEPS_DIR)/limine/limine.h lib/thirdparty/limine.h
	@echo "Downloading Freestanding C++ Headers ..."
	@-git clone https://github.com/ilobilo/libstdcxx-headers --depth=1 lib/thirdparty/libc++

$(BUILD_DIR)/%.cc.o: %.cc
	@mkdir -p $(dir $@)
	@echo "CC $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	@echo "NASM $<"
	@$(NASM) $(NASMFLAGS) $< -o $@

.clangd:
	@./scripts/generate_clangd_config.sh $(MACROS)
