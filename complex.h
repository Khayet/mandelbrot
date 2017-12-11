#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h> /* sqrt */

typedef struct Complex
{
  float r;
  float i;
} Complex;

Complex addx(Complex c1, Complex c2);
Complex multx(Complex c1, Complex c2);
float absx(Complex c);


#endif /* COMPLEX_H */
