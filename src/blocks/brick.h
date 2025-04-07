#ifndef BRICK_H
#define BRICK_H

#include "collision.h"
#include "../bombs/fire.h"
#include "../pickups/powerup.h"
#include "time.h"

#define MAX_BRICK_FRAMES 6

struct brick {
    Collision* coll;
    Sprite sprite;

    bool broken;
    bool visible;
    
    int timer;

    struct brick* next;
};
typedef struct brick Brick;

void add_brick(Brick** bricks, Collision** head, int x, int y, int width, int height);
void bri_update(Brick** bricks, Fire** fires, Collision** collision, Power_up** powers);
void spawn_power(Power_up** powers, int x, int y);

void free_all_bricks(Brick** head, Collision** head_coll);
void free_brick(Brick** head, Brick* coll, Collision** head_coll);

#endif