# Variables comunes
SRC = src/*.c src/blocks/*.c src/entities/*.c src/bombs/*.c src/graphics/*.c src/input/*.c src/pickups/*.c src/audio/*.c
LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
CC = gcc
CFLAGS = -Iinclude -Llib

all: w

# Compilando para Windows
w: 
	$(CC) $(CFLAGS) -o bomberman $(SRC) -lmingw32 $(LIBS)

# Compilando para Linux
l:
	$(CC) -o bomberman $(SRC) $(LIBS) -lm

gdb:
	$(CC) -g $(CFLAGS) -o bomberman $(SRC) -lmingw32 $(LIBS)

# Ejecutar
run:
	./bomberman

# Declaramos que estas reglas no son archivos
.PHONY: w l r gdb all