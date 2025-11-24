section .text
default rel
bits 64
global asm_coord
asm_coord:
    ; RCX   <- n
    ; RDX   <- *x1
    ; R8    <- *x2
    ; R9    <- *y1
    
    mov r10, [rsp+40] ; R10 <- *y2
    mov r11, [rsp+48] ; R11 <- *z
    
    xor rax, rax ; i = 0 for loop
    
    ; edge case
    cmp rcx, 0
    jle END
    
L1:
    ; loop condition
    cmp rax, rcx
    jge END

    ; (x2[i] - x1[i])^2
    movss xmm0, [r8+rax*4]  ; x2[i]
    subss xmm0, [rdx+rax*4] ; x2[i] - x1[i]
    mulss xmm0, xmm0        ; ^2
    
    ; (y2[i] - y1[i])^2
    movss xmm1, [r10+rax*4] ; y2[i]
    subss xmm1, [r9+rax*4]  ; y2[i] - y1[i]
    mulss xmm1, xmm1        ; ^2
    
    ; add then sqrt
    addss xmm0, xmm1
    sqrtss xmm0, xmm0
    
    ; store
    movss [r11+rax*4], xmm0
    
    ; i++
    inc rax
    jmp L1
  
END: 
    ret