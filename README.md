# mandelbrot
A tiny image generator for the mandelbrot fractal written in ANSI C (C89), using 4 samples per pixel. Single-threaded. Should run on basically everything, including your toaster. 

Avoids computations inside the main cardioid (the large butt-shaped figure in the center) and the period-2-bulb to its left, but there are way faster fractal generators ([The Computer Languages Benchmark Game](https://benchmarksgame-team.pages.debian.net/benchmarksgame/performance/mandelbrot.html)).

## Compilation
```
cd ~/mandelbrot
make
```
## Usage
I have yet to find an ANSI-compatible command-line library. Currently, the program only supports very rudimentary command line argument parsing (see args.c). 

`./mandelbrot`  
Generates an image (image.ppm) of the entire mandelbrot set with a resolution of 900x600.

`./mandelbrot --section -2.0 1.0 1.0 -1.0 -r 2400 1600`  
Generates an image of a section of the complex plane (upper left: -2+i, bottom right: 1-i) with a resolution of 2400x1600.

## Convert from ppm
Every decent graphics editor (like GIMP) should be able to open a ppm and convert it to a number of different formats.
On Linux, assuming you have ImageMagick installed, you can also use the `convert`target in Makefile or convert manually on the command line:
```
convert image.ppm image.png
```



## Customization

You can customize the look by using a different transfer function (see transfers.c) and passing it as argument to visualize in the main function (mandelbrot.c):  
```
/* use the 'confuse' transfer function */
visualize(img, samples, args.maxiterations, tr_confuse);
```

Transfer functions map every possible number of mandelbrot iterations to an RGB color value.

