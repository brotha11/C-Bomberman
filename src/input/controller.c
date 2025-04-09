#include "controller.h"

Controller new_controller(Uint8 up, Uint8 down, Uint8 left, Uint8 right, Uint8 bomb) {
    Controller new;

    new.key_up = false;
    new.key_down = false;
    new.key_left = false;
    new.key_right = false;

    new.key_bomb = false;

    new.up = up;
    new.down = down;
    new.rigth = right;
    new.left = left;

    new.bomb = bomb;

    return new;
}

void check_inputs(Controller* controller) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    controller->key_up = state[controller->up];
    controller->key_down = state[controller->down];
    controller->key_left = state[controller->left];
    controller->key_right = state[controller->rigth];
    controller->key_bomb = state[controller->bomb];
}