;========================================================================
; ENTRY POINT
;------------------------------------------------------------------------
; This is the entry point for stage2 bootloader
; 
; Initializes:
; - Stack segments and pointers
; - Interrupts are temporarily disabled during setup
;
; Input:
;   DL = Boot drive number (provided by BIOS)
;
; Flow:
; 1. Disable interrupts
; 2. Setup stack segment and pointers
; 3. Enable interrupts
; 4. Clear upper byte of DX
; 5. Push boot drive number as parameter
; 6. Call C entry point (_cstart_)
; 7. Disable interrupts and halt system
;========================================================================

bits 16

section _ENTRY class=CODE

extern _cstart_
global entry

entry:
    cli
    ; setup stack
    mov ax, ds
    mov ss, ax
    mov sp, 0
    mov bp, sp
    sti

    ; expect boot drive in dl, send it as argument to cstart function
    xor dh, dh
    push dx
    call _cstart_

    cli
    hlt