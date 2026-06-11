#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "estructuras.h"

// Variables externas
extern Token tokens_linea[];
extern int num_tokens;

// Buffer binario para el código máquina
uint8_t buffer_binario[2048];
int indice_binario = 0;

void emitir_byte(uint8_t b) {
    buffer_binario[indice_binario++] = b;
}

// Mapeo de registros IA-32 a sus 3 bits correspondientes
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

unsigned char build_sib(unsigned char scale, unsigned char index, unsigned char base) {
    return (scale << 6) | (index << 3) | base;
}

// Traductor de Opcodes y estructurador de formato máquina
void codificar_instruccion_dinamica() {
    if (num_tokens == 0) return;

    int i = 0;
    // Ignorar etiquetas en la segunda pasada
    if (tokens_linea[i].type == TOKEN_IDENTIFIER && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_COLON) {
        i = 2;
    }
    
    if (i >= num_tokens) return;

    if (tokens_linea[i].type == TOKEN_INSTRUCTION) {
        const char *mnemonico = tokens_linea[i].lexeme;

        // --- ARITMÉTICAS ---
        if (strcmp(mnemonico, "ADD") == 0) {
            if (tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_NUMBER) {
                emitir_byte(0x83); 
                unsigned char reg_code = get_register_code(tokens_linea[i+1].lexeme);
                emitir_byte(build_modrm(3, 0, reg_code)); // Mod=11, Ext=000
                emitir_byte((uint8_t)atoi(tokens_linea[i+3].lexeme)); 
            }
        }
        else if (strcmp(mnemonico, "SUB") == 0) {
            if (tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_REGISTER) {
                emitir_byte(0x2B); 
                unsigned char reg_dst = get_register_code(tokens_linea[i+1].lexeme);
                unsigned char reg_src = get_register_code(tokens_linea[i+3].lexeme);
                emitir_byte(build_modrm(3, reg_dst, reg_src)); 
            }
        }
        // --- SALTOS Y CONTROL ---
        else if (strcmp(mnemonico, "JMP") == 0 || strcmp(mnemonico, "JE") == 0 || strcmp(mnemonico, "CALL") == 0) {
            if (strcmp(mnemonico, "JMP") == 0) emitir_byte(0xE9);
            else if (strcmp(mnemonico, "CALL") == 0) emitir_byte(0xE8);
            else if (strcmp(mnemonico, "JE") == 0) { emitir_byte(0x0F); emitir_byte(0x84); }
            
            // Emitimos 4 bytes nulos (0x00) como placeholder para el linker
            emitir_byte(0x00); emitir_byte(0x00); emitir_byte(0x00); emitir_byte(0x00);
        }
        else if (strcmp(mnemonico, "RET") == 0) {
            emitir_byte(0xC3);
        }
        // --- TRANSFERENCIA DE DATOS Y SIB ---
        else if (strcmp(mnemonico, "MOV") == 0) {
            
            // MOV Registro, Inmediato (Ej. MOV EAX, 10)
            if (tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_NUMBER) {
                unsigned char reg_code = get_register_code(tokens_linea[i+1].lexeme);
                emitir_byte(0xB8 + reg_code);
                
                uint32_t inmm = (uint32_t)strtoul(tokens_linea[i+3].lexeme, NULL, 0);
                emitir_byte(inmm & 0xFF);
                emitir_byte((inmm >> 8) & 0xFF);
                emitir_byte((inmm >> 16) & 0xFF);
                emitir_byte((inmm >> 24) & 0xFF);
            }
            // MOV Registro, Registro (Ej. MOV EAX, EBX)
            else if (tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_REGISTER) {
                emitir_byte(0x89);
                unsigned char reg_dst = get_register_code(tokens_linea[i+1].lexeme);
                unsigned char reg_src = get_register_code(tokens_linea[i+3].lexeme);
                emitir_byte(build_modrm(3, reg_src, reg_dst)); 
            }
            // MOV Registro, [Memoria / SIB] (Ej. MOV EAX, [EBX+ECX*4+8])
            else if (tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_LBRACKET) {
                emitir_byte(0x8B); 
                
                unsigned char reg_dst = get_register_code(tokens_linea[i+1].lexeme);
                unsigned char base_code = 0, index_code = 0, scale_val = 0;
                uint32_t disp = 0;
                int tiene_disp = 0;

                int j = i + 4;
                while (j < num_tokens && tokens_linea[j].type != TOKEN_RBRACKET) {
                    if (tokens_linea[j].type == TOKEN_REGISTER) {
                        if (j + 1 < num_tokens && tokens_linea[j+1].type == TOKEN_STAR) {
                            index_code = get_register_code(tokens_linea[j].lexeme);
                            j++; 
                            if (j + 1 < num_tokens && tokens_linea[j+1].type == TOKEN_NUMBER) {
                                int s = atoi(tokens_linea[j+1].lexeme);
                                if (s == 2) scale_val = 1;
                                else if (s == 4) scale_val = 2;
                                else if (s == 8) scale_val = 3;
                                j++; 
                            }
                        } else {
                            base_code = get_register_code(tokens_linea[j].lexeme);
                        }
                    }
                    if (tokens_linea[j].type == TOKEN_PLUS && j + 1 < num_tokens && tokens_linea[j+1].type == TOKEN_NUMBER) {
                        disp = atoi(tokens_linea[j+1].lexeme);
                        tiene_disp = 1;
                        j++;
                    }
                    j++;
                }

                unsigned char mod = tiene_disp ? 1 : 0; // Disp8
                emitir_byte(build_modrm(mod, reg_dst, 4)); // rm=4 exige SIB
                emitir_byte(build_sib(scale_val, index_code, base_code));
                
                if (tiene_disp) {
                    emitir_byte(disp & 0xFF); 
                }
            }
        }
    }
}
