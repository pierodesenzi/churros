[BITS 32]
global _start

extern kernel_main

CODE_SEG equ 0x08; Selector
DATA_SEG equ 0x10; Selector

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    in al, 0x92; enable A20 line
    or al, 2
    out 0x92, al

    ; Remap the master PIC
    mov al, 00010001b; put PIC in initialization mode
    out 0x20, al; Tell master PIC

    mov al, 0x20; interrupt 0x20 is where the master ISR should start
    out 0x21, al

    mov al, 00000001b
    out 0x21, al

    call kernel_main
    jmp $


times 512-($ - $$) db 0; fill next unused bytes with 0 until 512th
