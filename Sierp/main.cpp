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
	
    grid<uint8_t> output(WIDTH, HEIGHT);
    render(output);
    
    char file_name[255];
	sprintf(file_name, "out.png");
	printf("Output\n");	
	png::write(output, file_name);

};

int render(grid<uint8_t> &output){
    grid<uint8_t> a(WIDTH, HEIGHT);
    grid<uint8_t> b(WIDTH, HEIGHT);
    
    
    
    grid<uint8_t> *current = a;
    grid<uint8_t> *next = b;
    sierp::Point<uint8_t> points[3];
    points[0].x = 0; points[0].y = HEIGHT;
    points[1].x = WIDTH; points[1].y = HEIGHT;
    points[2].x = WIDTH/2; points[2].y = 0;
    
    a.fill(0);
    a.get_point(WIDTH/2, HEIGHT/2) = 1;
    b.fill(0);
    output.fill(255);
    
    while (true){
        for (int y=0; y<HEIGHT; ++y){
            size_t y_ofs = y * WIDTH;
            for (int x=0; x<WIDTH; ++x){
                uint8_t val = current[x + y_ofs];
                if (val > 0){
                    for (int c=0; c < 3; ++c){
                        size_t new_x = (x + points[c].x) / 2;
                        size_t new_y = (y + points[c].y) / 2;
                        uint8_t &tval = output.get_point(new_x, new_y);
                        if (tval < val) return;
                        tval -= val;
                        next.get_point(new_x, new_y) += val;
                }
            }
        }
    }
}