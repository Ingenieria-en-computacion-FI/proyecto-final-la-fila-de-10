#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estructuras.h"

#define MAX_SIMBOLOS 100

// Nuestra Tabla de Símbolos en memoria global
Simbolo tabla_simbolos[MAX_SIMBOLOS];
int contador_simbolos = 0;

// Contador de Programa (PC) que rastrea la dirección actual en bytes
uint32_t posicion_memoria_actual = 0;

// Función para registrar una etiqueta en la Tabla de Símbolos
void agregar_simbolo(const char *nombre, uint32_t direccion) {
    // Evitar duplicados
    for (int i = 0; i < contador_simbolos; i++) {
        if (strcmp(tabla_simbolos[i].nombre, nombre) == 0) return;
    }

    if (contador_simbolos < MAX_SIMBOLOS) {
        // Copiar el nombre limpiando el ':' al final si lo tiene
        char nombre_limpio[64];
        strcpy(nombre_limpio, nombre);
        int len = strlen(nombre_limpio);
        if (nombre_limpio[len - 1] == ':') {
            nombre_limpio[len - 1] = '\0';
        }

        strcpy(tabla_simbolos[contador_simbolos].nombre, nombre_limpio);
        tabla_simbolos[contador_simbolos].direccion = direccion;
        tabla_simbolos[contador_simbolos].es_global = 0; // Por defecto local
        contador_simbolos++;
    }
}

// Simula la primera pasada: analiza el tamaño de la instrucción y calcula direcciones
void procesar_instruccion_para_tabla(const char *mnemonico, const char *operandos) {
    // Si es una etiqueta (termina con ':'), la registramos con la dirección actual
    if (mnemonico[strlen(mnemonico) - 1] == ':') {
        agregar_simbolo(mnemonico, posicion_memoria_actual);
        return;
    }

    // Aritmética matemática de bytes para IA-32:
    // Calculamos cuánto espacio ocupará la instrucción en el binario final
    if (strcmp(mnemonico, "MOV") == 0) {
        // Un MOV inmediato a registro suele ocupar 5 bytes (1 byte opcode + 4 bytes inmediato)
        // Un MOV con direccionamiento SIB complejo ocupa cerca de 7 bytes
        if (operandos != NULL && strchr(operandos, '[') != NULL) {
            posicion_memoria_actual += 7; // Simulación de instrucción compleja SIB
        } else {
            posicion_memoria_actual += 5; // MOV EAX, 10
        }
    } 
    else if (strcmp(mnemonico, "JMP") == 0) {
        posicion_memoria_actual += 5; // JMP relativo cercano/absoluto ocupa 5 bytes
    } 
    else if (strcmp(mnemonico, "ADD") == 0) {
        posicion_memoria_actual += 3; // ADD registro, inmediato chico
    } 
    else if (strcmp(mnemonico, "RET") == 0) {
        posicion_memoria_actual += 1; // Opcode 0xC3 ocupa exactamente 1 byte
    }
}

// Imprime los resultados acumulados de la Tabla de Símbolos
void imprimir_tabla_simbolos() {
    printf("\n[EXITO] Tabla de Simbolos Generada en C:\n");
    for (int i = 0; i < contador_simbolos; i++) {
        printf("  -> Etiqueta '%s' en la direccion %d (Sección .TEXT)\n", 
               tabla_simbolos[i].nombre, tabla_simbolos[i].direccion);
    }
    printf("\nTamano total estimado de la seccion .text: %d bytes\n", posicion_memoria_actual);
}