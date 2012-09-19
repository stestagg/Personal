#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "color.hpp"
#include <iostream>


typedef uint8_t in_T;
typedef rgba<in_T> in_type;

in_T ave(in_T a, in_T b, in_T c){
    unsigned long z=a;
    z += b;
    z += c;
    return in_T(z/3);
}

in_type average(in_type a, in_type b, in_type c){
    return rgba<in_T>(
        ave(a.r, b.r, c.r), 
        ave(a.g, b.g, c.g), 
        ave(a.b, b.b, c.b), 
        ave(a.a, b.a, c.a));
}

int main(int argc, const char **argv){

    if (argc != 5){
        printf("Usage: %s out_file file1 file2 file3\n", argv[0]);
        exit(1);
    }

    png::PngReader<in_type> reader;

    const char *out_file = argv[1];

    grid<in_type> * ina = reader.read(argv[2]);
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

    png::write(output, out_file);
};
