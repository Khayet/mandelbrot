#include <stdio.h> /* FILE, printf, fprintf, fwrite, fclose */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp */

#include "complex.h" /* Complex, addx, multx, absx */

#define THRESHOLD 2

int maxiterations = 500;

typedef struct Pixel
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

typedef struct Image
{
  Pixel** pxs;
  int w;
  int h;
} Image;

typedef struct Samples
{
  int** data;
  int w;
  int h;
} Samples;

typedef struct Args
{
  int width;
  int height;
  Complex top_left;
  Complex bottom_right;
  int maxiterations;
  char* filename;
} Args;


void display_help_msg()
{
  /* 
   * TODO: write help message including general usage, sample usage and 
   * available options
   * recommended aspect ratio
   */
  printf("Sample usage: mandelbrot \n");
}


Args parse_args(int argc, char* argv[])
{
  Args args = { 1800, 1200, { -2, 1 }, { 1, -1 }, 500, "image.ppm" };

  int i;
  for (i = 1; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "--help") || 0 == strcmp(argv[i], "-h")) {
      display_help_msg();
      exit(0);
    }

    if (0 == strcmp(argv[i], "--resolution") || 0 == strcmp(argv[i], "-r")) {
      if (i+2 > argc) {
        printf("ERROR: --resolution option requires width and height.\n");
        printf("Sample usage: ./mandelbrot --resolution 900 600\n");
        exit(1);
      }
      args.width = atoi(argv[i+1]);
      args.height = atoi(argv[i+2]);
    }

    /* if --section or -s, parse section */
    if (0 == strcmp(argv[i], "--section") || 0 == strcmp(argv[i], "-s")) {
      if (i+4 > argc) {
        printf("ERROR: --section option requires 4 numbers (floats):\n");
        printf("top_left.r top_left.i bottom_right.r bottom_right.i\n");
        printf("r and i represent the real and imaginary part in the "
               "complex plane\n");
        printf("Sample usage: ./mandelbrot --section -2.0 1.0 1.0 -1.0\n");
        exit(1);
      }
      args.top_left.r = atof(argv[i+1]);
      args.top_left.i = atof(argv[i+2]);
      args.bottom_right.r = atof(argv[i+3]);
      args.bottom_right.i = atof(argv[i+4]);
    }

    /* if --maxiterations or -m, parse maxiterations */
    if (0 == strcmp(argv[i], "--maxiterations") || 0 == strcmp(argv[i], "-m")) {
      if (i+1 > argc) {
        printf("ERROR: --maxiterations option but no number of iterations specified.\n");
        printf("Sample usage: ./mandelbrot --maxiterations 800\n"); 
      }
      args.maxiterations = atoi(argv[i+1]);
    }


    if (0 == strcmp(argv[i], "--filename") || 0 == strcmp(argv[i], "-f")) {
      if (i+1 > argc) {
        /* TODO: check if file location is writable */
        printf("ERROR: --filename option but no file specified.\n");
        printf("Sample usage: ./mandelbrot --filename image.ppm\n");
      }
      args.filename = argv[i+1];
    }
  }

  return args;
}


Image create_image(int w, int h)
{
  /* column-first -> our images are lists of columns */

  Image img;
  Pixel** pxdata = malloc(w * sizeof(unsigned char*));

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


Samples create_samples(int w, int h)
{
  /* column-first -> our images are lists of columns */

  Samples smps;
  int** sampledata = malloc(w * sizeof(unsigned char*));

  int i;
  for (i = 0; i < w; ++i)
  {
    sampledata[i] = malloc(h * sizeof(int));
  }

  smps.data = sampledata;
  smps.w = w;
  smps.h = h;
  return smps;
}


void free_samples(Samples smps)
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
      fwrite(&img.pxs[x][y], sizeof(Pixel), 1, f);
    }
  }

  fclose(f);
}


int mandel(Complex c, Complex z, int i)
{
  /*
   * Recursively returns number of iterations until the absolute value of z > 
   * THRESHOLD or we reach a recursion boundary.
   */

  if ((z.r*z.r + z.i*z.i) > THRESHOLD*THRESHOLD) return i;
  if (i >= maxiterations) return i;
  return mandel(c, addx(multx(z,z), c), ++i);
}


int mandelbrot(Complex c)
{
  /*
   * Returns number of mandelbrot iterations for a point in the complex plane. 
   */

  Complex zero = { 0, 0 };
  return mandel(c, zero, 0);
}


void sample_plane(Samples smps, Complex first, Complex second)
{
  /*
   * Samples the section of the complex plane specified by first (top left) 
   * and second (bottom right), using the resolution of smps. 
   * Calculates for each sample the number of mandelbrot iterations. 
   */

  /* TODO: super-sampling */
  /* TODO: optimizations: symmetry, omit center */
  float width = second.r - first.r;
  float height = first.i - second.i;

  float sample_dist_r = width / (float)smps.w;
  float sample_dist_i = height / (float)smps.h;  

  int x, y;
  Complex sample = first;
  for (x = 0; x < smps.w; ++x) {
    for (y = 0; y < smps.h; ++y) {
      smps.data[x][y] = mandelbrot(sample);
      sample.i -= sample_dist_i;
    }
    sample.i = first.i;
    sample.r += sample_dist_r;
  }
}

/* TODO: write more transfer functions and move them to their own file. */
Pixel confuse(int iterations)
{
  /*
   * Produces a funky looking yellow-green visualization.
   */

  Pixel px;
  px.r = (int)(((float)iterations / (float)maxiterations) * 255);
  px.g = (int)(((float)maxiterations / (float)iterations) * 255);
  px.b = 0;

  return px;
}


Pixel binary(int iterations)
{
  /*
   * A simple black-and-white transfer function.
   */

  Pixel px;
  
  if (iterations == maxiterations) {
    px.r = 255;
    px.g = 255;
    px.b = 255;
  }
  else {
    px.r = 0;
    px.g = 0;
    px.b = 0;
  }
  
  return px;
}


void visualize(Image img, Samples smps, Pixel (func)(int))
{
  /*
   * Applies the transfer function (given by function pointer) for each sample 
   * in the given sample plane and writes all pixel values into img.
   */

  int x, y;
  for (x = 0; x < img.w; ++x) {
    for (y = 0; y < img.h; ++y) {
      img.pxs[x][y] = (func)(smps.data[x][y]);
    }
  } 
}


int main(int argc, char* argv[])
{
  Args args = parse_args(argc, argv);

  Samples samples = create_samples(args.width, args.height);
  Image img = create_image(args.width, args.height);

  Complex c1 = args.top_left;
  Complex c2 = args.bottom_right;

  sample_plane(samples, c1, c2);

  visualize(img, samples, confuse);

  write_ppm(img, args.filename);

  free_samples(samples);
  free_image_memory(img);

  printf("Done!\n");
  return 0;
}
