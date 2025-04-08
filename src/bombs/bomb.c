#include "bomb.h"
#include "fire.h"

void add_bomb(Bomb** head, Collision** colls, int* owner_bomb, int col, int row, int blast) {
    Bomb* new_bomb = (Bomb*)malloc(sizeof(Bomb));
    
    add_collision(colls, col, row, 16, 16);

    new_bomb->move = new_entity(col,row,16,16,BOMB_KICK_SPEED,BOMB);

    new_bomb->coll = *colls; // Apunta a la colisión recién creada
    new_bomb->x = col;
    new_bomb->y = row;
    new_bomb->blast_radius = blast;

    new_bomb->timer = BOMB_TIMER;

    new_bomb->width = 16; new_bomb->height = 16;
    new_bomb->kick_x = 0; new_bomb->kick_y = 0;

    new_bomb->owner = owner_bomb;
    (*new_bomb->owner)++;

    new_bomb->sprite = new_sprite(16,16);
    new_bomb->sprite.image_speed = 14;
    new_bomb->sprite.frame_x_max = 4;

    new_bomb->next = *head;
    *head = new_bomb;
}

void b_update(Bomb** head, Fire** fires, Collision** collision, Power_up** powers) {
    Bomb* current = *head;
    Bomb* next_bomb = current;

    while(current != NULL) {

        next_bomb = current->next;

        if(current->timer != 0) current->timer--;
        else {
            b_explode(head, current, fires, collision, powers);
        }

        // Kicked
        current->move.hspeed = BOMB_KICK_SPEED * current->kick_x;
        current->move.vspeed = BOMB_KICK_SPEED * current->kick_y;

        if (current->kick_x !=0 || current->kick_y != 0) {
            e_move_all(&current->move, collision, powers);

            current->x = current->move.x;
            current->y = current->move.y;

            if (current->move.last_x == current->move.x && current->kick_x != 0) {
                current->kick_x = 0;
                current->coll->x = current->x;

                // Snap back into grid
                int tile_x, tile_y;
                get_tile_position(&tile_x, &tile_y, current->x, current->y, 16, 16);

                current->x = tile_x;
                current->y = tile_y;

            }
            else if (current->move.last_y == current->move.y && current->kick_y != 0) {
                current->kick_y = 0;
                current->coll->x = current->x;

                // Snap back into grid
                int tile_x, tile_y;
                get_tile_position(&tile_x, &tile_y, current->x, current->y, 16, 16);

                current->x = tile_x;
                current->y = tile_y;
            }
            else current->coll->x = -512;
        } else {
            current->coll->x = current->x;
            current->coll->y = current->y;
            current->move.x = current->x;
            current->move.y = current->y;
        }

        // Animation
        animate_sprite(&current->sprite);

        current = next_bomb;
    }
}

void b_explode(Bomb** head, Bomb* bomb, Fire** fires, Collision** collision, Power_up** powers) {

    int tile_x, tile_y;
    get_tile_position(&tile_x, &tile_y, bomb->x, bomb->y, 16, 16);

    bomb->x = tile_x;
    bomb->y = tile_y;

    add_fire(fires, head, collision, powers, bomb->x, bomb->y, UP, bomb->blast_radius,true, true);
    add_fire(fires, head, collision, powers, bomb->x, bomb->y, DOWN, bomb->blast_radius,true, true);
    add_fire(fires, head, collision, powers, bomb->x, bomb->y, LEFT, bomb->blast_radius, true, true);
    add_fire(fires, head, collision, powers, bomb->x, bomb->y, RIGHT, bomb->blast_radius, true, true);

    free_bomb(head, collision, bomb);
}

Bomb* coll_bomb(Bomb** head, int x, int y, int width, int height) {
    Bomb* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        // Verificamos si hay colisión entre la hitbox del personaje y el bloque
        if (x + width > current->x && x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
            y + height > current->y && y < current->y + current->height) {  // Revisa si el personaje está dentro de la altura del bloque
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

void get_tile_position(int* tile_x, int* tile_y, int x, int y, int w, int h) {
    int p_x = x + w / 2;  // Centro del jugador en X
    int p_y = y + h / 2; // Centro del jugador en Y

    *tile_x = p_x / 16;  // Tile en el que está el centro del jugador (horizontal)
    *tile_y = p_y / 16;  // Tile en el que está el centro del jugador (vertical)

    if (p_x % 16 >= 8) (*tile_x)++;

    *tile_x = *tile_x * 16 - 8;
    *tile_y = *tile_y * 16;
}

void free_bomb(Bomb** head, Collision** head_coll, Bomb* bomb) {
    Bomb *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == bomb) { 
        *head = temp->next; // Changed head 
        free_single(head_coll, temp->coll);
        free(temp); // free old head 
        (*temp->owner)--;

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
  
    (*temp->owner)--;
    free_single(head_coll, temp->coll);
    free(temp); // Free memory 
}

void free_all_bombs(Bomb** head, Collision** head_coll) {
    Bomb* current = *head;
    Bomb* next;

    while (current != NULL) {
        next = current->next;  // Guardar referencia al siguiente nodo
        free_single(head_coll, current->coll);
        free(current);
        current = next;  // Mover al siguiente nodo
    }

    *head = NULL;  // Evitar accesos a memoria liberada
}