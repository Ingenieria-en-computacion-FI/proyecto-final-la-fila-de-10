#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"

// Función auxiliar para limpiar espacios en blanco al inicio y final
void limpiar_espacios(char *str) {
    char *p = str;
    int l = strlen(p);
    while (isspace(p[l - 1])) p[--l] = 0;
    while (*p && isspace(*p)) ++p, --l;
    memmove(str, p, l + 1);
}

// El Lexer que analiza una línea y extrae sus componentes
void tokenizar_linea(char *linea) {
    // Ignorar comentarios
    char *comentario = strchr(linea, ';');
    if (comentario != NULL) {
        *comentario = '\0'; // Cortamos la cadena donde empieza el comentario
    }

    limpiar_espacios(linea);
    if (strlen(linea) == 0) return; // Línea vacía

    printf("\n[Lexer] Analizando: \"%s\"\n", linea);

    // Separadores comunes en ensamblador: espacios, tabuladores y comas
    char *delimitadores = " \t,";
    char *token_texto = strtok(linea, delimitadores);

    while (token_texto != NULL) {
        limpiar_espacios(token_texto);
        
        if (strlen(token_texto) > 0) {
            Token t;
            strcpy(t.valor, token_texto);

            // Clasificación básica de los Tokens
            if (token_texto[0] == '.') {
                t.tipo = TOKEN_DIRECTIVA;
                printf("  -> TOKEN: DIRECTIVA | Valor: %s\n", t.valor);
            } 
            else if (strcmp(token_texto, "MOV") == 0 || strcmp(token_texto, "ADD") == 0 || strcmp(token_texto, "JMP") == 0 || strcmp(token_texto, "RET") == 0) {
                t.tipo = TOKEN_INSTRUCCION;
                printf("  -> TOKEN: INSTRUCCION | Valor: %s\n", t.valor);
            } 
            else if (strcmp(token_texto, "EAX") == 0 || strcmp(token_texto, "EBX") == 0 || strcmp(token_texto, "ECX") == 0) {
                t.tipo = TOKEN_REGISTRO;
                printf("  -> TOKEN: REGISTRO | Valor: %s\n", t.valor);
            } 
            else if (token_texto[strlen(token_texto) - 1] == ':') {
                t.tipo = TOKEN_ETIQUETA;
                printf("  -> TOKEN: ETIQUETA | Valor: %s\n", t.valor);
            } 
            else if (isdigit(token_texto[0])) {
                t.tipo = TOKEN_INMEDIATO;
                printf("  -> TOKEN: INMEDIATO | Valor: %s\n", t.valor);
            } 
            else {
                t.tipo = TOKEN_DESCONOCIDO;
                printf("  -> TOKEN: REFERENCIA/MEMORIA | Valor: %s\n", t.valor);
            }
        }
        
        // Obtener el siguiente fragmento de la línea
        token_texto = strtok(NULL, delimitadores);
    }
}