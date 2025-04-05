#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GAME_NAME "C Bomberman"

#define BASE_WIDTH 256
#define BASE_HEIGHT 224

#define SCREEN_WIDTH BASE_WIDTH*4
#define SCREEN_HEIGHT BASE_HEIGHT*4

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "stdbool.h"

#include "../collision.h"
#include "../player.h"
#include "../bomb.h"
#include "../fire.h"
#include "../brick.h"
#include "sprite.h"

typedef struct sprite_data {
    SDL_Texture* sprite;
    int spr_width;
    int spr_height;
} Sprite_data;


struct graphics {

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Event window_event;
    SDL_Rect rect;

    Sprite_data* sprites;

    float x_multiplier;
    float y_multiplier;

}; 
typedef struct graphics Graphics;

void init_graphics(Graphics* graphics);
void render_game(Graphics* graphics, Entity* entity, Collision* collision, Bomb** bombs, Fire** fires, Brick** bricks, Player* player);

bool graphics_event(Graphics* graphics);

// Object draw
void tex_render(Graphics* graphics, Sprite* sprite, int spr, int x, int y);

void e_render(Graphics* graphics, Entity* entity);
void coll_render(Graphics* graphics, Collision* collision);
void b_render(Graphics* graphics, Bomb** bombs);
void f_render(Graphics* graphics, Fire** fires);
void bri_render(Graphics* graphics, Brick** bricks);

void free_graphics(Graphics* graphics);

void rect_resolution_fix(Graphics* graphics, SDL_Rect* rect, int x, int y, int w, int h);
void rect_set(Graphics* graphics, SDL_Rect* rect, int x, int y, int w, int h);

#endif