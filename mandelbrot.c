#include <stdio.h> /* FILE, printf, fprintf, fwrite, fclose */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp */

#include "complex.h" /* Complex, addx, multx, absx */
#include "color.h" /* Color */
#include "transfers.h" /* transfer functions */
#include "args.h"

#define THRESHOLD 2

int maxiterations = 500;

typedef struct Image
{
  Color** data;
  int w;
  int h;
} Image;

typedef struct Samples
{
  float** data; /* float instead of int due to averaging (super-sampling) */
  int w;
  int h;
} Samples;


Image allocate_image_memory(int w, int h)
{
  /* column-first -> our images are lists of columns */

  Image img;
  Color** cldata = malloc(w * sizeof(unsigned char*));

  int i;
  for (i = 0; i < w; ++i) {
    cldata[i] = malloc(h * sizeof(Color));
  }

  img.data = cldata;
  img.w = w;
  img.h = h;

  return img;
}


void free_image_memory(Image img)
{
  int i;
  for (i = 0; i < img.w; ++i) {
    free(img.data[i]);
  }
  free(img.data);
}


Samples allocate_samples_memory(int w, int h)
{
  /* column-first -> our images are lists of columns */

  Samples smps;
  float** sampledata = malloc(w * sizeof(unsigned char*));

  int i;
  for (i = 0; i < w; ++i)
  {
    sampledata[i] = malloc(h * sizeof(float));
  }

  smps.data = sampledata;
  smps.w = w;
  smps.h = h;
  return smps;
}


void free_samples_memory(Samples smps)
{
  int i;
  for (i = 0; i < smps.w; ++i) {
    free(smps.data[i]);
  }
  free(smps.data);
}


void write_ppm(Image img, char* dest)
{
  int x, y;

  FILE* f = fopen(dest, "wb");
  fprintf(f, "P6 %d %d 255 ", img.w, img.h); /* ppm header */

  /* row-first to conform to ppm specs. */
  for (y = 0; y < img.h; ++y) {
    for (x = 0; x < img.w; ++x) {
      fwrite(&img.data[x][y], sizeof(Color), 1, f);
    }
  }

  fclose(f);
}


/*
 * Recursively returns number of iterations until the absolute value of z >
 * THRESHOLD or we reach a recursion boundary.
 */
int mandel(Complex c, Complex z, int i)
{
  if ((z.r*z.r + z.i*z.i) > THRESHOLD*THRESHOLD) return i;
  if (i >= maxiterations) return i;
  return mandel(c, addx(multx(z,z), c), ++i);
}


/*
 * Returns number of mandelbrot iterations for a point in the complex plane.
 */
int mandelbrot(Complex c)
{
  Complex zero = { 0, 0 };

   /* sample in cardioid */
  float q = (c.r - 0.25)*(c.r - 0.25) + c.i*c.i;
  if (q*(q + (c.r - 0.25)) < 0.25*c.i*c.i) {
    return maxiterations;
  }

  /* sample in big bulb (period-2) */
  if ((c.r + 1)*(c.r + 1) + c.i*c.i < 0.0625) {
    return maxiterations;
  }

  return mandel(c, zero, 0);
}


/*
 * Samples the section of the complex plane specified by first (top left)
 * and second (bottom right), using the resolution of smps and
 * calculates for each sample the number of mandelbrot iterations.
 *
 * Performs simple 4x super-sampling (4 samples per pixel).
 */
void sample_plane(Samples smps, Complex first, Complex second)
{
  /* TODO: optimizations: symmetry, omit center */
  float width = second.r - first.r;
  float height = first.i - second.i;

  float sample_dist_r = width / (float)smps.w;
  float sample_dist_i = height / (float)smps.h;

  int x, y;
  int tmp_s;
  Complex sample = first;
  for (x = 0; x < smps.w; ++x) {
    for (y = 0; y < smps.h; ++y) {

      tmp_s = mandelbrot(sample);

      sample.r += sample_dist_r / 2.0;
      tmp_s += mandelbrot(sample);

      sample.i -= sample_dist_i / 2.0;
      tmp_s += mandelbrot(sample);

      sample.r -= sample_dist_r / 2.0;
      tmp_s += mandelbrot(sample);

      smps.data[x][y] = (float)tmp_s / 4;

      sample.i -= sample_dist_i / 2.0;
    }
    sample.i = first.i;
    sample.r += sample_dist_r;
  }
}


/*
 * Applies the callback transfer function for each sample in the sample plane
 * and writes the resulting color values into img.
 */
void visualize(Image img, Samples smps, Color (transfer_func)(int, int))
{
  int x, y;
  for (x = 0; x < img.w; ++x) {
    for (y = 0; y < img.h; ++y) {
      img.data[x][y] = (transfer_func)(smps.data[x][y], maxiterations);
    }
  }
}


int main(int argc, char* argv[])
{
  Args args = parse_args(argc, argv);

  Samples samples = allocate_samples_memory(args.width, args.height);
  Image img = allocate_image_memory(args.width, args.height);

  Complex c1 = args.top_left;
  Complex c2 = args.bottom_right;

  sample_plane(samples, c1, c2);

  visualize(img, samples, tr_linear);

  write_ppm(img, args.filename);

  free_samples_memory(samples);
  free_image_memory(img);

  printf("Done!\n");
  return 0;
}
