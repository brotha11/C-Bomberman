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

};
typedef struct controller Controller;

Controller new_controller();
void check_inputs(Controller* controller);

#endif