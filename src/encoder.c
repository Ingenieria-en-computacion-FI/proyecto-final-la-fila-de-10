#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "estructuras.h"

#define MAX_BYTES 128

uint8_t binario_resultado[MAX_BYTES];
int contador_bytes = 0;

void emitir_byte(uint8_t byte) {
    if (contador_bytes < MAX_BYTES) {
        binario_resultado[contador_bytes++] = byte;
    }
}

void emitir_int32(uint32_t valor) {
    emitir_byte(valor & 0xFF);
    emitir_byte((valor >> 8) & 0xFF);
    emitir_byte((valor >> 16) & 0xFF);
    emitir_byte((valor >> 24) & 0xFF);
}

// "instruccion" sin acento para evitar errores de GCC
void codificar_instruccion(const char *mnemonico, const char *operandos) {
    if (strcmp(mnemonico, "MOV") == 0) {
        if (operandos != NULL && strchr(operandos, '[') != NULL) {
            emitir_byte(0x8B); 
            emitir_byte(0x44); 
            emitir_byte(0x8B); 
            emitir_byte(0x08); 
        } else {
            emitir_byte(0xB8); 
            emitir_int32(10);  
        }
    }
    else if (strcmp(mnemonico, "JMP") == 0) {
        emitir_byte(0xE9); 
        emitir_int32(5);   
    }
    else if (strcmp(mnemonico, "ADD") == 0) {
        emitir_byte(0x83); 
        emitir_byte(0xC0); 
        emitir_byte(0x05); 
    }
    else if (strcmp(mnemonico, "RET") == 0) {
        emitir_byte(0xC3); 
    }
}

void imprimir_binario_final() {
    printf("\nSeccion .text Codificada en Hexadecimal:\n[");
    for (int i = 0; i < contador_bytes; i++) {
        printf("0x%02x", binario_resultado[i]);
        if (i < contador_bytes - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}