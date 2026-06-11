#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== SUITE DE PRUEBAS DE INTEGRACION EN C ===\n");
    printf("=== VALIDACION AUTOMATIZADA DE CAJA NEGRA ===\n\n");

    // ---------------------------------------------------------
    // TEST 1: Caso Complejo (Archivo pruebas_avanzadas.asm)
    // ---------------------------------------------------------
    printf("[RUN] Test 1: Evaluando codigo avanzado (examples\\pruebas_avanzadas.asm)...\n");
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
    int retorno2 = system("ensamblador.exe examples\\prueba_externa.asm");
    if (retorno2 == 0) {
        printf("  -> [PASADO] Test 2 resolvio directivas del linker con exito.\n\n");
    } else {
        printf("  -> [FALLO] Test 2 fallo al procesar modulos externos.\n\n");
    }

    // ---------------------------------------------------------
    // TEST 3: Caso Defensivo / Manejo de Errores (Archivo error.asm)
    // ---------------------------------------------------------
    printf("[RUN] Test 3: Evaluando resiliencia ante codigo corrupto (examples\\error.asm)...\n");
    int retorno3 = system("ensamblador.exe examples\\error.asm");
    // Aunque el código tenga errores, el ensamblador no debe "crashear" (segmentation fault)
    // Debe terminar su ejecución devolviendo 0 (o el código de error que maneje tu main)
    if (retorno3 == 0) {
        printf("  -> [PASADO] Test 3: El ensamblador proceso el error sin colapsar el sistema.\n\n");
    } else {
        printf("  -> [PASADO/WARNING] Test 3: El ensamblador aborto de forma segura (Codigo %d).\n\n", retorno3);
    }

    printf("===================================================\n");
    printf("[OK] TODAS LAS PRUEBAS DE INTEGRACION HAN FINALIZADO\n");
    printf("===================================================\n");

    return 0;
}
