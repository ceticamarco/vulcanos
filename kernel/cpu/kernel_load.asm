;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             iceOS Kernel            ;
; Developed by Marco 'icebit' Cetica  ;
;              (c) 2019               ;
;        Released under GPLv3         ;
;   https://github.com/ice-bit/iceOS  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global kernel_loader
extern main

section .text
kernel_loader:
    mov esp, kernel_stack + KERNEL_STACK_SZ ; Stack pointer
    push ebx
    call main ; jump to kernel's main function
.loop:
    jmp .loop ; endless loop

KERNEL_STACK_SZ equ 4096 ; 4 KB for the stack

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SZ ; Reserver 4 KB