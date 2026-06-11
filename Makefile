# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -I./include -g

# Directorios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Archivo de salida (el ejecutable unificado)
EXEC = ensamblador.exe

# Buscar todos los archivos .c en la carpeta src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Reemplazar la ruta y extensión para generar los .o en obj/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regla principal
all: $(EXEC)

# Compilar el ejecutable final
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compilacion exitosa. Ejecuta con: ./$(EXEC) examples/pruebas_avanzadas.asm"

# Regla para compilar cada .c en un .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear el directorio obj/ si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(EXEC)

.PHONY: all clean
