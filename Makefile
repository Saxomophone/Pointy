CC = g++ -o build/

all: compile run

compile:
	mkdir -p build
	mkdir -p generated
	$(CC) src/point.cpp -o build/point `sdl2-config --cflags --libs` -lSDL2

run:
	./build/point


old: compile_old run_old

compile_old:
	mkdir -p build
	mkdir -p generated
	$(CC) src/main.cpp -o build/main `sdl2-config --cflags --libs` -lSDL2

run_old:
	./build/main
