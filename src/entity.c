#include "entity.h"

Entity new_entity(int x, int y, int width, int height, float max, int spr) {
    Entity new;

    new.alive = true;
    new.x = x;
    new.y = y;
    new.width = width;
    new.height = height;
    new.facing_x = 0;
    new.facing_y = 1;

    new.final_x = new.final_y = 0;
    new.sub_x = new.sub_y = 0;

    new.hspeed = new.vspeed = 0;
    new.max_speed = max;

    new.sprite = new_sprite(width,height);

    return new;
}

void e_update(Entity* entity, Collision* collision, Fire** fires) {

    e_move(entity, collision);

    // Facing
    if (entity->hspeed != 0 && entity->vspeed == 0) {
        entity->facing_y = 0;
        entity->facing_x = e_sign(entity->hspeed);
    } else {
        entity->facing_y = e_sign(entity->vspeed);
        entity->facing_y = 0;
    }

    //get killed
    if (coll_fire(fires,entity->x,entity->y,entity->width, entity->height)) {
        entity->alive = false;
    }
}

void e_move(Entity* entity, Collision* collision) {

    float slide_factor = 2.75f;

    entity->sub_x += entity->hspeed;
    entity->final_x = round(entity->sub_x);
    entity->sub_x -= entity->final_x;

    for (int i = 0; i < abs(entity->final_x); ++i) {

        Collision* coll = coll_meeting(collision, entity->x + e_sign(entity->final_x), entity->y, entity->width, entity->height);

        if (!coll) {
            entity->x += e_sign(entity->hspeed); 
        } else if (coll && entity->vspeed == 0 && abs((entity->y + entity->height/2) - (coll->y + coll->height/2)) > coll->height / slide_factor) {
            if (entity->y + entity->height/2 < coll->y + coll->height/2) {
                if (!coll_meeting(collision,entity->x,entity->y - fabs(e_sign(entity->hspeed)),entity->width,entity->height)) entity->y -= fabs(e_sign(entity->hspeed));
                else break;
            }
            else {
                if (!coll_meeting(collision,entity->x,entity->y + entity->height + fabs(e_sign(entity->hspeed)),entity->width,entity->height)) entity->y += fabs(e_sign(entity->hspeed));
                else break;
            }
        } else break;
    }
    
    entity->sub_y += entity->vspeed;
    entity->final_y = round(entity->sub_y);
    entity->sub_y -= entity->final_y;

    for (int i = 0; i < abs(entity->final_y); ++i) {

        Collision* coll = coll_meeting(collision, entity->x, entity->y + e_sign(entity->final_y), entity->width, entity->height);

        if (!coll) {
            entity->y += e_sign(entity->vspeed); 
        } else if (coll && entity->hspeed == 0 && abs((entity->x + entity->width/2) - (coll->x + coll->width/2)) > coll->width / slide_factor) {
            if (entity->x + entity->width / 2 < coll->x + coll->width / 2) {
                if (!coll_meeting(collision, entity->x - fabs(e_sign(entity->vspeed)), entity->y, entity->width, entity->height)) 
                    entity->x -= fabs(e_sign(entity->vspeed));
                else break;
            } else {
                if (!coll_meeting(collision, entity->x + entity->width + fabs(e_sign(entity->vspeed)), entity->y, entity->width, entity->height)) 
                    entity->x += fabs(e_sign(entity->vspeed));
                else break;
            }
        } else break;
    }

}

int e_sign(int x) {
    if ((int)x > 0) return 1;
    else if ((int)x < 0) return -1;
    return 0;
}