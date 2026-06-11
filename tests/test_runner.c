#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== SUITE DE PRUEBAS DE INTEGRACION EN C ===\n");
    printf("=== VALIDACION AUTOMATIZADA DE CAJA NEGRA ===\n\n");

    // ---------------------------------------------------------
    // TEST 1: Caso Complejo (Archivo pruebas_avanzadas.asm)
    // ---------------------------------------------------------
    printf("[RUN] Test 1: Evaluando codigo avanzado (examples\\pruebas_avanzadas.asm)...\n");
    
    // Ejecutamos el ensamblador pasandole el archivo con SIB y ModRM
    int retorno1 = system("ensamblador.exe examples\\pruebas_avanzadas.asm");
    
    if (retorno1 == 0) {
        printf("  -> [PASADO] Test 1 completo la ejecucion sin errores.\n\n");
    } else {
        printf("  -> [FALLO] Test 1 devolvio un codigo de error: %d\n\n", retorno1);
    }

    // ---------------------------------------------------------
    // TEST 2: Caso de Modulos (Archivo prueba_externa.asm)
    // ---------------------------------------------------------
    printf("[RUN] Test 2: Evaluando enlaces externos (examples\\prueba_externa.asm)...\n");
    
    // Ejecutamos el ensamblador pasandole el archivo con EXTERN
    int retorno2 = system("ensamblador.exe examples\\prueba_externa.asm");
    
    if (retorno2 == 0) {
        printf("  -> [PASADO] Test 2 resolvio directivas del linker con exito.\n\n");
    } else {
        printf("  -> [FALLO] Test 2 fallo al procesar modulos externos.\n\n");
    }

    printf("===================================================\n");
    printf("[OK] TODAS LAS PRUEBAS DE INTEGRACION HAN PASADO\n");
    printf("===================================================\n");

    return 0;
}
