#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "mand.hpp"
#include "cubes.h"
#include "gradient.hpp"
#include "color.hpp"
#include <iostream>

typedef unsigned short in_T;
typedef grayalpha<in_T, in_T> in_type;

int main(int argc, const char **argv){

    if (argc < 4){
        printf("Usage: %s map_file in_file out_file\n", argv[0]);
        exit(1);
    }

    const char *map_file = argv[1];
    const char *in_file = argv[2];
    const char *out_file = argv[3];

    Gradient gradient = Gradient::from_file(map_file);

    png::PngReader<in_type> reader;

    grid<in_type> * grd = reader.read(in_file);
    if (!grd){
        std::cout << "Could not read input file" << std::endl;
        exit(1);
    }

    grid<rgba<> > output(grd->width, grd->height);
    size_t target=grd->width * grd->height;

    for (size_t i=0; i<target; ++i){
        double val = (*grd)[i].g;
        //double offset = (((*grd)[i].a)/((double)UIMAX(in_T)));
        double offset = cubes[(*grd)[i].a];
        double combined = (val + offset);
        output[i] = gradient.interpolate(combined);
    }

    png::write(output, out_file);
};
