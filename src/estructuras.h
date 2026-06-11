#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdint.h>

// 1. Tipos de Tokens que el Lexer va a identificar
typedef enum {
    TOKEN_DIRECTIVA,   // ej. .text, .data
    TOKEN_INSTRUCCION, // ej. MOV, ADD, JMP
    TOKEN_REGISTRO,    // ej. EAX, EBX
    TOKEN_INMEDIATO,   // ej. 10, 0x0A
    TOKEN_ETIQUETA,    // ej. main:
    TOKEN_SIMBOLO,     // ej. [, ], +, *
    TOKEN_DESCONOCIDO
} TipoToken;

// 2. Estructura de un Token individual
typedef struct {
    TipoToken tipo;
    char valor[32]; // Guardamos el texto del token (ej. "EAX")
} Token;

// 3. Estructura para la Tabla de Símbolos (Primera Pasada)
typedef struct {
    char nombre[64];      // Nombre de la etiqueta (ej. "main")
    uint32_t direccion;   // Dirección en memoria (offset)
    int es_global;        // 1 si es GLOBAL, 0 si es local
} Simbolo;

#endif