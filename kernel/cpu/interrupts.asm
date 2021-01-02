;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;             vulcanOS Kernel         ;
; Developed by Marco 'icebit' Cetica  ;
;              (c) 2019-2021          ;
;        Released under GPLv3         ;
;   https://github.com/ice-bit/iceOS  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

extern isr_handler ; Defined in drivers/isr.h
extern irq_handler ; Defined in drivers/isr.h

; Let's implement all ISR in a very handy way
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli             ; Disable interrupts
        push byte 0     ; Push dummy error code
        push byte %1    ; Push interrupt number
        jmp isr_common  ; goto ISR handler
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli             ; Disable interrupts
        push byte %1    ; Push interrupt number
        jmp isr_common  ; goto ISR handler
%endmacro

; Now we have to do the same thing for Interrupt Requests,
; in this case the first parameter is the IRQ number while
; the second one is the ISR number to be remapped to
%macro IRQ 2
    global irq%1
    irq%1:
        cli             ; Disable interrupts
        push byte 0     ; Push dummy error code
        push byte %2    ; Push interrupt number
        jmp irq_common  ; goto IRQ handler
%endmacro

; isr_common is a common handler for all 
; Interrupt Service Routines declared in the system
; It's main scope is to save current register's states 
; into the stack, call the C high level handler
; and restore the register's original values from
; the stack
isr_common:
    ;; Save register's content into the stack ;;
    pusha        ; Push edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds   ; Get 16 bits of eax(e.g ds)
    push eax

    mov ax, 0x10 ; Load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;; Call C handler ;;
    call isr_handler ; Call C handler

    ;; Restore register's content from the stack ;;
    pop eax      ; Restore original data segment selector

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa         ; Pop edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8   ; Cleans up pushed error code and ISR number
    sti          ; Re-enable interrupts
    iret         ; Pops 5 things: CS, EIP, EFLAGS, SS and ESp


; irq_common is a common handler for all
; Interrupt Requests, it's very similar to the 
; ISR one
irq_common:
    ;; Save register's content into the stack ;;
    pusha        ; Push edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds   ; Get 16 bits of eax(e.g ds)
    push eax

    mov ax, 0x10 ; Load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;; Call C handler ;;
    call irq_handler ; Call C handler

    ;; Restore register's content from the stack ;;
    pop ebx      ; Restore original data segment selector

    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa         ; Pop edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8   ; Cleans up pushed error code and ISR number
    sti          ; Re-enable interrupts
    iret         ; Pops 5 things: CS, EIP, EFLAGS, SS and ESp

; Standard x86 ISRs (only 8,10-14 and 17 requires to push error codes to the stack)
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
