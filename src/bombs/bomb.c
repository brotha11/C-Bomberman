#include "bomb.h"
#include "fire.h"
#include "../entities/player.h"

void add_bomb(Bomb** head, Collision** colls, Player* owner_bomb, Entity** e_head, int col, int row, int blast, int btype, double* delta) {
    Bomb* new_bomb = (Bomb*)malloc(sizeof(Bomb));
    

    // This order is important, since the head updates when creating a new collision
    add_collision(colls, col, row, BOMB_HB, BOMB_HB, COLL_BOMB);
    new_bomb->coll = *colls; // Apunta a la colisión recién creada
    add_collision(colls, col+BOMB2_DIFF, row+BOMB2_DIFF, BOMB2_HB, BOMB2_HB, COLL_BOMB);
    new_bomb->coll2 = *colls; // Apunta a la colisión recién creada

    new_bomb->move = new_entity(e_head, col, row, 16, 16, BOMB_KICK_SPEED, delta);
    new_bomb->x = col;
    new_bomb->y = row;
    new_bomb->blast_radius = blast;

    new_bomb->timer = new_timer(BOMB_TIMER);

    // Set bomb type
    new_bomb->type = btype;

    new_bomb->width = BOMB_HB; new_bomb->height = BOMB_HB;
    new_bomb->kick_x = 0; new_bomb->kick_y = 0;

    //new_bomb->x_offset = new_bomb->y_offset = 0;

    new_bomb->owner = owner_bomb;
    if (new_bomb->owner) new_bomb->owner->bombs_placed++;

    new_bomb->sprite = new_sprite(16,16);
    new_bomb->sprite.image_speed = BOMB_IMG_SPEED;
    new_bomb->sprite.frame_x_max = 4;
    new_bomb->sprite.x_off = 16-BOMB_HB;
    new_bomb->sprite.y_off = 16-BOMB_HB - 1;
    new_bomb->sprite.h_mult = 0.65;

    // Play sound for placing bomb
    play_sound(SFX_PLACE_BOMB);

    new_bomb->next = *head;
    *head = new_bomb;
}

void b_update(Bomb** head, Fire** fires, Collision** collision, Power_up** powers, Entity** e_head) {
    Bomb* current = *head;
    Bomb* next_bomb = current;

    while(current != NULL) {

        next_bomb = current->next;

        if (current->x == -1234) {
            current = next_bomb;
            continue;  
        }

        // Save offset
        //current->x_offset = base_x%16;
        //current->y_offset = base_y%16;

        // Kicked
        if (current->move->hspeed + current->move->vspeed == 0 && current->kick_x + current->kick_y != 0) {
            play_sound(SFX_KICK);
        }

        current->move->hspeed = BOMB_KICK_SPEED * current->kick_x;
        current->move->vspeed = BOMB_KICK_SPEED * current->kick_y;

        if (current->kick_x != 0 || current->kick_y != 0) {
            e_move_all(current->move, collision, powers, e_head);

            current->x = current->move->x;
            current->y = current->move->y;

            if (current->move->last_x == current->move->x && current->kick_x != 0) {
                current->kick_x = 0;
                current->coll->x = current->x;
                current->coll2->x = current->x+BOMB2_DIFF;

                // Snap back into grid
                int tile_x, tile_y;
                get_tile_position(&tile_x, &tile_y, current->x, current->y, 16, 16);

                current->x = tile_x;
                current->y = tile_y;

            }
            else if (current->move->last_y == current->move->y && current->kick_y != 0) {
                current->kick_y = 0;
                current->coll->x = current->x;
                current->coll2->x = current->x+BOMB2_DIFF;

                // Snap back into grid
                int tile_x, tile_y;
                get_tile_position(&tile_x, &tile_y, current->x, current->y, 16, 16);

                current->x = tile_x;
                current->y = tile_y;
            }
            else {
                current->coll->x = -512;
                current->coll2->x = -512;
            }
        } else {
            current->coll->x = current->x;
            current->coll->y = current->y;
            current->coll2->x = current->x+BOMB2_DIFF;
            current->coll2->y = current->y+BOMB2_DIFF;
            current->move->x = current->x;
            current->move->y = current->y;
        }

        // Animation

        // Bomb sprite
        current->sprite.frame_y = current->type;

    
        // Scale
        if (current->timer.time > BOMB_EXPLODE_CRITICAL) normalize_scale(&current->sprite, 1, 0.175, current->move->p_delta_time);
        else normalize_scale(&current->sprite, 2, 0.15, current->move->p_delta_time);

        animate_sprite(&current->sprite, current->move->p_delta_time);

        if (tick_timer(&current->timer, current->move->p_delta_time) == 0) {}
        else {
            b_explode(head, current, fires, collision, powers, e_head);
            current = next_bomb;
        }

        current = next_bomb;
    }
}

void b_explode(Bomb** head, Bomb* bomb, Fire** fires, Collision** collision, Power_up** powers, Entity** e_head) {

    int tile_x, tile_y;
    get_tile_position(&tile_x, &tile_y, bomb->x, bomb->y, 16, 16);

    bomb->x = tile_x;
    bomb->y = tile_y;

    add_fire(fires, head, collision, powers, bomb->owner, bomb->x, bomb->y, UP, bomb->blast_radius, bomb->type, true, true);
    add_fire(fires, head, collision, powers, bomb->owner, bomb->x, bomb->y, DOWN, bomb->blast_radius, bomb->type,true, true);
    add_fire(fires, head, collision, powers, bomb->owner, bomb->x, bomb->y, LEFT, bomb->blast_radius, bomb->type, true, true);
    add_fire(fires, head, collision, powers, bomb->owner, bomb->x, bomb->y, RIGHT, bomb->blast_radius, bomb->type, true, true);

    play_sound(SFX_BLOW_UP_R);

    free_bomb(head, collision, bomb, e_head);
}

Bomb* coll_bomb(Bomb** head, int x, int y, int width, int height) {
    Bomb* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
            current = current->next;
            continue;  
        }

        // Verificamos si hay colisión entre la hitbox del personaje y el bloque
        if (x + width > current->x && x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
            y + height > current->y && y < current->y + current->height) {  // Revisa si el personaje está dentro de la altura del bloque
            return current;  // Hay colisión
        }
        // Verificamos si hay colisión entre la hitbox del personaje y el bloque
        if (x + width > current->x+BOMB2_DIFF && x < current->x+BOMB2_DIFF + BOMB2_HB &&  // Revisa si el personaje está dentro del ancho del bloque
            y + height > current->y+BOMB2_DIFF && y < current->y+BOMB2_DIFF + BOMB2_HB) {  // Revisa si el personaje está dentro de la altura del bloque
            return current;  // Hay colisión
        }

        current = current->next;
    }
    return NULL;  // No hay colisión
}

Bomb* coll_bomb_ext(Bomb** head, int x, int y, int true_x, int true_y, int width, int height) {
    Bomb* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
            current = current->next;
            continue;  
        }
        if (true_x + width > current->x && true_x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
            true_y + height > current->y && true_y < current->y + current->height) {
            //current = current->next;
            //continue;
        } else {
            // Verificamos si hay colisión entre la hitbox del personaje y el bloque
            if (x + width > current->x && x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
                y + height > current->y && y < current->y + current->height) {  // Revisa si el personaje está dentro de la altura del bloque
                return current;  // Hay colisión
            }
        }

        // Segunda verificación, ahora estructurada igual
        if (true_x + width > current->x + BOMB2_DIFF && true_x < current->x + BOMB2_DIFF + BOMB2_HB && 
            true_y + height > current->y + BOMB2_DIFF && true_y < current->y + BOMB2_DIFF + BOMB2_HB) {
            //current = current->next;
            //continue;
        } else {
            if (x + width > current->x + BOMB2_DIFF && x < current->x + BOMB2_DIFF + BOMB2_HB && 
                y + height > current->y + BOMB2_DIFF && y < current->y + BOMB2_DIFF + BOMB2_HB) {
                return current;  // Hay colisión
            }
        }


        current = current->next;
    }
    return NULL;  // No hay colisión
}

void free_bomb(Bomb** head, Collision** head_coll, Bomb* bomb, Entity** e_head) {
    Bomb *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == bomb) { 
        *head = temp->next; // Changed head 

        if (temp->x != -1234) {
            if (temp->owner) temp->owner->bombs_placed--;
        }
        free_entity(e_head, temp->move);
        free_single(head_coll, temp->coll);
        free_single(head_coll, temp->coll2);
        free(temp); // free old head 

        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp != bomb) { 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) 
        return; 
  
    // Unlink the node from linked list 
    prev->next = temp->next; 
  
    if (temp->x != -1234) {
        if (temp->owner) temp->owner->bombs_placed--;
    }
    free_entity(e_head, temp->move);
    free_single(head_coll, temp->coll);
    free_single(head_coll, temp->coll2);
    free(temp); // Free memory 
}

void free_all_bombs(Bomb** head, Collision** head_coll, Entity** e_head) {
    Bomb* current = *head;
    Bomb* next;

    while (current != NULL) {
        next = current->next;  // Guardar referencia al siguiente nodo
        if (current->x != -1234) {
            if (current->owner) current->owner->bombs_placed--;
        }
        free_entity(e_head, current->move);
        free_single(head_coll, current->coll);
        free_single(head_coll, current->coll2);
        free(current);
        current = next;  // Mover al siguiente nodo
    }

    *head = NULL;  // Evitar accesos a memoria liberada
}