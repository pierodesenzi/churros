ORG 0x7c00; start of the code segment
BITS 16; only 16 bit code

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short pre_start
    nop

times 33 db 0
pre_start:
    jmp 0:start

start:
    cli; clears interrupts
    mov ax, 0x00
    mov ds, ax; set Data Segment as 0x7c0 through ax (direct set not possible)
    mov es, ax; set Extra Segment as 0x7c0 through ax (direct set not possible)
    mov ss, ax; set Stack Segment as 0 through ax (direct set not possible)
    mov sp, 0x7c00; set Stack Pointer
    sti; enables interrupts

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

; GDT
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code: ; CS should point to this
    dw 0xffff; Segment limit first 0-15 bits
    dw 0; Base first 0-15 bits
    db 0; Base 16-23 bits
    db 0x9a; Access byte
    db 11001111b; High 4 bit flags and the low 4 bit flags
    db 0; Base 24-31 bits

; offset 0x10
gdt_data:; DS, SS, ES, FS, GS
    dw 0xffff; Segment limit first 0-15 bits
    dw 0; Base first 0-15 bits
    db 0; Base 16-23 bits
    db 0x92; Access byte
    db 11001111b; High 4 bit flags and the low 4 bit flags
    db 0; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1; starting sector
    mov ecx, 100; total number of mem sectors
    mov edi, 0x0100000; address to receive the data
    call ata_lba_read
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax, ; Backup the LBA on EBX
    ; send highest 8 bits of the LBA to hard disk controller
    shr eax, 24
    or eax, 0xE0; selects the master drive
    mov dx, 0x1F6
    out dx, al; outputs as IO (out) with a code (dx) the 8 less significant bits (AL) of 16-bit AL, i.e the least significant 8 bits of 32-bit EAX.
    ; finished sending the highest 8 bits of the LBA

    ; send total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; finished

    ; send more bits of the LBA
    mov eax, ebx; restore backup LBA
    mov dx, 0x1F3
    out dx, al
    ; finished

    ; send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx; restore the backup LBA
    shr eax, 8
    out dx, al

    ; send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx; restore the backup LBA
    shr eax, 16
    out dx, al

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

; Read all sectors into memory
.next_sector:
    push ecx

;checking if we need to read
.try_again:
    mov dx, 0x1F7
    in al, dx; read from port 0x1F7
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x
    rep insw; repeat 256 (ecx) times
    pop ecx
    loop .next_sector
    ret; all over


message: db 'churrOS (c)2023', 0
times 510-($ - $$) db 0; fill next unused bytes with 0 until 510th
dw 0xAA55; DefineWord: 0xAA55 is the signature for the bootloader