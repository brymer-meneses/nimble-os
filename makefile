
KERNEL_CC_SOURCE_FILES 	:= $(wildcard kernel/**/*.cc kernel/*.cc) 
KERNEL_ASM_SOURCE_FILES := $(wildcard kernel/**/*.asm kernel/*.asm) 
BOOT_SOURCE_FILES 	:= $(wildcard boot/*.asm)

OBJECT_FILES := $(patsubst kernel/%.cc,  	build/kernel/%.cc.o, 			$(KERNEL_CC_SOURCE_FILES)) \
								$(patsubst kernel/%.asm, 	build/kernel/%.asm.o, 		$(KERNEL_ASM_SOURCE_FILES)) \
								$(patsubst boot/%.asm, 	 	build/boot/%.asm.o, 			$(BOOT_SOURCE_FILES))

CC 	:= x86_64-elf-gcc
LD	:= x86_64-elf-ld

LIBCPP := lib/libc++/include/
LIB := lib/

CXXFLAGS := \
    -Wall \
    -Wextra \
    -std=c++23\
		-g \
		-lgcc \
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
		-I $(LIBCPP) \
		-I $(LIB) \
 
LDFLAGS := \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T linker.ld

.PHONY: all clean

all: iso

debug:
	echo $(OBJECT_FILES)

install-deps:
	-git clone https://github.com/ilobilo/libstdcxx-headers --depth=1 lib/libc++

clean:
	$(RM) -rf build

run: iso
	qemu-system-x86_64 -cdrom build/nimble-os.iso

iso: nimble-os 
	mkdir -p build/iso/boot/grub
	cp build/nimble-os.bin build/iso/boot/nimble-os.bin
	cp boot/grub.cfg build/iso/boot/grub
	grub-mkrescue -o build/nimble-os.iso	build/iso 2> /dev/null
	rm -r build/iso

nimble-os: build/nimble-os.bin

build/nimble-os.bin: $(OBJECT_FILES)
	$(LD) $(LDFLAGS) $(OBJECT_FILES) -o $@
	
build/kernel/%.cc.o: kernel/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

build/kernel/%.asm.o: kernel/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

build/boot/%.asm.o: boot/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@



