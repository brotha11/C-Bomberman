#ifndef EVENT_H
#define EVENT_H

#include "SDL2/SDL.h"

typedef enum {
    EV_CLOSE_GAME,
    EV_NONE,
    EV_FULLSCREEN,
    EV_GAMEPAD_DISC,
    EV_GAMEPAD_CON,

} EVENTS;

int catch_events();

#endif