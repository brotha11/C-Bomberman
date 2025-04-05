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
    graphics->sprites = (Sprite_data*)malloc(TEXTURE_AMOUNT * sizeof(Sprite_data));

    graphics->sprites[BOMBER_WHITE].sprite = load_sprite(BOMBER_WHITE_PATH, graphics->renderer);
    graphics->sprites[BOMBER_WHITE].spr_width = 16;
    graphics->sprites[BOMBER_WHITE].spr_height = 24;
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

void render_game(Graphics* graphics, Entity* entity, Collision* collision, Bomb** bombs, Fire** fires, Brick** bricks, Player* player) {
    
    animate_sprite(&player->base.sprite);
    
    SDL_RenderClear(graphics->renderer);

    f_render(graphics, fires);
    coll_render(graphics, collision);
    bri_render(graphics, bricks);
    b_render(graphics, bombs);
    //e_render(graphics, entity);

    /*tex_render(graphics, BOMBER_WHITE, player->base.x, player->base.y, 
            player->base.sprite.frame_rect.w, player->base.sprite.frame_rect.h);*/
    tex_render(graphics, &player->base.sprite, BOMBER_WHITE, player->base.x, player->base.y);


    SDL_RenderPresent(graphics->renderer);
}

void e_render(Graphics* graphics, Entity* entity) {

    SDL_SetRenderDrawColor(graphics->renderer, 255,255,255,125);
    
    rect_resolution_fix(graphics, &graphics->rect, entity->x, entity->y, entity->width, entity->height);
    SDL_RenderFillRect(graphics->renderer, &graphics->rect);

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

void tex_render(Graphics* graphics, Sprite* sprite, int spr, int x, int y) {
    rect_set(graphics, &sprite->frame_rect, 
            sprite->frame_x*graphics->sprites[spr].spr_width, 
                    sprite->frame_y*graphics->sprites[spr].spr_height, 
                            graphics->sprites[spr].spr_width, graphics->sprites[spr].spr_height);
    rect_resolution_fix(graphics, &graphics->rect, x + sprite->x_off, y + sprite->y_off, sprite->frame_rect.w, sprite->frame_rect.h);

    SDL_RenderCopy(graphics->renderer, graphics->sprites[spr].sprite, &sprite->frame_rect, &graphics->rect);

}

void b_render(Graphics* graphics, Bomb** bombs) {

    SDL_SetRenderDrawColor(graphics->renderer, 150,150,150,255);
    
    Bomb* current = *bombs;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        rect_resolution_fix(graphics, &graphics->rect, current->x, current->y, current->width, current->height);
        SDL_RenderFillRect(graphics->renderer, &graphics->rect);

        current = current->next;
    }

    SDL_SetRenderDrawColor(graphics->renderer, 20,20,20,255);
}

void f_render(Graphics* graphics, Fire** fires) {
    SDL_SetRenderDrawColor(graphics->renderer, 255,80,0,255);
    
    Fire* current = *fires;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->visible) {
            rect_resolution_fix(graphics, &graphics->rect, current->x, current->y, current->width, current->height);
            SDL_RenderFillRect(graphics->renderer, &graphics->rect);
        }

        current = current->next;
    }

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

void bri_render(Graphics* graphics, Brick** bricks) {

    SDL_SetRenderDrawColor(graphics->renderer, 220,80,80,255);
    
    Brick* current = *bricks;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        rect_resolution_fix(graphics, &graphics->rect, current->coll->x, current->coll->y, current->coll->width, current->coll->height);
        SDL_RenderFillRect(graphics->renderer, &graphics->rect);

        current = current->next;
    }

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
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

    for(int i = 0; i<TEXTURE_AMOUNT; ++i) {
        SDL_DestroyTexture(graphics->sprites[i].sprite);
    }
    free(graphics->sprites);

    IMG_Quit();
    SDL_Quit();
}