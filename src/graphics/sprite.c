#include "sprite.h"

Sprite new_sprite(int w, int h) {
    Sprite tex;
    tex.frame_rect.x = 0;
    tex.frame_rect.y = 0;
    tex.frame_rect.w = tex.width = w;
    tex.frame_rect.h = tex.height = h;
    tex.frame = 0;
    tex.w_mult = 1;
    tex.h_mult = 1;

    tex.frame_x = tex.frame_y = 0;
    tex.x_off = tex.y_off = 0;
    tex.frame_x_max = 0;

    tex.image_change = new_timer(0);
    tex.image_speed = 0;
    tex.sprite = 0;

    return tex;
}

SDL_Texture* load_sprite(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* temp = IMG_Load(path);
    
    if (!temp) {
        printf("Error: No se pudo cargar la imagen '%s'. SDL_Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);

    if (!tex) {
        printf("Error: No se pudo crear la textura desde '%s'. SDL_Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    printf("Textura cargada exitosamente: %s\n", path);
    return tex;
}

void animate_sprite(Sprite* sprite, double* delta) {

    if(sprite->image_speed > 0) {
        if (tick_timer(&sprite->image_change, delta) == 0) {}
        else {
            sprite->frame = (sprite->frame += 1)%sprite->frame_x_max;
            sprite->image_change.time = sprite->image_speed;
        }

        // Update 
        sprite->frame_rect.x = sprite->frame_rect.w * (sprite->frame_x + sprite->frame);
        sprite->frame_rect.y = sprite->frame_rect.h * sprite->frame_y;
    }
}

void animate_sprite_timer(Sprite* sprite, double timer, double timer_max, double* delta) {
    if (sprite->frame != sprite->frame_x_max) {
        sprite->frame = (int)((1 - (timer / timer_max)) * sprite->frame_x_max);
        sprite->frame = sprite->frame%sprite->frame_x_max;
        // Update 
        sprite->frame_rect.x = sprite->frame_rect.w * (sprite->frame_x + sprite->frame);
        sprite->frame_rect.y = sprite->frame_rect.h * sprite->frame_y;
    }
}

void normalize_scale(Sprite* sprite, double goal, double speed, double* delta) {
    //double t = 1.0 - pow(1.0 - speed, *delta);
    double t = speed * (BASE_FPS * (*delta));

    sprite->w_mult = lerp(sprite->w_mult, goal, t);
    sprite->h_mult = lerp(sprite->h_mult, goal, t);
}
