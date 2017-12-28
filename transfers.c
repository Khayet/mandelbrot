#include "transfers.h"

Pixel confuse(int iterations, int maxiterations)
{
  /*
   * Produces a funky looking yellow-green visualization.
   */

  Pixel px;
  px.r = (int)(((float)iterations / (float)maxiterations) * 255);
  px.g = (int)(((float)maxiterations / (float)iterations) * 255);
  px.b = 0;

  return px;
}


Pixel maximum(int iterations, int maxiterations)
{
  /*
   * Shows samples with a maximum of iterations as white, others black.
   * That is, this shows mandelbrot-membership.
   */

  Pixel px;
  
  if (iterations == maxiterations) {
    px.r = 255;
    px.g = 255;
    px.b = 255;
  }
  else {
    px.r = 0;
    px.g = 0;
    px.b = 0;
  }
  
  return px;
}


Pixel minimum(int iterations, int maxiterations)
{
  /*
   * Shows samples with > 0 iterations as white, others black.
   */

  Pixel px;
  (void)maxiterations;

  if (iterations > 0) {
    px.r = 255;
    px.g = 255;
    px.b = 255;
  }
  else {
    px.r = 0;
    px.g = 0;
    px.b = 0;
  }
  
  return px;
}


Pixel linear(int iterations, int maxiterations)
{
  /*
   * Simply interpolates linearly between 0 and maxiterations.
   */

  Pixel px;
  
  px.r = (int)(((float)iterations / (float)maxiterations) * 255);
  px.g = (int)(((float)iterations / (float)maxiterations) * 255);
  px.b = (int)(((float)iterations / (float)maxiterations) * 255);
  
  return px;
}