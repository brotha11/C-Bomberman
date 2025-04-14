#include "brick.h"

void add_brick(Brick** bricks, Collision** head, int x, int y, int width, int height) {
    Brick* new_brick = (Brick*)malloc(sizeof(Brick));

    add_collision(head, x, y, width, height); // Agrega la colisión a la lista

    new_brick->coll = *head; // Apunta a la colisión recién creada
    new_brick->broken = false;
    new_brick->next = *bricks;
    new_brick->timer = EXPLOSION_TIME;
    new_brick->sprite = new_sprite(16,16);
    new_brick->sprite.frame_x_max = MAX_BRICK_FRAMES;
    new_brick->visible = true;
    new_brick->dummy = x;

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

        if (coll_fire(fires,current->coll->x,current->coll->y,current->coll->width, current->coll->height)) {
            current->broken = true;
        }

        // Animate
        if (current->broken) {
            if (current->sprite.frame_x < MAX_BRICK_FRAMES) animate_sprite_timer(&current->sprite, current->timer, EXPLOSION_TIME);
            if (current->timer == 1) current->visible = false;
        }

        if (current->broken) {
            if (current->timer != 0) current->timer--;
            else {
                spawn_power(powers, current->coll->x, current->coll->y);
                free_brick(bricks, current, collision);
                current = next_brick;
                continue;
            }
        }

        current = next_brick;
    }
}

void spawn_power(Power_up** powers, int x, int y) {
    unsigned seed = (clock() + x * y);
    srand(seed);
    if (rand()%100 <= 7) {
        add_powerup(powers,x,y,FIRE_UP);
        return;
    }

    srand(seed* 2);
    if (rand()%100 <= 7) {
        add_powerup(powers,x,y,BOMB_UP);
        return;
    }

    srand(seed* 3);
    if (rand()%100 <= 7) {
        add_powerup(powers,x,y,SPEED_UP);
        return;
    }

    srand(seed* 4);
    if (rand()%100 <= 3) {
        add_powerup(powers,x,y,BOMB_LINE);
        return;
    }
    srand(seed* 5);
    if (rand()%100 <= 4) {
        add_powerup(powers,x,y,KICK);
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