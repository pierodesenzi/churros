section .asm

global insb
global insw
global outb
global outw

insb:; read byte
    push ebp; save state
    mov ebp, esp; copy stack pointer to base pointer

    xor eax, eax; eax=0
    mov edx, [ebp+8]; copy to edx the parameter passed at insb call (port), whose address is stored in ebp+8
    in al, dx; put into al what is defined in the higher 16 bytes of edx

    pop ebp; load state
    ret

insw:; read word
    push ebp
    mov ebp, esp

    xor eax, eax; =0
    mov edx, [ebp+8]
    in ax, dx
    
    pop ebp
    ret

outb:; write byte
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al

    pop ebp
    ret

outw:; write word
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax

    pop ebp
    ret