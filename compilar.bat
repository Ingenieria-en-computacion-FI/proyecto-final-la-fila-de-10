@echo off
echo [INFO] Compilando el Ensamblador Modular en C...
gcc -Iinclude src/main.c src/lexer.c src/parser.c src/encoder.c src/linker.c -o ensamblador.exe

if %errorlevel% equ 0 (
    echo [OK] Ensamblador compilado con exito.
    echo ---------------------------------------------------
    echo [INFO] Compilando la Suite de Pruebas en C...
    gcc tests/test_runner.c -o tests/test_runner.exe

    if %errorlevel% equ 0 (
        echo [OK] Suite de pruebas lista. Iniciando ejecucion...
        echo ===================================================
        .\tests\test_runner.exe
    ) else (
        echo [ERROR] No se pudo compilar el archivo de pruebas tests/test_runner.c
    )
) else (
    echo [ERROR] Hubo un problema al compilar el codigo fuente del ensamblador.
)
pause
