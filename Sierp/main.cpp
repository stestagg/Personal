#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "sierp.hpp"
#include <math.h>


static const size_t WIDTH = 4000;
static const size_t HEIGHT = 4000;
static const uint8_t STEP = 1;
static const size_t ITERS = 50000000000;
static const size_t NUM_SATURATES = 1000;

int main(){
	
	for (int nn=3; nn<20; ++nn){
		size_t left = NUM_SATURATES;
		grid<uint8_t> grd(WIDTH,HEIGHT);
		grd.fill(255);

		sierp::Model<double> mm;
		mm.points.push_back(sierp::Point<double>(0.5,0.5));
		mm.add_regular(nn);

		for (int i=0; i< ITERS; ){
			mm.next();
			sierp::Point<size_t> pos = mm.get_position(WIDTH, HEIGHT);	
			uint8_t &point = grd.get_point(pos.x, pos.y);
			if (point >= STEP) point -= STEP;
			else{
				left -= 1;
				if (left == 0) break;
			}
		}
		char file_name[255];
		sprintf(file_name, "out-%i.png", nn);
		printf("Output: %i\n", nn);	
		png::write(grd, file_name);
	}
	
}