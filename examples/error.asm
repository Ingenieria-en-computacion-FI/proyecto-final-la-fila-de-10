SECTION .text
GLOBAL main
main:
    MOV @EAX, 10    ; [!] ERROR: Caracter '@' invalido para el Lexer
    ADD EBX         ; [!] ERROR: Falta operando fuente para el Parser