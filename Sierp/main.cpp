#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "sierp.hpp"
#include <math.h>


static const size_t WIDTH = 500;
static const size_t HEIGHT = 500;
static const uint16_t STEP = 500;
static const size_t ITERS = 50000000000;
static const size_t NUM_SATURATES = 10;

int main(int argc, const char **argv){
	
    grid<uint8_t> a(WIDTH, HEIGHT);
    grid<uint8_t> b(WIDTH, HEIGHT);
    
    a.fill(0);
    
    
    int blanks = atol(argv[1]);
	printf("Blanks: %i\n", blanks);

		for (int nn=3; nn<20; ++nn){
			size_t left = NUM_SATURATES;
			grid<uint16_t> grd(WIDTH,HEIGHT);
			grd.fill(UINT16_MAX);

			sierp::Model<double> mm;
			for (int blank=0; blank < blanks; ++blank){
				mm.points.push_back(sierp::Point<double>(0.5,0.5));	
			}
			mm.add_regular(nn);

			for (int i=0; i< ITERS; ){
				mm.next();
				sierp::Point<size_t> pos = mm.get_position(WIDTH, HEIGHT);	
				uint16_t &point = grd.get_point(pos.x, pos.y);
				if (point >= STEP) point -= STEP;
				else{
					left -= 1;
					if (left == 0) break;
				}
			}
			char file_name[255];
			sprintf(file_name, "out-%i-%i.png", blanks, nn);
			printf("Output: %i, %i\n", blanks, nn);	
			png::write(grd, file_name);
		}
}