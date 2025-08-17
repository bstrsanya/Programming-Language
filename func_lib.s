dq IN   - $ - 4
dq OUT  - $ + 4
dq HLT  - $ + 12
dq OUTC - $ + 20
dq SQRT - $ + 28

OUT:
    push rbp
    mov rbp, rsp

    mov rax, [rbp+16]

    mov rcx, 1
    dec rsp
    mov byte [rsp], 10
    mov rbx, 10

.get_digit:
    xor rdx, rdx
    div rbx             ; rax/10, остаток в rdx
    add dl, '0'         ; цифра ASCII
    dec rsp
    mov [rsp], dl       ; кладём на стек 1 байт
    inc rcx
    cmp rax, 0
    jg .get_digit

    mov rax, 1
    mov rdi, 1
    mov rsi, rsp
    mov rdx, rcx
    syscall

    mov rsp, rbp
    pop rbp
    ret


IN:
    push rbp
    mov rbp, rsp

    sub rsp, 32
    mov rsi, rsp
    
    mov rax, 0
    mov rdi, 0
    mov rdx, 16
    syscall

    xor rax, rax
    xor rcx, rcx    

    mov bl, [rsi]
    cmp bl, '-'       
    jne .parse_digits
    inc rsi       
    mov rcx, 1  

    .parse_digits:
    mov bl, [rsi]
    cmp bl, 10         
    je .done_parse
    cmp bl, '0'
    jl .done_parse
    cmp bl, '9'
    jg .done_parse

    sub bl, '0'
    movzx rbx, bl
    imul rax, rax, 10
    add rax, rbx

    inc rsi
    jmp .parse_digits

    .done_parse:
    cmp rcx, 0
    je .finish

    neg rax

    .finish:
    mov rsp, rbp
    pop rbp
    ret

OUTC:
    push rbp
    mov rbp, rsp
    
    mov rax, 1 
    mov rdi, 1
    lea rsi, [rbp + 16]
    mov rdx, 1
    syscall
    
    mov rsp, rbp
    pop rbp
    ret

SQRT:
    push rbp
    mov rbp, rsp

    cvtsi2sd xmm0, [rbp + 16]
    sqrtsd xmm0, xmm0
    cvttsd2si rax, xmm0

    mov rsp, rbp
    pop rbp
    ret

HLT:
    mov rax, 60
    xor edi, edi
    syscall
