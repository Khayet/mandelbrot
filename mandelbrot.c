#include <stdio.h>

struct Complex
{
  float r;
  float i;
};

int main()
{
  struct Complex c;
  c.r = 12;
  c.i = 11;

  printf("%f\n", c.r);
  return 0;
}
