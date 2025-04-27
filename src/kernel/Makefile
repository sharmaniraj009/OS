ASM? = nasm
BUILD_DIR?=build/


.PHONY = all clean srage2

all:stage2

kerne1: $(BUILD_DIR)/kerne1.bin

$(BUILD_DIR)/kernel.bin:
	$(ASM) main.asm -f bin -o $(BUILD_DIR)/kernel.bin

clean:
	rm -f $(BUILD_DIR)/kernel.bin