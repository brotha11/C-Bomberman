#ifndef MUSIC_H
#define MUSIC_H

#include "SDL2/SDL.h"

typedef enum MUSIC_LIST {
    BGM_BATTLE_00,
    BGM_BATTLE_01,
    BGM_BATTLE_94,
    BGM_AMOUNT,
} MUSIC_LIST;

#define BGM_BATTLE_00_PATH "res/bgm/battle_00.ogg"
#define BGM_BATTLE_01_PATH "res/bgm/battle_01.ogg"
#define BGM_BATTLE_94_PATH "res/bgm/battle_94.ogg"

typedef struct song {
    short* data;               // PCM 16-bit intercalado
    int sample_count;          // Cantidad de muestras por canal
    int total_bytes;           // Tamaño total en bytes
    int sample_rate;
    int channels;

    double loop_start;
    double loop_end;
} Song;

typedef struct music_manager {
    SDL_AudioDeviceID device;
    MUSIC_LIST current_song;
    float current_sample;
    int playing;
    float speed;
    float volume;
    SDL_AudioSpec spec;
} Music_manager;

void music_init(Music_manager* ms);
int song_load(Song* s, const char* path, double intro, double end);
void music_stop(Music_manager* mgr);
void music_shutdown(Music_manager* mgr);
void music_play(Music_manager* mgr, MUSIC_LIST song_id);
void music_set_speed(Music_manager* mgr, float new_speed);
void music_set_volume(Music_manager* mgr, float new_volume);
void music_goto_intro(Music_manager* mgr);

#endif
