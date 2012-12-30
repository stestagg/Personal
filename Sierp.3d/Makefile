
COMPILER=clang++ -stdlib=libc++ 
#COMPILER=g++

CC=${COMPILER} -std=c++0x -fno-exceptions -O3 -g

all: build

build: bin/calc bin/render bin/sierp2d

test: ico.png out-0-3.png

bin/calc: calc3d.cpp *.hpp 
	${CC} calc3d.cpp -o bin/calc -lz

bin/render: render3d.cpp *.hpp
	${CC} render3d.cpp -o bin/render -lpng -lz

bin/sierp2d: main.cpp *.hpp
	${CC} main.cpp -o bin/sierp2d -lpng -lz

ico.dat: bin/calc
	bin/calc shapes/cube.csv 0 100 ico.dat

ico.png: bin/render ico.dat
	bin/render ico.dat 200 200 0 0.2 3 1 0.1 ico.png

out-0-3.png: bin/sierp2d
	bin/sierp2d 3 0 200 200

clean:
	- rm -rf bin/*
	- rm ico.dat ico.png out-0-3.png