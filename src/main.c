#include "game.h"

int main(int argc, char* argv[]){

    Game game;

    init_game(&game);

    run_game(&game);

    free_game(&game);

    return 0;
}