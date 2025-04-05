#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "input/controller.h"
#include "bomb.h"

struct player {

    Entity base;
    int move_x, move_y;
    int blast_power;

    bool bomb_placed;
};
typedef struct player Player;

Player new_player(int x, int y, int width, int height, float max, int spr);
void p_update(Player* player, Controller* controller, Collision* collision, Bomb** bombs, Fire** fires);
void place_bomb(Player* player, Bomb** bombs);

#endif