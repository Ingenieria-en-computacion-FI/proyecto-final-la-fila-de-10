#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "estructuras.h"

extern Token tokens_linea[];
extern int num_tokens;

uint8_t buffer_binario[1024];
int indice_binario = 0;

void emitir_byte(uint8_t b) {
    buffer_binario[indice_binario++] = b;
}

unsigned char get_register_code(const char* reg_name) {
    if (strcmp(reg_name, "EAX") == 0) return 0;
    if (strcmp(reg_name, "ECX") == 0) return 1;
    if (strcmp(reg_name, "EDX") == 0) return 2;
    if (strcmp(reg_name, "EBX") == 0) return 3;
    if (strcmp(reg_name, "ESP") == 0) return 4;
    if (strcmp(reg_name, "EBP") == 0) return 5;
    if (strcmp(reg_name, "ESI") == 0) return 6;
    if (strcmp(reg_name, "EDI") == 0) return 7;
    return 0;
}

unsigned char build_modrm(unsigned char mod, unsigned char reg, unsigned char rm) {
    return (mod << 6) | (reg << 3) | rm;
}

// Generación de código máquina evaluando tokens
void codificar_instruccion_dinamica() {
    if (num_tokens == 0) return;

    int inicio = 0;
    // Saltar etiquetas en la segunda pasada
    if (tokens_linea[0].type == TOKEN_IDENTIFIER && tokens_linea[1].type == TOKEN_COLON) {
        inicio = 2;
    }

    if (inicio >= num_tokens) return;

    if (tokens_linea[inicio].type == TOKEN_INSTRUCTION) {
        const char *mnemonico = tokens_linea[inicio].lexeme;

        // Implementación de MOV
        if (strcmp(mnemonico, "MOV") == 0) {
            // Caso 1: MOV Registro, Inmediato (Ej. MOV EAX, 10)
            if (tokens_linea[inicio+1].type == TOKEN_REGISTER && tokens_linea[inicio+3].type == TOKEN_NUMBER) {
                unsigned char reg_code = get_register_code(tokens_linea[inicio+1].lexeme);
                emitir_byte(0xB8 + reg_code); // Opcode base 0xB8 + registro
                
                // Extraer el inmediato (asumimos 32 bits en Little-Endian)
                uint32_t inmm = (uint32_t)strtoul(tokens_linea[inicio+3].lexeme, NULL, 0);
                emitir_byte(inmm & 0xFF);
                emitir_byte((inmm >> 8) & 0xFF);
                emitir_byte((inmm >> 16) & 0xFF);
                emitir_byte((inmm >> 24) & 0xFF);
            }
            // Caso 2: MOV Registro, Registro (Ej. MOV EAX, EBX)
            else if (tokens_linea[inicio+1].type == TOKEN_REGISTER && tokens_linea[inicio+3].type == TOKEN_REGISTER) {
                emitir_byte(0x89); // Opcode MOV r/m32, r32
                unsigned char reg_dst = get_register_code(tokens_linea[inicio+1].lexeme);
                unsigned char reg_src = get_register_code(tokens_linea[inicio+3].lexeme);
                // Mod = 11 (Registro a Registro)
                emitir_byte(build_modrm(3, reg_src, reg_dst)); 
            }
        }
        else if (strcmp(mnemonico, "RET") == 0) {
            emitir_byte(0xC3);
        }
    }
}
