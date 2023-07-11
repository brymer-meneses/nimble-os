BUILD_DIR := build
LIBS_DIR := lib

CXX_SOURCES  := $(shell find . -type f -name '*.cc')
NASM_SOURCES := $(shell find . -type f -name '*.asm')

OBJECTS := $(patsubst ./%.cc,  $(BUILD_DIR)/%.cc.o,  $(CXX_SOURCES)) \
           $(patsubst ./%.asm, $(BUILD_DIR)/%.asm.o, $(NASM_SOURCES))

OBJECTS := $(filter-out $(BUILD_DIR)/crti.asm.o $(BUILD_DIR)/crtn.asm.o, $(OBJECTS))

ARCH := x86_64
CXX  := clang++
LD	 := $(ARCH)-elf-ld
NASM := nasm
QEMU := qemu-system-x86_64

CXXFLAGS := \
	--target=x86_64-unknown-elf \
	-g \
	-O0 \
	-std=c++20 \
	-Wall \
	-Werror \
	-Wextra \
	-Wpedantic \
	-ffreestanding \
	-fno-stack-protector \
	-fno-rtti \
	-fno-exceptions \
	-fno-lto \
	-m64 \
	-mabi=sysv \
	-masm=intel \
	-mno-80387 \
	-mcmodel=kernel \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-I lib/libc++/include \
	-I lib \
	-I kernel \
	-I . \

NASMFLAGS := \
	-Wall \
	-g \
	-dwarf \
	-f elf64 \

LDFLAGS := \
	-m elf_${ARCH} \
	-nostdlib \
	-static \
	--no-dynamic-linker \
	-z max-page-size=0x1000 \
	-T linker.ld 

QEMUFLAGS := \
		-D qemu-log.txt \
		-d int -M smm=off \
		-serial stdio 

.PHONY: clean

all: 
	$(MAKE) -j8 run

clean:
	$(RM) -r $(OBJECTS)

distclean:
	$(RM) -r $(BUILD_DIR)/limine
	$(RM) -r lib/limine.h
	$(RM) -r lib/libc++/

debug:
	echo $(address) | x86_64-elf-addr2line -e $(BUILD_DIR)/kernel.elf

install-deps:
	@mkdir -p $(BUILD_DIR)/limine
	@echo "Downloading Limine ..."
	@-git clone https://github.com/limine-bootloader/limine --depth=1 --branch=v5.x-branch-binary $(BUILD_DIR)/limine
	@$(MAKE) -C $(BUILD_DIR)/limine
	@cp $(BUILD_DIR)/limine/limine.h lib/limine.h
	@-git clone https://github.com/ilobilo/libstdcxx-headers --depth=1 lib/libc++

TEST_FLAG_FILE = $(BUILD_DIR)/test_flag

kernel: install-deps $(OBJECTS) 

iso: kernel
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/kernel.elf
	@mkdir -p $(BUILD_DIR)/isoroot
	@cp $(BUILD_DIR)/kernel.elf \
		limine.cfg \
		$(BUILD_DIR)/limine/limine-bios.sys \
		$(BUILD_DIR)/limine/limine-bios-cd.bin \
		$(BUILD_DIR)/limine/limine-uefi-cd.bin \
		$(BUILD_DIR)/isoroot
	@mkdir -p $(BUILD_DIR)/isoroot/EFI/BOOT
	@cp $(BUILD_DIR)/limine/BOOT*.EFI $(BUILD_DIR)/isoroot/EFI/BOOT/
	@xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(BUILD_DIR)/isoroot -o $(BUILD_DIR)/nimble-os.iso
	@$(BUILD_DIR)/limine/limine bios-install $(BUILD_DIR)/nimble-os.iso
	@$(RM) -r $(BUILD_DIR)/isoroot

run: iso
	$(QEMU) $(QEMUFLAGS) -cdrom $(BUILD_DIR)/nimble-os.iso

test: CXXFLAGS += -DENABLE_TESTS
test: clean iso
	$(QEMU) $(QEMUFLAGS) -cdrom $(BUILD_DIR)/nimble-os.iso

$(BUILD_DIR)/%.cc.o: %.cc
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	@$(NASM) $(NASMFLAGS) $< -o $@
