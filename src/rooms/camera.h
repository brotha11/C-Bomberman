#ifndef CAMERA_H
#define CAMERA_H

#include "../tools/utils.h"

typedef enum {
    CAM_STOP,
    CAM_FOLLOW,
} CAM_STATES;

typedef struct camera {

    double xf, yf;
    double speed;

    int x, y;
    int width, height;
    int follow_x, follow_y;
    int state;
} Camera;

Camera new_camera(int x, int y, int width, int height, double speed, int state);
void cam_update(Camera* cam);
void set_follow(Camera* cam, int x, int y, double speed);

#endif