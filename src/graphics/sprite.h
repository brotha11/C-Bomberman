#ifndef SPRITE_H
#define SPRITE_H

// LIST OF TEXTURES

typedef enum {
    TEX_BOMBER_WHITE,
    TEX_BOMB,
    TEX_FIRE,
    TEX_MAP_01,
    TEX_BRICK,
    TEX_POWER_UPS,
    TEX_ITEM_BURN,
    TEX_AMOUNT
} TEXTURES;

#define BOMBER_WHITE_PATH "res/bomberman_w.png"
#define BOMB_PATH "res/bomb.png"
#define FIRE_PATH "res/fire.png"
#define MAP_01_PATH "res/map_01.png"
#define BRICK_01_PATH "res/brick_01.png"
#define POWER_UPS_PATH "res/power_ups.png"
#define ITEM_BURN_PATH "res/item_burn.png"

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