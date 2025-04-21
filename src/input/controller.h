#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdbool.h"
#include "SDL2/SDL.h"

#include "../game/game_constants.h"

#define DEADZONE 16000

typedef struct profile {
    Uint8 up, down, right, left, bomb;
} Profile;

struct controller {
    SDL_GameController* gamepad;
    SDL_JoystickID gamepad_id;
    int id;

    bool using_pad;
    bool controller_on;

    bool key_up;
    bool key_down;
    bool key_right;
    bool key_left;

    bool key_bomb;

    Profile keys;
};
typedef struct controller Controller;

void input_init(Controller* controllers, int* players_on, Profile no);
void set_joys(Controller* controllers, int* players_on);

Controller new_controller(Profile keys);
void check_inputs(Controller* controller);
void input_exit(Controller* controller);
Profile set_profile(Uint8 up, Uint8 down, Uint8 left, Uint8 right, Uint8 bomb);
void assign_inputs(Controller* controllers, int* players_on, Profile m, Profile type0, Profile type1, Profile no);

#endif