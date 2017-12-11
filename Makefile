all: build run

build:
	gcc -g -pg -ansi -pedantic -Wall -Wextra mandelbrot.c complex.c -o mandelbrot.exe

run:
	./mandelbrot.exe
