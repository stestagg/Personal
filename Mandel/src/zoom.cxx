#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "color.hpp"
#include <iostream>

typedef PIXEL_TYPE in_type;


int main(int argc, const char **argv){

    if (argc != 4){
        printf("Usage: %s in_file out_file scale\n", argv[0]);
        exit(1);
    }

    png::PngReader<in_type> reader;

    const char *in_file = argv[1];
    const char *out_file = argv[2];
    double scale = strtod(argv[3], NULL);

    grid<in_type> * in = reader.read(in_file);
    grid<in_type> out = in->zoom(scale);

    png::write(out, out_file);
};
