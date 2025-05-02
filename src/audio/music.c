#define MINIAUDIO_IMPLEMENTATION
#include "music.h"

Music init_music_player() {
    Music ms;

    ma_engine_init(NULL, &ms.engine);
    ms.current_music = 0;
    //ms.sound = /** Cualquier cosa que sirva para inicializar un ma_sound */;
    ms.intro_start = 0;
    ms.loop_end = 0;
    ms.music_pos = 0;

    ms.play_speed = 0.0f;
    ms.volume = 0.0f;

    ma_sound_init_from_file(&ms.engine, "res/bgm/battle_00.ogg", 0, NULL, NULL, &ms.sound[BGM_BATTLE_00]);
    ms.intro_start = seconds_to_frames(&ms.engine, 6.530);
    ms.loop_end = seconds_to_frames(&ms.engine, 84.888);

    return ms;
}

void close_music_player(Music* music) {
    for (int i = 0; i < BGM_AMOUNT; ++i) {
        ma_sound_uninit(&music->sound[i]);
    }
    ma_engine_uninit(&music->engine);  
}

void start_song(Music* music, MUSIC_LIST song) {
    /* music->play_speed = 1.0f;
    music->volume = 1.0f;
    ma_sound_start(&music->sound[song]); */
}

void manage_music_loop(Music* music, MUSIC_LIST song) {
    /* ma_uint64 cursor = 0;
    ma_sound_get_cursor_in_pcm_frames(&music->sound[song], &cursor);
    // Go back to intro
    if (cursor >= music->loop_end) {
        ma_uint64 jump = (cursor - (music->loop_end + music->intro_start));

        ma_sound_seek_to_pcm_frame(&music->sound[song], jump);
    } */
}

void change_music_speed(Music* music, MUSIC_LIST song, float speed) {
    ma_sound_set_pitch(&music->sound[song], speed);
}

void change_music_volume(Music* music, MUSIC_LIST song, float volume) {
    ma_sound_set_volume(&music->sound[song], volume);
}

ma_uint64 seconds_to_frames(ma_engine* engine, double seconds) {
    return (ma_uint64)(seconds * ma_engine_get_sample_rate(engine));
}
