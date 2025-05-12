#define STB_VORBIS_IMPLEMENTATION
#include "music.h"
#include "../../builder_obj/stb_vorbis.c"

// Static array to store songs
static Song songs[BGM_AMOUNT];

void music_init(Music_manager* ms) {
    ms->device = 0;
    ms->current_song = BGM_BATTLE_00;
    ms->current_sample = 0;
    ms->playing = 0;
    ms->volume = 1.0f;
    ms->speed = 1.0f;
    
    song_load(&songs[BGM_BATTLE_00], BGM_BATTLE_00_PATH, 6.530, 84.888);
    song_load(&songs[BGM_BATTLE_01], BGM_BATTLE_01_PATH, 2.860, 51.450);
    song_load(&songs[BGM_BATTLE_94], BGM_BATTLE_94_PATH, 3.187, 79.645);
}


int song_load(Song* s, const char* path, double intro, double end) {
    int channels, sample_rate;
    short* output;
    int samples = stb_vorbis_decode_filename(path, &channels, &sample_rate, &output);

    if (samples < 0) {
        return 1;
    }

    s->data = output;
    s->sample_count = samples;
    s->channels = channels;
    s->sample_rate = sample_rate;
    s->total_bytes = samples * channels * sizeof(short);
    s->loop_start = intro;
    s->loop_end = end;

    printf("Loaded: %s | samples: %d | channels: %d | sample_rate: %d\n", path, samples, channels, sample_rate);
    return 0;
}

static void music_callback(void* userdata, Uint8* stream, int len) {
    Music_manager* p = (Music_manager*)userdata;
    Song* s = &songs[p->current_song];

    if (!p->playing || !s->data) {
        SDL_memset(stream, 0, len);
        return;
    }

    int samples_requested = len / (sizeof(short) * s->channels);
    int loop_end_sample = (int)(s->loop_end * s->sample_rate);
    int loop_start_sample = (int)(s->loop_start * s->sample_rate);

    short* out = (short*)stream;
    for (int i = 0; i < samples_requested; ++i) {
        // Manejar el loop con posición flotante
        if (p->current_sample >= loop_end_sample) {
            p->current_sample = loop_start_sample + (p->current_sample - loop_end_sample);
        }

        // Verificar límites superiores
        if (p->current_sample >= s->sample_count - 1) {
            memset(&out[i * s->channels], 0, sizeof(short) * s->channels);
            continue;
        }

        // Interpolación lineal
        int pos_int = (int)p->current_sample;
        float frac = p->current_sample - pos_int;
        
        for (int c = 0; c < s->channels; ++c) {
            short sample0 = s->data[pos_int * s->channels + c];
            short sample1 = s->data[(pos_int + 1) * s->channels + c];
            float interpolated = sample0 * (1.0f - frac) + sample1 * frac;
            out[i * s->channels + c] = (short)(interpolated * p->volume);
        }
        
        p->current_sample += p->speed;
    }
}

void music_play(Music_manager* mgr, MUSIC_LIST song_id) {
    Song* s = &songs[song_id];
    mgr->current_song = song_id;
    mgr->current_sample = 0.0f;
    mgr->playing = 1;

    if (mgr->device == 0) {
        SDL_AudioSpec spec;
        SDL_zero(spec);
        spec.freq = s->sample_rate;
        spec.format = AUDIO_S16SYS;
        spec.channels = s->channels;
        spec.samples = 4096;
        spec.callback = music_callback;
        spec.userdata = mgr;

        mgr->device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
        if (mgr->device == 0) {
            fprintf(stderr, "No se pudo abrir dispositivo de audio: %s\n", SDL_GetError());
            exit(1);
        } else {
            printf("Dispositivo de audio abierto correctamente, ID: %d\n", mgr->device);
        }
        mgr->spec = spec; // Save config for procedures that need restarts
    }

    SDL_PauseAudioDevice(mgr->device, 0);
    printf("Audio device started\n");
}

void music_set_speed(Music_manager* mgr, float new_speed) {
    mgr->speed = new_speed;
}

void music_set_volume(Music_manager* mgr, float volume) {
    mgr->volume = SDL_clamp(volume, 0.0f, 1.0f);
}

void music_goto_intro(Music_manager* mgr) {
    Song* s = &songs[mgr->current_song];

    int loop_start_sample = (int)(s->loop_start * s->sample_rate);
    mgr->current_sample = loop_start_sample;
}

void music_stop(Music_manager* mgr) {
    if (mgr->device) {
        SDL_PauseAudioDevice(mgr->device, 1);
    }
    mgr->playing = 0;
}

void music_shutdown(Music_manager* mgr) {
    if (mgr->device) {
        SDL_CloseAudioDevice(mgr->device);
        mgr->device = 0;
    }
    for (int s = 0; s < BGM_AMOUNT; ++s) {
        if (songs[s].data) {
            free(songs[s].data);
            songs[s].data = NULL;
        }
    }
}
