#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h> /* sqrt */

typedef struct Complex
{
  float r;
  float i;
} Complex;


Complex addx(Complex c1, Complex c2)
{
  Complex res;
  res.r = c1.r + c2.r;
  res.i = c1.i + c2.i;
  return res;
}


Complex multx(Complex c1, Complex c2)
{
  Complex res;
  res.r = c1.r * c2.r - c1.i * c2.i;
  res.i = c1.r * c2.i + c1.i * c2.r;
  return res;
}


float absx(Complex c)
{
  return sqrt(c.r*c.r + c.i*c.i);
}


#endif /* COMPLEX_H */
