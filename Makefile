ASM=nasm
CC=gcc

SRC_DIR=src
BUILD_DIR=build
TOOLS_DIR=tools


.PHONY = all floppy_image kernel bootloader clean always tools_fat


# all

all: floppy_image tools_fat

# FLOPPY image

floppy_image: $(BUILD_DIR)/main_floppy.img 

$(BUILD_DIR)/main_floppy.img: bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/main_floppy.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" $(BUILD_DIR)/main_floppy.img
	dd if=$(BUILD_DIR)/stage1.bin of=$(BUILD_DIR)/main_floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/stage2.bin "::kernel.bin"
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/kerne1=.bin "::kernel.bin"
	mcopy -i $(BUILD_DIR)/main_floppy.img test.txt "::text.txt"

# bootloader
bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin : always
	$(MAKE) $(SRC_DIR)/bootloader/stage1 BUILD_DIR=%(abspaath $(BUILD_DIR)/stage1)

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage1.bin : always
	$(MAKE) $(SRC_DIR)/bootloader/stage2 BUILD_DIR=%(abspaath $(BUILD_DIR)/stage2)

# Kernel

kernel: $(BUILD_DIR)/kernel.bin 

$(BUILD_DIR)/kernel.bin: always
	$(ASM) $(SRC_DIR)/kernel BUILD_DIR=%(abspaath $(BUILD_DIR)



# tools_fat
tools_fat: $(TOOLS_DIR)/tools/fat
$(TOOLS_DIR)/tools/fat: always $(TOOLS_DIR)/fat/fat.c
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -g -o $(BUILD_DIR)/tools/fat $(TOOLS_DIR)/fat/fat.c
#always
always:
	mkdir -p $(BUILD_DIR)


clean:
	$(MAKE) $(SRC_DIR)/bootloader/stage1 BUILD_DIR=%(abspaath $(BUILD_DIR)) clean
	$(MAKE) $(SRC_DIR)/bootloader/stage2 BUILD_DIR=%(abspaath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/Kernel BUILD_DIR=(abspaath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*