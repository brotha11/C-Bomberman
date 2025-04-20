#include "tile_manager.h"

void get_tile_position(int* tile_x, int* tile_y, int x, int y, int w, int h) {
    int p_x = x + w / 2;  // Centro del jugador en X
    int p_y = y + h / 2; // Centro del jugador en Y

    *tile_x = p_x / 16;  // Tile en el que está el centro del jugador (horizontal)
    *tile_y = p_y / 16;  // Tile en el que está el centro del jugador (vertical)

    //if (p_x % TILE >= 8) (*tile_x)++;
    //if (p_y % TILE >= 8) (*tile_y)++;

    *tile_x = *tile_x * TILE;
    *tile_y = *tile_y * TILE;
}

int get_center_x(int width) {
    int scale = (int)(SCREEN_HEIGHT/BASE_HEIGHT);
    return (width/2) - (SCREEN_WIDTH / scale) / 2;
}
int get_center_y(int height) {
    int scale = (int)(SCREEN_HEIGHT/BASE_HEIGHT);
    return (height/2) - (SCREEN_HEIGHT / scale) / 2;
}