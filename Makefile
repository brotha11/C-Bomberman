# Variables comunes
# SRC = src/game/*.c src/blocks/*.c src/entities/*.c src/bombs/*.c src/graphics/*.c src/input/*.c src/pickups/*.c src/audio/*.c src/rooms/*.c
SRC :=  $(wildcard src/audio/*.c) \
        $(wildcard src/blocks/*.c) \
        $(wildcard src/bombs/*.c) \
        $(wildcard src/entities/*.c) \
        $(wildcard src/game/*.c) \
        $(wildcard src/graphics/*.c) \
        $(wildcard src/input/*.c) \
        $(wildcard src/modes/**/*.c) \
        $(wildcard src/pickups/*.c) \
        $(wildcard src/rooms/*.c)

LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
CC = gcc
CFLAGS = -Iinclude -Llib
NAME = C-Bomberman

all: w

# Compilando para Windows
w: 
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -lmingw32 $(LIBS)

# Compilando para Linux
l:
	$(CC) -o $(NAME) $(SRC) $(LIBS) -lm

gdb:
	$(CC) -g $(CFLAGS) -o $(NAME) $(SRC) -lmingw32 $(LIBS)

# Ejecutar
run:
	./$(NAME)

# Declaramos que estas reglas no son archivos
.PHONY: w l r gdb all