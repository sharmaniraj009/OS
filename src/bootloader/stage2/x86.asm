bits 16

section _TEXT class=CODE

;
;void _cdecl x86_div64_32(uint64_t dividend, uint32_t divisor, uint64_t* quoOut, uint32_t* remainderOut);
;

global _x86_div64_32
_x86_div64_32:
    push bp             ; save old call frame
    mov bp, sp

    push bx

    mov eax, [bp + 4]
    mov ecx, [bp + 12]
    xor edx, edx
    div ecx
    
    
    mov bx, [bx + 16]
    mov [bx + 4], eax 


    mov eax, [bp + 4]

    div ecx
    
    
    mov [bx], eax
    mov bx, [bp + 18]
    mov [bx], edx

    mov sp, bp
    pop bp
    ret



;
; int 10h ah=0Eh
; args: character, page
;
global _x86_Video_WriteCharTeletype
_x86_Video_WriteCharTeletype:
    
    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; save bx
    push bx

    ; [bp + 0] - old call frame
    ; [bp + 2] - return address (small memory model => 2 bytes)
    ; [bp + 4] - first argument (character)
    ; [bp + 6] - second argument (page)
    ; note: bytes are converted to words (you can't push a single byte on the stack)
    mov ah, 0Eh
    mov al, [bp + 4]
    mov bh, [bp + 6]

    int 10h

    ; restore bx
    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp
    ret