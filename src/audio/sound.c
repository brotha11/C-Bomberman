#include "sound.h"

static Mix_Chunk* sfx[TOTAL_SOUNDS];

void sound_init() {

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer init failed: %s", Mix_GetError());
        return;
    }
    Mix_AllocateChannels(TOTAL_SOUNDS);

    sfx[SFX_PLACE_BOMB] = Mix_LoadWAV("res/sfx/place_bomb.wav");
    sfx[SFX_BLOW_UP_R] = Mix_LoadWAV("res/sfx/explosion.wav");
    sfx[SFX_POWER_UP] = Mix_LoadWAV("res/sfx/power.wav");
    sfx[SFX_KICK] = Mix_LoadWAV("res/sfx/kick.wav");
    sfx[SFX_DEATH] = Mix_LoadWAV("res/sfx/death.wav");
    sfx[SFX_DEATH_VC0] = Mix_LoadWAV("res/sfx/vc_death_00.wav");
    sfx[SFX_DEATH_VC1] = Mix_LoadWAV("res/sfx/vc_death_01.wav");
    sfx[SFX_DEATH_VC2] = Mix_LoadWAV("res/sfx/vc_death_02.wav");
    sfx[SFX_DEATH_VC3] = Mix_LoadWAV("res/sfx/vc_death_03.wav");

    for (int i = 0; i < TOTAL_SOUNDS; i++) {
        if (!sfx[i]) {
            SDL_Log("Failed to load sound %d: %s", i, Mix_GetError());
        }
    }
}

void play_sound(SFX sound) {
    if (sound >= 0 && sound < TOTAL_SOUNDS && sfx[sound]) {
        Mix_PlayChannel((int)sound, sfx[sound], 0);
    }
}

void sound_exit() {
    for (int i = 0; i < TOTAL_SOUNDS; i++) {
        if (sfx[i]) {
            Mix_FreeChunk(sfx[i]);
            sfx[i] = NULL;
        }
    }

    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}