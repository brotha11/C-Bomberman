#ifndef TIMER_H
#define TIMER_H

#include "inttypes.h"
#include "SDL2/SDL.h"

/**
 * @brief Manages time count downs
 */
typedef struct timer {
    double time;
    double saved_00;
} Timer;

/**
 * @brief Creates a new timer
 * @param initial Initial time for this timer
 * @return A new timer struct
 */
Timer new_timer(double initial);

/**
 * @brief Counts down a timer
 * @param timer Reference to a struct timer
 * @param delta Delta time of the game
 * @return 1 when timer reaches 0
 */
int tick_timer(Timer* timer, double* delta);

/**
 * @brief Calculates time elapsed between frames
 * @return Delta time of the past frame
 */
double get_delta_time();

#endif
