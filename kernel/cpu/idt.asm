;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             iceOS Kernel            ;
; Developed by Marco 'icebit' Cetica  ;
;              (c) 2019               ;
;        Released under GPLv3         ;
;   https://github.com/ice-bit/iceOS  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global idt_flush ; for drivers/idt.c

section .text
idt_flush:
    mov eax, [esp+4] ; Retrieve idt_ptr_t*
    lidt [eax]
    ret