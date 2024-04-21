CC = g++ -o build/

all: compile run

compile:
	mkdir -p build
	mkdir -p generated
	$(CC) src/main.cpp -o build/main `sdl2-config --cflags --libs` -lSDL2

run:
	./build/main
