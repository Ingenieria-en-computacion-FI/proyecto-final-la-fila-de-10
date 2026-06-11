#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estructuras.h"

#define MAX_SIMBOLOS 100

Symbol tabla_simbolos[MAX_SIMBOLOS];
int contador_simbolos = 0;
uint32_t pc_actual = 0; 

extern Token tokens_linea[];
extern int num_tokens;

void agregar_simbolo(const char *nombre, int direccion) {
    for (int i = 0; i < contador_simbolos; i++) {
        if (strcmp(tabla_simbolos[i].name, nombre) == 0) return;
    }
    if (contador_simbolos < MAX_SIMBOLOS) {
        strcpy(tabla_simbolos[contador_simbolos].name, nombre);
        tabla_simbolos[contador_simbolos].address = direccion;
        tabla_simbolos[contador_simbolos].defined = 1;
        contador_simbolos++;
    }
}

void calcular_tamano_instruccion() {
    if (num_tokens == 0) return;

    int i = 0;

    // 1. Detección de Etiquetas
    if (tokens_linea[i].type == TOKEN_IDENTIFIER && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_COLON) {
        agregar_simbolo(tokens_linea[i].lexeme, pc_actual);
        i += 2;
        if (i >= num_tokens) return;
    }

    // 2. Manejo de Directivas de Memoria y Control
    if (tokens_linea[i].type == TOKEN_IDENTIFIER || tokens_linea[i].type == TOKEN_DIRECTIVE) {
        const char *mnemonico = tokens_linea[i].lexeme;
        
        if (strcmp(mnemonico, "DB") == 0) { pc_actual += 1; return; }
        if (strcmp(mnemonico, "DW") == 0) { pc_actual += 2; return; }
        if (strcmp(mnemonico, "DD") == 0) { pc_actual += 4; return; }
        
        if (strcmp(mnemonico, "RESB") == 0 && i + 1 < num_tokens) {
            pc_actual += atoi(tokens_linea[i+1].lexeme);
            return;
        }
        if (strcmp(mnemonico, "RESW") == 0 && i + 1 < num_tokens) {
            pc_actual += (atoi(tokens_linea[i+1].lexeme) * 2);
            return;
        }
        if (strcmp(mnemonico, "RESD") == 0 && i + 1 < num_tokens) {
            pc_actual += (atoi(tokens_linea[i+1].lexeme) * 4);
            return;
        }
        if (strcmp(mnemonico, "ORG") == 0 && i + 1 < num_tokens) {
            pc_actual = (uint32_t)strtoul(tokens_linea[i+1].lexeme, NULL, 0);
            return;
        }
        if (strcmp(mnemonico, "GLOBAL") == 0 || strcmp(mnemonico, "EXTERN") == 0) {
            return; // No consumen espacio
        }
    }

    // 3. Cálculo para Instrucciones (.text)
    if (tokens_linea[i].type == TOKEN_INSTRUCTION) {
        const char *mnemonico = tokens_linea[i].lexeme;
        int tamano_base = 1; 
        int requiere_modrm = 0;
        int requiere_sib = 0;
        int tamano_inmediato = 0;
        int tamano_desplazamiento = 0;

        // Tamaños fijos para saltos y sin operandos
        if (strcmp(mnemonico, "RET") == 0 || strcmp(mnemonico, "NOP") == 0) { pc_actual += 1; return; }
        if (strcmp(mnemonico, "JMP") == 0 || strcmp(mnemonico, "CALL") == 0) { pc_actual += 5; return; }
        if (mnemonico[0] == 'J' && strcmp(mnemonico, "JMP") != 0) { pc_actual += 6; return; } // Saltos condicionales (JE, JNE...)
        if (strcmp(mnemonico, "INT") == 0) { pc_actual += 2; return; }

        i++; 

        // Análisis de operandos dinámicos
        while (i < num_tokens) {
            if (tokens_linea[i].type == TOKEN_REGISTER) {
                requiere_modrm = 1; 
            } 
            else if (tokens_linea[i].type == TOKEN_NUMBER) {
                tamano_inmediato = 4; 
            }
            else if (tokens_linea[i].type == TOKEN_LBRACKET) {
                requiere_modrm = 1;
                int j = i;
                while (j < num_tokens && tokens_linea[j].type != TOKEN_RBRACKET) {
                    if (tokens_linea[j].type == TOKEN_STAR) requiere_sib = 1;
                    if (tokens_linea[j].type == TOKEN_NUMBER) tamano_desplazamiento = 1; 
                    j++;
                }
                i = j; 
            }
            i++;
        }

        pc_actual += tamano_base + requiere_modrm + requiere_sib + tamano_inmediato + tamano_desplazamiento;
    }
}
