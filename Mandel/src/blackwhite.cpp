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

    double min_val = UIMAX(in_T);
    double max_val = 0;

    for (size_t i=0; i<grd->len; ++i){
        uint16_t val = (*grd)[i].g;
        if (val == UIMAX(in_T)) continue;

        double offset = cubes[(*grd)[i].a];
        double combined = val + offset;
        max_val = MAX(max_val, combined);
        min_val = MIN(min_val, combined);
    }

    grayalpha<uint16_t> maxga = grd->max();
    double scaling = (UIMAX(in_T) - 1) / (max_val - min_val);

    grid<in_T> output(grd->width, grd->height);

    for (size_t i=0; i<grd->len; ++i){
        uint16_t val = (*grd)[i].g;
        if (val == UIMAX(in_T)){
            continue;
        }
        double offset = cubes[(*grd)[i].a];
        double combined = (val + offset);
        output[i] = (uint16_t)((combined - min_val) * scaling) ;
    }

    png::write(output, out_file);
};
