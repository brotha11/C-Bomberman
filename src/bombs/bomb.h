#ifndef BOMB_H
#define BOMB_H

#include "stdlib.h"
#include "../blocks/collision.h"
#include "../graphics/sprite.h"
#include "../pickups/powerup.h"
#include "../entities/entity.h"
#include "../audio/sound.h"
#include "../rooms/tile_manager.h"
#include "../game/timer.h"

#define BOMB_KICK_SPEED 2
#define BOMB_TIMER 3
#define BOMB_HB 16
#define BOMB2_HB 6
#define BOMB2_DIFF (BOMB_HB - BOMB2_HB)/2
#define BOMB_EXPLODE_CRITICAL 0.08

#define BOMB_IMG_SPEED 0.224

typedef enum BOMB_TYPE {
    BT_REGULAR,
    BT_PIERCING
} BOMB_TYPE;

struct fire;
typedef struct fire Fire;

struct player;
typedef struct player Player;

struct bomb {
    Collision* coll;
    Collision* coll2;
    Entity* move;
    Player* owner;

    int x,y;
    int width, height;
    int held;

    Timer timer;
    int blast_radius;
    int type;

    int kick_x, kick_y;
    //int x_offset, y_offset;

    Sprite sprite;
    struct bomb* next;
};
typedef struct bomb Bomb;

void add_bomb(Bomb** head, Collision** colls, Player* owner_bomb, Entity** e_head, int col, int row, int blast, int btype, double* delta);
Bomb* coll_bomb(Bomb** head, int x, int y, int width, int height);
Bomb* coll_bomb_ext(Bomb** head, int x, int y, int true_x, int true_y, int width, int height);
void free_bomb(Bomb** head, Collision** head_coll, Bomb* bomb, Entity** e_head);
void free_all_bombs(Bomb** head, Collision** head_coll, Entity** e_head);

void b_update(Bomb** head, Fire** fire, Collision** collision, Power_up** powers, Entity** e_head);
void b_explode(Bomb** head, Bomb* bomb, Fire** fire, Collision** collision, Power_up** powers, Entity** e_head);

#endif