#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef struct Complex
{
  float r;
  float i;
} Complex;


Complex multx(Complex c1, Complex c2)
{
  Complex res;
  res.r = c1.r * c2.r - c1.i * c2.i;
  res.i = c1.r * c2.i + c1.i * c2.r;
  return res;
}


float absx(Complex c)
{
  return sqrt(c.r*c.r + c.i*c.i);
}


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

  FILE* f = fopen(dest, "w");
  fprintf(f, "P6 %d %d 255 ", img.w, img.h); /* ppm header */

  /* row-first to conform to ppm specs. */
  for (y = 0; y < img.h; ++y) {
    for (x = 0; x < img.w; ++x) {
      fwrite(&img.pxs[x][y], sizeof(Pixel), 1, f);
    }
  }

  fclose(f);
}


int mandelbrot(int c, int z, int i)
{
  /*
   *  Returns number of iterations until the absolute value of z > 2 or
   * we reach a recursion boundary.
   */
}


Pixel mandelbrot_pixel(Complex c)
{
  /*
   * Receives a complex number (location in complex plane) and returns the
   * appropriate Pixel.
   */

  Pixel px;
  if (c.i >= 0) {
    px.r = 255, px.g = 0, px.b = 255;
  }
  else {
    px.r = 0, px.g = 0, px.b = 255;
  }

  if (c.r >= 0) {
    px.g = 255;
  }
  else {
    px.g = 0;
  }

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
      img.pxs[x][y] = mandelbrot_pixel(sample);
      sample.i -= sample_size_i;
    }
    sample.i = first.i;
    sample.r += sample_size_r;
  }
}


int main()
{
  Image img = create_image(1024, 1024);
  Complex c1 = { -2, 1 };
  Complex c2 = { 1, -1 };

  visualize_mandelbrot(img, c1, c2);

  write_ppm(img, "image.ppm");

  free_image_memory(img);
  return 0;
}
