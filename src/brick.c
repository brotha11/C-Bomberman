#include "brick.h"

void add_brick(Brick** bricks, Collision** head, int x, int y, int width, int height) {
    Brick* new_brick = (Brick*)malloc(sizeof(Brick));

    add_collision(head, x, y, width, height); // Agrega la colisión a la lista

    new_brick->coll = *head; // Apunta a la colisión recién creada
    new_brick->broken = false;
    new_brick->next = *bricks;
    new_brick->timer = 45;

    *bricks = new_brick;
}

void bri_update(Brick** bricks, Fire** fires, Collision** collision) {
    
    Brick* current = *bricks;
    Brick* next_brick;

    while(current != NULL) {

        next_brick = current->next;

        if (current->broken) {
            if (current->timer != 0) current->timer--;
            else free_brick(bricks, current, collision);
        }

        if (coll_fire(fires,current->coll->x,current->coll->y,current->coll->width, current->coll->height)) {
            current->broken = true;
        }

        current = next_brick;
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