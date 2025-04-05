#include "bomb.h"
#include "fire.h"

void add_bomb(Bomb** head, int col, int row, int blast) {
    Bomb* new_bomb = (Bomb*)malloc(sizeof(Bomb));
    
    new_bomb->x = col;
    new_bomb->y = row;
    new_bomb->blast_radius = blast;

    new_bomb->timer = 120;

    new_bomb->width = 16; new_bomb->height = 16;

    new_bomb->next = *head;
    *head = new_bomb;
}

void b_update(Bomb** head, Fire** fires, Collision* collision) {
    Bomb* current = *head;
    Bomb* next_bomb = current;

    while(current != NULL) {

        next_bomb = current->next;

        if(current->timer != 0) current->timer--;
        else {
            b_explode(head, current, fires, collision);
        }

        current = next_bomb;
    }
}

void b_explode(Bomb** head, Bomb* bomb, Fire** fires, Collision* collision) {

    add_fire(fires, head, collision, bomb->x, bomb->y, UP, bomb->blast_radius,true);
    add_fire(fires, head, collision, bomb->x, bomb->y, DOWN, bomb->blast_radius,true);
    add_fire(fires, head, collision, bomb->x, bomb->y, LEFT, bomb->blast_radius, true);
    add_fire(fires, head, collision, bomb->x, bomb->y, RIGHT, bomb->blast_radius, true);

    free_bomb(head,bomb);
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

void free_bomb(Bomb** head, Bomb* bomb) {
    Bomb *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == bomb) { 
        *head = temp->next; // Changed head 
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
  
    free(temp); // Free memory 
}

void free_all_bombs(Bomb** head) {
    Bomb* current = *head;
    Bomb* next;

    while (current != NULL) {
        next = current->next;  // Guardar referencia al siguiente nodo
        free(current);
        current = next;  // Mover al siguiente nodo
    }

    *head = NULL;  // Evitar accesos a memoria liberada
}