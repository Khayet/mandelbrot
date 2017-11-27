#include <stdio.h>

struct Complex
{
  float real;
  float imaginary;
};

int main()
{
  struct Complex c;
  c.real = 12;
  c.imaginary = 11;

  printf("%f", c.real);
  return 0;
}
