#include "fire.h"
#include "bomb.h"

void add_fire(Fire** fires, Bomb** bombs, Collision** collision, Power_up** powers, Player* owner,
    int x, int y, int direction, int length, int type, bool visible, bool center) {
    // Si length es menor o igual a 0, terminamos la recursión

    Fire *new = (Fire*)malloc(sizeof(Fire));

    int m_x = 0, m_y = 0;
    int next_x = 0, next_y = 0;

    new->owner = owner;
    new->type = type;

    new->direction = direction;
    new->length = length;
    new->visible = visible;
    new->swap_visible = visible;
    new->center = center;

    new->time_mult = 1;

    new->timer = new_timer(EXPLOSION_TIME*new->time_mult);

    new->height = 16;
    new->width = 16;

    new->sprite = new_sprite(16,16);
    new->sprite.frame_x_max = 8;

// Para fuego vertical (arriba o abajo)
    if (direction == UP || direction == DOWN) {
        m_x = 6; // Ajuste para alinear correctamente
        new->width = 4; // Reducimos el ancho para fuego vertical

        if (direction == UP) {
            next_y = -16; // Fuego hacia arriba
            if (length == 0) {
                m_y = 4;
                new->height = 16 - 4; // Ajuste de altura para la última propagación
            }
        } else {
            next_y = 16; // Fuego hacia abajo
            if (length == 0) {
                //m_y = -4;
                new->height = 16 - 4; // Ajuste de altura para la última propagación
            }
        }

    } else { // Para fuego horizontal (izquierda o derecha)
        m_y = 6; // Ajuste para alinear correctamente
        new->height = 4; // Reducimos la altura para fuego horizontal

        if (direction == LEFT) {
            next_x = -16; // Fuego hacia la izquierda
            if (length == 0) {
                m_x = 4;
                new->width = 16 - 4; // Ajuste de ancho para la última propagación
            }
        } else {
            next_x = 16; // Fuego hacia la derecha
            if (length == 0) {
               // m_x = -4;
                new->width = 16 - 4; // Ajuste de ancho para la última propagación
            }
        }
    }

    // Asignamos las coordenadas del fuego
    new->x = x + m_x;
    new->y = y + m_y;

    new->sprite.x_off = -abs(m_x);
    new->sprite.y_off = -abs(m_y);

    // Fire image
    if (!center) {
        if (length == 0) {
            new->sprite.frame_y = 1;
        } else {
            new->sprite.frame_y = 2;
        }
    } else {
        new->sprite.frame_y = 0;
    }



    if (center) {
        new->sprite.angle = 0;
        //new->sprite.x_off += 12;
    } else {
        // Angle
        if (direction == UP) {
            new->sprite.angle = 90;
            //new->sprite.x_off += 12;
            //new->sprite.y_off -= 12;
        }
        else if (direction == DOWN) {
            new->sprite.angle = 270;
            //new->sprite.x_off -= 16;
            //new->sprite.y_off += 12;

        }
        else if (direction == LEFT) {
            new->sprite.angle = 0;
           // new->sprite.y_off -= 16;
        }
        else {
            new->sprite.angle = 180;
        }
    }

    new->sprite.center_point.x = 16/2;
    new->sprite.center_point.y = 16/2;


    // Apply color
    new->sprite.frame_y += (3*type);

    // Insertamos el nuevo segmento de fuego en la lista
    new->next = *fires;
    *fires = new;

    if (length > 0)
    {
        //Bomb* cbomb = coll_bomb(bombs, new->x + next_x - m_x, new->y + next_y - m_y, new->width, new->height);
        Collision* cblock = coll_meeting(collision, new->x + next_x - m_x, new->y + next_y - m_y, new->width, new->height);
        Power_up* cpower = grab_powerup(powers, new->x + next_x - m_x, new->y + next_y - m_y, 16, 16);

        int follow = length - 1;
        bool vis = true;

        // Judge collisions
        if (cblock || cpower) {
            if (cpower) {
                // Stop blow at power up
                if (type != BT_PIERCING) {
                    follow = 0;
                    vis = false;
                }
            }
            if (cblock) {
                // Stop piercing blow only on non-brick blocks
                if (type != BT_PIERCING || cblock->type != BRICK) {
                    follow = 0;
                    vis = false;
                }
            }
        }

        // Generate fire
        add_fire(fires, bombs, collision, powers, owner, new->x + next_x - m_x,
            new->y + next_y - m_y, direction, follow, type, vis, false);
    }
}


void f_update(Fire** fires, Bomb** bombs, double* delta) {
    Fire* current = *fires;
    Fire* next_f;

    while(current != NULL) {
        next_f = current->next;

        if (current->x == -1234) {
            current = next_f;
            continue;
        }

        //Blow up bombs
        Bomb* blow = coll_bomb(bombs,current->x,current->y,current->width, current->height);
        if (blow) {
            if (blow->timer.time > BOMB_EXPLODE_CRITICAL) blow->timer.time = BOMB_EXPLODE_CRITICAL;
        }

        if (current->timer.time == 1) {
            current->swap_visible = false;
            current->visible = false;
        }

        // Animation
        if (current->swap_visible) {
            Fire* coll = coll_fire_exclude(fires, current, current->x, current->y, current->width, current->height);

            if (coll && coll->timer.time > current->timer.time && !current->center) {
                current->visible = false;
            } else {
                current->visible = true;
            }
        }
        animate_sprite_timer(&current->sprite, current->timer.time, EXPLOSION_TIME*current->time_mult, delta);

        if (tick_timer(&current->timer, delta) == 0) {}
        else {
            free_fire(fires,current);
            current = next_f;
        }

        current = next_f;
    }
}

Fire* coll_fire(Fire** head, int x, int y, int width, int height) {
    Fire* current = *head;
    
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


Fire* coll_fire_exclude(Fire** head, Fire* ignore, int x, int y, int width, int height) {
    Fire* current = *head;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->x == -1234) {
            current = current->next;
            continue;  
        }

        if (current == ignore) {
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
