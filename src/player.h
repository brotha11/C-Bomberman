#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "input/controller.h"
#include "bomb.h"

#define P_DOWN 0
#define P_UP 1
#define P_LEFT 2
#define P_RIGHT 3
#define P_DEAD 4
#define P_BLOWN 5

#define DEATH_TIMER_MAX 100
#define DEATH_START DEATH_TIMER_MAX - 25
#define DEATH_END DEATH_START - 30

struct player {

    Entity base;
    int move_x, move_y;
    int blast_power;
    int bomb_amount, bombs_placed;

    int death_timer;

    bool bomb_placed;
};
typedef struct player Player;

Player new_player(int x, int y, int width, int height, float max, int spr);
void p_update(Player* player, Controller* controller, Collision** collision, Bomb** bombs, Fire** fires);
void place_bomb(Player* player, Bomb** bombs, Collision** collision);

#endif