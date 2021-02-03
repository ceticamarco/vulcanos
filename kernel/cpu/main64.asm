;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             vulcanOS Kernel           ;
; Developed by Marco 'icebit' Cetica    ;
;              (c) 2019-2021            ;
;        Released under GPLv3           ;
;   https://github.com/ice-bit/vulcanos ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GLOBAL long_mode_start
GLOBAL kernel_loader
EXTERN kernel_main
section .text
[BITS 64] ; Ensure we are in long mode

long_mode_start:
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the kernel's main method
    call kernel_main ; Call kernel's main function
    .loop:
        jmp .loop ; If the kernel returns, go into an endless loop
                ; This will prevent the CPU to execure any non-kernel 
                ; instructions.