
KERNEL_CC_SOURCE_FILES 	:= $(wildcard kernel/**/*.cc kernel/*.cc) 
KERNEL_ASM_SOURCE_FILES := $(wildcard kernel/**/*.asm kernel/*.asm) 

BOOT_SOURCE_FILES 	:= $(wildcard boot/*.asm)

OBJECT_FILES := $(patsubst kernel/%.cc,  	build/kernel/%.cc.o, 			$(KERNEL_CC_SOURCE_FILES)) \
								$(patsubst kernel/%.asm, 	build/kernel/%.asm.o, 		$(KERNEL_ASM_SOURCE_FILES)) \
								$(patsubst boot/%.asm, 	 	build/boot/%.asm.o, 			$(BOOT_SOURCE_FILES))

OBJECT_FILES := $(filter-out build/boot/crti.asm.o build/boot/crtn.asm.o, $(OBJECT_FILES))

# TODO:
# filter out test files so that it won't get linked when building the iso

CXX 	:= x86_64-elf-gcc
LD	:= x86_64-elf-ld

LIBCXX_FREESTANDING := deps/libc++/include/

TESTFLAGS := \
		-DENABLE_TESTS

CXXFLAGS := \
		-Wall \
		-Wextra \
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
		-I kernel \
		-I $(LIBCXX_FREESTANDING) \
		-I deps \
 
LDFLAGS := \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T linker.ld

CRTI_OBJ		 := build/boot/crti.asm.o
CRTBEGIN_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)

CRTEND_OBJ 	 := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_OBJ		 := build/boot/crtn.asm.o

OBJ_LINK_LIST := $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJECT_FILES) $(CRTEND_OBJ) $(CRTN_OBJ)

.PHONY: all clean

all: iso

install-deps:
	-git clone https://github.com/ilobilo/libstdcxx-headers --depth=1 deps/libc++

clean:
	$(RM) -rf build

run: iso
	qemu-system-x86_64 -cdrom build/nimble-os.iso

test: CXXFLAGS += $(TESTFLAGS)
test: clean iso
	qemu-system-x86_64 -cdrom build/nimble-os.iso

iso: nimble-os 
	mkdir -p build/iso/boot/grub
	cp build/nimble-os.bin build/iso/boot/nimble-os.bin
	cp boot/grub.cfg build/iso/boot/grub
	grub-mkrescue -o build/nimble-os.iso	build/iso 2> /dev/null
	rm -r build/iso

nimble-os: build/nimble-os.bin install-deps

build/nimble-os.bin: $(OBJECT_FILES) $(CRTI_OBJ) $(CRTN_OBJ)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -o $@
	
build/kernel/%.cc.o: kernel/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/kernel/%.asm.o: kernel/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

build/boot/%.asm.o: boot/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@



