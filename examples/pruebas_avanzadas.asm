SECTION .data
    variable_memoria DD 100       ; Validar directiva DD (4 bytes)
    buffer_vacio RESB 16          ; Validar directiva RESB (16 bytes)

SECTION .text
GLOBAL main

main:
    MOV EAX, 10                   ; Movimiento de Inmediato
    MOV EBX, 5                    
    SUB EAX, EBX                  ; Aritmética Registro-Registro
    ADD ECX, 2                    ; Aritmética Inmediata
    
    MOV EAX, [EBX+ECX*4+8]        ; PRUEBA DE FUEGO: Byte ModRM y Byte SIB + Disp8
    
    CMP EAX, 0
    JE fin                        ; Salto Condicional (Referencia adelantada)
    
    JMP main                      ; Salto Absoluto

fin:
    RET                           ; Retorno de sistema
