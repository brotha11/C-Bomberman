#ifndef ENTITY_H
#define ENTITY_H

#include "math.h"
#include "../blocks/collision.h"
#include "../bombs/fire.h"
#include "stdio.h"
#include "../graphics/sprite.h"
#include "../audio/sound.h"

struct entity {
    int x,y;
    int width, height;
    int facing_x, facing_y;

    int last_x, last_y;
    int is_colliding_x;
    int is_colliding_y;

    float hspeed, vspeed;
    float max_speed;

    float sub_x, sub_y;
    int final_x, final_y;

    int slide_w, slide_h;

    bool alive;
    Sprite sprite;
};
typedef struct entity Entity;

Entity new_entity(int x, int y, int width, int height, float max);
void e_move(Entity* entity, Collision** collision);
void e_move_all(Entity* entity, Collision** collision, Power_up** powers);
void e_update(Entity* entity, Collision** collision, Fire** fires);
int e_sign(int x);

#endif