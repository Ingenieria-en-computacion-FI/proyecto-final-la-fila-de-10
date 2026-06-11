; ==========================================
; Constantes del Sistema (Cabecera)
; ==========================================

; Codigos de interrupcion de Linux (Syscalls)
SYS_EXIT  EQU 1
SYS_READ  EQU 3
SYS_WRITE EQU 4

; Descriptores de archivo estandar
STDIN     EQU 0
STDOUT    EQU 1
STDERR    EQU 2