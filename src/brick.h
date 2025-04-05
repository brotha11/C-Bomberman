#ifndef BRICK_H
#define BRICK_H

#include "collision.h"
#include "fire.h"

struct brick {
    Collision* coll;

    bool broken;
    int timer;

    struct brick* next;

};
typedef struct brick Brick;

void add_brick(Brick** bricks, Collision** head, int x, int y, int width, int height);
void bri_update(Brick** bricks, Fire** fires, Collision** collision);

void free_all_bricks(Brick** head, Collision** head_coll);
void free_brick(Brick** head, Brick* coll, Collision** head_coll);

#endif