SECTION .text
GLOBAL main

main:
    MOV EAX, 10       ; Carga el valor 10 en EAX
    ADD EAX, 5        ; Suma 5
    JMP fin           ; Salta a la etiqueta 'fin'

fin:
    RET               ; Retorna al sistema
