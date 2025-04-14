#include "collision.h"

void add_collision(Collision** head, int x, int y, int width, int height) {
    Collision* new_Collision = (Collision*)malloc(sizeof(Collision));

    new_Collision->x = x;
    new_Collision->y = y;
    new_Collision->width = width;
    new_Collision->height = height;
    new_Collision->next = *head;
    *head = new_Collision;
}

Collision* coll_meeting(Collision** head, int x, int y, int width, int height) {
    Collision* current = *head;
    
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

Collision* coll_meeting_ext(Collision** head, int x, int y, int true_x, int true_y, int width, int height) {
    Collision* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
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

bool coll_point(Collision* head, int x, int y) {
    Collision* current = head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
            current = current->next;
            continue;  
        }

        // Verificamos si hay colisión entre la hitbox del personaje y el bloque
        if (x > current->x && x < current->x + current->width &&  // Revisa si el personaje está dentro del ancho del bloque
            y > current->y && y < current->y + current->height) {  // Revisa si el personaje está dentro de la altura del bloque
            return true;  // Hay colisión
        }

        current = current->next;
    }
    return false;  // No hay colisión
}

void free_collisions(Collision** head) {
    Collision* current = *head;
    Collision* next_Collision;
    
    while (current != NULL) {
        next_Collision = current->next;
        free(current);
        current = next_Collision;
    }

    *head = NULL;
}

void free_single(Collision** head, Collision* coll) {
    Collision *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == coll) { 
        *head = temp->next; // Changed head 
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
  
    free(temp); // Free memory 
}

int sign(int x) {
    if (x > 0) return 1;
    else if (x < 0) return -1;
    else return 0;
}