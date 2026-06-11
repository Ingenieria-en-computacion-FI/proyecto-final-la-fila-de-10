#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "estructuras.h"

extern Token tokens_linea[];
extern int num_tokens;

uint8_t buffer_binario[2048];
int indice_binario = 0;

void emitir_byte(uint8_t b) { buffer_binario[indice_binario++] = b; }

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

unsigned char build_modrm(unsigned char mod, unsigned char reg, unsigned char rm) { return (mod << 6) | (reg << 3) | rm; }
unsigned char build_sib(unsigned char scale, unsigned char index, unsigned char base) { return (scale << 6) | (index << 3) | base; }

void codificar_instruccion_dinamica() {
    if (num_tokens == 0) return;

    int i = 0;
    // Ignorar etiquetas en la segunda pasada
    if (tokens_linea[i].type == TOKEN_IDENTIFIER && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_COLON) i = 2;
    if (i >= num_tokens) return;

    if (tokens_linea[i].type == TOKEN_INSTRUCTION) {
        const char *mnemonico = tokens_linea[i].lexeme;

        // --- ARITMÉTICAS BÁSICAS ---
        if (strcmp(mnemonico, "ADD") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_NUMBER) {
            emitir_byte(0x83); 
            emitir_byte(build_modrm(3, 0, get_register_code(tokens_linea[i+1].lexeme)));
            emitir_byte((uint8_t)atoi(tokens_linea[i+3].lexeme)); 
        }
        else if (strcmp(mnemonico, "SUB") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_REGISTER) {
            emitir_byte(0x2B); 
            emitir_byte(build_modrm(3, get_register_code(tokens_linea[i+1].lexeme), get_register_code(tokens_linea[i+3].lexeme))); 
        }
        
        // --- ARITMÉTICAS COMPLEJAS Y UNARIAS (Grupo F7) ---
        else if ((strcmp(mnemonico, "NOT") == 0 || strcmp(mnemonico, "NEG") == 0 ||
                  strcmp(mnemonico, "MUL") == 0 || strcmp(mnemonico, "DIV") == 0 ||
                  strcmp(mnemonico, "IMUL") == 0 || strcmp(mnemonico, "IDIV") == 0) &&
                 tokens_linea[i+1].type == TOKEN_REGISTER) {
            
            emitir_byte(0xF7); // Opcode base para operaciones unarias r/m32
            unsigned char reg_dst = get_register_code(tokens_linea[i+1].lexeme);
            unsigned char extension = 0;
            
            if (strcmp(mnemonico, "NOT") == 0) extension = 2;
            else if (strcmp(mnemonico, "NEG") == 0) extension = 3;
            else if (strcmp(mnemonico, "MUL") == 0) extension = 4;
            else if (strcmp(mnemonico, "IMUL") == 0) extension = 5;
            else if (strcmp(mnemonico, "DIV") == 0) extension = 6;
            else if (strcmp(mnemonico, "IDIV") == 0) extension = 7;
            
            emitir_byte(build_modrm(3, extension, reg_dst)); // Mod=11 (Registro)
        }
        
        // --- LÓGICAS Y COMPARACIONES ---
        else if ((strcmp(mnemonico, "CMP") == 0 || strcmp(mnemonico, "AND") == 0 || strcmp(mnemonico, "OR") == 0 || strcmp(mnemonico, "XOR") == 0) &&
                 tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_REGISTER) {
            if (strcmp(mnemonico, "CMP") == 0) emitir_byte(0x39);
            else if (strcmp(mnemonico, "AND") == 0) emitir_byte(0x21);
            else if (strcmp(mnemonico, "OR") == 0) emitir_byte(0x09);
            else if (strcmp(mnemonico, "XOR") == 0) emitir_byte(0x31);
            emitir_byte(build_modrm(3, get_register_code(tokens_linea[i+3].lexeme), get_register_code(tokens_linea[i+1].lexeme))); 
        }

        // --- OPERACIONES DE PILA Y SISTEMA ---
        else if (strcmp(mnemonico, "PUSH") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER) {
            emitir_byte(0x50 + get_register_code(tokens_linea[i+1].lexeme)); 
        }
        else if (strcmp(mnemonico, "POP") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER) {
            emitir_byte(0x58 + get_register_code(tokens_linea[i+1].lexeme)); 
        }
        else if (strcmp(mnemonico, "INC") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER) {
            emitir_byte(0x40 + get_register_code(tokens_linea[i+1].lexeme)); 
        }
        else if (strcmp(mnemonico, "DEC") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER) {
            emitir_byte(0x48 + get_register_code(tokens_linea[i+1].lexeme)); 
        }
        else if (strcmp(mnemonico, "NOP") == 0) { emitir_byte(0x90); }
        else if (strcmp(mnemonico, "INT") == 0 && tokens_linea[i+1].type == TOKEN_NUMBER) {
            emitir_byte(0xCD); 
            emitir_byte((uint8_t)strtoul(tokens_linea[i+1].lexeme, NULL, 0)); 
        }

        // --- SALTOS Y CONTROL ---
        else if (strcmp(mnemonico, "JMP") == 0 || mnemonico[0] == 'J' || strcmp(mnemonico, "CALL") == 0) {
            if (strcmp(mnemonico, "JMP") == 0) emitir_byte(0xE9);
            else if (strcmp(mnemonico, "CALL") == 0) emitir_byte(0xE8);
            else {
                emitir_byte(0x0F); // Prefijo para saltos condicionales de 32 bits
                if (strcmp(mnemonico, "JE") == 0) emitir_byte(0x84);
                else if (strcmp(mnemonico, "JNE") == 0) emitir_byte(0x85);
                else if (strcmp(mnemonico, "JL") == 0) emitir_byte(0x8C);
                else if (strcmp(mnemonico, "JGE") == 0) emitir_byte(0x8D);
                else if (strcmp(mnemonico, "JLE") == 0) emitir_byte(0x8E);
                else if (strcmp(mnemonico, "JG") == 0) emitir_byte(0x8F);
            }
            emitir_byte(0x00); emitir_byte(0x00); emitir_byte(0x00); emitir_byte(0x00); // Espacio para relocalización
        }
        else if (strcmp(mnemonico, "RET") == 0) { emitir_byte(0xC3); }

        // --- TRANSFERENCIA DE DATOS (MOV) Y CÁLCULO DE DIRECCIONES (LEA) ---
        else if (strcmp(mnemonico, "MOV") == 0 || strcmp(mnemonico, "LEA") == 0) {
            
            // Caso: Instrucción Registro, Inmediato (Ej. MOV EAX, 10)
            if (strcmp(mnemonico, "MOV") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_NUMBER) {
                emitir_byte(0xB8 + get_register_code(tokens_linea[i+1].lexeme));
                uint32_t inmm = (uint32_t)strtoul(tokens_linea[i+3].lexeme, NULL, 0);
                emitir_byte(inmm & 0xFF); emitir_byte((inmm >> 8) & 0xFF); emitir_byte((inmm >> 16) & 0xFF); emitir_byte((inmm >> 24) & 0xFF);
            }
            // Caso: Instrucción Registro, Registro (Ej. MOV EAX, EBX)
            else if (strcmp(mnemonico, "MOV") == 0 && tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_REGISTER) {
                emitir_byte(0x89);
                emitir_byte(build_modrm(3, get_register_code(tokens_linea[i+3].lexeme), get_register_code(tokens_linea[i+1].lexeme))); 
            }
            // Caso complejo con ModRM y SIB (Ej. MOV EAX, [EBX+ECX*4+8] o LEA EAX, [EBX+ECX*4])
            else if (tokens_linea[i+1].type == TOKEN_REGISTER && tokens_linea[i+3].type == TOKEN_LBRACKET) {
                
                if (strcmp(mnemonico, "MOV") == 0) emitir_byte(0x8B); 
                else if (strcmp(mnemonico, "LEA") == 0) emitir_byte(0x8D);
                
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
                                if (s == 2) scale_val = 1; else if (s == 4) scale_val = 2; else if (s == 8) scale_val = 3;
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
                unsigned char mod = tiene_disp ? 1 : 0; 
                emitir_byte(build_modrm(mod, reg_dst, 4)); 
                emitir_byte(build_sib(scale_val, index_code, base_code));
                if (tiene_disp) emitir_byte(disp & 0xFF); 
            }
        }
    }
}
