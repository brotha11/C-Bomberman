#ifndef FIRE_H
#define FIRE_H

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define EXPLOSION_TIME 0.48

#include "../blocks/collision.h"
#include "../graphics/sprite.h"
#include "../pickups/powerup.h"
#include "../game/timer.h"

struct bomb;
typedef struct bomb Bomb;

struct player;
typedef struct player Player;

struct brick;
typedef struct brick Brick;

struct fire {

    int x, y;
    int width, height;
    int type;
    Timer timer;
    double time_mult;
    int length;
    int direction;
    bool visible;
    bool swap_visible;
    bool center;

    Player* owner;
    Sprite sprite;
    struct fire* next;
};
typedef struct fire Fire;

void add_fire(Fire** fires, Bomb** bombs, Collision** collision, Power_up** powers, Player* owner,
    int x, int y, int direction, int length, int type, bool visible, bool center);
Fire* coll_fire(Fire** head, int x, int y, int width, int height);
Fire* coll_fire_exclude(Fire** head, Fire* ignore, int x, int y, int width, int height);
void f_update(Fire** fires, Bomb** bombs, double* delta);
void free_fire(Fire** fires, Fire* fire);

void free_all_fires(Fire** fires);

#endif