#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "estructuras.h"

#define MAX_EXEC_BYTES 256

uint8_t binario_ejecutable_final[MAX_EXEC_BYTES];
int tamano_ejecutable = 0;

// Simulación de resolución de referencias cruzadas
void enlazar_modulos(uint8_t *seccion_text_actual, int tamano_actual) {
    printf("\n[5] Ejecutando Linker (Resolucion de dependencias externas)...\n");

    // 1. Copiamos el código máquina del módulo principal que ya generamos
    for(int i = 0; i < tamano_actual; i++) {
        binario_ejecutable_final[tamano_ejecutable++] = seccion_text_actual[i];
    }

    printf("  -> Fusionando Seccion .TEXT principal (%d bytes)\n", tamano_actual);
    printf("  -> Resolviendo Simbolo Global Externo: 'sumar' (Encontrado en modulo_matematico.obj)\n");
    
    // 2. Simulamos la inyección del código del segundo módulo (ejemplo: la rutina sumar)
    // Agregamos un opcode simulado de un MOV y un RET del segundo archivo (0xB9 0x01 0x00 0x00 0x00 0xC3)
    uint8_t codigo_modulo_externo[] = {0xB9, 0x01, 0x00, 0x00, 0x00, 0xC3};
    int tamano_externo = sizeof(codigo_modulo_externo);

    printf("  -> Inyectando Tabla de Relocaciones en formato Little-Endian...\n");
    
    for(int i = 0; i < tamano_externo; i++) {
        binario_ejecutable_final[tamano_ejecutable++] = codigo_modulo_externo[i];
    }

    // 3. Desplegar el binario definitivo unificado (.EXE simulado)
    printf("\n=== BINARIO EJECUTABLE FINAL (.EXE GENERADO) ===\n");
    printf("Simbolos Globales Resueltos -> main: 0x0000 | sumar: 0x0015\n");
    printf("Seccion .text Fusionada y Reubicada:\n[");
    for (int i = 0; i < tamano_ejecutable; i++) {
        printf("0x%02x", binario_ejecutable_final[i]);
        if (i < tamano_ejecutable - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}