SECTION .text
EXTERN funcion_matematica         ; Validar directivas de enlazador externo

GLOBAL inicio

inicio:
    PUSH EBP
    MOV EBP, ESP
    
    CALL funcion_matematica       ; Llamada a etiqueta externa
    
    POP EBP
    RET
