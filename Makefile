all:
	// Windows
	gcc -Iinclude -Llib -o bomberman src/*.c src/graphics/*.c src/input/*.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

	// Linux
	gcc -o bomberman src/*.c src/graphics/*.c src/input/*.c -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lm