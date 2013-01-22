#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "color.hpp"
#include <iostream>
#include <vector>

typedef PIXEL_TYPE in_type;

int main(int argc, const char **argv){

    if (argc < 4){
        printf("Usage: %s out_file file1 factor [file2 factor]...\n", argv[0]);
        exit(1);
    }

    png::PngReader<in_type> reader;

    const char *out_file = argv[1];

    for (int argi=2; argi < argc; argi+=2){
        const char *merge_filename = argv[argi];
        const char *factor_a = argv[argi + 1];
        double factor = atof(factor_a);
        printf("%s %f factor\n", merge_filename, factor);


    }
    return 1;
    /*grid<in_type> * ina = reader.read(argv[2]);
    grid<in_type> * inb = reader.read(argv[3]);
    grid<in_type> * inc = reader.read(argv[4]);
    grid<in_type> output(ina->width, ina->height);

    ssize_t offsetbx = ((ssize_t)ina->width - (ssize_t)inb->width) / 2;
    ssize_t offsetby = ((ssize_t)ina->height - (ssize_t)inb->height) / 2;

    ssize_t offsetcx = ((ssize_t)ina->width - (ssize_t)inc->width) / 2;
    ssize_t offsetcy = ((ssize_t)ina->height - (ssize_t)inc->height) / 2;

    for (size_t y=0; y < ina->height; ++y){
        for (size_t x=0; x < ina->width; ++x){
            in_type a = ina->get_point(x, y);
            in_type b = a;
            in_type c = a;

            int b_x = x - offsetbx;
            int b_y = y - offsetby;
            if (b_x >= 0 && b_y >= 0 && b_x < inb->width && b_y < inb->height){
                b = inb->get_point(b_x, b_y);
            }

            int c_x = x - offsetcx;
            int c_y = y - offsetcy;
            if (c_x >= 0 && c_y >= 0 && c_x < inc->width && c_y < inc->height){
                c = inc->get_point(c_x, c_y);
            }

            output.get_point(x, y) = average(a, b, c);
        }
    }

    png::write(output, out_file);*/
};
