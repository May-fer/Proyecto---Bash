# --- Makefile para BASH---

# Compilador a utilizar
CC = gcc

# Flags (Banderas) de compilación:
# -Wall: Activa todos los warnings (importante para código limpio)
# -Wextra: Activa warnings adicionales
# -g: Agrega información de depuración (para usar gdb si algo falla)
CFLAGS = -Wall -Wextra -g

# Nombre del archivo ejecutable final
TARGET = mybash

# Nombre del archivo fuente
SRC = Narvaez-Bash.c

# Regla por defecto (se ejecuta si escribes solo 'make')
all: $(TARGET)

# Regla para construir el ejecutable
# Depende del archivo fuente (myNarvaez-bash.c)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regla para limpiar (borrar) el ejecutable
# Se ejecuta escribiendo 'make clean'
clean:
	rm -f $(TARGET)

# Regla para compilar y ejecutar inmediatamente
run: $(TARGET)
	./$(TARGET)
