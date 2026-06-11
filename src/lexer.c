#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"

#define MAX_TOKENS 100

// Arreglo global temporal para almacenar los tokens de la línea actual
Token tokens_linea[MAX_TOKENS];
int num_tokens = 0;

// Función auxiliar para identificar si un string es un registro IA-32
int es_registro(const char *str) {
    const char *registros[] = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"};
    for (int i = 0; i < 8; i++) {
        if (strcmp(str, registros[i]) == 0) return 1;
    }
    return 0;
}

// Función auxiliar para identificar instrucciones básicas (expandible según rúbrica)
int es_instruccion(const char *str) {
    const char *instrucciones[] = {
        "MOV", "PUSH", "POP", "LEA", "ADD", "SUB", "INC", "DEC", 
        "CMP", "NEG", "MUL", "DIV", "JMP", "RET", "CALL"
    };
    int total_instrucciones = sizeof(instrucciones) / sizeof(instrucciones[0]);
    
    for (int i = 0; i < total_instrucciones; i++) {
        if (strcmp(str, instrucciones[i]) == 0) return 1;
    }
    return 0;
}

// Guarda un token recién descubierto en nuestro arreglo
void agregar_token(TokenType tipo, const char *lexema) {
    if (num_tokens < MAX_TOKENS) {
        tokens_linea[num_tokens].type = tipo;
        strncpy(tokens_linea[num_tokens].lexeme, lexema, 63);
        tokens_linea[num_tokens].lexeme[63] = '\0'; // Seguridad
        num_tokens++;
    }
}

// El Lexer principal: Analiza carácter por carácter
void tokenizar_linea_dinamica(const char *linea) {
    num_tokens = 0; // Reiniciar contador para cada nueva línea
    int i = 0;
    int len = strlen(linea);

    while (i < len) {
        char c = linea[i];

        // 1. Ignorar espacios, tabulaciones y saltos de línea
        if (isspace(c)) {
            i++;
            continue;
        }

        // 2. Ignorar comentarios (corta el análisis del resto de la línea)
        if (c == ';') {
            break;
        }

        // 3. Detectar Símbolos Especiales (Esenciales para ModRM y SIB)
        if (c == ',') { agregar_token(TOKEN_COMMA, ","); i++; continue; }
        if (c == ':') { agregar_token(TOKEN_COLON, ":"); i++; continue; }
        if (c == '[') { agregar_token(TOKEN_LBRACKET, "["); i++; continue; }
        if (c == ']') { agregar_token(TOKEN_RBRACKET, "]"); i++; continue; }
        if (c == '+') { agregar_token(TOKEN_PLUS, "+"); i++; continue; }
        if (c == '*') { agregar_token(TOKEN_STAR, "*"); i++; continue; }

        // 4. Detectar Directivas (empiezan con '.')
        if (c == '.') {
            char buffer[64] = {0};
            int j = 0;
            buffer[j++] = c;
            i++;
            while (i < len && isalpha(linea[i]) && j < 63) {
                buffer[j++] = linea[i++];
            }
            agregar_token(TOKEN_DIRECTIVE, buffer);
            continue;
        }

        // 5. Detectar Números (Soporta base 10 y base 16 estilo 0x)
        if (isdigit(c)) {
            char buffer[64] = {0};
            int j = 0;
            // Extrae mientras sea número, o letra hexadecimal (A-F, x)
            while (i < len && (isalnum(linea[i])) && j < 63) {
                buffer[j++] = linea[i++];
            }
            agregar_token(TOKEN_NUMBER, buffer);
            continue;
        }

        // 6. Detectar Identificadores (Instrucciones, Registros o Etiquetas)
        if (isalpha(c) || c == '_') {
            char buffer[64] = {0};
            int j = 0;
            while (i < len && (isalnum(linea[i]) || linea[i] == '_') && j < 63) {
                buffer[j++] = toupper(linea[i++]); // Forzamos mayúsculas para unificar
            }
            
            // Clasificación del texto extraído
            if (es_registro(buffer)) {
                agregar_token(TOKEN_REGISTER, buffer);
            } else if (es_instruccion(buffer)) {
                agregar_token(TOKEN_INSTRUCTION, buffer);
            } else {
                agregar_token(TOKEN_IDENTIFIER, buffer); // Si no es instruccion ni registro, es etiqueta/variable
            }
            continue;
        }

        // 7. Manejo de Errores Lógicos (Caracteres extraños)
        char buffer_error[2] = {c, '\0'};
        agregar_token(TOKEN_UNKNOWN, buffer_error);
        i++;
    }

    // --- Módulo de Debug: Visualización temporal en consola ---
    if (num_tokens > 0) {
        printf("Tokens detectados:\n");
        for (int k = 0; k < num_tokens; k++) {
            printf("  -> Tipo: %2d | Lexema: '%s'\n", tokens_linea[k].type, tokens_linea[k].lexeme);
        }
        printf("--------------------------------------\n");
    }
}
