#include "game.h"

void init_game(Game* game) {

    game->game_running = true;
    init_graphics(&game->graphics);

    game->collision = NULL;
    game->bombs = NULL;
    game->fires = NULL;
    game->bricks = NULL;

    start_room(game);
    game->controller = new_controller();

}

void update(Game* game) {

    check_inputs(&game->controller);
    p_update(&game->player, &game->controller, game->collision, &game->bombs, &game->fires);
    b_update(&game->bombs, &game->fires, game->collision);
    f_update(&game->fires, &game->bombs);
    bri_update(&game->bricks, &game->fires, &game->collision);

    if (!game->player.base.alive) {
        start_room(game);
    }

}

void free_room(Game* game) {
    free_all_bombs(&game->bombs);
    free_all_fires(&game->fires);
    free_all_bricks(&game->bricks, &game->collision);
    free_collisions(&game->collision);
}

void start_room(Game* game) {
    int s_x = 40;
    int s_y = 48;

    free_room(game);
    add_collision(&game->collision, s_x-16,s_y-32,16*14,16);
    add_collision(&game->collision, s_x-16,s_y+16*10,16*14,16);

    add_collision(&game->collision, s_x-32,s_y-32,16,16*13);
    add_collision(&game->collision, s_x+16*12,s_y-32,16,16*13);

    for (int i = 0; i < 6; ++i) {
        for(int j = 0; j < 5; ++j){
            add_collision(&game->collision, s_x + 32*i, s_y + 32*j, 16, 16);
        }
    }

    game->player = new_player(s_x-16,s_y-16,16,16,1.25,BOMBER_WHITE);
    game->player.base.sprite.image_speed = 8;
}

void render(Game* game) {

    render_game(&game->graphics, &game->player.base, game->collision, &game->bombs, &game->fires, &game->bricks, &game->player);
}

void run_game(Game* game) {

    while (game->game_running) {

        update(game);
        render(game);
        game->game_running = (graphics_event(&game->graphics));
    }

}

void free_game(Game* game) {
    free_graphics(&game->graphics);
    free_all_bricks(&game->bricks, &game->collision);
    free_collisions(&game->collision);
    free_all_bombs(&game->bombs);
    free_all_fires(&game->fires);
}