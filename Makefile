# Variables comunes
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

# Compile for Windows
w: 
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -lmingw32 $(LIBS)

# Compile for Linux
l:
	$(CC) -o $(NAME) $(SRC) $(LIBS) -lm

gdb:
	$(CC) -g $(CFLAGS) -o $(NAME) $(SRC) -lmingw32 $(LIBS)

# Execute
run:
	./$(NAME)

# Declare rules
.PHONY: w l r gdb all