;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             iceOS Kernel            ;
; Developed by Marco 'icebit' Cetica  ;
;              (c) 2019               ;
;        Released under GPLv3         ;
;   https://github.com/ice-bit/iceOS  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .multiboot
head_s:
    dd 0xe85250d6       ; Magic number
    dd 0                ; Code for protected mode
    dd head_e - head_s  ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (head_e - head_s)) ; Checksum of above

    ; Various flags
    dw 0 ; type
    dw 0 ; flags
    dd 0 ; size
head_e: