#include "sprite.h"

Sprite new_sprite(int w, int h) {
    Sprite tex;
    tex.frame_rect.x = 0;
    tex.frame_rect.y = 0;
    tex.frame_rect.w = tex.width = w;
    tex.frame_rect.h = tex.height = h;
    tex.frame = 0;

    tex.frame_x = tex.frame_y = 0;
    tex.x_off = tex.y_off = 0;
    tex.frame_x_max = 0;

    tex.image_change = tex.image_speed = 0;
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

void animate_sprite(Sprite* sprite) {

    if(sprite->image_speed > 0) {
        if (sprite->image_change > 0) sprite->image_change--;
        else {
            sprite->frame = (sprite->frame += 1)%sprite->frame_x_max;
            sprite->image_change = sprite->image_speed;
        }

        // Update 
        sprite->frame_rect.x = sprite->frame_rect.w * (sprite->frame_x + sprite->frame);
        sprite->frame_rect.y = sprite->frame_rect.h * sprite->frame_y;
    }
}

void animate_sprite_timer(Sprite* sprite, int timer, int timer_max) {
    if (sprite->frame != sprite->frame_x_max) {
        sprite->frame = (int)((1.0f - ((float)timer / timer_max)) * sprite->frame_x_max);
        sprite->frame = sprite->frame%sprite->frame_x_max;
        // Update 
        sprite->frame_rect.x = sprite->frame_rect.w * (sprite->frame_x + sprite->frame);
        sprite->frame_rect.y = sprite->frame_rect.h * sprite->frame_y;
    }
}