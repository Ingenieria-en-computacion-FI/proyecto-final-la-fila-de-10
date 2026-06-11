@echo off
title AUTOMATIZACION IA-32 C
echo ===================================================
echo [INFO] Compilando Ensamblador en C nativo...
echo ===================================================
gcc -Iinclude src\main.c src\lexer.c src\parser.c src\encoder.c src\linker.c -o ensamblador.exe

echo.
echo [RUN] Corriendo prueba avanzada...
echo ---------------------------------------------------
ensamblador.exe examples\pruebas_avanzadas.asm
pause
