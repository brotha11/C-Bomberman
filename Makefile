# ==============================
# C-Bomberman Makefile (Linux only)
# ==============================

# Variables
SRC_DIRS := SRC_DIRS := $(shell find src -type d)
SRC := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJ_DIR := obj
OBJ := $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

NAME := C-Bomberman
CC := gcc
CFLAGS := -Iinclude -Llib
LIBS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

# Opciones
all: l

# Crear carpeta obj/ si no existe
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar para Linux
l: $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS) -lm

# Ejecutar
run:
	./$(NAME)

# Limpiar
clean:
	rm -rf $(OBJ_DIR) $(NAME)

cleanobj:
	rm -rf $(OBJ_DIR)

.PHONY: all l run clean cleanobj
