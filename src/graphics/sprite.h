#ifndef SPRITE_H
#define SPRITE_H

// LIST OF TEXTURES
#define BOMBER_WHITE 0
#define BOMBER_WHITE_PATH "res/bomberman_w.png"

#define TEXTURE_AMOUNT 1
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


} Sprite;

SDL_Texture* load_sprite(const char* path,  SDL_Renderer* renderer);
Sprite new_sprite(int w, int h);
void animate_sprite(Sprite* sprite);

#endif 