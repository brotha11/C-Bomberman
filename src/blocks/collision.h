#ifndef COLLISION_H
#define COLLISION_H

#include "stdbool.h"
#include <stdlib.h>

typedef enum COLL_TYPE {
    BLOCK,
    BRICK,
    COLL_BOMB,
} COLL_TYPE;

struct collision {

    int x;
    int y;
    int width;
    int height;
    int type;
    int broken;

    struct collision* next;
};

typedef struct collision Collision;

void add_collision(Collision** head, int x, int y, int width, int height, int type);
Collision* coll_meeting(Collision** head, int x, int y, int width, int height);
Collision* coll_meeting_ext(Collision** head, int x, int y, int true_x, int true_y, int width, int height);

bool coll_point(Collision* head, int x, int y);

int sign(int x);

void free_collisions(Collision** head);
void free_single(Collision** head, Collision* coll);

#endif