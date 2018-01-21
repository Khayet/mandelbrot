#include <stdlib.h>
#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "args.h"

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
  Args args = { 900, 600, { -2, 1 }, { 1, -1 }, 500, "image.ppm" };

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