#include <stdio.h>

// Firmas de las funciones de otros módulos
void tokenizar_linea_dinamica(const char *linea);
void calcular_tamano_instruccion();
void codificar_instruccion_dinamica();
void generar_archivo_objeto(const char *nombre_archivo);

#define MAX_LINEA 256

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    // PRIMERA PASADA: Calcular memoria y Tabla de Símbolos
    FILE *archivo = fopen(argv[1], "r");
    char linea[MAX_LINEA];
    
    printf("[1] Ejecutando Pasada 1 (Tabla de Simbolos y Parsing)...\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        tokenizar_linea_dinamica(linea);
        calcular_tamano_instruccion();
    }
    rewind(archivo); // Reiniciamos el cursor del archivo para la segunda pasada

    // SEGUNDA PASADA: Generación de Opcodes y ModRM
    printf("[2] Ejecutando Pasada 2 (Encoder IA-32)...\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        tokenizar_linea_dinamica(linea);
        codificar_instruccion_dinamica();
    }
    fclose(archivo);

    // FASE FINAL: Creación del archivo objeto intermedio
    printf("[3] Generando Formato Objeto...\n");
    generar_archivo_objeto("salida.obj");

    return 0;
}
