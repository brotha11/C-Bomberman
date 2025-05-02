#include "powerup.h"
#include "../bombs/fire.h"

void add_powerup(Power_up** head, int x, int y, int power_type, double* delta) {
    Power_up* new = (Power_up*)malloc(sizeof(Power_up));

    if (new == NULL) {
        printf("Error al asignar memoria para Power_up\n");
        return;
    }

    new->x = x;
    new->y = y;

    new->height = 16;
    new->width = 16;

    new->grabable = 1;
    new->timer = new_timer(EXPLOSION_TIME);
    new->visible = true;

    new->power_up_type = power_type;
    new->sprite = new_sprite(16,16);
    new->sprite.frame_x_max = 4;
    new->sprite.image_speed = PW_IMG_SPEED;
    new->sprite.x_off = 0;
    new->sprite.y_off = 0;
    new->p_delta_time = delta;

    new->sprite.frame_y = power_type;

    new->next = *head;
    *head = new;
}

void pw_update(Power_up** head, Fire** fires) {
    if (*head == NULL) return;

    Power_up* current = *head;
    Power_up* next = current->next;

    while(current != NULL) {
        next = current->next;

        if (current->x == -1234) {
            current = next;
            continue;
        }

        Fire* fire = coll_fire(fires, current->x, current->y, current->width, current->height);

        if (fire && (fire->visible || !fire->visible && fire->timer.time > EXPLOSION_TIME/4) && current->grabable == 1) {
            current->grabable = 0;
            current->sprite.frame_x = 0;
            current->sprite.frame_y = 0;
            current->sprite.y_off = -20;
            current->sprite.height = 32;
            current->sprite.frame_x_max = 8;
        }

        if (current->grabable == 0) {
            if (tick_timer(&current->timer, current->p_delta_time) == 0) {}
            else {
                free_powerup(head, current);
                current = next;
                continue;
            }
            if (current->timer.time <= (*current->p_delta_time)) {
                current->visible = false;
            }
            animate_sprite_timer(&current->sprite, current->timer.time, EXPLOSION_TIME, current->p_delta_time);
        } else {
            animate_sprite(&current->sprite, current->p_delta_time);
        }


        current = next;
    }
}

Power_up* grab_powerup(Power_up** head, int x, int y, int width, int height) {
    if (*head == NULL) {
        return NULL;
    }

    Power_up* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
            current = current->next;
            continue;
        }

        // Verificamos si hay colisión entre la hitbox del personaje y el bloque
        if (x + width > current->x+SIZE && x < current->x+SIZE + current->width-SIZE &&  // Revisa si el personaje está dentro del ancho del bloque
            y + height > current->y+SIZE && y < current->y+SIZE + current->height-SIZE) {  // Revisa si el personaje está dentro de la altura del bloque
            return current;  // Hay colisión
        }

        current = current->next;
    }
    return NULL;  // No hay colisión
}

Power_up* coll_powerup(Power_up** head, int x, int y, int width, int height) {
    if (*head == NULL) {
        return NULL;
    }

    Power_up* current = *head;
    
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

        current = current->next;
    }
    return NULL;  // No hay colisión
}

void power_up_blow(Power_up** head, Power_up* power) {
    free_powerup(head, power);
}

void free_powerup(Power_up** head, Power_up* power) {
    Power_up *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == power) { 
        *head = temp->next; // Changed head 
        free(temp); // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp != power) { 
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

void free_all_powerups(Power_up** head) {
    Power_up* current = *head;
    Power_up* next;

    while (current != NULL) {
        next = current->next;  // Guardar referencia al siguiente nodo
        free(current);
        current = next;  // Mover al siguiente nodo
    }

    *head = NULL;  // Evitar accesos a memoria liberada
}
