#ifndef GAME_H
#define GAME_h

#include "stdbool.h"
#include "stdio.h"
#include "graphics/graphics.h"
#include "entities/entity.h"
#include "entities/player.h"
#include "input/controller.h"
#include "blocks/collision.h"
#include "bombs/fire.h"
#include "bombs/bomb.h"
#include "blocks/brick.h"
#include "pickups/powerup.h"
#include "time.h"

struct game
{
    bool game_running;
    Graphics graphics;
    Controller controller;
    Player player;

    Bomb* bombs;
    Fire* fires;
    Brick* bricks;
    Power_up* power_ups;
    Collision* collision;
};
typedef struct game Game;

void init_game(Game* game);
void run_game(Game* game);
void start_room(Game* game);
void free_room(Game* game);
void free_game(Game* game);

void update(Game* game);
void render(Game* game);

#endif