#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "../input/controller.h"
#include "../bombs/bomb.h"
#include "../pickups/powerup.h"
#include "time.h"
#include "../game/timer.h"

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
    P_HOLD = 4,
    P_THROW = 8,
    P_KICK = 10
} PLAYER_ACTION;

#define ANIM_SPEED 0.1333

#define DEATH_TIMER_MAX 1.6666
#define DEATH_START DEATH_TIMER_MAX - 0.41
#define DEATH_END DEATH_START - 0.5

#define KILL_CELEBRATION 1.333

#define BOMB_COOLDOWN 0.2
#define ACTION_COOLDOWN 0.26

struct player {

    Entity* base;
    int move_x, move_y;
    int blast_power;
    int bomb_amount, bombs_placed;
    int bomb_action;
    int kick_power;
    int id;
    int action;
    int speed_ups;

    int bomb_type;

    Timer death_timer;
    Timer kill_celebration_timer;
    Timer action_timer;
    Timer bomb_placed_timer;

    bool bomb_released;
    bool player_on;
    bool burned;
    bool played_death_cry;
    bool com;
};
typedef struct player Player;

Player new_player(Entity** head, int x, int y, int width, int height, double max, int id, double* delta);
void p_update(Player* player, Controller* controller, Entity** head, Collision** collision, Bomb** bombs, Fire** fires, Power_up** powers);
void place_bomb(Player* player, Entity** head, Bomb** bombs, Collision** collision);
void place_bomb_line(Player* player, Entity** head, Bomb** bombs, Collision** collision, Power_up** powers);
void player_kill(Player* player, bool burnt);

#endif