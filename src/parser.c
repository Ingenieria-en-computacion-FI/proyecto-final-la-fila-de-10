#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estructuras.h"

#define MAX_SIMBOLOS 100

// Tabla de Símbolos y Contador de Programa global
Symbol tabla_simbolos[MAX_SIMBOLOS];
int contador_simbolos = 0;
uint32_t pc_actual = 0; 

// Variables externas alimentadas por el Lexer
extern Token tokens_linea[];
extern int num_tokens;

// Registra una etiqueta y su dirección en memoria
void agregar_simbolo(const char *nombre, int direccion) {
    // Evitar duplicados
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

// Analizador principal de la Primera Pasada
void calcular_tamano_instruccion() {
    if (num_tokens == 0) return;

    int i = 0;

    // 1. Detección de Etiquetas
    if (tokens_linea[i].type == TOKEN_IDENTIFIER && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_COLON) {
        agregar_simbolo(tokens_linea[i].lexeme, pc_actual);
        i += 2;
        if (i >= num_tokens) return; // Si la línea solo tenía la etiqueta, terminamos
    }

    // 2. Manejo de Directivas de Memoria (.data / .bss)
    if (tokens_linea[i].type == TOKEN_IDENTIFIER || tokens_linea[i].type == TOKEN_DIRECTIVE) {
        const char *mnemonico = tokens_linea[i].lexeme;
        
        if (strcmp(mnemonico, "DB") == 0) { pc_actual += 1; return; }
        if (strcmp(mnemonico, "DW") == 0) { pc_actual += 2; return; }
        if (strcmp(mnemonico, "DD") == 0) { pc_actual += 4; return; }
        
        if (strcmp(mnemonico, "RESB") == 0 && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_NUMBER) {
            pc_actual += atoi(tokens_linea[i+1].lexeme);
            return;
        }
        if (strcmp(mnemonico, "RESD") == 0 && i + 1 < num_tokens && tokens_linea[i+1].type == TOKEN_NUMBER) {
            pc_actual += (atoi(tokens_linea[i+1].lexeme) * 4);
            return;
        }
    }

    // 3. Cálculo de tamaño para Instrucciones (.text)
    if (tokens_linea[i].type == TOKEN_INSTRUCTION) {
        const char *mnemonico = tokens_linea[i].lexeme;
        int tamano_base = 1; // La mayoría tiene 1 byte de Opcode
        int requiere_modrm = 0;
        int requiere_sib = 0;
        int tamano_inmediato = 0;
        int tamano_desplazamiento = 0;

        // Casos especiales fijos
        if (strcmp(mnemonico, "RET") == 0) { pc_actual += 1; return; }
        if (strcmp(mnemonico, "JMP") == 0 || strcmp(mnemonico, "CALL") == 0) { pc_actual += 5; return; }
        if (strcmp(mnemonico, "JE") == 0 || strcmp(mnemonico, "JNE") == 0) { pc_actual += 6; return; } // Opcode de 2 bytes + 32-bit relativo

        i++; // Avanzamos a los operandos

        // Análisis heurístico de operandos
        while (i < num_tokens) {
            if (tokens_linea[i].type == TOKEN_REGISTER) {
                requiere_modrm = 1; 
            } 
            else if (tokens_linea[i].type == TOKEN_NUMBER) {
                tamano_inmediato = 4; // Asumimos inmediatos de 32 bits
            }
            else if (tokens_linea[i].type == TOKEN_LBRACKET) {
                requiere_modrm = 1;
                // Escanear SIB y Desplazamientos dentro de los corchetes
                int j = i;
                while (j < num_tokens && tokens_linea[j].type != TOKEN_RBRACKET) {
                    if (tokens_linea[j].type == TOKEN_STAR) requiere_sib = 1;
                    if (tokens_linea[j].type == TOKEN_NUMBER) tamano_desplazamiento = 1; // Asumimos Disp8 básico
                    j++;
                }
                i = j; 
            }
            i++;
        }

        // Sumatoria y actualización del PC
        pc_actual += tamano_base + requiere_modrm + requiere_sib + tamano_inmediato + tamano_desplazamiento;
    }
}

// Función auxiliar para depuración
void imprimir_tabla_simbolos() {
    printf("\n--- TABLA DE SIMBOLOS ---\n");
    for (int i = 0; i < contador_simbolos; i++) {
        printf(" [%s] -> Offset: 0x%04X\n", tabla_simbolos[i].name, tabla_simbolos[i].address);
    }
    printf("Tamano total calculado: %d bytes\n", pc_actual);
}
