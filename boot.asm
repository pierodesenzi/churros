ORG 0; ? start of the code segment
BITS 16; only 16 bit code
_start:
    jmp short pre_start
    nop

times 33 db 0
pre_start:
    jmp 0x7c0:start

start:
    cli; clears interrupts
    mov ax, 0x7c0
    mov ds, ax; set Data Segment as 0x7c0 through ax (direct set not possible)
    mov es, ax; set Extra Segment as 0x7c0 through ax (direct set not possible)
    mov ax, 0x00
    mov ss, ax; set Stack Segment as 0 through ax (direct set not possible)
    mov sp, 0x7c00; set Stack Pointer
    sti; enables interrupts

    mov si, message
    call print
    jmp $; jump to itself

print:
    mov bx, 0
.loop:
    lodsb; loads the character pointed by SI to AL and shifts to next
    cmp al, 0; compare
    je .done; Jump if Equal (result of the op above)
    call print_char
    jmp .loop
.done:
    ret

print_char: 
    mov ah, 0eh; set operation as write on screen the contents of al
    int 0x10; trigger the operation call to BIOS (x86)
    ret

message: db '0000000000111111111122222222223333333333444444444455555555556666666666', 0
;message: db 'churrOS (c)2023', 0
times 510-($ - $$) db 0; fill next unused bytes with 0 until 510th
dw 0xAA55; DefineWord: 0xAA55 is the signature for the bootloader