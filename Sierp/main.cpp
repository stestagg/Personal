#include <stdint.h>
#include "common.hpp"
#include "output.hpp"

int main(){
	grid<uint8_t> grd(255, 255);
	for (int y=0; y<255; ++y){
		for (int x=0; x<255; ++x){
			grd.data[x + (y * grd.width)] = x;
		}
	}
	png::write(grd, "out2.png");
}