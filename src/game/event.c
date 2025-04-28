#include "event.h"

int catch_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            return EV_CLOSE_GAME;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return EV_CLOSE_GAME;
            }
            if (event.key.keysym.sym == SDLK_F11) {
                return EV_FULLSCREEN;
            }
        }

        // (Opcional) Detectar conexión/desconexión de controladores
        if (event.type == SDL_CONTROLLERDEVICEADDED)
            return EV_GAMEPAD_CON;
        if (event.type == SDL_CONTROLLERDEVICEREMOVED)
            return EV_GAMEPAD_DISC;
    }
    return EV_NONE;
}
