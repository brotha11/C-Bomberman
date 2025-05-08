#ifndef BRICK_H
#define BRICK_H

#include "collision.h"
#include "../bombs/fire.h"
#include "../pickups/powerup.h"
#include "time.h"
#include "../game/timer.h"

#define MAX_BRICK_FRAMES 8

struct brick {
    Collision* coll;
    Sprite sprite;

    bool broken;
    bool visible;
    
    Timer timer;
    double break_mult;
    int dummy;
    double* p_delta_time;

    struct brick* next;
};
typedef struct brick Brick;

void add_brick(Brick** bricks, Collision** head, int x, int y, int width, int height, double* delta);
void bri_update(Brick** bricks, Fire** fires, Collision** collision, Power_up** powers);
void spawn_power(Power_up** powers, int x, int y, double* delta);

void free_all_bricks(Brick** head, Collision** head_coll);
void free_brick(Brick** head, Brick* coll, Collision** head_coll);

#endif