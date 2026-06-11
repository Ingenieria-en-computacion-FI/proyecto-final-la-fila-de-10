#include <stdio.h>
#include <stdint.h>
#include "estructuras.h"

extern uint8_t buffer_binario[];
extern int indice_binario;
extern Symbol tabla_simbolos[];
extern int contador_simbolos;

typedef struct {
    char firma[4];          
    int tamano_texto;       
    int num_simbolos;       
} CabeceraObjeto;

void generar_archivo_objeto(const char *nombre_archivo) {
    FILE *out = fopen(nombre_archivo, "wb");
    if (!out) {
        printf("[ERROR] No se pudo crear el archivo objeto.\n");
        return;
    }

    CabeceraObjeto cabecera = { {'O', 'B', 'J', '1'}, indice_binario, contador_simbolos };
    fwrite(&cabecera, sizeof(CabeceraObjeto), 1, out);
    fwrite(buffer_binario, sizeof(uint8_t), indice_binario, out);
    fwrite(tabla_simbolos, sizeof(Symbol), contador_simbolos, out);

    fclose(out);
    printf("[EXITO] Archivo objeto '%s' generado correctamente (%d bytes de texto).\n", nombre_archivo, indice_binario);
}

void generar_binario_final(const char *archivo_obj, const char *archivo_bin) {
    FILE *in = fopen(archivo_obj, "rb");
    FILE *out = fopen(archivo_bin, "wb");
    
    if (!in || !out) {
        printf("[ERROR DEL LINKER] Fallo al manipular archivos finales.\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    CabeceraObjeto cabecera;
    fread(&cabecera, sizeof(CabeceraObjeto), 1, in);

    uint8_t temp_text[2048];
    fread(temp_text, sizeof(uint8_t), cabecera.tamano_texto, in);

    Symbol temp_simbolos[100];
    fread(temp_simbolos, sizeof(Symbol), cabecera.num_simbolos, in);

    fwrite(temp_text, sizeof(uint8_t), cabecera.tamano_texto, out);

    fclose(in);
    fclose(out);
    printf("[EXITO] Linker completado. Binario '%s' generado.\n", archivo_bin);
}
