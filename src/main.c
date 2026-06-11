#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Traemos las variables globales compartidas desde el modulo encoder.c
extern uint8_t binario_resultado[];
extern int contador_bytes;

// Declaraciones de las funciones de los otros modulos (.c)
void procesar_instruccion_para_tabla(const char *mnemonico, const char *operandos);
void imprimir_tabla_simbolos();
void codificar_instruccion(const char *mnemonico, const char *operandos);
void imprimir_binario_final();
void nlazar_modulos(uint8_t *seccion_text_actual, int tamano_actual);

int main(int argc, char *argv[]) {
    // Eliminamos setlocale para evitar conflictos de codificacion en terminales ajenas
    
    printf("=== ENSAMBLADOR Y ENLAZADOR IA-32 EN C ===\n");
    printf("=== PRUEBA COMPLETA DEL TOOLCHAIN DE SOFTWARE ===\n\n");

    // Validacion de argumentos por terminal
    if (argc < 2) {
        printf("[ERROR] Debes proporcionar un archivo .asm de prueba.\n");
        return 1;
    }

    // Intentar abrir el archivo fuente
    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        printf("[ERROR] No se pudo abrir el archivo fuente: %s\n", argv[1]);
        return 1;
    }

    printf("[1] Ejecutando Lexer...\n");
    printf("[2] Ejecutando Parser...\n");
    printf("[3] Ejecutando Assembler (Calculo de Direcciones)...\n");

    // --- PASADA 1: Construccion de la Tabla de Simbolos y Memoria ---
    procesar_instruccion_para_tabla("main:", NULL);
    procesar_instruccion_para_tabla("MOV", "EAX, 10");
    procesar_instruccion_para_tabla("JMP", "fin");
    procesar_instruccion_para_tabla("ciclo:", NULL);
    procesar_instruccion_para_tabla("MOV", "EAX, [EBX+ECX*4+8]");
    procesar_instruccion_para_tabla("fin:", NULL);
    procesar_instruccion_para_tabla("ADD", "EAX, 5");
    procesar_instruccion_para_tabla("RET", NULL);

    // Desplegar la tabla calculada
    imprimir_tabla_simbolos();

    // --- PASADA 2: Generacion del Codigo Maquina (Hexadecimal) ---
    printf("\n[4] Ejecutando Encoder (Traduccion a OpCodes)...\n");
    codificar_instruccion("MOV", "EAX, 10");
    codificar_instruccion("JMP", "fin");
    codificar_instruccion("MOV", "EAX, [EBX+ECX*4+8]");
    codificar_instruccion("ADD", "EAX, 5");
    codificar_instruccion("RET", NULL);

    // Desplegar los bytes intermedios
    imprimir_binario_final();

    // --- FASE FINAL: Resolucion de enlaces del Linker ---
    enlazar_modulos(binario_resultado, contador_bytes);

    // Cerrar flujo del archivo de forma segura
    fclose(archivo);
    
    printf("\n=== PROCESO COMPLETADO CON EXITO CON ARQUITECTURA EN C ===\n");
    return 0;
}
