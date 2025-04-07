#ifndef POWERUP_H
#define POWERUP_H

#include "../graphics/sprite.h"

#define FIRE_UP 0
#define BOMB_UP 1
#define SPEED_UP 2
#define BOMB_LINE 3

typedef struct power_up {
    int x;
    int y;
    int width, height;

    int power_up_type;
    int grabable;
    int timer;
    int visible;

    Sprite sprite;
    struct power_up* next;

} Power_up;

typedef struct fire Fire;

void add_powerup(Power_up** head, int x, int y, int power_type);
Power_up* grab_powerup(Power_up** head, int x, int y, int width, int height);
void pw_update(Power_up** head, Fire** fires);
void power_up_blow(Power_up** head, Power_up* power);

void free_powerup(Power_up** head, Power_up* power);
void free_all_powerups(Power_up** head);

#endif
