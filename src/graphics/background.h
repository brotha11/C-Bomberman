#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../game/game_constants.h"
#include "../graphics/sprite.h"

typedef enum {
    BG_OCEAN_00,
} BG_LIST;

typedef struct background {
    int x;
    int y;

    int start_x, start_y;

    float xf, yf;

    int tile_x; // Boolean
    int tile_y; // Boolean

    float speed_x;
    float speed_y;    

    Sprite sprite;
    int used;
} Background;

Background new_bg(int x, int y, int width, int height, int tile_x, int tile_y, float speed_x, float speed_y, int used);
void bg_update(Background* background);


#endif
