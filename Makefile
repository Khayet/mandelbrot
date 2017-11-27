all: build run

build:
	gcc -g -ansi -pedantic -Wall -Wextra mandelbrot.c -o mandelbrot.exe

run:
	./mandelbrot.exe
