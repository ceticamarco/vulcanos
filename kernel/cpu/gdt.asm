global gdt_flush ; for drivers/gdt.c

section .text
gdt_flush:
    mov eax, [esp+4] ; get address of gdt_ptr_t
    lgdt [eax]  ; Load GDT
    mov ax, 0x10 ; offset in the GDT of the data segment
    mov ds, ax ; Load data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush ; offset in the GDT of the code segment
.flush:
    ret