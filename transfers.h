#ifndef TRANSFERS_H
#define TRANSFERS_H

#include "color.h" /* Color */


/*
 * A transfer function is a function mapping input values
 * (in our case: number of mandelbrot iterations, and the maximum number
 *  of iterations) to a color value (RGB).
 *
 * You can chose one of the following functions or define your own.
 */

Color tr_confuse(int iterations, int maxiterations);
Color tr_maximum(int iterations, int maxiterations);
Color tr_minimum(int iterations, int maxiterations);
Color tr_linear(int iterations, int maxiterations);

#endif /* TRANSFERS_H */
