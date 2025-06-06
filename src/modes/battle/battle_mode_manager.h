// Anything regarding the battle mode

#ifndef BATTLE_MODE_MANAGER_H
#define BATTLE_MODE_MANAGER_H

#include "../../entities/entity.h"
#include "../../entities/player.h"
#include "../../blocks/collision.h"
#include "../../bombs/fire.h"
#include "../../bombs/bomb.h"
#include "../../blocks/brick.h"
#include "../../pickups/powerup.h"
#include "../../rooms/camera.h"
#include "../../game/timer.h"
#include "../../audio/music.h"

#define TRANSITION_WIN 2

typedef enum {
    BATTLE_MENU_PLAYERS,
    BATTLE_MENU_RULES,
    BATTLE_MENU_MAP,
    BATTLE_GAME,
    BATTLE_GAME_RESULTS,
    BATTLE_RESULTS
} BATTLE_STATE;

typedef struct battle_manager {
    Player players[MAX_BATTLE_PLAYERS];
    int players_on[MAX_BATTLE_PLAYERS];
    int players_wins[MAX_BATTLE_PLAYERS];

    Bomb* bombs;
    Fire* fires;
    Brick* bricks;
    Power_up* power_ups;
    Collision* collision;
    Entity* entities;

    Music_manager* p_music;
    
    Camera camera;

    Timer battle_time;
    Timer transition_time;
    
    double* p_delta_time;

    int state;
    int win_goal;
    int current_map;
    int restart_music_on_load;

} Battle_manager;

void battle_init(Battle_manager* battle, Music_manager* p_mus, double* delta_time);
void battle_selection(Battle_manager* battle);
void battle_load(Battle_manager* battle, Screen* screen);
void battle_update(Battle_manager* battle, Controller* controllers, Screen* screen);
void battle_free(Battle_manager* battle);

#endif