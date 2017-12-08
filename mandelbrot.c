#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
  int x=0, y=0;

  FILE* f = fopen(dest, "w");
  fprintf(f, "P6 %d %d 255\n", img.w, img.h); /* ppm header */

  for (y=0; y < img.h; y++) {
    for (x=0; x < img.w; x++) {
      fwrite(img.pxs[x], sizeof(img.pxs[x]), 1, f);
    }
  }

  fclose(f);
}


int main()
{
  Image img = create_image(1024, 1024);

  write_ppm(img, "image.ppm");

  free_image_memory(img);
  return 0;
}
