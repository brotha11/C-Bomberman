#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "../input/controller.h"
#include "../bombs/bomb.h"
#include "../pickups/powerup.h"
#include "time.h"

typedef enum PLAYER_SPRITE {
    P_DOWN,
    P_UP,
    P_LEFT,
    P_RIGHT,
    P_DEAD,
    P_BLOWN
} PLAYER_SPRITE;

typedef enum PLAYER_ACTION {
    P_NONE,
    P_HOLD,
    P_THROW,
    P_KICK
} PLAYER_ACTION;

#define ANIM_SPEED 8

#define DEATH_TIMER_MAX 100
#define DEATH_START DEATH_TIMER_MAX - 25
#define DEATH_END DEATH_START - 30

#define KILL_CELEBRATION 80

#define BOMB_COOLDOWN 12
#define ACTION_COOLDOWN 12

struct player {

    Entity* base;
    int move_x, move_y;
    int blast_power;
    int bomb_amount, bombs_placed;
    int bomb_action;
    int kick_power;
    int id;

    int death_timer;
    int kill_celebration_timer;
    int action;
    int action_timer;

    int bomb_placed_timer;
    bool bomb_released;
    bool player_on;
    bool burned;
    bool com;
};
typedef struct player Player;

Player new_player(Entity** head, int x, int y, int width, int height, float max, int id);
void p_update(Player* player, Controller* controller, Entity** head, Collision** collision, Bomb** bombs, Fire** fires, Power_up** powers);
void place_bomb(Player* player, Entity** head, Bomb** bombs, Collision** collision);
void place_bomb_line(Player* player, Entity** head, Bomb** bombs, Collision** collision, Power_up** powers);
void player_kill(Player* player, bool burnt);

#endif