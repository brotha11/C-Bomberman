#include "graphics.h"

void init_graphics(Graphics* graphics) {
    SDL_Init (SDL_INIT_EVERYTHING);

    graphics->window = SDL_CreateWindow(GAME_NAME,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    graphics->renderer = SDL_CreateRenderer(graphics->window,-1,SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(graphics->renderer, 20,20,20,255);

    graphics->x_multiplier = (SCREEN_WIDTH/BASE_WIDTH);
    graphics->y_multiplier = (SCREEN_HEIGHT/BASE_HEIGHT);

    graphics->rect.x = 0;
    graphics->rect.y = 0;
    graphics->rect.w = 0;
    graphics->rect.h = 0;

    // Load textures
    graphics->sprites = (Sprite_data*)malloc(TEX_AMOUNT * sizeof(Sprite_data));

    // White bomber
    graphics->sprites[TEX_BOMBER_WHITE].sprite = load_sprite(BOMBER_WHITE_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMBER_WHITE].spr_width = 20;
    graphics->sprites[TEX_BOMBER_WHITE].spr_height = 24;

    // Bombs
    graphics->sprites[TEX_BOMB].sprite = load_sprite(BOMB_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMB].spr_width = 16;
    graphics->sprites[TEX_BOMB].spr_height = 16;

    // Fire exp
    graphics->sprites[TEX_FIRE].sprite = load_sprite(FIRE_PATH, graphics->renderer);
    graphics->sprites[TEX_FIRE].spr_width = 16;
    graphics->sprites[TEX_FIRE].spr_height = 16;

    // Map 01
    graphics->sprites[TEX_MAP_01].sprite = load_sprite(MAP_01_PATH, graphics->renderer);
    graphics->sprites[TEX_MAP_01].spr_width = 256;
    graphics->sprites[TEX_MAP_01].spr_height = 224;

    graphics->sprites[TEX_BRICK].sprite = load_sprite(BRICK_01_PATH, graphics->renderer);
    graphics->sprites[TEX_BRICK].spr_width = 16;
    graphics->sprites[TEX_BRICK].spr_height = 16;

    graphics->sprites[TEX_POWER_UPS].sprite = load_sprite(POWER_UPS_PATH, graphics->renderer);
    graphics->sprites[TEX_POWER_UPS].spr_width = 16;
    graphics->sprites[TEX_POWER_UPS].spr_height = 16;


    graphics->sprites[TEX_ITEM_BURN].sprite = load_sprite(ITEM_BURN_PATH, graphics->renderer);
    graphics->sprites[TEX_ITEM_BURN].spr_width = 16;
    graphics->sprites[TEX_ITEM_BURN].spr_height = 32;

    graphics->background = new_sprite(256,224);
}

bool graphics_event(Graphics* graphics) {
    // Close Window
    if (SDL_PollEvent( &graphics->window_event )){

        if (SDL_QUIT == graphics->window_event.type){
            return false;
        }
        else if (graphics->window_event.type == SDL_KEYDOWN){
            if (graphics->window_event.key.keysym.sym == SDLK_ESCAPE){
                return false;
            }
        }
    }
    return true;
}

void tex_render(Graphics* graphics, Sprite* sprite, int spr, int x, int y) {

    if (!sprite) return;

    rect_set(graphics, &sprite->frame_rect, 
            sprite->frame_x*graphics->sprites[spr].spr_width, 
                    sprite->frame_y*graphics->sprites[spr].spr_height, 
                            graphics->sprites[spr].spr_width, graphics->sprites[spr].spr_height);
    rect_resolution_fix(graphics, &graphics->rect, x + sprite->x_off, y + sprite->y_off, sprite->frame_rect.w, sprite->frame_rect.h);

    SDL_RenderCopy(graphics->renderer, graphics->sprites[spr].sprite, &sprite->frame_rect, &graphics->rect);

}

void e_render(Graphics* graphics, Entity* entity) {

    SDL_SetRenderDrawColor(graphics->renderer, 255,255,255,125);
    
    rect_resolution_fix(graphics, &graphics->rect, entity->x, entity->y, entity->width, entity->height);
    SDL_RenderFillRect(graphics->renderer, &graphics->rect);

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

void b_render(Graphics* graphics, Bomb** bombs) {
    Bomb* current = *bombs;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {
        tex_render(graphics, &current->sprite, TEX_BOMB, current->x, current->y);
        current = current->next;
    }
}

void f_render(Graphics* graphics, Fire** fires) {
    SDL_SetRenderDrawColor(graphics->renderer, 20,80,100,255);
    
    Fire* current = *fires;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->visible) {
            tex_render(graphics, &current->sprite, TEX_FIRE, current->x, current->y);
        }
        current = current->next;
    }

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

void bri_render(Graphics* graphics, Brick** bricks) {
    Brick* current = *bricks;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (!current->visible) {
            current = current->next;
            continue;
        }

        tex_render(graphics, &current->sprite, TEX_BRICK, current->coll->x, current->coll->y);

        current = current->next;
    }
}

void pw_render(Graphics* graphics, Power_up** powers) {
    Power_up* current = *powers;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->visible) {
            if (current->grabable == 1) {
                tex_render(graphics, &current->sprite, TEX_POWER_UPS, current->x, current->y);
            } else {
                tex_render(graphics, &current->sprite, TEX_ITEM_BURN, current->x, current->y);
            }
        }

        current = current->next;
    }
}


void coll_render(Graphics* graphics, Collision* collision) {

    SDL_SetRenderDrawColor(graphics->renderer, 25,25,255,125);
    
    Collision* current = collision;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        rect_resolution_fix(graphics, &graphics->rect, current->x, current->y, current->width, current->height);
        SDL_RenderFillRect(graphics->renderer, &graphics->rect);

        current = current->next;
    }

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

// Fix size depending on the resolution of the game
void rect_resolution_fix(Graphics* graphics, SDL_Rect* rect, int x, int y, int w, int h) {

    rect->x = (int) x * graphics->x_multiplier;
    rect->y = (int) y * graphics->y_multiplier;
    rect->w = (int) w * graphics->x_multiplier;
    rect->h = (int) h * graphics->y_multiplier;

}

// Fix size depending on the resolution of the game
void rect_set(Graphics* graphics, SDL_Rect* rect, int x, int y, int w, int h) {
    rect->x = (int) x;
    rect->y = (int) y;
    rect->w = (int) w;
    rect->h = (int) h;
}

void free_graphics(Graphics* graphics) {
    SDL_DestroyWindow(graphics->window);
    SDL_DestroyRenderer(graphics->renderer);

    for(int i = 0; i < TEX_AMOUNT; ++i) {
        SDL_DestroyTexture(graphics->sprites[i].sprite);
    }
    free(graphics->sprites);

    IMG_Quit();
    SDL_Quit();
}