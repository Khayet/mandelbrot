#include "transfers.h"

Color tr_confuse(int iterations, int maxiterations)
{
  /*
   * Produces a funky looking yellow-green visualization.
   */

  Color c;
  c.r = (int)(((float)iterations / (float)maxiterations) * 255);
  c.g = (int)(((float)maxiterations / (float)iterations) * 255);
  c.b = 0;

  return c;
}


Color tr_maximum(int iterations, int maxiterations)
{
  /*
   * Shows samples with a maximum of iterations as white, others black.
   * That is, this shows mandelbrot-membership.
   */

  Color c;
  
  if (iterations == maxiterations) {
    c.r = 255;
    c.g = 255;
    c.b = 255;
  }
  else {
    c.r = 0;
    c.g = 0;
    c.b = 0;
  }
  
  return c;
}


Color tr_minimum(int iterations, int maxiterations)
{
  /*
   * Shows samples with > 0 iterations as white, others black.
   */

  Color c;
  (void)maxiterations;

  if (iterations > 0) {
    c.r = 255;
    c.g = 255;
    c.b = 255;
  }
  else {
    c.r = 0;
    c.g = 0;
    c.b = 0;
  }
  
  return c;
}


Color tr_linear(int iterations, int maxiterations)
{
  /*
   * Simply interpolates linearly between 0 and maxiterations.
   */

  Color c;
  
  c.r = (int)(((float)iterations / (float)maxiterations) * 255);
  c.g = (int)(((float)iterations / (float)maxiterations) * 255);
  c.b = (int)(((float)iterations / (float)maxiterations) * 255);
  
  return c;
}