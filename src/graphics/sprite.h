#ifndef SPRITE_H
#define SPRITE_H

// LIST OF TEXTURES
#define BOMBER_WHITE 0
#define BOMBER_WHITE_PATH "res/bomberman_w.png"

#define BOMB 1
#define BOMB_PATH "res/bomb.png"

#define FIRE 2
#define FIRE_PATH "res/fire.png"

#define MAP_01 3
#define MAP_01_PATH "res/map_01.png"

#define BRICK_01 4
#define BRICK_01_PATH "res/brick_01.png"

#define POWER_UPS 5
#define POWER_UPS_PATH "res/power_ups.png"

#define ITEM_BURN 6
#define ITEM_BURN_PATH "res/item_burn.png"

#define TEXTURE_AMOUNT 7
#define SPR 0
#define SPR_WIDTH 1
#define SPR_HEIGHT 2

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "stdio.h"

typedef struct sprite {

    int sprite;
    SDL_Rect frame_rect;

    int image_speed, image_change;
    int frame_x, frame_y;
    int width, height;

    int x_off, y_off;
    int frame_x_max;


} Sprite;

SDL_Texture* load_sprite(const char* path,  SDL_Renderer* renderer);
Sprite new_sprite(int w, int h);
void animate_sprite(Sprite* sprite);
void animate_sprite_timer(Sprite* sprite, int timer, int timer_max);

#endif 