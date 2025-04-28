#include "game.h"

void init_game(Game* game) {

    game->game_running = true;
    init_graphics(&game->graphics);
    sound_init();
    
    // Key profiles
    game->main_profile = set_profile(SDL_SCANCODE_W, SDL_SCANCODE_S,
        SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_RSHIFT);
    game->type0_profile = set_profile(SDL_SCANCODE_W, SDL_SCANCODE_S,
        SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_E);
    game->type1_profile = set_profile(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_RSHIFT);
    game->nouse_profile = set_profile(0, 0, 0, 0, 0);

    battle_init(&game->battle);


    start_room(game);
}

void run_game(Game* game) {
    while (game->game_running) {

        update(game);
        render(game);
       
        int event = catch_events();
        switch(event) {
            case EV_CLOSE_GAME: 
                game->game_running = false; 
                break;
            case EV_GAMEPAD_DISC:
            case EV_GAMEPAD_CON: 
                set_joys(game->controllers, game->battle.players_on); 
                assign_inputs(game->controllers, game->battle.players_on, game->main_profile, 
                    game->type0_profile, game->type1_profile, game->nouse_profile);
                break;
            case EV_FULLSCREEN:
                set_screen_size(&game->graphics.screen, 800, 600, 1);
                update_window_size(&game->graphics);
                break;
        }
    }
}

void free_game(Game* game) {
    free_graphics(&game->graphics);
    battle_free(&game->battle);
    sound_exit();
    input_exit(game->controllers);
}

void update(Game* game) {
    battle_update(&game->battle, game->controllers, &game->graphics.screen);
    bg_update(game->graphics.backgrounds);
}

void start_room(Game* game) {
    battle_load(&game->battle, &game->graphics.screen);
    input_init(game->controllers, game->battle.players_on, game->nouse_profile);
    assign_inputs(game->controllers, game->battle.players_on, game->main_profile, 
        game->type0_profile, game->type1_profile, game->nouse_profile);

    game->graphics.backgrounds[0] = new_bg(-game->battle.camera.x*2,0,256,256,1,1,0,0,1);
    //game->graphics.backgrounds[1] = new_bg(game->battle.camera.x,133,256,137,1,0,0,0,1);
}

void render(Game* game) {

    Camera* r_cam = &game->battle.camera;
    Fire** r_fires = &game->battle.fires;
    Bomb** r_bombs = &game->battle.bombs;
    Brick** r_bricks = &game->battle.bricks;
    Power_up** r_pw = &game->battle.power_ups;
    Player* r_players = game->battle.players;

    SDL_RenderClear(game->graphics.renderer);
    ////
    //  REGULAR
    //  IS AFFECTED BY CAMERA POSITION
    ////
    
    int c_x = r_cam->x;
    int c_y = r_cam->y;

    // Background
    background_render(&game->graphics, game->graphics.backgrounds, r_cam);

    // Map
    tex_render(&game->graphics, &game->graphics.ground, TEX_MAP_01, 8 - c_x, 0 - c_y);

    f_render(&game->graphics, r_fires, c_x, c_y);
    bri_render(&game->graphics, r_bricks, c_x, c_y);
    b_render(&game->graphics, r_bombs, c_x, c_y);
    pw_render(&game->graphics, r_pw, c_x, c_y);


    // Players
    Player* order[MAX_BATTLE_PLAYERS];
    int ordered[MAX_BATTLE_PLAYERS] = {0};

    for (int p = 0; p < MAX_BATTLE_PLAYERS; ++p) {
        int best = -1;
        for (int o = 0; o < MAX_BATTLE_PLAYERS; ++o) {
            if (!r_players[o].player_on || ordered[o]) continue;
            if (best == -1 || r_players[o].base->y < r_players[best].base->y) {
                best = o;
            }
        }
        if (best != -1) {
            order[p] = &r_players[best];
            ordered[best] = 1;
        } else {
            order[p] = NULL; // No hay jugador disponible
        }
    }

    // Render
    for (int p = 0; p < MAX_BATTLE_PLAYERS; ++p) {
        if (order[p] == NULL || !order[p]->player_on) continue;
        int sh_x = order[p]->base->shake_x;
        int sh_y = order[p]->base->shake_y;

        tex_render(&game->graphics, &order[p]->base->sprite, TEX_BOMBER_WHITE + order[p]->id, 
            order[p]->base->x + sh_x - c_x, order[p]->base->y + sh_y - c_y);

        if (game->graphics.draw_hitboxes == 1) {
            draw_hitbox(&game->graphics, order[p]->base->x - c_x, order[p]->base->y - c_y,
                order[p]->base->width, order[p]->base->height);
        }
    }


    ////
    //  GUI
    //  NOT AFFECTED BY CAMERA POSITION
    ////
    gui_battle_render(&game->graphics, &game->battle);


    SDL_RenderPresent(game->graphics.renderer);
}
