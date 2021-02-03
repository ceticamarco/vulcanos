;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             vulcanOS Kernel           ;
; Developed by Marco 'icebit' Cetica    ;
;              (c) 2019-2021            ;
;        Released under GPLv3           ;
;   https://github.com/ice-bit/vulcanos ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GLOBAL kernel_loader
EXTERN kernel_main
[BITS 32] ; Ensure we are in protected mode

section .text
kernel_loader:
    mov esp, kernel_stack + KERNEL_STACK_SZ ; Define stack pointer
    push eax ; Set multiboot header register
    call kernel_main ; Call kernel's main function
.loop:
    jmp .loop ; If the kernel returns, go into an endless loop
              ; This will prevent the CPU to execure any non-kernel 
              ; instructions.

KERNEL_STACK_SZ equ 4096 ; Stack size(4KiB)

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SZ ; Reserver 4KiB for kernel's stack