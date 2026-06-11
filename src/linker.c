#include <stdio.h>
#include <stdint.h>
#include "estructuras.h"

extern uint8_t buffer_binario[];
extern int indice_binario;
extern Symbol tabla_simbolos[];
extern int contador_simbolos;

// Estructura clásica de un archivo objeto simple
typedef struct {
    char firma[4];          // "OBJ1"
    int tamano_texto;       // Tamaño del código máquina
    int num_simbolos;       // Cantidad de etiquetas exportadas
} CabeceraObjeto;

void generar_archivo_objeto(const char *nombre_archivo) {
    FILE *out = fopen(nombre_archivo, "wb");
    if (!out) {
        printf("[ERROR] No se pudo crear el archivo objeto.\n");
        return;
    }

    // 1. Escribir Cabecera
    CabeceraObjeto cabecera = { {'O', 'B', 'J', '1'}, indice_binario, contador_simbolos };
    fwrite(&cabecera, sizeof(CabeceraObjeto), 1, out);

    // 2. Escribir la sección de código máquina (.text)
    fwrite(buffer_binario, sizeof(uint8_t), indice_binario, out);

    // 3. Escribir Tabla de Símbolos para que el Linker final la resuelva
    fwrite(tabla_simbolos, sizeof(Symbol), contador_simbolos, out);

    fclose(out);
    printf("[EXITO] Archivo objeto '%s' generado correctamente (%d bytes de texto).\n", nombre_archivo, indice_binario);
}
