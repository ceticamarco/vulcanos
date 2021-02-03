;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             vulcanOS Kernel           ;
; Developed by Marco 'icebit' Cetica    ;
;              (c) 2019-2021            ;
;        Released under GPLv3           ;
;   https://github.com/ice-bit/vulcanos ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .multiboot_header
header_s:
    dd 0xE85250D6 ; Magic number for multiboot
    dd 0 ; Protected mode flag
    dd header_e - header_s ; Header length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_e - header_s)) ; Checksum

    ; Other flags
    dw 0 ; Type
    dw 0 ; Flags
    dw 0 ; Size
header_e: