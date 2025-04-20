#ifndef TILE_MANAGER_H
#define TILE_MANAGER_H

#include "../game/game_constants.h"

void get_tile_position(int* tile_x, int* tile_y, int x, int y, int w, int h);
int get_center_x(int width);
int get_center_y(int height);

#endif