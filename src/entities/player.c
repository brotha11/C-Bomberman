#include "player.h"

Player new_player(Entity** head, int x, int y, int w, int h, double max, int id, double* delta) {

    Player new;

    new.base = new_entity(head, x, y, w, h, max, delta);
    new.blast_power = 2;
    new.bomb_amount = 1;
    new.action = P_NONE;
    new.bomb_type = BT_REGULAR;

    new.bombs_placed = 0;
    new.bomb_action = 0;
    new.kick_power = 0;

    new.base->sprite.frame_x_max = 4;
    new.base->sprite.x_off = -2;
    new.player_on = false;
    new.id = id;
    new.com = false;
    new.played_death_cry = false;

    // Timers
    new.bomb_placed_timer = new_timer(BOMB_COOLDOWN);
    new.action_timer = new_timer(0);
    new.death_timer = new_timer(DEATH_TIMER_MAX);
    new.kill_celebration_timer = new_timer(0);

    return new;
}

void p_update(Player* player, Controller* controller, Entity** head, Collision** collision, Bomb** bombs, Fire** fires, Power_up** powers) {

    if (player->base->alive) {

        if (!player->com) {
            player->move_x = (int)(controller->key_right) - (int)(controller->key_left);
            player->move_y = (int)(controller->key_down) - (int)(controller->key_up);
        } else {
            player->move_x = 1;
            player->move_y = 1;  
        }

        player->base->hspeed = player->base->max_speed * player->move_x;
        player->base->vspeed = player->base->max_speed * player->move_y;
        
        // Move

        e_move(player->base, collision);

        // Facing

        if (player->action != P_KICK && player->action != P_THROW) { // Dont update facing when kicking or throwing
            if (player->base->hspeed != 0 && player->base->vspeed == 0) {
                player->base->facing_y = 0;
                player->base->facing_x = e_sign(player->base->hspeed);
            } else if (player->base->hspeed == 0 && player->base->vspeed != 0) {
                player->base->facing_y = e_sign(player->base->vspeed);
                player->base->facing_x = 0;
            }
            
            if (player->base->hspeed != 0 && player->base->vspeed != 0) {
                if ((player->base->last_x - player->base->x) != 0 && (player->base->last_y - player->base->y) == 0) {
                    player->base->facing_y = 0;
                    player->base->facing_x = e_sign((player->base->x - player->base->last_x));
                } else if ((player->base->last_x - player->base->x) == 0 && (player->base->last_y - player->base->y) != 0) {
                    player->base->facing_y = e_sign((player->base->y - player->base->last_y));
                    player->base->facing_x = 0;
                }
            }
        }

        // Action manager
        switch(player->action) {
            case P_KICK:
            case P_THROW:
                player->base->sprite.frame_x_max = 2;
                break;
            case P_NONE:
            case P_HOLD:
                player->base->sprite.frame_x_max = 4;
                break;
            default:
                break;
        }
        // Reduce action timer
        if (tick_timer(&player->action_timer, player->base->p_delta_time) == 0) {}
        if (player->action_timer.time <= (*player->base->p_delta_time)) {
            switch(player->action) {
                case P_KICK:
                case P_THROW:
                    player->action = P_NONE;
                    player->base->sprite.frame = 0;
                    break;
                default:
                    break;
            }
        }

        // Place bomb
        tick_timer(&player->bomb_placed_timer, player->base->p_delta_time);

        if (controller->key_bomb) {
            if (player->bombs_placed < player->bomb_amount && player->bomb_placed_timer.time == 0) 
                place_bomb(player, head, bombs, collision);

            // Standing on bomb actions
           if (coll_bomb(bombs, player->base->x, player->base->y, 16, 16) 
                && player->bomb_released && player->bomb_placed_timer.time < BOMB_COOLDOWN/2) {
                switch(player->bomb_action) {
                    case BOMB_LINE:
                        place_bomb_line(player, head, bombs, collision, powers);
                        break;
                    default:
                        break;
                }
           }
            player->bomb_released = false;

        } else player->bomb_released = true;

        // Bomb effects

        // Kick
        if (player->kick_power == 1) {
            Bomb* coll_x = (coll_bomb_ext(bombs,player->base->x + player->move_x, player->base->y,
                    player->base->x, player->base->y, 16, 16));
            Bomb* coll_y = (coll_bomb_ext(bombs,player->base->x, player->base->y + player->move_y,
                    player->base->x, player->base->y, 16, 16));
            
            if (coll_x) {
                if (abs(coll_x->y - player->base->y) <= 6 && coll_x->kick_x == 0) {
                    //play_sound(SFX_KICK);
                    coll_x->kick_x = player->move_x;

                    // Set timer
                    if (player->action != P_KICK) {
                        player->action = P_KICK;
                        player->action_timer.time = ACTION_COOLDOWN;
                    }
                }
            }
            if (coll_y) {
                if (abs(coll_y->x - player->base->x) <= 6 && coll_y->kick_y == 0) {
                    //play_sound(SFX_KICK);
                    coll_y->kick_y = player->move_y;

                    // Set timer
                    if (player->action != P_KICK) {
                        player->action = P_KICK;
                        player->action_timer.time = ACTION_COOLDOWN;
                    }
                }
            }
        }

        // Grab Power Ups
        Power_up* grab = grab_powerup(powers, player->base->x, player->base->y, player->base->width, player->base->height);

        if (grab != NULL && grab->grabable == 1) {
            switch(grab->power_up_type) {
            case FIRE_UP:
                player->blast_power++;
                break;
            case BOMB_UP:
                player->bomb_amount++;
                break;
            case SPEED_UP:
                player->base->max_speed += 0.1;
                break;
            case BOMB_LINE:
                player->bomb_action = BOMB_LINE;
                break;
            case KICK:
                player->kick_power = 1;
                break;
            case BOMB_PIERCING:
                player->bomb_type = BT_PIERCING;
                break;
            default:
                break;
            }
            play_sound(SFX_POWER_UP);
            free_powerup(powers, grab);
        }

        //get killed

        // Explosion
        Fire* fire = coll_fire(fires,player->base->x,player->base->y,player->base->width, player->base->height);
        if (fire) {
            if (player->base->alive) 
                fire->owner->kill_celebration_timer.time = KILL_CELEBRATION;
            player_kill(player, true);
        }


        // Reduce kill timer
        tick_timer(&player->kill_celebration_timer, player->base->p_delta_time);

    } // ALIVE END IF
    // Reduce death timer
    else {
        tick_timer(&player->death_timer, player->base->p_delta_time);

        if (player->base->sprite.frame == 7) {
            player->base->sprite.image_speed = -1;
        }
    }

    // Update sprite

    // Shake
    e_shake(&player->base->shake_timer, &player->base->shake_x, &player->base->shake_y,
        0.03, 1, player->base->p_delta_time);


    // States
    if (player->base->alive) { // Alive
        // Animation speed
        if (player->base->hspeed == 0 && player->base->vspeed == 0) {
            if (player->base->sprite.frame == 0 || player->base->sprite.frame == 2) {
                player->base->sprite.image_speed = 0;
                player->base->sprite.frame = 0;
            }
        }
        else {
            player->base->sprite.image_speed = ANIM_SPEED / player->base->max_speed;
        }

        //player->base->sprite.frame_x_max = 4;
        player->base->sprite.y_off = (16-26);

        // Asign sprites

        if (player->base->facing_y == 1) player->base->sprite.frame_y = P_DOWN/* + player->action * player->base->sprite.frame_x_max*/;
        else if (player->base->facing_y == -1) player->base->sprite.frame_y = P_UP /*+ player->action * player->base->sprite.frame_x_max*/;
        if (player->base->facing_x == -1) player->base->sprite.frame_y = P_LEFT /*+ player->action * player->base->sprite.frame_x_max*/;
        else if (player->base->facing_x == 1) player->base->sprite.frame_y = P_RIGHT /*+ player->action * player->base->sprite.frame_x_max*/;

        // Action sprite
        player->base->sprite.frame_x = player->action;

    } else { // Dead
        if (player->burned) player->base->sprite.frame_y = P_BLOWN;
        else player->base->sprite.frame_y = P_DEAD;

        if (player->death_timer.time > DEATH_START) {
            player->base->sprite.frame = 0;
            player->base->sprite.image_speed = 0;
        } else {
            // Play death cry
            if (player->death_timer.time <= DEATH_START && !player->played_death_cry) {
                unsigned seed = (clock() + player->base->x * player->base->y);

                int plus = round(seed % DEATH_VC);
                play_sound(SFX_DEATH_VC0+plus);

                player->played_death_cry = true;
            }

            if (player->base->sprite.image_speed != -1) {
                player->base->sprite.frame_x_max = 8;
                player->base->sprite.image_speed = 0.044;
            }
        }
    }

    if (player->action == P_NONE || player->action == P_HOLD) {
        animate_sprite(&player->base->sprite, player->base->p_delta_time);
    } else {
        animate_sprite_timer(&player->base->sprite, player->action_timer.time, ACTION_COOLDOWN, player->base->p_delta_time);
    }
}

void place_bomb(Player* player, Entity** head, Bomb** bombs, Collision** collision) {

    int tile_x, tile_y;
    get_tile_position(&tile_x, &tile_y, player->base->x, player->base->y, 16, 16);

    if (!coll_bomb(bombs, tile_x, tile_y, 16, 16)) {
        add_bomb(bombs, collision, player, head, tile_x, tile_y, player->blast_power, player->bomb_type, player->base->p_delta_time);
        player->bomb_placed_timer.time = BOMB_COOLDOWN;
    }
}

void place_bomb_line(Player* player, Entity** head, Bomb** bombs, Collision** collision, Power_up** powers) {
    int index = 1;  // Iniciamos el índice para colocar bombas

    // Recorremos las posiciones en la dirección en la que el jugador está mirando
    while (player->bombs_placed < player->bomb_amount) {
        int tile_x, tile_y;
        get_tile_position(&tile_x, &tile_y, player->base->x + (player->base->facing_x * index*16),
             player->base->y + player->base->facing_y * index*16, 16, 16);
        // Verificamos si la bomba colisionaría con otra bomba o pared
        if (coll_meeting(collision, tile_x, tile_y, 16, 16) || grab_powerup(powers, tile_x, tile_y, 16, 16)) {
            break;  // Detenemos la colocación de bombas si hay una colisión
        }

        // Colocamos la bomba en la posición calculada
        add_bomb(bombs, collision, player, head, tile_x, tile_y, player->blast_power, player->bomb_type, player->base->p_delta_time);

        // Aumentamos el índice para colocar la siguiente bomba más lejos
        ++index;
    }
}

void player_kill(Player* player, bool burnt) {
    if (player->base->alive) {
        play_sound(SFX_DEATH);

        player->burned = burnt;
        player->base->alive = false;
        player->death_timer.time = DEATH_TIMER_MAX;
        player->base->shake_timer.time = fabs(DEATH_TIMER_MAX - DEATH_START) * 0.8;
        player->action = P_NONE;
        player->base->sprite.frame_x = 0;
    }
}
