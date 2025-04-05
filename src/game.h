#ifndef GAME_H
#define GAME_h

#include "stdbool.h"
#include "stdio.h"
#include "bomb.h"
#include "graphics/graphics.h"
#include "entity.h"
#include "player.h"
#include "input/controller.h"
#include "collision.h"
#include "fire.h"
#include "brick.h"

struct game
{
    bool game_running;
    Graphics graphics;
    Controller controller;
    Player player;
    Bomb* bombs;
    Fire* fires;
    Brick* bricks;

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