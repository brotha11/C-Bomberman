#include "brick.h"

void add_brick(Brick** bricks, Collision** head, int x, int y, int width, int height, double* delta) {
    Brick* new_brick = (Brick*)malloc(sizeof(Brick));

    add_collision(head, x, y, width, height, BRICK); // Agrega la colisión a la lista

    new_brick->coll = *head; // Apunta a la colisión recién creada
    new_brick->broken = false;
    new_brick->next = *bricks;
    new_brick->timer = new_timer(EXPLOSION_TIME);
    new_brick->sprite = new_sprite(16,16);
    new_brick->sprite.frame_x_max = MAX_BRICK_FRAMES;
    new_brick->visible = true;
    new_brick->dummy = x;
    new_brick->p_delta_time = delta;
    new_brick->break_mult = 1;

    *bricks = new_brick;
}

void bri_update(Brick** bricks, Fire** fires, Collision** collision, Power_up** powers) {
    Brick* current = *bricks;
    Brick* next_brick;

    while(current != NULL) {

        next_brick = current->next;

        if (current->dummy == -1234) {
            current = next_brick;
            continue;  
        }

        if (!current->broken) {
            Fire* fire = coll_fire(fires,current->coll->x,current->coll->y,current->coll->width, current->coll->height);
            if (fire) {
                current->broken = true;
                current->break_mult = fire->time_mult;
                current->timer.time = EXPLOSION_TIME*current->break_mult;
                current->coll->broken = 1; // Set broken flag on collision
            }
        } else {
            if (tick_timer(&current->timer, current->p_delta_time) == 0) {}
            else {
                spawn_power(powers, current->coll->x, current->coll->y, current->p_delta_time);
                free_brick(bricks, current, collision);
                current = next_brick;
                continue;
            }
        }
        // Animate
        if (current->broken) {
            if (current->sprite.frame_x < MAX_BRICK_FRAMES) 
                animate_sprite_timer(&current->sprite, current->timer.time, EXPLOSION_TIME*current->break_mult, current->p_delta_time);
            if (current->timer.time <= *current->p_delta_time) current->visible = false;
        }

        current = next_brick;
    }
}

void spawn_power(Power_up** powers, int x, int y, double* delta) {
    unsigned seed = (clock() + x * y);
    srand(seed);
    if (rand()%100 <= 7) {
        add_powerup(powers,x,y,FIRE_UP,delta);
        return;
    }

    srand(seed* 2);
    if (rand()%100 <= 7) {
        add_powerup(powers,x,y,BOMB_UP,delta);
        return;
    }

    srand(seed* 3);
    if (rand()%100 <= 7) {
        add_powerup(powers,x,y,SPEED_UP,delta);
        return;
    }

    srand(seed* 4);
    if (rand()%100 <= 3) {
        add_powerup(powers,x,y,BOMB_LINE,delta);
        return;
    }
    srand(seed* 5);
    if (rand()%100 <= 4) {
        add_powerup(powers,x,y,KICK,delta);
        return;
    }

    srand(seed* 6);
    if (rand()%100 <= 2) {
        add_powerup(powers,x,y,BOMB_PIERCING,delta);
        return;
    }
}

void free_all_bricks(Brick** head, Collision** head_coll) {
    Brick* current = *head;
    Brick* next_brick;
    
    while (current != NULL) {
        next_brick = current->next;
        free_single(head_coll, current->coll);
        free(current);
        current = next_brick;
    }

    *head = NULL;
}

void free_brick(Brick** head, Brick* coll, Collision** head_coll) {
    Brick *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == coll) { 
        *head = temp->next; // Changed head 
        free_single(head_coll, temp->coll);
        free(temp); // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp != coll) { 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) 
        return; 
  
    // Unlink the node from linked list 
    prev->next = temp->next; 
    
    free_single(head_coll, temp->coll);
    free(temp); // Free memory 
}