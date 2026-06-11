#include <stdio.h>
#include "estructuras.h"

// Construcción del byte ModRM
// mod: 2 bits (Modo de direccionamiento)
// reg: 3 bits (Registro destino o extensión de opcode)
// rm:  3 bits (Registro fuente o memoria)
unsigned char build_modrm(unsigned char mod, unsigned char reg, unsigned char rm) {
    return (mod << 6) | (reg << 3) | rm;
}

// Construcción del byte SIB
// scale: 2 bits (Factor de escala: 1, 2, 4, 8)
// index: 3 bits (Registro índice)
// base:  3 bits (Registro base)
unsigned char build_sib(unsigned char scale, unsigned char index, unsigned char base) {
    return (scale << 6) | (index << 3) | base;
}

// Mapeo numérico de los registros IA-32 (0 a 7)
// Esto te servirá para alimentar las funciones de arriba
unsigned char get_register_code(const char* reg_name) {
    if (strcmp(reg_name, "EAX") == 0) return 0; // 000
    if (strcmp(reg_name, "ECX") == 0) return 1; // 001
    if (strcmp(reg_name, "EDX") == 0) return 2; // 010
    if (strcmp(reg_name, "EBX") == 0) return 3; // 011
    if (strcmp(reg_name, "ESP") == 0) return 4; // 100
    if (strcmp(reg_name, "EBP") == 0) return 5; // 101
    if (strcmp(reg_name, "ESI") == 0) return 6; // 110
    if (strcmp(reg_name, "EDI") == 0) return 7; // 111
    return 0xFF; // Error / No encontrado
}
