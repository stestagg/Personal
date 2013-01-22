#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "mand.hpp"
#include "cubes.h"
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

    double min_val;
    double max_val;
    size_t histogram[UIMAX(in_T)] = {0};

    for (size_t i=0; i<grd->len; ++i){
        uint16_t val = (*grd)[i].g;
        if (val == UIMAX(in_T)) continue;
        histogram[val] += 1;
    }

    size_t cutoff = ((grd->len) * 0.5) / 100;
    size_t seen = 0;

    for (size_t i=0; i<grd->len; ++i){
        seen += histogram[i];
        min_val = i;
        if (seen >= cutoff) break;
    }

    seen=0;
    for (size_t i=(UIMAX(in_T) - 1); i>=0; --i){
        seen += histogram[i];
        max_val = i;
        if (seen >= cutoff) break;
    }

    double scaling = (UIMAX(in_T) - 1) / (max_val - min_val);

    grid<in_T> output(grd->width, grd->height);

    for (size_t i=0; i<grd->len; ++i){
        uint16_t val = (*grd)[i].g;
        if (val == UIMAX(in_T)){
            continue;
        }
        double offset = cubes[(*grd)[i].a];
        double combined = (val + offset);
        if (combined < min_val) output[i] = 0;
        else output[i] = (uint16_t)MIN(((combined - min_val) * scaling), (double)UIMAX(in_T)) ;
    }

    png::write(output, out_file);
};
