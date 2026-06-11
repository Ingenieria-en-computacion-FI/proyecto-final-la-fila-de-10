#include <stdio.h>

void tokenizar_linea_dinamica(const char *linea);
void calcular_tamano_instruccion();
void codificar_instruccion_dinamica();
void generar_archivo_objeto(const char *nombre_archivo);
void generar_binario_final(const char *archivo_obj, const char *archivo_bin);

#define MAX_LINEA 256

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("[ERROR] Uso: ./assembler <archivo.asm>\n");
        return 1;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (!archivo) {
        printf("[ERROR] No se pudo abrir: %s\n", argv[1]);
        return 1;
    }

    char linea[MAX_LINEA];
    
    printf("\n--- ENSAMBLADOR IA-32 Y LINKER ---\n");
    printf("[1] Ejecutando Pasada 1 (Tabla de Simbolos y Memoria)...\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        tokenizar_linea_dinamica(linea);
        calcular_tamano_instruccion();
    }
    
    rewind(archivo); 

    printf("[2] Ejecutando Pasada 2 (Generacion Opcodes/ModRM/SIB)...\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        tokenizar_linea_dinamica(linea);
        codificar_instruccion_dinamica();
    }
    fclose(archivo);

    printf("[3] Creando Modulos (Formato Objeto)...\n");
    generar_archivo_objeto("salida.obj");

    printf("[4] Ejecutando Linker...\n");
    generar_binario_final("salida.obj", "programa.bin");

    printf("--- PROCESO FINALIZADO EXITOSAMENTE ---\n");
    return 0;
}
