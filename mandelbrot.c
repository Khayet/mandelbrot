#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Complex
{
  float r;
  float i;
} Complex;


typedef struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Pixel;


typedef struct Image
{
  Pixel** pxs;
  int w;
  int h;
} Image;


Image create_image(int w, int h)
{
  /* column-first -> our images are lists of columns */
  /* TODO: this is performance critical, research. */

  Image img;
  Pixel** pxdata = malloc(w * sizeof(uint8_t*));

  int i;
  for (i = 0; i < w; i++) {
    pxdata[i] = malloc(h * sizeof(Pixel));
  }

  img.pxs = pxdata;
  img.w = w;
  img.h = h;

  return img;
}

void free_image_memory(Image img)
{
  int i;
  for (i = 0; i < img.w; i++) {
    free(img.pxs[i]);
  }
  free(img.pxs);
}

void write_ppm(Image img, char* dest)
{
  /* open dest file.. */
  /* write ppm header.. */
  /* write pixel data.. */
  /* close file.. */

  img.pxs[0][0].r = 255;
  img.pxs[0][0].g = 255;
  img.pxs[0][0].b = 255;

  printf("%i\n", img.pxs[0][0].r);
}


int main()
{
  Image img = create_image(1024, 1024);

  write_ppm(img, "img/image.ppm");

  free_image_memory(img);
  return 0;
}
