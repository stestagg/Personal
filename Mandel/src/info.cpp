#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "cubes.h"
#include <iostream>

typedef unsigned short in_T;
typedef grayalpha<in_T, in_T> in_type;

int main(int argc, const char **argv){

    if (argc < 2){
        printf("Usage: %s in_file\n", argv[0]);
        exit(1);
    }

    const char *in_file = argv[1];

    png::PngReader<in_type> reader;

    grid<in_type> * grd = reader.read(in_file);
    if (!grd){
        std::cout << "Could not read input file" << std::endl;
        exit(1);
    }

    size_t target=grd->width * grd->height;

    double min_val=UIMAX(in_T);
    double max_val=0;

    for (size_t i=0; i<target; ++i){
        if ((*grd)[i].g == UIMAX(in_T)) continue;
        double val = (*grd)[i].g;

        //double offset = (((*grd)[i].a)/((double)UIMAX(in_T)));
        double offset = cubes[(*grd)[i].a];
        double combined = (val + offset);
        min_val = MIN(min_val, combined);
        max_val = MAX(max_val, combined);
    }
    std::cout << min_val << ", " << max_val << std::endl;
};
