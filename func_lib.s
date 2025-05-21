jmp IN
jmp OUT
jmp HLT

OUT:
    pop rbx        ; return address
    pop rax        ; value to print
    push rbx       ; restore return address

    sub rsp, 32    ; выделим 32 байта буфера в стеке
    mov rdi, rsp   ; указатель на конец буфера (вниз по стеку)
    mov rbx, rdi
    add rbx, 20    ; указатель на конец строки (буфера)
    mov byte [rbx], 10  ; newline
    dec rbx

.convert_loop:
    xor rdx, rdx
    mov rcx, 10
    div rcx
    add dl, '0'
    mov [rbx], dl
    dec rbx
    test rax, rax
    jnz .convert_loop

    inc rbx ; теперь rbx указывает на начало строки

    mov rax, 1      ; syscall write
    mov rdi, 1      ; stdout
    mov rsi, rbx    ; строка
    mov rdx, rsp
    add rdx, 21
    sub rdx, rbx    ; длина строки
    syscall

    add rsp, 32     ; освободим буфер
    ret

IN:
    sub rsp, 16         ; резервируем буфер в стеке
    mov rsi, rsp        ; rsi указывает на буфер

    mov rax, 0          ; syscall read
    mov rdi, 0          ; stdin
    mov rdx, 16         ; читаем до 16 байт
    syscall

    xor rax, rax        ; обнуляем число
    xor rcx, rcx

.parse_loop:
    mov bl, [rsi]
    cmp bl, 10
    je .done
    cmp bl, '0'
    jl .done
    cmp bl, '9'
    jg .done

    sub bl, '0'
    imul rax, rax, 10
    add rax, rbx
    inc rsi
    jmp .parse_loop

.done:
    add rsp, 16         ; очистить буфер
    pop rbx             ; достаём return address
    push rax            ; число
    push rbx            ; вернуть return address
    ret

HLT:
    mov rax, 60
    xor edi, edi
    syscall
