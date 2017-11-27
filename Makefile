all: build run

build:
	gcc -ansi -pedantic -Wall -Wextra mandelbrot.c -o mandelbrot.exe

run:
	./mandelbrot.exe
