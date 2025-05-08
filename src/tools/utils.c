#include "utils.h"

// Lerp function with a treshold
double lerp(double a, double b, double t) {
    const double threshold = 0.01;
    if (fabs(b - a) < threshold) {
        return b;
    }

    return a + (b - a) * t;
}