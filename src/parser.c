#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estructuras.h"

#define MAX_SIMBOLOS 100

Symbol tabla_simbolos[MAX_SIMBOLOS];
int contador_simbolos = 0;
uint32_t pc_actual = 0; // Program Counter

// Variables externas que vienen del Lexer
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

// Analizador heurístico para determinar el tamaño de la instrucción
void calcular_tamano_instruccion() {
    if (num_tokens == 0) return;

    int i = 0;

    // Detectar si la línea es una etiqueta
    if (tokens_linea[i].type == TOKEN_IDENTIFIER && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_COLON) {
        agregar_simbolo(tokens_linea[i].lexeme, pc_actual);
        i += 2;
        if (i >= num_tokens) return; // Línea que solo tenía la etiqueta
    }

    if (tokens_linea[i].type == TOKEN_INSTRUCTION) {
        const char *mnemonico = tokens_linea[i].lexeme;
        int tamano_base = 1; // La mayoría tiene 1 byte de Opcode
        int requiere_modrm = 0;
        int requiere_sib = 0;
        int tamano_inmediato = 0;
        int tamano_desplazamiento = 0;

        i++; // Avanzamos a los operandos

        // Análisis de operandos
        while (i < num_tokens) {
            if (tokens_linea[i].type == TOKEN_REGISTER) {
                requiere_modrm = 1; 
            } 
            else if (tokens_linea[i].type == TOKEN_NUMBER) {
                // Si hay un número suelto (no SIB), es un inmediato (32 bits = 4 bytes)
                tamano_inmediato = 4; 
            }
            else if (tokens_linea[i].type == TOKEN_LBRACKET) {
                // Modo direccionamiento a memoria detectado
                requiere_modrm = 1;
                // Escanear dentro de los corchetes buscando multiplicadores para SIB
                int j = i;
                while (j < num_tokens && tokens_linea[j].type != TOKEN_RBRACKET) {
                    if (tokens_linea[j].type == TOKEN_STAR) requiere_sib = 1;
                    if (tokens_linea[j].type == TOKEN_NUMBER) tamano_desplazamiento = 1; // Disp8 básico
                    j++;
                }
                i = j; // Saltar todo el contenido de los corchetes
            }
            i++;
        }

        // Casos especiales (Saltos y retornos)
        if (strcmp(mnemonico, "RET") == 0) {
            pc_actual += 1;
            return;
        }
        if (strcmp(mnemonico, "JMP") == 0 || strcmp(mnemonico, "CALL") == 0) {
            pc_actual += 5; // Opcode + 32-bit relativo
            return;
        }

        // Sumatoria final del tamaño dinámico
        pc_actual += tamano_base + requiere_modrm + requiere_sib + tamano_inmediato + tamano_desplazamiento;
    }
}
