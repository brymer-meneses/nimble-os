
BUILD_DIR := $(abspath build)

ARCH := x86_64

CXX := $(ARCH)-elf-gcc
LD	:= $(ARCH)-elf-ld

BOOTLOADER := grub
CXX := x86_64-elf-gcc
LD := x86_64-elf-ld

DEPS_DIR := $(abspath deps)
LIBCXX_FREESTANDING := $(DEPS_DIR)/libc++/include/

CXX_TESTFLAGS := \
		-DENABLE_TESTS

CXXFLAGS := \
		-Wall \
		-Wextra \
		-Wpedantic \
		-Wconversion \
		-pedantic-errors \
		-O0 \
		-std=c++20\
		-g \
		-masm=intel \
		-lgcc \
		-lc \
		-ffreestanding \
		-fno-stack-protector \
		-fno-threadsafe-statics \
		-fno-stack-check \
		-fno-exceptions \
		-fno-unwind-tables \
		-fno-rtti \
		-fno-lto \
		-fPIE \
		-m64 \
		-march=x86-64 \
		-mabi=sysv \
		-mno-80387 \
		-mno-mmx \
		-mno-sse \
		-mno-sse2 \
		-mno-red-zone \
		-DARCH=$(ARCH) \
		-I $(abspath kernel) \
		-I $(LIBCXX_FREESTANDING) \
		-I $(DEPS_DIR) \

LDFLAGS := \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
		-T boot/$(BOOTLOADER)/linker.ld \

QEMUFLAGS := \
		-D qemu-log.txt \
		-d int -M smm=off \
		-serial stdio 

NASM := nasm
NASMFLAGS := -f elf64

ifeq ($(ARCH),x86_64)
    NASM_FLAGS := -f elf64
else
    NASM_FLAGS := -f elf32
endif

MAKE_FLAGS := \
	ARCH=$(ARCH) \
	BUILD_DIR=$(BUILD_DIR) \
	BOOTLOADER=$(BOOTLOADER) \
	NASM=$(NASM) \
	CXX=$(CXX) \
	LD=$(LD) \
	NASMFLAGS="$(NASMFLAGS)" \
	CXXFLAGS="$(CXXFLAGS)" \
	LDFLAGS="$(LDFLAGS)" \

OBJECTS 			  := 	$(shell find 	$(BUILD_DIR)   -type f -name '*.o')

crti.o 					:= $(filter %/crti.asm.o, $(OBJECTS))
crtn.o 					:= $(filter %/crtn.asm.o, $(OBJECTS))

crtbegin.o 				:= $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
crtend.o	 				:= $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)

OBJECTS 			:= 	$(filter-out %/crti.asm.o %/crtin.asm.o, $(OBJECTS))

OBJECTS_LINK_LIST := $(crti.o) $(crtbegin.o) \
										 $(sort $(OBJECTS)) \
										 $(crtn.o) $(crtend.o) \

.PHONY: arch kernel clean

all: iso

#################################
# Useful Functions
#################################

debug:
	@echo $(OBJECTS_LINK_LIST) | grep "boot.asm.o"

clean:
	@rm -rf $(BUILD_DIR)

install-deps:
	-git clone https://github.com/ilobilo/libstdcxx-headers --depth=1 deps/libc++

run: iso
	qemu-system-x86_64 $(QEMUFLAGS) -cdrom build/nimble-os.iso

test: CXXFLAGS += $(CXX_TESTFLAGS)
test: clean iso
	qemu-system-x86_64 $(QEMUFLAGS) -cdrom build/nimble-os.iso


#################################
# Build Process
#################################


arch-objects:
	@$(MAKE) -C arch/ $(MAKE_FLAGS)

kernel-objects:
	@$(MAKE) -C kernel/ $(MAKE_FLAGS)

boot-objects:
	@$(MAKE) -C boot/ $(MAKE_FLAGS)

kernel: kernel-objects boot-objects arch-objects
	@$(LD) $(LDFLAGS) $(OBJECTS_LINK_LIST) -o $(BUILD_DIR)/kernel-$(ARCH).elf

iso: kernel
	mkdir -p build/iso/boot/grub
	cp build/kernel-$(ARCH).elf build/iso/boot/nimble-os.elf
	cp boot/grub/grub.cfg build/iso/boot/grub
	grub-mkrescue -o build/nimble-os.iso	build/iso 2> /dev/null
	rm -r build/iso



