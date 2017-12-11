all: build run

build:
	gcc -g -ansi -pedantic -Wall -Wextra mandelbrot.c complex.c -o mandelbrot.exe

run:
	./mandelbrot.exe
