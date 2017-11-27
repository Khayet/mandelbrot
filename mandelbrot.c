#include <stdio.h>

typedef struct
{
  float r;
  float i;
} Complex;


int main()
{
  Complex c = { 12, 11 };

  printf("%f\n", c.r);

  return 0;
}
