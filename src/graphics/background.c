#include "background.h"

Background new_bg(int x, int y, int width, int height, int tile_x, int tile_y, float speed_x, float speed_y, int used) {
    Background bg;

    bg.sprite = new_sprite(width, height);
    bg.speed_x = speed_x;
    bg.speed_y = speed_y;

    bg.tile_x = tile_x;
    bg.tile_y = tile_y;

    bg.x = bg.start_x = x;
    bg.y = bg.start_y = y;

    bg.xf = x;
    bg.yf = y;
    bg.used = used;

    return bg;
}

void bg_update(Background* background) {
    for (int i = 0; i < MAX_BACKGROUND_COUNT; ++i) {
        Background* bg = &background[i];

        // Skip if background is not used
        if (bg->used == 0) continue;

        // Speed
        if (abs(bg->x - bg->start_x) != bg->sprite.width) {
            bg->xf = bg->xf + bg->speed_x;
            bg->x = (int)bg->xf;
        } else {
            bg->x = bg->start_x;
            bg->xf = bg->start_x;
        }

        if (abs(bg->y - bg->start_y) != bg->sprite.height) {
            bg->yf = bg->yf + bg->speed_y;
            bg->y = (int)bg->yf;
        } else {
            bg->y = bg->start_y;
            bg->yf = bg->start_y;
        }
    }

}