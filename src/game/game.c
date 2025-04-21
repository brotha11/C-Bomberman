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

void update(Game* game) {
    battle_update(&game->battle, game->controllers);
    bg_update(game->graphics.backgrounds);
}

void start_room(Game* game) {
    battle_load(&game->battle);
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
    background_render(&game->graphics, game->graphics.backgrounds);

    // Map
    tex_render(&game->graphics, &game->graphics.ground, TEX_MAP_01, 8 - c_x,0 - c_y);

    f_render(&game->graphics, r_fires, c_x, c_y);
    bri_render(&game->graphics, r_bricks, c_x, c_y);
    b_render(&game->graphics, r_bombs, c_x, c_y);
    pw_render(&game->graphics, r_pw, c_x, c_y);

    for (int p = 0; p < MAX_BATTLE_PLAYERS; ++p) {
        if (!r_players[p].player_on) continue;

        tex_render(&game->graphics, &r_players[p].base.sprite, TEX_BOMBER_WHITE + r_players[p].id, 
            r_players[p].base.x - c_x, r_players[p].base.y - c_y);
    }

    ////
    //  GUI
    //  NOT AFFECTED BY CAMERA POSITION
    ////
    gui_battle_render(&game->graphics, &game->battle);


    SDL_RenderPresent(game->graphics.renderer);
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
        }
    }
}

void free_game(Game* game) {
    free_graphics(&game->graphics);
    battle_free(&game->battle);
    sound_exit();
    input_exit(game->controllers);
}