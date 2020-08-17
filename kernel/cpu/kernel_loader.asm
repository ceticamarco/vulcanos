;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             iceOS Kernel            ;
; Developed by Marco 'icebit' Cetica  ;
;              (c) 2019-2020          ;
;        Released under GPLv3         ;
;   https://github.com/ice-bit/iceOS  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32] ; We should be in protected mode
section .multiboot

head_s:
    dd 0xE85250D6        ; Multiboot header magic number
    dd 0                ; Protected mode flag
    dd head_e - head_s  ; Header length
    dd 0x100000000 - (0xE85250D6 + 0 + (head_e - head_s)) ; Checksum of above

    ; Other flags
    dw 0 ; type
    dw 0 ; flags
    dd 0 ; size

head_e:

GLOBAL kernel_loader
EXTERN kernel_main

section .text
kernel_loader:
    mov esp, kernel_stack + KERNEL_STACK_SZ ; Define stack pointer
    push ebx ; Set multiboot header
    call kernel_main ; Jump into kernel's main function
.loop:
    jmp .loop ; If the kernel returns, go into an infinite loop.
              ; This will prevent the CPU to run non-kernel instructions
              ; from the memory

KERNEL_STACK_SZ equ 4096 ; Stack size(4KiB)

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SZ ; Reserve 4 KiB
