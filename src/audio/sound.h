#ifndef SOUND_H
#define SOUND_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#define DEATH_VC 3

typedef enum {
    SFX_PLACE_BOMB, 
    SFX_BLOW_UP_R,
    SFX_POWER_UP,
    SFX_KICK,
    SFX_DEATH,
    SFX_DEATH_VC0,
    SFX_DEATH_VC1,
    SFX_DEATH_VC2,
    TOTAL_SOUNDS
} SFX;

void sound_init();
void sound_exit();

void play_sound(SFX sound);


#endif