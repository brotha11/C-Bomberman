#include "player.h"

Player new_player(int x, int y, int w, int h, float max, int spr) {

    Player new;

    new.base = new_entity(x,y,w,h,max,spr);
    new.bomb_placed_timer = BOMB_COOLDOWN;
    new.blast_power = 2;
    new.bomb_amount = 1;

    new.bombs_placed = 0;
    new.death_timer = DEATH_TIMER_MAX;
    new.bomb_action = 0;

    new.base.sprite.frame_x_max = 4;
    new.base.sprite.x_off = -2;

    return new;

}

void p_update(Player* player, Controller* controller, Collision** collision, Bomb** bombs, Fire** fires, Power_up** powers) {

    if (player->base.alive) {
        player->move_x = (int)(controller->key_right) - (int)(controller->key_left);
        player->move_y = (int)(controller->key_down) - (int)(controller->key_up);

        player->base.hspeed = player->base.max_speed * player->move_x;
        player->base.vspeed = player->base.max_speed * player->move_y;
        
        // Move

        e_move(&player->base, collision);

        // Facing
        if (player->base.hspeed != 0 && player->base.vspeed == 0) {
            player->base.facing_y = 0;
            player->base.facing_x = e_sign(player->base.hspeed);
        } else if (player->base.hspeed == 0 && player->base.vspeed != 0) {
            player->base.facing_y = e_sign(player->base.vspeed);
            player->base.facing_x = 0;
        }
        

        // Place bomb

        if (player->bomb_placed_timer != 0) player->bomb_placed_timer--;

        if (controller->key_bomb) {
            if (player->bombs_placed < player->bomb_amount && player->bomb_placed_timer == 0) 
                place_bomb(player,bombs, collision);

            // Standing on bomb actions
           if (coll_bomb(bombs, player->base.x, player->base.y, 16, 16) 
                && player->bomb_released && player->bomb_placed_timer < (int)BOMB_COOLDOWN/2) {
                switch(player->bomb_action) {
                    case BOMB_LINE:
                        place_bomb_line(player,bombs, collision);
                        break;
                    default:
                        break;
                }
           }
            player->bomb_released = false;

        } else player->bomb_released = true;


        // Grab Power Ups
        Power_up* grab = grab_powerup(powers, player->base.x, player->base.y, player->base.width, player->base.height);

        if (grab != NULL && grab->grabable == 1) {
            switch(grab->power_up_type) {
            case FIRE_UP:
                player->blast_power++;
                break;
            case BOMB_UP:
                player->bomb_amount++;
                break;
            case SPEED_UP:
                player->base.max_speed += 0.1f;
                break;
            case BOMB_LINE:
                player->bomb_action = BOMB_LINE;
            }
            free_powerup(powers, grab);
        }

        //get killed
        if (coll_fire(fires,player->base.x,player->base.y,player->base.width, player->base.height)) {
            if (player->base.alive) {
                player->base.alive = false;
                player->death_timer = DEATH_TIMER_MAX;
            }
        }
        // Get last x and y
        player->base.last_x = player->base.x;
        player->base.last_y = player->base.y;
    }
    // Reduce death timer
    else {
        if (player->death_timer != 0) player->death_timer--;

        if (player->base.sprite.frame_x == 7) {
            player->base.sprite.image_speed = -1;
        }
    }

    // Update sprite

    // States
    if (player->base.alive) { // Alive
        // Animation speed
        if (player->base.hspeed == 0 && player->base.vspeed == 0) {
            if (player->base.sprite.frame_x == 0 || player->base.sprite.frame_x == 2) {
                player->base.sprite.image_speed = 0;
                player->base.sprite.frame_x = 0;
            }
        }
        else {
            player->base.sprite.image_speed = (int)ANIM_SPEED / player->base.max_speed;
        }

        player->base.sprite.frame_x_max = 4;

        player->base.sprite.y_off = (16-26);

        // Asign sprites

        if (player->base.facing_y == 1) player->base.sprite.frame_y = P_DOWN;
        else if (player->base.facing_y == -1) player->base.sprite.frame_y = P_UP;
        if (player->base.facing_x == -1) player->base.sprite.frame_y = P_LEFT;
        else if (player->base.facing_x == 1) player->base.sprite.frame_y = P_RIGHT;
    } else { // Dead
        player->base.sprite.frame_y = P_BLOWN;

        if (player->death_timer > DEATH_START) {
            player->base.sprite.frame_x = 0;
            player->base.sprite. image_speed = 0;
        } else {
            if (player->base.sprite.image_speed != -1) {
                player->base.sprite.frame_x_max = 8;
                player->base.sprite.image_speed = 4;
            }
        }
    }
    animate_sprite(&player->base.sprite);
}

void place_bomb(Player* player, Bomb** bombs, Collision** collision) {
    int p_x = player->base.x + player->base.width / 2;  // Centro del jugador en X
    int p_y = player->base.y + player->base.height / 2; // Centro del jugador en Y

    int tile_x = p_x / 16;  // Tile en el que está el centro del jugador (horizontal)
    int tile_y = p_y / 16;  // Tile en el que está el centro del jugador (vertical)

    if (p_x % 16 >= 8) tile_x++;

    if (!coll_bomb(bombs, tile_x*16 - 8, tile_y * 16, 16, 16)) {
        add_bomb(bombs, collision, &player->bombs_placed, tile_x * 16 - 8, tile_y * 16, player->blast_power);
        player->bomb_placed_timer = BOMB_COOLDOWN;
    }
}

void place_bomb_line(Player* player, Bomb** bombs, Collision** collision) {
    int index = 1;  // Iniciamos el índice para colocar bombas

    // Recorremos las posiciones en la dirección en la que el jugador está mirando
    while (player->bombs_placed < player->bomb_amount) {
        // Calculamos las posiciones X e Y de las bombas a colocar
        // Si el jugador está mirando a la derecha o izquierda, solo se ajusta X
        int p_x = player->base.x + (player->base.facing_x * index*16) + player->base.width / 2;
        // Si el jugador está mirando arriba o abajo, solo se ajusta Y
        int p_y = player->base.y + (player->base.facing_y * index*16) + player->base.height / 2;

        // Calculamos la posición en términos de tiles
        int tile_x = p_x / 16;  // Tile en el que está el centro de la bomba (horizontal)
        int tile_y = p_y / 16;  // Tile en el que está el centro de la bomba (vertical)

        // Ajustamos si el centro está cerca del borde de un tile
        if (p_x % 16 >= 8) tile_x++;

        // Verificamos si la bomba colisionaría con otra bomba o pared
        if (coll_meeting(collision, tile_x * 16 - 8, tile_y * 16, 16, 16)) {
            break;  // Detenemos la colocación de bombas si hay una colisión
        }

        // Colocamos la bomba en la posición calculada
        add_bomb(bombs, collision, &player->bombs_placed, tile_x * 16 - 8, tile_y * 16, player->blast_power);

        // Aumentamos el índice para colocar la siguiente bomba más lejos
        ++index;
    }
}
