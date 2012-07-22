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

    grid<grayalpha<uint8_t, uint8_t> > output(grd->width, grd->height);

    size_t target=grd->width * grd->height;
    for (size_t i=0; i<target; ++i){
        if ((*grd)[i].g == UIMAX(in_T)){
            (*grd)[i].g = 0;
            output[i].a = 0;
        }else{
            output[i].a = 255;
        }
    }

    in_type max_val = grd->max();
    double scaling = 255/(double)max_val.g;
    
    for (size_t i=0; i<target; ++i){
        double val = (*grd)[i].g;
        //double offset = (((*grd)[i].a)/((double)UIMAX(in_T)));
        double offset = cubes[(*grd)[i].a];
        output[i].g = (uint8_t)((val + (offset)) * scaling);
    }

    png::write(output, out_file);
};
