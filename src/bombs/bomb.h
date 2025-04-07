#ifndef BOMB_H
#define BOMB_H

#include "stdlib.h"
#include "../blocks/collision.h"
#include "../graphics/sprite.h"
#include "../pickups/powerup.h"

struct fire;
typedef struct fire Fire;

struct bomb {
    Collision* coll;

    int x,y;
    int width, height;

    int timer;
    int blast_radius;
    int* owner;

    Sprite sprite;
    struct bomb* next;
};
typedef struct bomb Bomb;

void add_bomb(Bomb** head, Collision** colls, int* owner_bomb, int col, int row, int blast);
Bomb* coll_bomb(Bomb** head, int x, int y, int width, int height);
void free_bomb(Bomb** head, Collision** head_coll, Bomb* bomb);
void free_all_bombs(Bomb** head, Collision** head_coll);

void b_update(Bomb** head, Fire** fire, Collision** collision, Power_up** powers);
void b_explode(Bomb** head, Bomb* bomb, Fire** fire, Collision** collision, Power_up** powers);

#endif