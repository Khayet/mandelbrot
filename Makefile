all: build run

build:
	gcc -O3 -g -ansi -pedantic -Wall -Wextra mandelbrot.c complex.c transfers.c args.c -o mandelbrot.exe

dump:
	objdump -S --disassemble mandelbrot.exe > mandelbrot.dump

run:
	./mandelbrot.exe -r 2400 1600
