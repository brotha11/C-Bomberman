#ifndef MUSIC_H
#define MUSIC_H

#include "miniaudio.h"

typedef enum MUSIC_LIST {
    BGM_BATTLE_00,
    BGM_AMOUNT,
} MUSIC_LIST;

typedef struct music {
    ma_engine engine;
    ma_sound sound[BGM_AMOUNT];

    MUSIC_LIST current_music;
    ma_uint64 music_pos;

    ma_uint64 intro_start;
    ma_uint64 loop_end;
    float play_speed;
    float volume;
} Music;

Music init_music_player();
void close_music_player(Music* music);
void start_song(Music* music, MUSIC_LIST song);
void manage_music_loop(Music* music, MUSIC_LIST song);
void change_music_speed(Music* music, MUSIC_LIST song, float speed);
void change_music_volume(Music* music, MUSIC_LIST song, float volume);

ma_uint64 seconds_to_frames(ma_engine* engine, double seconds);


#endif
