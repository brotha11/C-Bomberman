#include "game.h"

void init_game(Game* game) {

    game->game_running = true;
    init_graphics(&game->graphics);

    game->collision = NULL;
    game->bombs = NULL;
    game->fires = NULL;
    game->bricks = NULL;
    game->power_ups = NULL;

    start_room(game);
}

void update(Game* game) {

    check_inputs(&game->player.input);
    p_update(&game->player, &game->player.input, &game->collision, &game->bombs, &game->fires, &game->power_ups);

    check_inputs(&game->player2.input);
    p_update(&game->player2, &game->player2.input, &game->collision, &game->bombs, &game->fires, &game->power_ups);

    b_update(&game->bombs, &game->fires, &game->collision, &game->power_ups);
    f_update(&game->fires, &game->bombs);
    bri_update(&game->bricks, &game->fires, &game->collision, &game->power_ups);
    pw_update(&game->power_ups, &game->fires);

    if (!game->player.base.alive && game->player.death_timer == 0) {
        start_room(game);
    }
}

void start_room(Game* game) {
    int s_x = 40;
    int s_y = 48;

    srand(clock());

    free_room(game);
    add_collision(&game->collision, s_x-16,s_y-32,16*14,16);
    add_collision(&game->collision, s_x-16,s_y+16*10,16*14,16);

    add_collision(&game->collision, s_x-32,s_y-32,16,16*13);
    add_collision(&game->collision, s_x+16*12,s_y-32,16,16*13);

    //for (int i = 0; i < 10000000; ++i) add_collision(&game->collision, i*16,-16,16,16);

    for (int i = 0; i < 6; ++i) {
        for(int j = 0; j < 5; ++j){
            add_collision(&game->collision, s_x + 32*i, s_y + 32*j, 16, 16);
        }
    }

    for (int i = 0; i < 11; ++i) {
        for(int j = 0; j < 13; ++j){
            if(i == 0 && j == 0 || i == 0 && j == 1 || i == 1 && j == 0 || i == 10 && j == 12 || i == 9 && j == 12 || i == 10 && j == 11 ||
                coll_meeting(&game->collision, (s_x-16) + j*16, (s_y-16) + i*16, 16, 16)) continue;

            if (rand()%100 < 65)
                add_brick(&game->bricks, &game->collision, (s_x-16) + j*16, (s_y-16) + i*16, 16, 16);
        }
    }

    game->player = new_player(s_x-16,s_y-16,16,16,1,BOMBER_WHITE);
    game->player.input = new_controller(SDL_SCANCODE_W, SDL_SCANCODE_S,
        SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_G);

    game->player2 = new_player(s_x-16 + (16*13),s_y-16, + (16*11),16,1,BOMBER_WHITE);
    game->player2.input = new_controller(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_RSHIFT);
}

void render(Game* game) {
    SDL_RenderClear(game->graphics.renderer);

    // Map
    tex_render(&game->graphics, &game->graphics.background, MAP_01, 0,0);

    f_render(&game->graphics, &game->fires);
    bri_render(&game->graphics, &game->bricks);
    b_render(&game->graphics, &game->bombs);
    pw_render(&game->graphics, &game->power_ups);

    tex_render(&game->graphics, &game->player.base.sprite, BOMBER_WHITE, game->player.base.x, game->player.base.y);
    tex_render(&game->graphics, &game->player2.base.sprite, BOMBER_WHITE, game->player2.base.x, game->player2.base.y);


    SDL_RenderPresent(game->graphics.renderer);
}

void run_game(Game* game) {

    while (game->game_running) {

        update(game);
        render(game);
        game->game_running = (graphics_event(&game->graphics));
    }

}

void free_room(Game* game) {
    free_all_bricks(&game->bricks, &game->collision);
    free_collisions(&game->collision);
    free_all_bombs(&game->bombs, &game->collision);
    free_all_fires(&game->fires);
    free_all_powerups(&game->power_ups);
}

void free_game(Game* game) {
    free_graphics(&game->graphics);
    free_room(game);
}