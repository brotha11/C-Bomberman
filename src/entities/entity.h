#ifndef ENTITY_H
#define ENTITY_H

#include "math.h"
#include "time.h"
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

    Timer shake_timer;
    int shake_x, shake_y;

    double hspeed, vspeed;
    double max_speed;

    double sub_x, sub_y;
    int final_x, final_y;

    int slide_w, slide_h;

    double* p_delta_time;

    bool alive;
    Sprite sprite;
    struct entity* next;
};
typedef struct entity Entity;

Entity* new_entity(Entity** head, int x, int y, int width, int height, double max, double* delta);
void e_move(Entity* entity, Collision** collision);
void e_move_all(Entity* entity, Collision** collision, Power_up** powers, Entity** entities);
void e_update(Entity* entity, Collision** collision, Fire** fires);
void free_entity(Entity** head, Entity* ent);
void free_all_entities(Entity** head);
void e_shake(Timer* timer, int* sh_x, int* sh_y, double shake_oft, int dist, double* delta);
Entity* coll_entity_ext(Entity** head, Entity* self, int x, int y, int true_x, int true_y, int width, int height);

int e_sign(int x);

#endif