#include "timer.h"

Timer new_timer(double initial) {
    Timer new;

    new.time = initial;
    new.saved_00 = 0;

    return new;
}

double get_delta_time() {
    static uint64_t last = 0;
    uint64_t now = SDL_GetPerformanceCounter();

    if (last == 0) {
        last = now;
        return 0.0;
    }

    double delta = (double)(now - last) / SDL_GetPerformanceFrequency();
    last = now;
    return delta;
}

int tick_timer(Timer* timer, double* delta) {
    // Tick down seconds
    if (timer->time >= 0) {
        timer->time -= (*delta);
        return 0;
    } else {
        timer->time = 0;
        return 1;
    }
}
