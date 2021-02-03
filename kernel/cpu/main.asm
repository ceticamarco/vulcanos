;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             vulcanOS Kernel           ;
; Developed by Marco 'icebit' Cetica    ;
;              (c) 2019-2021            ;
;        Released under GPLv3           ;
;   https://github.com/ice-bit/vulcanos ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN long_mode_start
[BITS 32] ; Ensure we are in protected mode

section .text
kernel_loader:
    mov esp, kernel_stack + KERNEL_STACK_SZ ; Define stack pointer
    ; Now we need to switch to Long Mode(i.e., 64bit mode)
    ; to do that, we first need to check if we actually are
    ; being loaded by a multiboot compliant bootloader. Then,
    ; we will execute CPUID instruction in order to obtain details
    ; about out CPU(i.e., if the CPU supports long mode).
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; After that, we need to setup paging
    call setup_page_tables
    call enable_paging

    ; We also need to load a GDT
    lgdt[gdt64.pointer]
    jmp gdt64.code_segment:long_mode_start
    push eax ; Set multiboot header register


check_multiboot:
    ; To prove that we are being loaded by a multiboot 
    ; bootloader, we can read the EAX register to see if 
    ; it contains the magic number
    cmp eax, 0xE85250D6
    jne .multiboot_panic
    ret

.multiboot_panic:
    ; We handle this error by showing a special code
    mov al, "M"
    jmp error ; Call print routine

check_cpuid:
    ; To check if CPUID is available we need to flip
    ; ID bit of the flag register, if this is possibile
    ; then CPUID instruction is available. In order to achieve
    pushfd ; Push flag register into the stack
    pop eax ; Pop the stack
    mov ecx, eax ; Make a copy of the stack
    xor eax, 1 << 21 ; Flip ID bit(21th) from the eax register
    push eax
    popfd ; Restore flag register into the stack
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx ; If values are the same, then CPUID is not available
    je .cpuid_panic
    ret ; Otherwise return

.cpuid_panic:
    mov al, "C"
    jmp error ; Call print routine


check_long_mode:
    ; Check if CPU supports extended processor info
    mov eax, 0x80000000 ; Store magic number
    cpuid ; CPUID instruction takes EAX by default
    ; CPUID will set EAX a value greater than the magic number 
    ; if the processor supports extended processor info
    cmp eax, 0x80000001 ; Magic number + 1
    jb .long_mode_panic
    ; Otherwise we can use extended processor info 
    ; to check if long mode is available
    mov eax, 0x80000001
    cpuid
    ; CPUID, with this magic number, will set LM bit(29th)
    ; if and only if long mode is available
    test edx, 1 << 29
    jz .long_mode_panic
    ret

.long_mode_panic:
    mov al, "L"
    jmp error ; Call print routine


setup_page_tables:
    mov eax, page_table_l3
    or eax, 0b11
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, 0b11
    mov [page_table_l3], eax

.loop:
    mov eax, 0x200000 ; 2MiB
    mul ecx
    or eax, 0b10000011
    mov [page_table_l2 + ecx * 8], eax

    mov ecx, 0 ; Counter
    inc ecx ; Increment counter
    cmp ecx, 512 ; Check if whole table is being mapped
    jne .loop ; if not, repeat
    ret

enable_paging:
    ; Pass page table location to the cpu
    mov eax, page_table_l4
    mov cr3, eax
    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax;
    ; Enable long mode
    mov ecx, 0xC0000000
    rdmsr
    or eax, 1 << 8
    wrmsr
    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    
    ret

error: ; Prints "ERR: X" where X stands ofr the given error code
    ; This routine will just print the error code in video memory
    ; Let's understand what those hex codes actually mean:
    ; 0xB8000 is the begin of the VGA text buffer, each byte will 
    ; contain one character. 0x4F is the color code; it means 
    ; white text on read background. 0x52 is the ASCII code for
    ; the letter 'R', while 0x45 stands for the letter 'E',
    ; 0x3A stands for the symbol ':' and 0x20 is just a space.
    ; Do note that addressing is in little endian ordering and 
    ; that one space is begin overwritten by the ASCII byte.
    ;                                 previous      NEW CONTENT
    mov dword [0xB8000], 0x4F524F45 ; []            ER
    mov dword [0xB8004], 0x4F3A4F52 ; [ER]          R:
    mov dword [0xB8008], 0x4F204F20 ; [ERR:]        <space>
    mov byte  [0xB800a], al ; Print the error code
    hlt ; Halt the CPU

KERNEL_STACK_SZ equ 4096 ; Stack size(4KiB)

section .bss
align 4096
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096
kernel_stack:
    resb KERNEL_STACK_SZ ; Reserver 4KiB for kernel's stack


section .rodata
gdt64:
    dq 0 ; zero entry
.code_segment: equ $ - gdt64
    dq (1 << 4) | (1 << 44) | (1 << 47) | (1 << 53) ; Code segment
    ; executable, descriptor,  present     64bit    flags
.pointer:
    dw $ - gdt64 - 1
    dd gdt64