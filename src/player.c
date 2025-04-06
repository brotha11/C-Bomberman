#include "player.h"

Player new_player(int x, int y, int w, int h, float max, int spr) {

    Player new;

    new.base = new_entity(x,y,w,h,max,spr);
    new.bomb_placed = false;
    new.blast_power = 4;
    new.bomb_amount = 4;

    new.bombs_placed = 0;
    new.death_timer = DEATH_TIMER_MAX;

    new.base.sprite.frame_x_max = 4;

    return new;

}

void p_update(Player* player, Controller* controller, Collision** collision, Bomb** bombs, Fire** fires) {

    if (player->base.alive) {
        player->move_x = (int)(controller->key_right) - (int)(controller->key_left);
        player->move_y = (int)(controller->key_down) - (int)(controller->key_up);

        player->base.hspeed = player->base.max_speed * player->move_x;
        player->base.vspeed = player->base.max_speed * player->move_y;

        // Place bomb
        if (controller->key_bomb) {
            if (!player->bomb_placed && player->bombs_placed < player->bomb_amount) place_bomb(player,bombs, collision);
        } else player->bomb_placed = false;

        e_move(&player->base, collision);

        // Facing
        if (player->base.hspeed != 0 && player->base.vspeed == 0) {
            player->base.facing_y = 0;
            player->base.facing_x = e_sign(player->base.hspeed);
        } else {
            player->base.facing_y = e_sign(player->base.vspeed);
            player->base.facing_x = 0;
        }

        //get killed
        if (coll_fire(fires,player->base.x,player->base.y,player->base.width, player->base.height)) {
            if (player->base.alive) {
                player->base.alive = false;
                player->death_timer = DEATH_TIMER_MAX;
            }
        }
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
            player->base.sprite.image_speed = 0;
            player->base.sprite.frame_x = 0;
        }
        else player->base.sprite.image_speed = 8;

        player->base.sprite.frame_x_max = 4;

        player->base.sprite.y_off = (16-26);

        if (player->base.sprite.frame_y == 2)  player->base.sprite.x_off = 0;
        else player->base.sprite.x_off = -1;

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
        player->bomb_placed = true;
    }
}
