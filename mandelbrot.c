#include <stdio.h> /* FILE, printf, fprintf, fwrite, fclose */
#include <stdint.h> /* uint8_t */
#include <stdlib.h> /* malloc, free */

#include "complex.h" /* Complex, addx, multx, absx */

#define LIMIT 255

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

  Image img;
  Pixel** pxdata = malloc(w * sizeof(uint8_t*));

  int i;
  for (i = 0; i < w; ++i) {
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
  for (i = 0; i < img.w; ++i) {
    free(img.pxs[i]);
  }
  free(img.pxs);
}


void write_ppm(Image img, char* dest)
{
  int x, y;

  FILE* f = fopen(dest, "wb");
  fprintf(f, "P6 %d %d 255 ", img.w, img.h); /* ppm header */

  /* row-first to conform to ppm specs. */
  for (y = 0; y < img.h; ++y) {
    for (x = 0; x < img.w; ++x) {
      fwrite(&img.pxs[x][y], sizeof(Pixel), 1, f);
    }
  }

  fclose(f);
}


uint8_t mandel(Complex c, Complex z, uint8_t i)
{
  /*
   * Returns number of iterations until the absolute value of z > 2 or
   * we reach a recursion boundary.
   */

  if (absx(z) > 20000) return i;
  if (i >= LIMIT) return i;
  return mandel(c, addx(multx(z,z), c), ++i);
}


uint8_t mandelbrot(Complex c)
{
  Complex zero = { 0, 0 };
  return mandel(c, zero, 0);
}


Pixel transfer(Complex c)
{
  /*
   * Receives a complex number (location in complex plane) and returns the
   * appropriate Pixel.
   */

  Pixel px;
  px.r = 0;
  px.g = 0;
  px.b = 0;

  px.r = mandelbrot(c);

  return px;
}


void visualize_mandelbrot(Image img, Complex first, Complex second)
{
  /*
   * Visualizes the detail of the complex plane inbetween the two given
   * complex numbers. first: upper left corner, second: lower right corner.
   */

  float width = second.r - first.r;
  float height = first.i - second.i;

  float sample_size_r = width / (float)img.w;
  float sample_size_i = height / (float)img.h;

  int x, y;
  Complex sample = first;
  for (x = 0; x < img.w; ++x) {
    for (y = 0; y < img.h; ++y) {
      img.pxs[x][y] = transfer(sample);
      sample.i -= sample_size_i;
    }
    sample.i = first.i;
    sample.r += sample_size_r;
  }
}


int main()
{
  Image img = create_image(300, 200);
  Complex c1 = { -2, 1 };
  Complex c2 = { 1, -1 };

  visualize_mandelbrot(img, c1, c2);

  write_ppm(img, "image.ppm");

  free_image_memory(img);
  printf("Done!\n");
  return 0;
}
