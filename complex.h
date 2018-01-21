#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct Complex
{
  float r;
  float i;
} Complex;

Complex addx(Complex, Complex);
Complex multx(Complex, Complex);


#endif /* COMPLEX_H */
