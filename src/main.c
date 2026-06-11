#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 256

int main(int argc, char *argv[]) {
    printf("=== ENSAMBLADOR IA-32 Y MINI ENLAZADOR ===\n\n");

    if (argc < 2) {
        printf("[ERROR] Uso correcto: ./assembler <archivo.asm>\n");
        return 1;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        printf("[ERROR] No se pudo abrir el archivo: %s\n", argv[1]);
        return 1;
    }

    char linea[MAX_LINEA];
    int numero_linea = 1;

    // Lectura real del archivo línea por línea
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Aquí es donde en el futuro llamarás a tu nuevo Lexer
        // tokenizar_linea_dinamica(linea);
        
        printf("[Linea %d] %s", numero_linea, linea);
        numero_linea++;
    }

    fclose(archivo);
    
    // Aquí irán después las llamadas al Parser, Encoder y Linker
    // utilizando la lista de tokens generada por el Lexer.

    return 0;
}
