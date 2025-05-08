#ifndef GAME_H
#define GAME_h

#define DUMMY -1234

// Libraries
#include "stdbool.h"
#include "stdio.h"
#include "time.h"

// Game libraries
#include "../graphics/graphics.h"
#include "../audio/sound.h"
#include "../modes/battle/battle_mode_manager.h"
#include "../input/controller.h"
#include "../rooms/camera.h"
#include "../audio/music.h"
#include "timer.h"
#include "event.h"

struct game
{
    bool game_running;
    Graphics graphics;
    Controller controllers[MAX_BATTLE_PLAYERS];
    Music_manager music;
    Battle_manager battle;

    int base_x, base_y;

    Profile main_profile;
    Profile type0_profile;
    Profile type1_profile;
    Profile nouse_profile;

    double delta_time;
};
typedef struct game Game;

void init_game(Game* game);
void run_game(Game* game);
void start_room(Game* game);
void free_game(Game* game);

void update(Game* game);
void render(Game* game);

#endif