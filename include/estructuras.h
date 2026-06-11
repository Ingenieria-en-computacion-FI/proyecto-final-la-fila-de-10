#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdint.h>

// 1. Tipos de Tokens exigidos por la rúbrica
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_REGISTER,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_LBRACKET,   // '['
    TOKEN_RBRACKET,   // ']'
    TOKEN_PLUS,       // '+'
    TOKEN_STAR,       // '*'
    TOKEN_NEWLINE,
    TOKEN_EOF,
    // Tipos extra útiles para tu clasificación
    TOKEN_DIRECTIVE,
    TOKEN_INSTRUCTION,
    TOKEN_UNKNOWN
} TokenType;

// 2. Ficha de Token según la sugerencia inicial
typedef struct {
    TokenType type;
    char lexeme[64];
} Token;

// 3. Tabla de Símbolos sugerida
typedef struct {
    char name[64];
    int address;    // Dirección en memoria (offset)
    int defined;    // 1 si ya fue definida, 0 si es referencia adelantada
} Symbol;

#endif
