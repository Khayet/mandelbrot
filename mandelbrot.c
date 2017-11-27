#include <stdio.h>
#include <stdint.h>

typedef struct
{
  float r;
  float i;
} Complex;


typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Pixel;


int main()
{
  /*Complex c = { 12, 11 };*/
  Pixel black = { 0, 0, 0 };

  printf("(%i,%i,%i)\n", black.r, black.g, black.b);

  return 0;
}
