#include "graphics.h"

void init_graphics(Graphics* graphics) {
    SDL_Init (SDL_INIT_EVERYTHING);

    graphics->screen = init_screen(1280, 720, 0);

    graphics->window = SDL_CreateWindow(GAME_NAME,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
            graphics->screen.SCREEN_WIDTH,graphics->screen.SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    graphics->renderer = SDL_CreateRenderer(graphics->window,-1,SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(graphics->renderer, 20,20,20,255);

    if (graphics->screen.fullscreen == 1) SDL_SetWindowFullscreen(graphics->window,SDL_WINDOW_FULLSCREEN);

    graphics->y_multiplier = (int)(graphics->screen.SCREEN_HEIGHT/BASE_HEIGHT);
    graphics->x_multiplier = graphics->y_multiplier;

    graphics->rect.x = 0;
    graphics->rect.y = 0;
    graphics->rect.w = 0;
    graphics->rect.h = 0;

    graphics->draw_hitboxes = 0;

    // Backgrounds
    for (int i = 0; i < MAX_BACKGROUND_COUNT; ++i) {
        graphics->backgrounds[i] = new_bg(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    // Load textures
    graphics->sprites = (Sprite_data*)malloc(TEX_AMOUNT * sizeof(Sprite_data));

    // White bomber
    graphics->sprites[TEX_BOMBER_WHITE].sprite = load_sprite(BOMBER_WHITE_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMBER_WHITE].spr_width = 20;
    graphics->sprites[TEX_BOMBER_WHITE].spr_height = 24;

    // Black bomber
    graphics->sprites[TEX_BOMBER_BLACK].sprite = load_sprite(BOMBER_BLACK_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMBER_BLACK].spr_width = 20;
    graphics->sprites[TEX_BOMBER_BLACK].spr_height = 24;

    // Red bomber
    graphics->sprites[TEX_BOMBER_RED].sprite = load_sprite(BOMBER_RED_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMBER_RED].spr_width = 20;
    graphics->sprites[TEX_BOMBER_RED].spr_height = 24;

    // Blue bomber
    graphics->sprites[TEX_BOMBER_BLUE].sprite = load_sprite(BOMBER_BLUE_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMBER_BLUE].spr_width = 20;
    graphics->sprites[TEX_BOMBER_BLUE].spr_height = 24;

    // Green bomber
    graphics->sprites[TEX_BOMBER_GREEN].sprite = load_sprite(BOMBER_GREEN_PATH, graphics->renderer);
    graphics->sprites[TEX_BOMBER_GREEN].spr_width = 20;
    graphics->sprites[TEX_BOMBER_GREEN].spr_height = 24;

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
    graphics->sprites[TEX_MAP_01].spr_height = 256;

    graphics->sprites[TEX_BRICK].sprite = load_sprite(BRICK_01_PATH, graphics->renderer);
    graphics->sprites[TEX_BRICK].spr_width = 16;
    graphics->sprites[TEX_BRICK].spr_height = 16;

    graphics->sprites[TEX_POWER_UPS].sprite = load_sprite(POWER_UPS_PATH, graphics->renderer);
    graphics->sprites[TEX_POWER_UPS].spr_width = 16;
    graphics->sprites[TEX_POWER_UPS].spr_height = 16;


    graphics->sprites[TEX_ITEM_BURN].sprite = load_sprite(ITEM_BURN_PATH, graphics->renderer);
    graphics->sprites[TEX_ITEM_BURN].spr_width = 16;
    graphics->sprites[TEX_ITEM_BURN].spr_height = 32;

    graphics->sprites[TEX_BATTLE_MODE_SCORE].sprite = load_sprite(BATTLE_MODE_SCORE_PATH, graphics->renderer);
    graphics->sprites[TEX_BATTLE_MODE_SCORE].spr_width = 256;
    graphics->sprites[TEX_BATTLE_MODE_SCORE].spr_height = 24;

    graphics->sprites[TEX_GUI_SYMBOLS].sprite = load_sprite(GUI_SYMBOLS_PATH, graphics->renderer);
    graphics->sprites[TEX_GUI_SYMBOLS].spr_width = 16;
    graphics->sprites[TEX_GUI_SYMBOLS].spr_height = 16;

    graphics->sprites[TEX_BG_OCEAN_00].sprite = load_sprite(BG_OCEAN_00_PATH, graphics->renderer);
    graphics->sprites[TEX_BG_OCEAN_00].spr_width = 128;
    graphics->sprites[TEX_BG_OCEAN_00].spr_height = 64;

    graphics->sprites[TEX_BG_GRASS_00].sprite = load_sprite(BG_GRASS_00_PATH, graphics->renderer);
    graphics->sprites[TEX_BG_GRASS_00].spr_width = 256;
    graphics->sprites[TEX_BG_GRASS_00].spr_height = 256;

    graphics->ground = new_sprite(256,256);
    graphics->score_gui = new_sprite(256,24);
    graphics->gui_symbols = new_sprite(14,14);
}

void tex_render(Graphics* graphics, Sprite* sprite, int spr, int x, int y) {

    if (!sprite) return;

    rect_set(graphics, &sprite->frame_rect, 
            (sprite->frame_x + sprite->frame)*graphics->sprites[spr].spr_width, 
                    sprite->frame_y*graphics->sprites[spr].spr_height, 
                            graphics->sprites[spr].spr_width, graphics->sprites[spr].spr_height);
    rect_resolution_fix(graphics, &graphics->rect, x + sprite->x_off, y + sprite->y_off, sprite->frame_rect.w, sprite->frame_rect.h);

    SDL_RenderCopy(graphics->renderer, graphics->sprites[spr].sprite, &sprite->frame_rect, &graphics->rect);

}

void e_render(Graphics* graphics, Entity* entity, int cam_x, int cam_y) {

    SDL_SetRenderDrawColor(graphics->renderer, 255,255,255,125);
    
    rect_resolution_fix(graphics, &graphics->rect, entity->x - cam_x, entity->y - cam_y, entity->width, entity->height);
    SDL_RenderFillRect(graphics->renderer, &graphics->rect);

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

void b_render(Graphics* graphics, Bomb** bombs, int cam_x, int cam_y) {
    Bomb* current = *bombs;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {
        tex_render(graphics, &current->sprite, TEX_BOMB, current->x - cam_x, current->y - cam_y);

        if (graphics->draw_hitboxes == 1) {
            draw_hitbox(graphics, current->coll->x - cam_x, current->coll->y - cam_y,
                current->coll->width, current->coll->height);
            draw_hitbox(graphics, current->coll2->x - cam_x, current->coll2->y - cam_y,
                current->coll2->width, current->coll2->height);
        }

        current = current->next;
    }
}

void f_render(Graphics* graphics, Fire** fires, int cam_x, int cam_y) {
    SDL_SetRenderDrawColor(graphics->renderer, 20,80,100,255);
    
    Fire* current = *fires;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->visible) {
            tex_render(graphics, &current->sprite, TEX_FIRE, current->x - cam_x, current->y - cam_y);

            if (graphics->draw_hitboxes == 1) draw_hitbox(graphics, current->x - cam_x, current->y - cam_y,
                current->width, current->height);
        }
        current = current->next;
    }

    SDL_SetRenderDrawColor(graphics->renderer, 0,0,0,255);
}

void bri_render(Graphics* graphics, Brick** bricks, int cam_x, int cam_y) {
    Brick* current = *bricks;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (!current->visible) {
            current = current->next;
            continue;
        }

        tex_render(graphics, &current->sprite, TEX_BRICK, current->coll->x - cam_x, current->coll->y - cam_y);

        current = current->next;
    }
}

void pw_render(Graphics* graphics, Power_up** powers, int cam_x, int cam_y) {
    Power_up* current = *powers;
    
    // Recorremos todos los bloques de colisión
    while (current != NULL) {

        if (current->visible) {
            if (current->grabable == 1) {
                tex_render(graphics, &current->sprite, TEX_POWER_UPS, current->x - cam_x, current->y - cam_y);
            } else {
                tex_render(graphics, &current->sprite, TEX_ITEM_BURN, current->x - cam_x, current->y - cam_y);
            }
            if (graphics->draw_hitboxes == 1) draw_hitbox(graphics, current->x - cam_x, current->y - cam_y,
                current->width, current->height);
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

void background_render(Graphics* graphics, Background* backgrounds, Camera* camera) {
    for (int i = 0; i < MAX_BACKGROUND_COUNT; ++i) {
        Background* bg = &backgrounds[i];
        if (!bg->used) continue;

        int tile_w = bg->sprite.width;
        int tile_h = bg->sprite.height;

        int start_x = (camera->x / tile_w) - 1;
        int end_x = ((camera->x + (int)(graphics->screen.SCREEN_WIDTH / get_game_scale(&graphics->screen))) / tile_w) + 1;

        int start_y = (camera->y / tile_h) - 1;
        int end_y = ((camera->y + (int)(graphics->screen.SCREEN_HEIGHT / get_game_scale(&graphics->screen))) / tile_h) + 1;

        for (int tx = start_x; tx <= end_x; ++tx) {
            for (int ty = start_y; ty <= end_y; ++ty) {
                int draw_x = tx * tile_w - camera->x;
                int draw_y = ty * tile_h - camera->y;

                tex_render(graphics, &bg->sprite, TEX_BG_GRASS_00 + i, draw_x, draw_y);
            }
        }
    }
}


void gui_battle_render(Graphics* graphics, Battle_manager* battle) {
    // Score gui
    tex_render(graphics, &graphics->score_gui, TEX_BATTLE_MODE_SCORE, -get_center_x(&graphics->screen, 256),0);
    int CLOCK_X = -get_center_x(&graphics->screen, 256) + 10;

    // CLOCK
    graphics->gui_symbols.frame_x = SY_CLOCK;
        tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, CLOCK_X, 4); 
    // Minutes digits
    graphics->gui_symbols.frame_x = (int)(battle->battle_clock/60);
    tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, CLOCK_X+14, 4);

    // Colon
    graphics->gui_symbols.frame_x = SY_COLON;
    tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, CLOCK_X+22, 4);

    // Tens digit of seconds
    int seconds = battle->battle_clock % 60;
    graphics->gui_symbols.frame_x = seconds / 10;
    tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, CLOCK_X+30, 4);

    // Units digit of seconds
    graphics->gui_symbols.frame_x = seconds % 10;
    tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, CLOCK_X+40, 4);


    // Bomber faces
    int bomber_places = CLOCK_X+64;
    int separation = 36;
    int pos = 0;

    for (int i = 0; i < MAX_BATTLE_PLAYERS; ++i) {
        if (battle->players_on[i] == 1) {
            // Face
            int spr = SY_BOMBER_W;

            // DEAD
            if (battle->players[i].base->alive == false) {
                if (battle->players[i].death_timer <= DEATH_TIMER_MAX/10) spr = SY_DBOMB_W + i;
                else {
                    int frame = (int)((DEATH_TIMER_MAX - battle->players[i].death_timer) / (DEATH_TIMER_MAX/8));
                    spr = SY_KILLED + (frame % 2);
                }
                graphics->gui_symbols.frame_x = spr;

            } else { // ALIVE
                // Idle
                if (battle->players[i].kill_celebration_timer == 0) {
                    graphics->gui_symbols.frame_x = spr + i;
                } else { // Celebrating
                    int face = SY_KBOMB_W + i*2;
                    int frame = (int)((KILL_CELEBRATION - battle->players[i].kill_celebration_timer) / (KILL_CELEBRATION/6));
                    graphics->gui_symbols.frame_x = face + (frame % 2);
                }
            }

            tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, bomber_places + pos * separation, 4);

            // Wins
            graphics->gui_symbols.frame_x = battle->players_wins[i];
            tex_render(graphics, &graphics->gui_symbols, TEX_GUI_SYMBOLS, 14 + bomber_places + pos * separation, 4);
            ++pos;
        }
    }

}

// Update screen size
void update_window_size(Graphics* graphics) {

    int new_width = graphics->screen.window_width;
    int new_height = graphics->screen.window_height;

    if (graphics->screen.fullscreen == 1) {
        SDL_Rect display;
        SDL_GetDisplayBounds(0, &display);

        new_height = graphics->screen.SCREEN_HEIGHT = display.h;
        new_width = graphics->screen.SCREEN_WIDTH = display.w;

        SDL_SetWindowFullscreen(graphics->window,SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(graphics->window,0);
    }

    graphics->y_multiplier = (int)(graphics->screen.SCREEN_HEIGHT/BASE_HEIGHT);
    graphics->x_multiplier = graphics->y_multiplier;
    SDL_SetWindowSize(graphics->window, new_width, new_height);
}

// Hit box draw
void draw_hitbox(Graphics* graphics, int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(graphics->renderer, 255,255,255,255);

    rect_resolution_fix(graphics, &graphics->rect, x, y, w, h);
    SDL_RenderDrawRect(graphics->renderer, &graphics->rect);
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
