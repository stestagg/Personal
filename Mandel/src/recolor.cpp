#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "color.hpp"
#include <iostream>

#define PIXEL_TYPE rgba<uint8_t>


void rec(grid<PIXEL_TYPE> &grd,const  PIXEL_TYPE color, const char *name, const char *base){
    for (size_t i=0; i< grd.len; ++i){
        PIXEL_TYPE dest = color;
        dest.a = grd.data[i].a;
        grd.data[i] = dest;
    }
    char out_name[PATH_MAX]={};
    snprintf(out_name, PATH_MAX, base, name);
    png::write(grd, out_name);
}

int main(int argc, const char **argv){

    if (argc != 3){
        printf("Usage: %s in_file out_template\n", argv[0]);
        exit(1);
    }

    png::PngReader<PIXEL_TYPE> reader;

    const char *in_file = argv[1];
    const char *out_base = argv[2];
    grid<PIXEL_TYPE> * in = reader.read(in_file);

    rec(*in, rgba<uint8_t>(255,255,255), "white", out_base);
    rec(*in, rgba<uint8_t>(24,24,24), "black", out_base);
    rec(*in, rgba<uint8_t>(8,73,108), "primary_dark", out_base);
    rec(*in, rgba<uint8_t>(102, 200, 251), "primary_light", out_base);
    rec(*in, rgba<uint8_t>(13,125,184), "primary", out_base);
};
