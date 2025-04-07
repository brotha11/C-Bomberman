#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "../input/controller.h"
#include "../bombs/bomb.h"
#include "../pickups/powerup.h"

#define P_DOWN 0
#define P_UP 1
#define P_LEFT 2
#define P_RIGHT 3
#define P_DEAD 4
#define P_BLOWN 5

#define ANIM_SPEED 8

#define DEATH_TIMER_MAX 100
#define DEATH_START DEATH_TIMER_MAX - 25
#define DEATH_END DEATH_START - 30

#define BOMB_COOLDOWN 10

struct player {

    Entity base;
    int move_x, move_y;
    int blast_power;
    int bomb_amount, bombs_placed;
    int bomb_action;

    int death_timer;

    int bomb_placed_timer;
    bool bomb_released;
};
typedef struct player Player;

Player new_player(int x, int y, int width, int height, float max, int spr);
void p_update(Player* player, Controller* controller, Collision** collision, Bomb** bombs, Fire** fires, Power_up** powers);
void place_bomb(Player* player, Bomb** bombs, Collision** collision);
void place_bomb_line(Player* player, Bomb** bombs, Collision** collision);

#endif