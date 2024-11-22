.globl testes
.text

testes:

    movq %rdi, %rsi
    movq %rdi, %rdx

    movq %rsi, %rdi
    movq %rsi, %rdx

    movq %rdx, %rdi
    movq %rdx, %rsi