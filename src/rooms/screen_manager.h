#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "../game/game_constants.h"

typedef struct screen {
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;

    int window_width;
    int window_height;

    int fullscreen;
    int update_size;
} Screen;

Screen init_screen(int W, int H, int F);
void set_screen_size(Screen* screen, int W, int H, int F);

int get_center_x(Screen* screen, int width);
int get_center_y(Screen* screen, int height);
int get_game_scale(Screen* screen);

#endif
