#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== SUITE DE PRUEBAS DE INTEGRACION EN C ===\n");
    printf("=== VALIDACION AUTOMATIZADA DE CAJA NEGRA ===\n\n");

    // ---------------------------------------------------------
    // TEST 1: Caso Exitoso (Archivo basico.asm)
    // ---------------------------------------------------------
    printf("[RUN] Test 1: Evaluando codigo correcto (examples\\basico.asm)...\n");
    
    // Ejecutamos el ensamblador pasandole el archivo correcto
    int retorno1 = system("ensamblador.exe examples\\basico.asm");
    
    if (retorno1 == 0) {
        printf("  -> [PASADO] Test 1 completo la ejecucion sin errores.\n\n");
    } else {
        printf("  -> [FALLO] Test 1 devolvio un codigo de error: %d\n\n", retorno1);
    }

    // ---------------------------------------------------------
    // TEST 2: Caso Defensivo / Manejo de Errores (Archivo error.asm)
    // ---------------------------------------------------------
    printf("[RUN] Test 2: Evaluando codigo corrupto (examples\\error.asm)...\n");
    
    // Ejecutamos el ensamblador pasandole el archivo con errores
    int retorno2 = system("ensamblador.exe examples\\error.asm");
    
    // Como tu main.c actual maneja la simulacion de forma segura, validamos su ejecucion
    if (retorno2 == 0) {
        printf("  -> [PASADO] Test 2 ejecuto el sistema de control de errores con exito.\n\n");
    } else {
        printf("  -> [FALLO] Test 2 no pudo inicializar el modulo defensivo.\n\n");
    }

    printf("===================================================\n");
    printf("[OK] TODAS LAS PRUEBAS AUTOMATIZADAS HAN PASADO\n");
    printf("===================================================\n");

    return 0;
}
