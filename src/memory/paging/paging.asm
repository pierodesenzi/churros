[BITS 32]

section .asm

global paging_load_directory
global enable_paging

paging_load_directory:
    push ebp
    mov ebp, esp ; moving stack pointer to base pointer
    mov eax, [ebp+8] ; putting received pointer value into the eax register
    mov cr3, eax ; cr3 should contain address to page directory
    pop ebp
    ret

enable_paging:
    push ebp
    mov ebp, esp ; moving stack pointer to base pointer
    mov eax, cr0 ; cr0 cannot be directly changed
    or eax, 0x80000000 ; applying bit 31: enabling paging
    mov cr0, eax
    pop ebp
    ret