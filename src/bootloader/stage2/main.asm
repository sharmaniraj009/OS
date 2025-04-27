;
; Simple 16-bit bootloader kernel entry point
; This code runs in real mode (16-bit) and uses BIOS interrupts
;
; Entry point:
;   - No parameters required
;   - Prints "Hello world from KERNEL!" message using BIOS
;   - Halts the CPU after printing
;
; Routines:
;   puts:
;     Prints a null-terminated string to screen using BIOS int 0x10
;     Input: ds:si - Pointer to null-terminated string
;     Output: None
;     Modifies: ax, bx (preserved via stack)
;     Notes: Uses BIOS teletype output (int 0x10, ah=0x0E)
;
; Constants:
;   ENDL - Carriage return + line feed sequence (0x0D, 0x0A)
;
; Assembly directives:
;   org 0x0 - Code origin at offset 0
;   bits 16 - Generate 16-bit code
;
org 0x0
bits 16


%define ENDL 0x0D, 0x0A


start:
    ; print hello world message
    mov si, msg_hello
    call puts

.halt:
    cli
    hlt

;
; Prints a string to the screen
; Params:
;   - ds:si points to string
;
puts:
    ; save registers we will modify
    push si
    push ax
    push bx

.loop:
    lodsb               ; loads next character in al
    or al, al           ; verify if next character is null?
    jz .done

    mov ah, 0x0E        ; call bios interrupt
    mov bh, 0           ; set page number to 0
    int 0x10

    jmp .loop

.done:
    pop bx
    pop ax
    pop si    
    ret

msg_hello: db 'Hello world from KERNEL!', ENDL, 0