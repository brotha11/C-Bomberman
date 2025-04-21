#include "controller.h"
#include "stdio.h"
void input_init(Controller* controllers, int* players_on, Profile no) {

    for (int i = 0; i < MAX_BATTLE_PLAYERS; ++i) {
        controllers[i] = new_controller(no);
    }
    set_joys(controllers, players_on);
}

void set_joys(Controller* controllers, int* players_on) {
    int num_joy = SDL_NumJoysticks();
    printf("Detectados %d dispositivos de entrada.\n", num_joy);

    int count = 0;

    for (int i = 0; i < num_joy && count < MAX_BATTLE_PLAYERS; ++i) {
        printf("Probando dispositivo %d...\n", i);

        if (SDL_IsGameController(i)) {
            printf("El dispositivo %d es un gamepad válido.\n", i);

            if (controllers[count].using_pad && controllers[count].gamepad) {
                SDL_GameControllerClose(controllers[count].gamepad);
                controllers[count].gamepad = NULL;
            }

            SDL_GameController* gc = SDL_GameControllerOpen(i);
            if (!gc) {
                printf("Error al abrir el gamepad %d: %s\n", i, SDL_GetError());
                continue;
            }

            SDL_Joystick* joy = SDL_GameControllerGetJoystick(gc);
            SDL_JoystickID instance_id = SDL_JoystickInstanceID(joy);

            controllers[count].gamepad = gc;
            controllers[count].id = i;
            controllers[count].gamepad_id = instance_id;
            controllers[count].using_pad = true;

            printf("Gamepad #%d conectado correctamente. ID de instancia: %d\n", i, instance_id);

            count++;
        } else {
            printf("El dispositivo %d **NO** es compatible con SDL_GameController.\n", i);
            controllers[count].gamepad = NULL;
            controllers[count].id = 0;
            controllers[count].gamepad_id = 0;
            controllers[count].using_pad = false;
        }
    }

    if (count == 0) {
        printf("No se conectaron gamepads compatibles.\n");
    }
}


Controller new_controller(Profile keys) {
    Controller new;

    new.using_pad = false;

    new.key_up = false;
    new.key_down = false;
    new.key_left = false;
    new.key_right = false;

    new.key_bomb = false;
    new.controller_on = false;

    new.keys = keys;

    return new;
}

Profile set_profile(Uint8 up, Uint8 down, Uint8 left, Uint8 right, Uint8 bomb) {
    Profile new;

    new.up = up;
    new.down = down;
    new.left = left;
    new.right = right;
    new.bomb = bomb;

    return new;
}

void assign_inputs(Controller* controllers, int* players_on, Profile m, Profile type0, Profile type1, Profile no) {
    int count = 0;
    for (int i = 0; i < MAX_BATTLE_PLAYERS; ++i) {
        if (players_on[i] == 1) ++count;
    }

    int num_joy = SDL_NumJoysticks();
    int no_joy = abs(count - num_joy);

    if (no_joy == 1) {
        for (int pad = 0; pad < MAX_BATTLE_PLAYERS; ++pad) {
            if (players_on[pad] == 0) continue;

            if (!controllers[pad].using_pad) {
                controllers[pad].keys = m;
            }
        }
    } else {
        int set = 0;
        for (int pad = 0; pad < MAX_BATTLE_PLAYERS; ++pad) {
            if (players_on[pad] == 0) continue;

            if (!controllers[pad].using_pad) {
                if (set == 0) controllers[pad].keys = type0;
                else if (set == 1) controllers[pad].keys = type1;
                ++set;
            }
        }
    }
}

void check_inputs(Controller* controller) {
    if (!controller->using_pad) {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        controller->key_up = state[controller->keys.up];
        controller->key_down = state[controller->keys.down];
        controller->key_left = state[controller->keys.left];
        controller->key_right = state[controller->keys.right];
        controller->key_bomb = state[controller->keys.bomb];
    } else if (controller->gamepad && SDL_GameControllerGetAttached(controller->gamepad)) {
        SDL_GameController* gc = controller->gamepad;

        if (gc) {
            controller->key_bomb = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_B);

            Sint16 x_axis = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX);
            
            if (x_axis < -DEADZONE) {
                controller->key_left = true;
                controller->key_right = false;
            } else if (x_axis > DEADZONE) {
                controller->key_right = true;
                controller->key_left = false;
            } else {
                controller->key_left = false;
                controller->key_right = false;
            }

            Sint16 y_axis = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY);
            if (y_axis < -DEADZONE) {
                controller->key_up = true;
                controller->key_down = false;
            } else if (y_axis > DEADZONE) {
                controller->key_down = true;
                controller->key_up = false;
            } else {
                controller->key_up = false;
                controller->key_down = false;
            }

            if (!controller->key_left) controller->key_left = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            if (!controller->key_up) controller->key_up = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_UP);
            if (!controller->key_right) controller->key_right = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
            if (!controller->key_down) controller->key_down = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        }
    }
}

void input_exit(Controller* controller) {
    for (int i = 0; i < MAX_BATTLE_PLAYERS; i++) {
        if (controller[i].using_pad && controller[i].gamepad) {
            SDL_GameController* gc = controller[i].gamepad;

            if (gc) {
                SDL_GameControllerClose(gc);
                controller[i].gamepad = NULL;
            }

            controller[i].using_pad = false;
        }
    }
}
