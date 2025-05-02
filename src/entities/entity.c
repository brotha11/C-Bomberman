#include "entity.h"

Entity* new_entity(Entity** head, int x, int y, int width, int height, double max, double* delta) {
    Entity *new = (Entity*)malloc(sizeof(Entity));

    new->alive = true;

    // Save memory dir from delta time
    new->p_delta_time = delta;

    new->x = x;
    new->y = y;
    new->last_x = x;
    new->last_y = y;

    new->width = width;
    new->height = height;
    new->facing_x = 0;
    new->facing_y = 1;

    new->shake_x = new->shake_y = 0;
    new->shake_timer = new_timer(0);

    new->slide_w = width / 4;
    new->slide_h = height;

    new->final_x = new->final_y = 0;
    new->sub_x = new->sub_y = 0;

    new->hspeed = new->vspeed = 0;
    new->max_speed = max;

    new->sprite = new_sprite(width,height);
    
    new->next = *head;
    *head = new;

    return new;
}

void e_update(Entity* entity, Collision** collision, Fire** fires) {

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

void e_move(Entity* entity, Collision** collision) {
    entity->last_x = entity->x;
    entity->last_y = entity->y;

    //float slide_factor = 2.75f;
    int divide = 512;
    int slide_factor = (entity->height - abs(entity->height - entity->height/divide) / 2);

    entity->sub_x += (entity->hspeed * (*entity->p_delta_time) * 60);
    entity->final_x = round(entity->sub_x);
    entity->sub_x -= entity->final_x;

    if (entity->final_x == 0) entity->is_colliding_x = 0;

    for (int i = 0; i < abs(entity->final_x); ++i) {
        Collision* coll = coll_meeting_ext(collision, entity->x + e_sign(entity->final_x), 
        entity->y, entity->x, entity->y, entity->width, entity->height);
        Collision* coll_ext = coll_meeting_ext(collision, entity->x + e_sign(entity->final_x), 
            entity->y + slide_factor, entity->x, entity->y + slide_factor, entity->width, entity->height/divide);

        if (!coll) {
            entity->x += e_sign(entity->hspeed);     
            entity->is_colliding_x = 0;
        } 
        else if (coll && !coll_ext && entity->vspeed == 0) {
            if (entity->y + entity->height/2 < coll->y + coll->height/2) {
                //if (!coll_meeting(collision,entity->x,entity->y, entity->width,entity->height)) 
                entity->y -= fabs(e_sign(entity->hspeed)); 
                entity->is_colliding_x = 0;
                //else break;               
            } else {
                //if (!coll_meeting(collision,entity->x,entity->y + entity->height + fabs(e_sign(entity->hspeed)),entity->width,entity->height)) 
                entity->y += fabs(e_sign(entity->hspeed));
                entity->is_colliding_x = 0;
                //else break;
            }
        } else break;
    }
    
    entity->sub_y += (entity->vspeed * (*entity->p_delta_time) * 60);
    entity->final_y = round(entity->sub_y);
    entity->sub_y -= entity->final_y;

    if (entity->final_y == 0) entity->is_colliding_y = 0;

    for (int i = 0; i < abs(entity->final_y); ++i) {

        Collision* coll = coll_meeting_ext(collision, entity->x, entity->y + e_sign(entity->final_y), 
            entity->x, entity->y, entity->width, entity->height);
        Collision* coll_ext = coll_meeting_ext(collision, entity->x + slide_factor, entity->y + e_sign(entity->final_y), 
            entity->x + slide_factor, entity->y, entity->width/divide, entity->height);

        if (!coll) {
            entity->y += e_sign(entity->vspeed);
            entity->is_colliding_y = 0;
        } 
       else if (coll && !coll_ext && entity->hspeed == 0) {
            if (entity->x + entity->width/2 < coll->x + coll->width/2) {
                //if (!coll_meeting(collision,entity->x,entity->y, entity->width,entity->height)) 
                entity->x -= fabs(e_sign(entity->vspeed)); 
                entity->is_colliding_y = 1;
                //else break;               
            } else {
                //if (!coll_meeting(collision,entity->x,entity->y + entity->height + fabs(e_sign(entity->hspeed)),entity->width,entity->height)) 
                entity->x += fabs(e_sign(entity->vspeed));
                entity->is_colliding_y = 1;
                //else break;
            }
        } else break;
    }

}

void e_move_all(Entity* entity, Collision** collision, Power_up** powers, Entity** entities) {
    entity->last_x = entity->x;
    entity->last_y = entity->y;

    entity->sub_x += (entity->hspeed * (*entity->p_delta_time) * 60);
    entity->final_x = round(entity->sub_x);
    entity->sub_x -= entity->final_x;

    int coll_x = 0;

    if (entity->final_x == 0) entity->is_colliding_x = 0;

    for (int i = 0; i < abs(entity->final_x); ++i) {
        if (coll_meeting_ext(collision, entity->x + e_sign(entity->final_x), 
        entity->y, entity->x, entity->y, entity->width, entity->height)) {
            coll_x = 1;
        }
        Power_up* pw = (coll_powerup(powers, entity->x + e_sign(entity->final_x), 
        entity->y, entity->width, entity->height));
        if (pw) {
            free_powerup(powers, pw);
        }

        // Other entities
        if (coll_entity_ext(entities, entity, entity->x + e_sign(entity->final_x), 
        entity->y, entity->x, entity->y, entity->width, entity->height)) {
            coll_x = 1;
        }

        if (coll_x == 0) {
            entity->x += e_sign(entity->hspeed);     
            entity->is_colliding_x = 0;
        } else {
            break;
        }
    }
    
    entity->sub_y += (entity->vspeed * (*entity->p_delta_time) * 60);
    entity->final_y = round(entity->sub_y);
    entity->sub_y -= entity->final_y;

    int coll_y = 0;

    if (entity->final_y == 0) entity->is_colliding_y = 0;

    for (int i = 0; i < abs(entity->final_y); ++i) {

        if (coll_meeting_ext(collision, entity->x, entity->y + e_sign(entity->final_y), 
            entity->x, entity->y, entity->width, entity->height)) {
                coll_y = 1;
            }
        Power_up* pw = coll_powerup(powers, entity->x, entity->y + e_sign(entity->final_y), 
            entity->width, entity->height);
        if (pw) {
            free_powerup(powers, pw);
        }

        // Other entities
        if (coll_entity_ext(entities, entity, entity->x, entity->y + e_sign(entity->final_y), 
            entity->x, entity->y, entity->width, entity->height)) {
            coll_y = 1;
        }

        if (coll_y == 0) {
            entity->y += e_sign(entity->vspeed);
            entity->is_colliding_y = 0;
        } else break;
    }
}

Entity* coll_entity_ext(Entity** head, Entity* self, int x, int y, int true_x, int true_y, int width, int height) {
    Entity* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
            current = current->next;
            continue;  
        }

        if (current == self) {
            current = current->next;
            continue;     
        }

        if (true_x + width > current->x && true_x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
            true_y + height > current->y && true_y < current->y + current->height) {
                current = current->next;
                continue;
            }

        // Verificamos si hay colisión entre la hitbox del personaje y el bloque
        if (x + width > current->x && x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
            y + height > current->y && y < current->y + current->height) {  // Revisa si el personaje está dentro de la altura del bloque
            return current;  // Hay colisión
        }

        current = current->next;
    }
    return NULL;  // No hay colisión
}

void e_shake(Timer* timer, int* sh_x, int* sh_y, double shake_oft, int dist, double* delta) {
    tick_timer(timer, delta);

    if (timer->time > (*delta)) {
        // Tick saved value
        timer->saved_00 -= (*delta);

        if (timer->saved_00 <= 0) {
            srand(clock() + *sh_x + *sh_y);
            *sh_x = (int) round(rand()%(dist*2)+1) - dist;

            srand(clock() + *sh_x + *sh_y);
            *sh_y = (int) round(rand()%5) - 2;

            timer->saved_00 = shake_oft;
        }
    } else {
        *sh_x = *sh_y = 0;
        timer->saved_00 = 0;
    }

}

void free_entity(Entity** head, Entity* ent) {
    Entity *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == ent) { 
        *head = temp->next; // Changed head 
        free(temp); // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp != ent) { 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) 
        return; 
  
    // Unlink the node from linked list 
    prev->next = temp->next; 
  
    free(temp); // Free memory 
}

void free_all_entities(Entity** head) {
    Entity* current = *head;
    Entity* next;

    while (current != NULL) {
        next = current->next;  // Guardar referencia al siguiente nodo
        free(current);
        current = next;  // Mover al siguiente nodo
    }

    *head = NULL;  // Evitar accesos a memoria liberada
}

int e_sign(int x) {
    if ((int)x > 0) return 1;
    else if ((int)x < 0) return -1;
    return 0;
}
