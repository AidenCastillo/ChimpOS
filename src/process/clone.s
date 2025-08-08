.section .text
.global clone
.type clone, @function
clone:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 4]

    push ebx
    push esi
    push edi

    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]
    mov esi, [ebp + 20]
    mov edi, [ebp + 24]

    test ecx, ecx

    jz nonfork

    sub ecx, 8
    mov [ecx], ecx
    mov [ecx + 4], eax

nonfork:

    mov eax, 120 # sys_clone

    int 0x80

    test eax, eax

    jnz parent
    
    mov esp, ebp
    pop ebp

    ret

parent:
    pop edi
    pop esi
    pop ecx
    
    mov esp, ebp
    pop ebp
    ret
