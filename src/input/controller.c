#include "controller.h"

Controller new_controller() {
    Controller new;

    new.key_up = false;
    new.key_down = false;
    new.key_left = false;
    new.key_right = false;

    new.key_bomb = false;

    return new;
}

void check_inputs(Controller* controller) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    controller->key_up = state[SDL_SCANCODE_W];
    controller->key_down = state[SDL_SCANCODE_S];
    controller->key_left = state[SDL_SCANCODE_A];
    controller->key_right = state[SDL_SCANCODE_D];
    controller->key_bomb = state[SDL_SCANCODE_RSHIFT];
}