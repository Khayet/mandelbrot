#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct Complex
{
  float r;
  float i;
} Complex;

Complex addx(Complex c1, Complex c2);
Complex multx(Complex c1, Complex c2);


#endif /* COMPLEX_H */
