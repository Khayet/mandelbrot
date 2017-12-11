#include "complex.h"

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
