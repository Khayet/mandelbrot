#include <stdio.h> /* FILE, printf, fprintf, fwrite, fclose */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp */

#include "complex.h" /* Complex, addx, multx, absx */

#define LIMIT 500
#define THRESHOLD 2

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

typedef struct Args
{
  int width;
  int height;
  Complex top_left;
  Complex bottom_right;
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
  Args args = { 900, 600, { -2, 1 }, { 1, -1 }, "image.ppm" };

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
   * Returns number of iterations until the absolute value of z > 2 or
   * we reach a recursion boundary.
   */

  if ((z.r*z.r + z.i*z.i) > THRESHOLD*THRESHOLD) return i;
  if (i >= LIMIT) return i;
  return mandel(c, addx(multx(z,z), c), ++i);
}


int mandelbrot(Complex c)
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
  int iterations = mandelbrot(c);
  px.r = iterations;
  px.g = iterations;
  px.b = iterations;

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


int main(int argc, char* argv[])
{
  Args args = parse_args(argc, argv);

  Image img = create_image(args.width, args.height);
  Complex c1 = args.top_left;
  Complex c2 = args.bottom_right;

  /* TODO: separate calculation of iterations and visualization */
  visualize_mandelbrot(img, c1, c2);

  write_ppm(img, args.filename);

  free_image_memory(img);
  printf("Done!\n");
  return 0;
}
