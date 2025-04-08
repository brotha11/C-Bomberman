#ifndef BOMB_H
#define BOMB_H

#include "stdlib.h"
#include "../blocks/collision.h"
#include "../graphics/sprite.h"
#include "../pickups/powerup.h"
#include "../entities/entity.h"

#define BOMB_KICK_SPEED 1.75
#define BOMB_TIMER 180

struct fire;
typedef struct fire Fire;


struct bomb {
    Collision* coll;
    Entity move;

    int x,y;
    int width, height;

    int timer;
    int blast_radius;
    int* owner;

    int kick_x, kick_y;

    Sprite sprite;
    struct bomb* next;
};
typedef struct bomb Bomb;

void add_bomb(Bomb** head, Collision** colls, int* owner_bomb, int col, int row, int blast);
Bomb* coll_bomb(Bomb** head, int x, int y, int width, int height);
Bomb* coll_bomb_ext(Bomb** head, int x, int y, int true_x, int true_y, int width, int height);
void free_bomb(Bomb** head, Collision** head_coll, Bomb* bomb);
void free_all_bombs(Bomb** head, Collision** head_coll);

void b_update(Bomb** head, Fire** fire, Collision** collision, Power_up** powers);
void b_explode(Bomb** head, Bomb* bomb, Fire** fire, Collision** collision, Power_up** powers);
void get_tile_position(int* tile_x, int* tile_y, int x, int y, int w, int h);

#endif