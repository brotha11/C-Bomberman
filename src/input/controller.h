#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdbool.h"
#include "SDL2/SDL.h"

struct controller {

    bool key_up;
    bool key_down;
    bool key_right;
    bool key_left;

    bool key_bomb;

    Uint8 up, down, rigth, left, bomb;

};
typedef struct controller Controller;

Controller new_controller(Uint8 up, Uint8 down, Uint8 left, Uint8 right, Uint8 bomb);
void check_inputs(Controller* controller);

#endif