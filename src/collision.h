#ifndef COLLISION_H
#define COLLISION_H

#include "stdbool.h"
#include <stdlib.h>

struct collision {

    int x;
    int y;
    int width;
    int height;

    struct collision* next;
};

typedef struct collision Collision;

void add_collision(Collision** head, int x, int y, int width, int height);
Collision* coll_meeting(Collision** head, int x, int y, int width, int height);
Collision* coll_meeting_ext(Collision** head, int x, int y, int true_x, int true_y, int width, int height);

bool coll_point(Collision* head, int x, int y);

int sign(int x);

void free_collisions(Collision** head);
void free_single(Collision** head, Collision* coll);

#endif