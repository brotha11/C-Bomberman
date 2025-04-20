#include "camera.h"

Camera new_camera(int x, int y, int width, int height, float speed, int state) {
    Camera cam;

    cam.follow_x = 0;
    cam.follow_y = 0;

    cam.x = x;
    cam.y = y;

    cam.xf = x;
    cam.yf = y;

    cam.width = width;
    cam.height = height;

    cam.speed = speed;
    cam.state = state;

    return cam;
}

void cam_update(Camera* cam) {
    switch(cam->state) {
        case CAM_STOP:
            break;
        case CAM_FOLLOW:
            cam->xf = lerp(cam->xf, cam->follow_x, cam->speed);
            cam->yf = lerp(cam->yf, cam->follow_y, cam->speed);

            cam->x = (int)cam->xf;
            cam->y = (int)cam->yf;

            break;
    }
}

void set_follow(Camera* cam, int x, int y, float speed) {
    cam->follow_x = x;
    cam->follow_y = y;
    cam->speed = speed;
}

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}
