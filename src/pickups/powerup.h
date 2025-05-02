#ifndef POWERUP_H
#define POWERUP_H

#include "../graphics/sprite.h"
#include "../game/timer.h"

#define SIZE 4

typedef enum POWER_LIST {
    FIRE_UP,
    BOMB_UP,
    SPEED_UP,
    BOMB_LINE,
    KICK,
    BOMB_PIERCING,
} POWER_LIST;

#define PW_IMG_SPEED 0.16

typedef struct power_up {
    int x;
    int y;
    int width, height;

    int power_up_type;
    int grabable;
    Timer timer;
    double* p_delta_time;
    int visible;

    Sprite sprite;
    struct power_up* next;

} Power_up;

typedef struct fire Fire;

void add_powerup(Power_up** head, int x, int y, int power_type, double* delta);
Power_up* grab_powerup(Power_up** head, int x, int y, int width, int height);
Power_up* coll_powerup(Power_up** head, int x, int y, int width, int height);
void pw_update(Power_up** head, Fire** fires);
void power_up_blow(Power_up** head, Power_up* power);

void free_powerup(Power_up** head, Power_up* power);
void free_all_powerups(Power_up** head);

#endif
