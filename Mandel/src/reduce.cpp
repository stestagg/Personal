#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "mand.hpp"
#import "cubes.h"
#include "color.hpp"
#include <iostream>

typedef unsigned short in_T;
typedef grayalpha<in_T, in_T> in_type;

int main(int argc, const char **argv){

    if (argc < 3){
        printf("Usage: %s in_file out_file\n", argv[0]);
        exit(1);
    }

    const char *in_file = argv[1];
    const char *out_file = argv[2];

    png::PngReader<in_type> reader;

    grid<in_type> * grd = reader.read(in_file);
    if (!grd){
        std::cout << "Could not read input file" << std::endl;
        exit(1);
    }

    grid<grayalpha<uint16_t> > output(grd->width/4, grd->height/4);
    
    for (size_t y=0; y < grd->height/4; ++y){
        for (size_t x=0; x < grd->width/4; ++x){
            output.get_point(x, y) = grd->get_point(x*4, y*4);
        }
    } 

    png::write(output, out_file);
};
