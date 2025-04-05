#include "fire.h"
#include "bomb.h"

void add_fire(Fire** fires, Bomb** bombs, Collision* collision, int x, int y, int direction, int length, bool visible) {
    // Si length es menor o igual a 0, terminamos la recursión

    Fire *new = (Fire*)malloc(sizeof(Fire));

    int m_x = 0, m_y = 0;
    int next_x = 0, next_y = 0;

    new->direction = direction;
    new->length = length;
    new->timer = 45;
    new->visible = visible;

    new->height = 16;
    new->width = 16;

    // Para fuego vertical (arriba o abajo)
    if (direction == UP || direction == DOWN) {
        m_x = 6; // Ajuste para alinear correctamente
        new->width = 4; // Reducimos el ancho para fuego vertical

        if (direction == UP) next_y = -16; // Fuego hacia arriba
        else next_y = 16; // Fuego hacia abajo

    } else { // Para fuego horizontal (izquierda o derecha)
        m_y = 6; // Ajuste para alinear correctamente
        new->height = 4; // Reducimos la altura para fuego horizontal

        if (direction == LEFT) next_x = -16; // Fuego hacia la izquierda
        else next_x = 16; // Fuego hacia la derecha
    }

    // Asignamos las coordenadas del fuego
    new->x = x + m_x;
    new->y = y + m_y;

    // Insertamos el nuevo segmento de fuego en la lista
    new->next = *fires;
    *fires = new;

    if (length > 0)
    {
        if (!coll_bomb(bombs, new->x + next_x - m_x, new->y + next_y - m_y, new->width, new->height) && !coll_meeting(collision, new->x + next_x - m_x, new->y + next_y - m_y, new->width, new->height))
            add_fire(fires, bombs, collision, new->x + next_x - m_x, new->y + next_y - m_y, direction, length - 1, true);
        else add_fire(fires, bombs, collision, new->x + next_x - m_x, new->y + next_y - m_y, direction, 0, false);
    }
}


void f_update(Fire** fires, Bomb** bombs) {
    Fire* current = *fires;
    Fire* next_f;

    while(current != NULL) {
        next_f = current->next;

        //Blow up bombs
        Bomb* blow = coll_bomb(bombs,current->x,current->y,current->width, current->height);
        if (blow) {
            if (blow->timer > 5) blow->timer = 5;
        }

        if (current->timer != 0) current->timer--;
        else {
            free_fire(fires,current);
        }

        current = next_f;
    }
}

Fire* coll_fire(Fire** head, int x, int y, int width, int height) {
    Fire* current = *head;
    
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

void free_fire(Fire** head, Fire* fire) {
    Fire *temp = *head, *prev = NULL;
  
    if (temp != NULL && temp == fire) { 
        *head = temp->next; // Changed head 
        free(temp); // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp != fire) { 
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

void free_all_fires(Fire** head) {
    Fire* current = *head;
    Fire* next;

    while (current != NULL) {
        next = current->next;  // Guardar referencia al siguiente nodo
        free(current);
        current = next;  // Mover al siguiente nodo
    }

    *head = NULL;  // Evitar accesos a memoria liberada
}
