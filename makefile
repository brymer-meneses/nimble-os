
PROFILE := debug
ARCH := x86_64
CXX  := clang++
NASM := nasm
LD	 := $(ARCH)-elf-ld
QEMU := qemu-system-$(ARCH)

override DEPS_DIR := build/deps
override LIBS_DIR := lib

ifneq ($(CXX), clang++)
  $(error Only clang++ is supported for now)
endif

CXXFLAGS := \
	--target=x86_64-unknown-elf \
	-std=c++20 \
	-Wall \
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
	-T linker.ld 

QEMUFLAGS := \
	-m 512M \
	-smp cpus=2 \
	-serial stdio 

ifeq ($(PROFILE), debug)
	BUILD_DIR := build/debug
	CXXFLAGS += -g -O0 \
							-DDEBUG
	NASMFLAGS += -g -dwarf 
	QEMUFLAGS += -D qemu-log.txt \
							 -d int -M smm=off
else ifeq ($(PROFILE), test)
	BUILD_DIR := build/test
	CXXFLAGS += -g -O0 \
							-DENABLE_TESTS
	NASMFLAGS += -g -dwarf 
	QEMUFLAGS += -D qemu-log.txt \
							 -d int -M smm=off
else ifeq ($(PROFILE), release)
	BUILD_DIR := build/release
	CXXFLAGS += -O3
else 
  $(error Invalid argument for variable PROFILE. Must be either debug, release, or test.)
endif

CXX_SOURCES  := $(shell find . -type f -name '*.cc' | grep -v build/deps)
NASM_SOURCES := $(shell find . -type f -name '*.asm'| grep -v build/deps)

OBJECTS := $(patsubst ./%.cc,  $(BUILD_DIR)/%.cc.o,  $(CXX_SOURCES)) \
           $(patsubst ./%.asm, $(BUILD_DIR)/%.asm.o, $(NASM_SOURCES))

ifeq ($(PROFILE), test)
  OBJECTS := $(filter-out $(BUILD_DIR)/test/%.cc.o, $(OBJECTS))
endif

.PHONY: clean .clangd

all: run

clean:
	$(RM) -r build/debug build/release build/test

distclean:
	$(RM) -rf build .clangd qemu-log.txt

slocs:
	tokei . --exclude=assets/fonts/pixeloperator.cc

debug-address:
	echo $(address) | x86_64-elf-addr2line -e $(BUILD_DIR)/kernel.elf

run: iso
	$(QEMU) $(QEMUFLAGS) -cdrom $(BUILD_DIR)/nimble-os.iso

iso: .clangd dependencies $(OBJECTS)
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
	@./scripts/generate_clangd_config.sh
