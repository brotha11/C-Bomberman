#ifndef SPRITE_H
#define SPRITE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "stdio.h"
#include "../game/timer.h"
#include "../game/game_constants.h"
#include "../tools/utils.h"
#include <math.h>


// LIST OF TEXTURES
typedef enum {
    TEX_BOMBER_WHITE,
    TEX_BOMBER_BLACK,
    TEX_BOMBER_RED,
    TEX_BOMBER_BLUE,
    TEX_BOMBER_GREEN,
    TEX_BOMB,
    TEX_FIRE,
    TEX_MAP_01,
    TEX_BRICK,
    TEX_POWER_UPS,
    TEX_ITEM_BURN,
    TEX_BATTLE_MODE_SCORE,
    TEX_GUI_SYMBOLS,
    TEX_BG_OCEAN_00,
    TEX_BG_GRASS_00,
    TEX_AMOUNT
} TEXTURES;

typedef enum {
    SY_0,
    SY_1,
    SY_2,
    SY_3,
    SY_4,
    SY_5,
    SY_6,
    SY_7,
    SY_8,
    SY_9,
    SY_COLON,
    SY_CLOCK,
    SY_BOMBER_W,
    SY_BOMBER_B,
    SY_BOMBER_R,
    SY_BOMBER_BL,
    SY_BOMBER_G,
    SY_DBOMB_W,
    SY_DBOMB_B,
    SY_DBOMB_R,
    SY_DBOMB_BL,
    SY_DBOMB_G,
    SY_KBOMB_W,
    SY_KBOMB_W1,
    SY_KBOMB_B,
    SY_KBOMB_B1,
    SY_KBOMB_R,
    SY_KBOMB_R1,
    SY_KBOMB_BL,
    SY_KBOMB_BL1,
    SY_KBOMB_G,
    SY_KBOMB_G1,
    SY_KILLED,
    SY_KILLED1,
} SYMBOLS;

#define BOMBER_WHITE_PATH "res/bomberman_w.png"
#define BOMBER_BLACK_PATH "res/bomberman_b.png"
#define BOMBER_RED_PATH   "res/bomberman_r.png"
#define BOMBER_BLUE_PATH  "res/bomberman_bl.png"
#define BOMBER_GREEN_PATH "res/bomberman_g.png"

#define BOMB_PATH "res/bomb.png"
#define FIRE_PATH "res/fire_exp.png"
#define MAP_01_PATH "res/map_01.png"
#define BRICK_01_PATH "res/brick_01.png"
#define POWER_UPS_PATH "res/power_ups.png"
#define ITEM_BURN_PATH "res/item_burn.png"
#define BATTLE_MODE_SCORE_PATH "res/battle_mode_score.png"
#define GUI_SYMBOLS_PATH "res/gui_numbers.png"
#define BG_OCEAN_00_PATH "res/bg_ocean_00.png"
#define BG_GRASS_00_PATH "res/bg_grass_00.png"

#define SPR 0
#define SPR_WIDTH 1
#define SPR_HEIGHT 2

typedef struct sprite {

    int sprite;
    SDL_Rect frame_rect;
    SDL_Point center_point;

    double image_speed;
    Timer image_change;
    int frame;
    int frame_x, frame_y;
    int width, height;

    double w_mult, h_mult;
    double angle;

    int x_off, y_off;
    int frame_x_max;


} Sprite;

SDL_Texture* load_sprite(const char* path,  SDL_Renderer* renderer);
Sprite new_sprite(int w, int h);
void animate_sprite(Sprite* sprite, double* delta);
void animate_sprite_timer(Sprite* sprite, double timer, double timer_max, double* delta);
void normalize_scale(Sprite* sprite, double goal, double speed, double* delta);

#endif
