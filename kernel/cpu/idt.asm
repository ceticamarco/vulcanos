global idt_flush ; for drivers/idt.c

section .text
idt_flush:
    mov eax, [esp+4] ; Retrieve idt_ptr_t*
    lidt [eax]
    ret