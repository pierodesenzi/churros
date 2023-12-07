[BITS 32]

section .asm

global draw_pixel

draw_pixel:
    ; setting mode 13h (320x200, 256 colours)
    mov ah, 00h
    mov al, 13h
    int 10h

    ; drawing pixel and setting colour
    mov ah, 0ch
    mov cx, 160
    mov dx, 100
    mov al, 4
    int 10h

    ; wait for key press
    ;mov ah, 00h
    ;int 16h

    ; return to text mode - 03h
    ;mov ah, 00h
    ;mov al, 03h
    ;int 10h

    ; terminate
    ;mov ah, 4ch
    ;int 21h