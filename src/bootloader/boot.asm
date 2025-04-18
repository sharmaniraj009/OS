org 0x7C00                ; Sets the origin (starting address) to 0x7C00, where BIOS loads the bootloader
bits 16                   ; Tells the assembler we're working in 16-bit mode (real mode)
%define ENDL 0x0D, 0x0A   ; Define ENDL as carriage return + line feed




jmp short start
nop

bdb_oem:              db'MSW4.1'
number_of_bytes_per_sector: 




start:
    jmp main

; Function to print a null-terminated string
; Input: SI = pointer to string
puts:
    push si               ; Save registers we'll modify
    push ax
.loop:
    lodsb                 ; Load byte at SI into AL and increment SI
    or al, al             ; Test if character is null (end of string)
    jz .done              ; If zero flag is set, we're done
    mov ah, 0x0e          ; Set function code for teletype output
    int 0x10              ; Call BIOS interrupt for video services
    jmp .loop             ; Process next character
.done:
    pop ax                ; Restore registers
    pop si
    ret

main:                     ; Main entry point label
    ; Set up segments and stack
    mov ax, 0             ; Can't write to DS/ES directly
    mov ds, ax            ; Data segment = 0
    mov es, ax            ; Extra segment = 0
    mov ss, ax            ; Stack segment = 0
    mov sp, 0x7C00        ; Stack grows downward from where we're loaded
    
    ; Print the hello message
    mov si, msg_hello     ; Load address of our message
    call puts             ; Call our string printing function
    
    hlt                   ; Halts the CPU (stops execution until an interrupt occurs)
.halt:                    ; Local label that creates an infinite loop
    jmp .halt             ; Jump back to .halt, creating an infinite loop if execution continues

msg_hello: db 'Hello world', ENDL, 0   ; Our message string, terminated with 0

times 510-($-$$) db 0     ; Fill the rest of the 510 bytes with zeros
                          ; $ is current address, $$ is start address, so ($-$$) is the size so far
dw 0xAA55                 ; Boot signature (0xAA55 in little-endian) that tells BIOS this is bootable