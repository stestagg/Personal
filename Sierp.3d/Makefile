
CC=clang++ -stdlib=libc++ -std=c++0x -O2 -g -lz

all: build

build: bin/calc bin/render bin/sierp2d

test: ico.png 2d.png

bin/calc: calc3d.cpp *.hpp 
	${CC} calc3d.cpp -o bin/calc

bin/render: render3d.cpp *.hpp
	${CC} render3d.cpp -o bin/render -lpng

bin/sierp2d: main.cpp *.hpp
	${CC} main.cpp -o bin/sierp2d -lpng

ico.dat: bin/calc
	bin/calc shapes/cube.csv 0 100 ico.dat

ico.png: bin/render ico.dat
	bin/render ico.dat 200 200 0 0 3 1 0.1 ico.png

2d.png: bin/sierp2d
	bin/sierp2d 3 0 200 200