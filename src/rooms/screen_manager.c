#include "screen_manager.h"

Screen init_screen(int W, int H, int F) {
    Screen new;

    set_screen_size(&new, W, H , F);
    new.update_size = 0;

    return new;
}

void set_screen_size(Screen* screen, int W, int H, int F) {
    screen->SCREEN_HEIGHT = screen->window_height = H;
    screen->SCREEN_WIDTH = screen->window_width = W;
    
    if (F == 1) {
        screen->fullscreen = !screen->fullscreen;
    }
    //screen->update_size = 1;
}

int get_center_x(Screen* screen, int width) {
    return (width/2) - (screen->SCREEN_WIDTH / get_game_scale(screen)) / 2;
}
int get_center_y(Screen* screen, int height) {
    return (height/2) - (screen->SCREEN_HEIGHT / get_game_scale(screen)) / 2;
}
int get_game_scale(Screen* screen) {
    return (int)(screen->SCREEN_HEIGHT/BASE_HEIGHT);
}
