@echo off
title AUTOMATIZACION IA-32 C
echo ===========================================
echo [INFO] Compilando Ensamblador en C nativo...
echo ===========================================
gcc -Isrc src\main.c src\lexer.c src\parser.c src\encoder.c src\linker.c -o ensamblador.exe

echo.
echo [RUN] Corriendo prueba basica...
echo -------------------------------------------
ensamblador.exe examples\basico.asm
pause
